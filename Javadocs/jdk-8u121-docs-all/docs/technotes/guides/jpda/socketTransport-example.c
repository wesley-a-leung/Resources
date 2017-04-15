/*
 * Copyright (c) 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

#include "jdwpTransport.h"
#include "sysSocket.h"

/*
 * The Socket Transport Library.
 *
 * This module is an implementation of the Java Debug Wire Protocol Transport
 * Service Provider Interface - see src/share/javavm/export/jdwpTransport.h.
 */

static int serverSocketFD;
static int socketFD = -1;
static jdwpTransportCallback *callback;
static JavaVM *jvm;
static int tlsIndex;
static jboolean initialized;
static struct jdwpTransportNativeInterface_ interface;
static jdwpTransportEnv single_env = (jdwpTransportEnv)&interface;

#define RETURN_ERROR(err, msg) \
	if (1==1) { \
            setLastError(err, msg); \
            return err; \
        } 

#define RETURN_IO_ERROR(msg)	RETURN_ERROR(JDWPTRANSPORT_ERROR_IO_ERROR, msg);

#define RETURN_RECV_ERROR(n) \
	if (n == 0) { \
	    RETURN_ERROR(JDWPTRANSPORT_ERROR_IO_ERROR, "premature EOF"); \
	} else { \
	    RETURN_IO_ERROR("recv error"); \
	} 

/*
 * Record the last error for this thread.
 */
static void
setLastError(jdwpTransportError err, char *newmsg) {
    char buf[255];
    char *msg;
    
    /* get any I/O first in case any system calls override errno */
    if (err == JDWPTRANSPORT_ERROR_IO_ERROR) {
	dbgsysGetLastIOError(buf, sizeof(buf));
    }

    msg = (char *)dbgsysTlsGet(tlsIndex);
    if (msg != NULL) {
        (*callback->free)(msg);
    }

    if (err == JDWPTRANSPORT_ERROR_IO_ERROR) {
	char *join_str = ": ";
	int msg_len = strlen(newmsg) + strlen(join_str) + strlen(buf) + 3;	
	msg = (*callback->alloc)(msg_len);
	if (msg != NULL) {
	    strcpy(msg, newmsg);
	    strcat(msg, join_str);
	    strcat(msg, buf);
	}
    } else {
	msg = (*callback->alloc)(strlen(newmsg)+1);
        if (msg != NULL) {
            strcpy(msg, newmsg); 
        }
    }

    dbgsysTlsPut(tlsIndex, msg);
}

static jdwpTransportError 
setOptions(int fd) 
{
    jvalue dontcare;
    int err; 

    
    err = dbgsysSetSocketOption(fd, SO_REUSEADDR, JNI_TRUE, dontcare);                     
    if (err < 0) {                                                              
	RETURN_IO_ERROR("setsockopt SO_REUSEADDR failed");
    }
    
    err = dbgsysSetSocketOption(fd, TCP_NODELAY, JNI_TRUE, dontcare);
    if (err < 0) {
        RETURN_IO_ERROR("setsockopt TCPNODELAY failed");
    }

    return JDWPTRANSPORT_ERROR_NONE;
}

static jdwpTransportError 
handshake(int fd, jlong timeout) {
    char *hello = "JDWP-Handshake";
    char b[16];
    int rv, received, i;

    if (timeout > 0) {
	dbgsysConfigureBlocking(fd, JNI_FALSE);
    }
    received = 0;
    while (received < (int)strlen(hello)) {
        int n;
 	char *buf;	
	if (timeout > 0) {
	    rv = dbgsysPoll(fd, JNI_TRUE, JNI_FALSE, timeout);
	    if (rv <= 0) {
		RETURN_IO_ERROR("timeout during handshake");
	    }
	}
	buf = b;
	buf += received;
	n = dbgsysRecv(fd, buf, strlen(hello)-received, 0);
	if (n == 0) {
	    setLastError(0, "handshake failed - connection prematurally closed");
	    return JDWPTRANSPORT_ERROR_IO_ERROR;
	}
	if (n < 0) {
	    RETURN_IO_ERROR("recv failed during handshake");
	}
	received += n;
    }
    if (timeout > 0) {
	dbgsysConfigureBlocking(fd, JNI_TRUE);
    }
    for (i=0; i<(int)strlen(hello); i++) {
        if (b[i] != hello[i]) {
	    char msg[64];
	    strcpy(msg, "handshake failed - received >");
	    strncat(msg, b, strlen(hello));
	    strcat(msg, "< - excepted >");
	    strcat(msg, hello);
	    strcat(msg, "<");
	    setLastError(0, msg);
	    return JDWPTRANSPORT_ERROR_IO_ERROR;
	}
    }

    if (dbgsysSend(fd, hello, strlen(hello), 0) != (int)strlen(hello)) {
	RETURN_IO_ERROR("send failed during handshake");
    }
    return JDWPTRANSPORT_ERROR_NONE;
}

