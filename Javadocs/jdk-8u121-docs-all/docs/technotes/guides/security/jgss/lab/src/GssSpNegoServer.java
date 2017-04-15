/*
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistributions of source code must retain the above copyright
 * notice, this  list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduct the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Oracle nor the names of
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY
 * EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY
 * DAMAGES OR LIABILITIES  SUFFERED BY LICENSEE AS A RESULT OF  OR
 * RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR
 * ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE
 * FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT,
 * SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF
 * THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed, licensed or
 * intended for use in the design, construction, operation or
 * maintenance of any nuclear facility.
 */

import org.ietf.jgss.*;
import java.io.*;
import java.net.Socket;
import java.net.ServerSocket;
import java.security.*;
import java.util.Date;

/**
 * A sample server application that uses JGSS to do mutual authentication
 * with a client using Kerberos as the underlying mechanism. It then
 * exchanges data securely with the client.
 *
 * Every message exchanged with the client includes a 4-byte application-
 * level header that contains the big-endian integer value for the number
 * of bytes that will follow as part of the JGSS token.
 *
 * The protocol is:
 *    1.  Context establishment loop:
 *         a. client sends init sec context token to server
 *         b. server sends accept sec context token to client
 *         ....
 *    2. client sends a wrap token to the server.
 *    3. server sends a wrap token back to the client.
 *
 * Start GssSpNegoServer first before starting GssClient.
 *
 * Usage:  java <options> GssSpNegoServer
 *
 * Example: java -Djava.security.auth.login.config=jaas-krb5.conf \
 *               GssSpNegoServer
 *
 * Add -Djava.security.krb5.conf=krb5.conf to specify application-specific
 * Kerberos configuration (different from operating system's Kerberos
 * configuration).
 */

public class GssSpNegoServer  {
    private static final int PORT = 4567;
    private static final boolean verbose = false;
    private static final int LOOP_LIMIT = 1;
    private static int loopCount = 0;

    public static void main(String[] args) throws Exception {

        PrivilegedExceptionAction action = new GssServerAction(PORT);

        Jaas.loginAndAction("server", action);
    }

    static class GssServerAction implements PrivilegedExceptionAction {
        private int localPort;

        GssServerAction(int port) {
            this.localPort = port;
        }

        public Object run() throws Exception {

            ServerSocket ss = new ServerSocket(localPort);

            // Get own Kerberos credentials for accepting connection
            GSSManager manager = GSSManager.getInstance();
            Oid spnegoOid = new Oid("1.3.6.1.5.5.2");
            GSSCredential serverCreds = manager.createCredential(null,
                                             GSSCredential.DEFAULT_LIFETIME,
                                             spnegoOid,
                                             GSSCredential.ACCEPT_ONLY);
            while (loopCount++ < LOOP_LIMIT) {

                System.out.println("Waiting for incoming connection...");

                Socket socket = ss.accept();
                DataInputStream inStream =
                    new DataInputStream(socket.getInputStream());

                DataOutputStream outStream =
                    new DataOutputStream(socket.getOutputStream());

                System.out.println("Got connection from client " +
                    socket.getInetAddress());

                /*
                 * Create a GSSContext to receive the incoming request
                 * from the client. Use null for the server credentials
                 * passed in. This tells the underlying mechanism
                 * to use whatever credentials it has available that
                 * can be used to accept this connection.
                 */

                GSSContext context = manager.createContext(
                    (GSSCredential)serverCreds);

                // Do the context establishment loop

                byte[] token = null;

                while (!context.isEstablished()) {

                    if (verbose) {
                        System.out.println("Reading ...");
                    }
                    token = new byte[inStream.readInt()];

                    if (verbose) {
                        System.out.println("Will read input token of size " +
                            token.length + " for processing by acceptSecContext");
                    }
                    inStream.readFully(token);

                    if (token.length == 0) {
                        if (verbose) {
                            System.out.println("skipping zero length token");
                        }
                        continue;
                    }
                    if (verbose) {
                        System.out.println("Token = " + getHexBytes(token));
                        System.out.println("acceptSecContext..");
                    }
                    token = context.acceptSecContext(token, 0, token.length);

                    // Send a token to the peer if one was generated by
                    // acceptSecContext
                    if (token != null) {
                        if (verbose) {
                            System.out.println("Will send token of size " +
                                token.length + " from acceptSecContext.");
                        }

                        outStream.writeInt(token.length);
                        outStream.write(token);
                        outStream.flush();
                    }
                }

                System.out.println("Context Established! ");
                System.out.println("Client principal is " + context.getSrcName());
                System.out.println("Server principal is " + context.getTargName());

                /*
                 * If mutual authentication did not take place, then
                 * only the client was authenticated to the
                 * server. Otherwise, both client and server were
                 * authenticated to each other.
                 */
                if (context.getMutualAuthState())
                    System.out.println("Mutual authentication took place!");

                /*
                 * Create a MessageProp which unwrap will use to return
                 * information such as the Quality-of-Protection that was
                 * applied to the wrapped token, whether or not it was
                 * encrypted, etc. Since the initial MessageProp values
                 * are ignored, just set them to the defaults of 0 and false.
                 */
                MessageProp prop = new MessageProp(0, false);

                /*
                 * Read the token. This uses the same token byte array
                 * as that used during context establishment.
                 */
                token = new byte[inStream.readInt()];
                if (verbose) {
                    System.out.println("Will read token of size " + token.length);
                }
                inStream.readFully(token);

                byte[] input = context.unwrap(token, 0, token.length, prop);
                String str = new String(input, "UTF-8");

                System.out.println("Received data \"" +
                    str + "\" of length " + str.length());

                System.out.println("Confidentiality applied: " +
                    prop.getPrivacy());

                /*
                 * Now generate reply that is the concatenation of the
                 * incoming string with the current time.
                 */

                /*
                 * First reset the QOP of the MessageProp to 0
                 * to ensure the default Quality-of-Protection
                 * is applied.
                 */
                prop.setQOP(0);

                String now = new Date().toString();
                byte[] nowBytes = now.getBytes("UTF-8");
                int len = input.length + 1 + nowBytes.length;
                byte[] reply = new byte[len];
                System.arraycopy(input, 0, reply, 0, input.length);
                reply[input.length] = ' ';
                System.arraycopy(nowBytes, 0, reply, input.length+1,
                    nowBytes.length);

                System.out.println("Sending: " + new String(reply, "UTF-8"));
                token = context.wrap(reply, 0, reply.length, prop);

                outStream.writeInt(token.length);
                outStream.write(token);
                outStream.flush();

                System.out.println("Closing connection with client " +
                    socket.getInetAddress());
                context.dispose();
                socket.close();
            }
            return null;
        }
    }

    private static final String getHexBytes(byte[] bytes, int pos, int len) {

        StringBuffer sb = new StringBuffer();
        for (int i = pos; i < (pos+len); i++) {

            int b1 = (bytes[i]>>4) & 0x0f;
            int b2 = bytes[i] & 0x0f;

            sb.append(Integer.toHexString(b1));
            sb.append(Integer.toHexString(b2));
            sb.append(' ');
        }
        return sb.toString();
    }

    private static final String getHexBytes(byte[] bytes) {
        return getHexBytes(bytes, 0, bytes.length);
    }
}
