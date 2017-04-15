/*
 *
 * Copyright (c) 1994, 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistribution of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistribution in binary form must reproduce the above copyright
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
 * EXCLUDED. SUN MICROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL
 * NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT
 * OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR
 * ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT,
 * SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF
 * THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF SUN HAS
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that this software is not designed, licensed or
 * intended for use in the design, construction, operation or
 * maintenance of any nuclear facility.
 */

import java.net.*;
import java.io.*;
import javax.net.ssl.*;

/*
 * This example illustrates how to do proxy Tunneling to access a
 * secure web server from behind a firewall.
 *
 * Please set the following Java system properties
 * to the appropriate values:
 *
 *   https.proxyHost = <secure proxy server hostname>
 *   https.proxyPort = <secure proxy server port>
 */

public class SSLSocketClientWithTunneling {

    public static void main(String[] args) throws Exception {
        new SSLSocketClientWithTunneling().doIt("www.verisign.com", 443);
    }

    String tunnelHost;
    int tunnelPort;

    public void doIt(String host, int port) {
        try {

            /*
             * Let's setup the SSLContext first, as there's a lot of
             * computations to be done.  If the socket were created
             * before the SSLContext, the server/proxy might timeout
             * waiting for the client to actually send something.
             */
            SSLSocketFactory factory =
                (SSLSocketFactory)SSLSocketFactory.getDefault();

            /*
             * Set up a socket to do tunneling through the proxy.
             * Start it off as a regular socket, then layer SSL
             * over the top of it.
             */
            tunnelHost = System.getProperty("https.proxyHost");
            tunnelPort = Integer.getInteger("https.proxyPort").intValue();

            Socket tunnel = new Socket(tunnelHost, tunnelPort);
            doTunnelHandshake(tunnel, host, port);

            /*
             * Ok, let's overlay the tunnel socket with SSL.
             */
            SSLSocket socket =
                (SSLSocket)factory.createSocket(tunnel, host, port, true);

            /*
             * register a callback for handshaking completion event
             */
            socket.addHandshakeCompletedListener(
                new HandshakeCompletedListener() {
                    public void handshakeCompleted(
                            HandshakeCompletedEvent event) {
                        System.out.println("Handshake finished!");
                        System.out.println(
                            "\t CipherSuite:" + event.getCipherSuite());
                        System.out.println(
                            "\t SessionId " + event.getSession());
                        System.out.println(
                            "\t PeerHost " + event.getSession().getPeerHost());
                    }
                }
            );

            /*
             * send http request
             *
             * See SSLSocketClient.java for more information about why
             * there is a forced handshake here when using PrintWriters.
             */
            socket.startHandshake();

            PrintWriter out = new PrintWriter(
                                  new BufferedWriter(
                                  new OutputStreamWriter(
                                  socket.getOutputStream())));

            out.println("GET / HTTP/1.0");
            out.println();
            out.flush();

            /*
             * Make sure there were no surprises
             */
            if (out.checkError())
                System.out.println(
                    "SSLSocketClient:  java.io.PrintWriter error");

            /* read response */
            BufferedReader in = new BufferedReader(
                                    new InputStreamReader(
                                    socket.getInputStream()));

            String inputLine;

            while ((inputLine = in.readLine()) != null)
                System.out.println(inputLine);

            in.close();
            out.close();
            socket.close();
            tunnel.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /*
     * Tell our tunnel where we want to CONNECT, and look for the
     * right reply.  Throw IOException if anything goes wrong.
     */
    private void doTunnelHandshake(Socket tunnel, String host, int port)
    throws IOException
    {
        OutputStream out = tunnel.getOutputStream();
        String msg = "CONNECT " + host + ":" + port + " HTTP/1.0\n"
                     + "User-Agent: "
                     + sun.net.www.protocol.http.HttpURLConnection.userAgent
                     + "\r\n\r\n";
        byte b[];
        try {
            /*
             * We really do want ASCII7 -- the http protocol doesn't change
             * with locale.
             */
            b = msg.getBytes("ASCII7");
        } catch (UnsupportedEncodingException ignored) {
            /*
             * If ASCII7 isn't there, something serious is wrong, but
             * Paranoia Is Good (tm)
             */
            b = msg.getBytes();
        }
        out.write(b);
        out.flush();

        /*
         * We need to store the reply so we can create a detailed
         * error message to the user.
         */
        byte            reply[] = new byte[200];
        int             replyLen = 0;
        int             newlinesSeen = 0;
        boolean         headerDone = false;     /* Done on first newline */

        InputStream     in = tunnel.getInputStream();
        boolean         error = false;

        while (newlinesSeen < 2) {
            int i = in.read();
            if (i < 0) {
                throw new IOException("Unexpected EOF from proxy");
            }
            if (i == '\n') {
                headerDone = true;
                ++newlinesSeen;
            } else if (i != '\r') {
                newlinesSeen = 0;
                if (!headerDone && replyLen < reply.length) {
                    reply[replyLen++] = (byte) i;
                }
            }
        }

        /*
         * Converting the byte array to a string is slightly wasteful
         * in the case where the connection was successful, but it's
         * insignificant compared to the network overhead.
         */
        String replyStr;
        try {
            replyStr = new String(reply, 0, replyLen, "ASCII7");
        } catch (UnsupportedEncodingException ignored) {
            replyStr = new String(reply, 0, replyLen);
        }

        /* We asked for HTTP/1.0, so we should get that back */
        if (!replyStr.startsWith("HTTP/1.0 200")) {
            throw new IOException("Unable to tunnel through "
                    + tunnelHost + ":" + tunnelPort
                    + ".  Proxy returns \"" + replyStr + "\"");
        }

        /* tunneling Handshake was successful! */
    }
}