static jdwpTransportError
parseAddress(const char *address, struct sockaddr_in *sa, UINT32 defaultHost) {
    int err;
    char *colon;

    memset((void *)sa,0,sizeof(struct sockaddr_in));
    sa->sin_family = AF_INET;

    /* check for host:port or port */
    colon = strchr(address, ':');
    if (colon == NULL) {
        u_short port = (u_short)atoi(address);
        sa->sin_port = dbgsysHostToNetworkShort(port);
        sa->sin_addr.s_addr = dbgsysHostToNetworkLong(defaultHost);
    } else {
        char *buf;
        char *hostname;
        u_short port;
        UINT32 addr;

        buf = (*callback->alloc)(strlen(address)+1);
        if (buf == NULL) {
            RETURN_ERROR(JDWPTRANSPORT_ERROR_OUT_OF_MEMORY, "out of memory");
        }
        strcpy(buf, address);
        buf[colon - address] = '\0';
        hostname = buf;
        port = atoi(colon + 1);
        sa->sin_port = dbgsysHostToNetworkShort(port);

        /*
         * First see if the host is a literal IP address.
         * If not then try to resolve it.
         */
        addr = dbgsysInetAddr(hostname);
        if (addr == 0xffffffff) {
            struct hostent *hp = dbgsysGetHostByName(hostname);
            if (hp == NULL) {
                /* don't use RETURN_IO_ERROR as unknown host is normal */
                setLastError(0, "gethostbyname: unknown host");
                (*callback->free)(buf);
                return JDWPTRANSPORT_ERROR_IO_ERROR;
            }

            /* lookup was successful */
            memcpy(&(sa->sin_addr), hp->h_addr_list[0], hp->h_length);
        } else {
            sa->sin_addr.s_addr = addr;
        }

        (*callback->free)(buf);
    }

    return JDWPTRANSPORT_ERROR_NONE;
}


static jdwpTransportError JNICALL
socketTransport_getCapabilities(jdwpTransportEnv* env, 
	JDWPTransportCapabilities* capabilitiesPtr) 
{
    JDWPTransportCapabilities result;

    memset(&result, 0, sizeof(result));
    result.can_timeout_attach = JNI_TRUE;
    result.can_timeout_accept = JNI_TRUE;
    result.can_timeout_handshake = JNI_TRUE;

    *capabilitiesPtr = result;

    return JDWPTRANSPORT_ERROR_NONE;
}


static jdwpTransportError JNICALL
socketTransport_startListening(jdwpTransportEnv* env, const char* address, 
		               char** actualAddress)
{
    struct sockaddr_in sa;		
    int err;
    char *colon;

    memset((void *)&sa,0,sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;

    /* no address provided */
    if ((address == NULL) || (address[0] == '\0')) {
	address = "0";
    }

    err = parseAddress(address, &sa, INADDR_ANY);
    if (err != JDWPTRANSPORT_ERROR_NONE) {
	return err;
    }

    serverSocketFD = dbgsysSocket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketFD < 0) {
        RETURN_IO_ERROR("socket creation failed");
    }

    err = setOptions(serverSocketFD);
    if (err) {
        return err;
    }

    err = dbgsysBind(serverSocketFD, (struct sockaddr *)&sa, sizeof(sa));
    if (err < 0) {
	RETURN_IO_ERROR("bind failed");
    }

    err = dbgsysListen(serverSocketFD, 1);
    if (err < 0) {
        RETURN_IO_ERROR("listen failed");
    }

    {
        char buf[20];
        int len = sizeof(sa);
	jint portNum;
        err = dbgsysGetSocketName(serverSocketFD, 
                               (struct sockaddr *)&sa, &len);
        portNum = dbgsysNetworkToHostShort(sa.sin_port);
        sprintf(buf, "%d", portNum);
        *actualAddress = (*callback->alloc)(strlen(buf) + 1);
	if (*actualAddress == NULL) {
	    RETURN_ERROR(JDWPTRANSPORT_ERROR_OUT_OF_MEMORY, "out of memory");
	} else {
	    strcpy(*actualAddress, buf);
	}
    }

    return JDWPTRANSPORT_ERROR_NONE;
}

