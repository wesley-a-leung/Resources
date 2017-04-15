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

import java.io.*;
import java.net.*;
import javax.net.ssl.*;
import java.util.Date;
import java.security.PrivilegedExceptionAction;
import java.security.Principal;

/*
 * Tests support for RFC 2712. Specify use of only a KRB5 cipher for both
 * client and server, by first doing a JAAS login for the server
 * without first doing a JAAS login for the client.
 */

public class JsseServer {

    private static final String KRB5_CIPHER = "TLS_KRB5_WITH_3DES_EDE_CBC_SHA";


    private static final int PORT = 4569;
    private static final boolean verbose = false;
    private static final int LOOP_LIMIT = 1;
    private static int loopCount = 0;

    public static void main(String[] args) throws Exception {

        PrivilegedExceptionAction action = new JsseServerAction(PORT);

        Jaas.loginAndAction("server", action);
    }

    static class JsseServerAction implements PrivilegedExceptionAction {
        private int localPort;

        JsseServerAction(int port) {
            this.localPort = port;
        }

        public Object run() throws Exception {

            SSLServerSocketFactory sslssf =
                (SSLServerSocketFactory) SSLServerSocketFactory.getDefault();
            SSLServerSocket sslServerSocket =
                (SSLServerSocket) sslssf.createServerSocket(localPort);

            // Enable only a KRB5 cipher suite.
            String enabledSuites[] = { KRB5_CIPHER };
            sslServerSocket.setEnabledCipherSuites(enabledSuites);
            // Should check for exception if enabledSuites is not supported

            while (loopCount++ < LOOP_LIMIT) {
                System.out.println("Waiting for incoming connection...");

                SSLSocket sslSocket = (SSLSocket) sslServerSocket.accept();

                System.out.println("Got connection from client " +
                    sslSocket.getInetAddress());

                BufferedReader in = new BufferedReader(new InputStreamReader(
                    sslSocket.getInputStream()));
                BufferedWriter out = new BufferedWriter(new OutputStreamWriter(
                    sslSocket.getOutputStream()));

                String inStr = in.readLine();
                System.out.println("Received " + inStr);

                String outStr = inStr + " " + new Date().toString() + "\n";
                out.write(outStr);
                System.out.println("Sending " + outStr);
                out.flush();

                String cipherSuiteChosen = sslSocket.getSession().getCipherSuite();
                System.out.println("Cipher suite in use: " + cipherSuiteChosen);
                Principal self = sslSocket.getSession().getLocalPrincipal();
                System.out.println("I am: " + self.toString());
                Principal peer = sslSocket.getSession().getPeerPrincipal();
                System.out.println("Client is: " + peer.toString());

                sslSocket.close();
            }
            return null;
        }
    }
}