static jdwpTransportError JNICALL
socketTransport_accept(jdwpTransportEnv* env, jlong acceptTimeout, jlong handshakeTimeout) 
{
    int socketLen, err;
    struct sockaddr_in socket;

    memset((void *)&socket,0,sizeof(struct sockaddr_in));
    socketLen = sizeof(socket);

    if (acceptTimeout > 0) {
	int rv;
	dbgsysConfigureBlocking(serverSocketFD, JNI_FALSE);
	rv = dbgsysPoll(serverSocketFD, JNI_TRUE, JNI_FALSE, acceptTimeout);
	if (rv <= 0) {
	    /* set the last error here as could be overridden by configureBlocking */
	    if (rv == 0) {
	        setLastError(JDWPTRANSPORT_ERROR_IO_ERROR, "poll failed");
	    }
	    dbgsysConfigureBlocking(serverSocketFD, JNI_TRUE);
	    if (rv == 0) {
	        RETURN_ERROR(JDWPTRANSPORT_ERROR_TIMEOUT, "timed out waiting for connection");
	    } else {		
		return JDWPTRANSPORT_ERROR_IO_ERROR;
	    }
	}
    }

    socketFD = dbgsysAccept(serverSocketFD,
                               (struct sockaddr *)&socket,
                                &socketLen);
    /* set the last error here as could be overridden by configureBlocking */
    if (socketFD < 0) {
	setLastError(JDWPTRANSPORT_ERROR_IO_ERROR, "accept failed");
    }
    if (acceptTimeout > 0) {
	dbgsysConfigureBlocking(serverSocketFD, JNI_TRUE);
    }
    if (socketFD < 0) {
        return JDWPTRANSPORT_ERROR_IO_ERROR;
    }

    err = handshake(socketFD, handshakeTimeout);
    if (err) {
	dbgsysSocketClose(socketFD);
        socketFD = -1;
        return err;
    }

    return JDWPTRANSPORT_ERROR_NONE;
}

static jdwpTransportError JNICALL
socketTransport_stopListening(jdwpTransportEnv *env)
{
    if (serverSocketFD < 0) {
	RETURN_ERROR(JDWPTRANSPORT_ERROR_ILLEGAL_STATE, "connection not open");
    }
    if (dbgsysSocketClose(serverSocketFD) < 0) {
	RETURN_IO_ERROR("close failed");
    }
    serverSocketFD = -1;
    return JDWPTRANSPORT_ERROR_NONE;
}

static jdwpTransportError JNICALL
socketTransport_attach(jdwpTransportEnv* env, const char* addressString, jlong attachTimeout,
		       jlong handshakeTimeout)
{
    struct sockaddr_in sa;
    int err;

    if (addressString == NULL || addressString[0] == '\0') {
	RETURN_ERROR(JDWPTRANSPORT_ERROR_ILLEGAL_ARGUMENT, "address is missing");
    }
     
    err = parseAddress(addressString, &sa, 0x7f000001);
    if (err != JDWPTRANSPORT_ERROR_NONE) {
        return err;
    }

    socketFD = dbgsysSocket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
	RETURN_IO_ERROR("unable to create socket");
    }

    err = setOptions(socketFD);
    if (err) {
        return err;
    }

    /* 
     * To do a timed connect we make the socket non-blocking
     * and poll with a timeout;
     */
    if (attachTimeout > 0) {
        dbgsysConfigureBlocking(socketFD, JNI_FALSE);
    }

    err = dbgsysConnect(socketFD, (struct sockaddr *)&sa, sizeof(sa));
    if (err == DBG_EINPROGRESS && attachTimeout > 0) {
	err = dbgsysFinishConnect(socketFD, attachTimeout);

	if (err == DBG_ETIMEOUT) {
	    dbgsysConfigureBlocking(socketFD, JNI_TRUE);
	    RETURN_ERROR(JDWPTRANSPORT_ERROR_TIMEOUT, "connect timed out");
	}
    }

    if (err < 0) {
	RETURN_IO_ERROR("connect failed");
    }

    if (attachTimeout > 0) {
	dbgsysConfigureBlocking(socketFD, JNI_TRUE); 
    }

    err = handshake(socketFD, handshakeTimeout);
    if (err) {
	dbgsysSocketClose(socketFD);
	socketFD = -1;
 	return err;
    }

    return JDWPTRANSPORT_ERROR_NONE;
}

static jboolean JNICALL
socketTransport_isOpen(jdwpTransportEnv* env) 
{
    if (socketFD >= 0) {
  	return JNI_TRUE;
    } else {
	return JNI_FALSE;
    }
}

static jdwpTransportError JNICALL
socketTransport_close(jdwpTransportEnv* env)
{
    int fd = socketFD;
    socketFD = -1;
    if (fd < 0) {
	return JDWPTRANSPORT_ERROR_NONE;
    }
    if (dbgsysSocketClose(fd) < 0) {
	/*
	 * close failed - it's pointless to restore socketFD here because
	 * any subsequent close will likely fail aswell.
 	 */
	RETURN_IO_ERROR("close failed");
    }
    return JDWPTRANSPORT_ERROR_NONE;
}

static jdwpTransportError JNICALL
socketTransport_writePacket(jdwpTransportEnv* env, const jdwpPacket *packet)
{
    jint len, data_len, id;
    jbyte *data;

    /* packet can't be null */
    if (packet == NULL) {
	RETURN_ERROR(JDWPTRANSPORT_ERROR_ILLEGAL_ARGUMENT, "packet is NULL");
    }

    len = packet->type.cmd.len;		/* includes header */
    data_len = len - 11;

    /* bad packet */
    if (data_len < 0) {
	RETURN_ERROR(JDWPTRANSPORT_ERROR_ILLEGAL_ARGUMENT, "invalid length");
    }

    len = (jint)dbgsysHostToNetworkLong(len);

    if (dbgsysSend(socketFD,(char *)&len,sizeof(jint),0) != sizeof(jint)) {
	RETURN_IO_ERROR("send failed");
    }

    id = (jint)dbgsysHostToNetworkLong(packet->type.cmd.id);

    if (dbgsysSend(socketFD,(char *)&(id),sizeof(jint),0) != sizeof(jint)) {
	RETURN_IO_ERROR("send failed");
    }

    if (dbgsysSend(socketFD,(char *)&(packet->type.cmd.flags)
	    ,sizeof(jbyte),0) != sizeof(jbyte)) {        
	RETURN_IO_ERROR("send failed");
    }

    if (packet->type.cmd.flags & JDWPTRANSPORT_FLAGS_REPLY) {
        jshort errorCode = dbgsysHostToNetworkShort(packet->type.reply.errorCode);
        if (dbgsysSend(socketFD,(char *)&(errorCode)
		       ,sizeof(jshort),0) != sizeof(jshort)) {
	    RETURN_IO_ERROR("send failed");
	}
    } else {
        if (dbgsysSend(socketFD,(char *)&(packet->type.cmd.cmdSet)
			,sizeof(jbyte),0) != sizeof(jbyte)) {
	    RETURN_IO_ERROR("send failed");
	}
        if (dbgsysSend(socketFD,(char *)&(packet->type.cmd.cmd)
		,sizeof(jbyte),0) != sizeof(jbyte)) {
	    RETURN_IO_ERROR("send failed");
	}
    }

    data = packet->type.cmd.data;
    if (dbgsysSend(socketFD,(char *)data,data_len,0) != data_len) {
	RETURN_IO_ERROR("send failed");
    }

    return JDWPTRANSPORT_ERROR_NONE;
}

static jint
recv_fully(int f, char *buf, int len)
{
    int nbytes = 0;
    while (nbytes < len) {
        int res = dbgsysRecv(f, buf + nbytes, len - nbytes, 0);
        if (res < 0) {
            return res;
        } else if (res == 0) {
            break; /* eof, return nbytes which is less than len */
        }
        nbytes += res;
    }
    return nbytes;
}

static jdwpTransportError JNICALL
socketTransport_readPacket(jdwpTransportEnv* env, jdwpPacket* packet) {
    jint length, data_len;
    jint n;

    /* packet can't be null */
    if (packet == NULL) {
	RETURN_ERROR(JDWPTRANSPORT_ERROR_ILLEGAL_ARGUMENT, "packet is null");
    }

    /* read the length field */
    n = recv_fully(socketFD, (char *)&length, sizeof(jint));

    /* check for EOF */
    if (n == 0) {
	packet->type.cmd.len = 0;
	return JDWPTRANSPORT_ERROR_NONE;
    }
    if (n != sizeof(jint)) {
	RETURN_RECV_ERROR(n);
    }

    length = (jint)dbgsysNetworkToHostLong(length);
    packet->type.cmd.len = length;


    n = recv_fully(socketFD,(char *)&(packet->type.cmd.id),sizeof(jint));
    if (n < (int)sizeof(jint)) {
	RETURN_RECV_ERROR(n);
    }

    packet->type.cmd.id = (jint)dbgsysNetworkToHostLong(packet->type.cmd.id);

    n = recv_fully(socketFD,(char *)&(packet->type.cmd.flags),sizeof(jbyte));
    if (n < (int)sizeof(jbyte)) {
	RETURN_RECV_ERROR(n);
    }

    if (packet->type.cmd.flags & JDWPTRANSPORT_FLAGS_REPLY) {
        n = recv_fully(socketFD,(char *)&(packet->type.reply.errorCode),sizeof(jbyte));
	if (n < (int)sizeof(jshort)) {
	    RETURN_RECV_ERROR(n);
	}
    } else {
        n = recv_fully(socketFD,(char *)&(packet->type.cmd.cmdSet),sizeof(jbyte));
	if (n < (int)sizeof(jbyte)) {
	    RETURN_RECV_ERROR(n);
	}
    
        n = recv_fully(socketFD,(char *)&(packet->type.cmd.cmd),sizeof(jbyte));
        if (n < (int)sizeof(jbyte)) {
	    RETURN_RECV_ERROR(n);
	}
    }

    data_len = length - ((sizeof(jint) * 2) + (sizeof(jbyte) * 3));

    if (data_len < 0) {
	setLastError(0, "Badly formed packet received - invalid length");
	return JDWPTRANSPORT_ERROR_IO_ERROR;
    } else if (data_len == 0) {
        packet->type.cmd.data = NULL;
    } else {
        packet->type.cmd.data= (*callback->alloc)(data_len);

        if (packet->type.cmd.data == NULL) {
            RETURN_ERROR(JDWPTRANSPORT_ERROR_OUT_OF_MEMORY, "out of memory");
	}

        n = recv_fully(socketFD,(char *)packet->type.cmd.data, data_len);
	if (n < data_len) {
            (*callback->free)(packet->type.cmd.data);
	    RETURN_RECV_ERROR(n);
        }
    }

    return JDWPTRANSPORT_ERROR_NONE;
}

static jdwpTransportError JNICALL
socketTransport_getLastError(jdwpTransportEnv* env, char** msgP) {
    char *msg = (char *)dbgsysTlsGet(tlsIndex);
    if (msg == NULL) {
	return JDWPTRANSPORT_ERROR_MSG_NOT_AVAILABLE;
    }
    *msgP = (*callback->alloc)(strlen(msg)+1);
    if (*msgP == NULL) {
	return JDWPTRANSPORT_ERROR_OUT_OF_MEMORY;
    }
    strcpy(*msgP, msg);
    return JDWPTRANSPORT_ERROR_NONE;  
}

JNIEXPORT jint JNICALL 
jdwpTransport_OnLoad(JavaVM *vm, jdwpTransportCallback* cbTablePtr,
		     jint version, jdwpTransportEnv** result)
{
    if (version != JDWPTRANSPORT_VERSION_1_0) {
	return JNI_EVERSION;
    }
    if (initialized) {
	/* 
	 * This library doesn't support multiple environments (yet)
	 */
	return JNI_EEXIST;
    }
    initialized = JNI_TRUE;
    jvm = vm;
    callback = cbTablePtr;

    /* initialize interface table */
    interface.GetCapabilities = &socketTransport_getCapabilities;
    interface.Attach = &socketTransport_attach;
    interface.StartListening = &socketTransport_startListening;
    interface.StopListening = &socketTransport_stopListening;
    interface.Accept = &socketTransport_accept;
    interface.IsOpen = &socketTransport_isOpen;
    interface.Close = &socketTransport_close;
    interface.ReadPacket = &socketTransport_readPacket;
    interface.WritePacket = &socketTransport_writePacket;
    interface.GetLastError = &socketTransport_getLastError;
    *result = &single_env;

    /* initialized TLS */
    tlsIndex = dbgsysTlsAlloc();
    return JNI_OK;
}


