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
import javax.security.sasl.*;
import javax.security.auth.callback.*;
import java.security.*;
import javax.security.auth.Subject;
import javax.security.auth.login.*;
import com.sun.security.auth.callback.*;
import java.util.HashMap;

/**
 * A sample client application that uses SASL to authenticate to
 * a server using Kerberos as the underlying mechanism. It then
 * exchanges data securely with the server.
 *
 * This sample program uses a ficticious application-level protocol.
 * Every message exchanged between the client and server an 8-byte
 * header that consists of two integers: the first integer represesents
 * the application-level command or status code while the second integer
 * indicates the length of the SASL buffer. This header is followed by
 * the SASL buffer.
 *
 * The protocol is:
 *    1.  Authentication
 *         a. client sends initial response to server containing authentication
 *            information
 *         b. server accepts and evaluates response to generate challenge; it
 *            sends the challenge to the server.
 *         c. client evaluates challenge to generate response; it sends the
 *            response;
 *         d. Steps b and c are repeated until authentication succeeds or fails.
 *    2. client sends an encrypted message to the server.
 *    3. server decryptes the message and sends an encrypted one back
 *       that contains the original message plus the current time.
 *
 * Start SaslTestServer first before starting SaslTestClient.
 *
 * Usage:  java <options> SaslTestClient service serverName
 *
 * Example: java -Djava.security.auth.login.config=jaas-krb5.conf \
 *               SaslTestClient host machine.imc.org
 *
 * Add -Djava.security.krb5.conf=krb5.conf to specify application-specific
 * Kerberos configuration (different from operating system's Kerberos
 * configuration).
 */

public class SaslTestClient {
    private static final String MECH = "GSSAPI"; // SASL name for GSS-API/Kerberos
    private static final int PORT = 4568;

    private static final byte[] EMPTY = new byte[0];

    public static void main(String[] args) throws Exception {
        // Obtain the command-line arguments and parse the server's principal

        if (args.length < 2) {
            System.err.println(
                "Usage: java <options> SaslTestClient <service> <serverName>");
            System.exit(-1);
        }

        PrivilegedExceptionAction action =
            new SaslClientAction(args[0], args[1], PORT);

        Jaas.loginAndAction("client", action);
    }

    static class SaslClientAction implements PrivilegedExceptionAction {
        private String service;      // used for SASL authentication
        private String serverName;   // name used for SASL authentication
        private int port;
        private CallbackHandler cbh = null; // Don't need handler for GSSAPI

        SaslClientAction(String service, String serverName, int port) {
            this.service = service;
            this.serverName = serverName;
            this.port = port;
        }

        public Object run() throws Exception {
            // Create application-level connection
            AppConnection conn = new AppConnection(serverName, port);

            HashMap<String,Object> props = new HashMap<String,Object>();
            // Request confidentiality
            props.put(Sasl.QOP, "auth-conf");

            // Create SaslClient to perform authentication
            SaslClient clnt = Sasl.createSaslClient(
                new String[]{MECH}, null, service, serverName, props, cbh);

            if (clnt == null) {
                throw new Exception(
                    "Unable to find client implementation for " + MECH);
            }

            byte[] response;
            byte[] challenge;

            // Get initial response for authentication
            response = clnt.hasInitialResponse() ?
                clnt.evaluateChallenge(EMPTY) : EMPTY;

            // Send initial response to server
            AppConnection.AppReply reply =
                conn.send(AppConnection.AUTH_CMD, response);

            // Repeat until authentication terminates
            while (!clnt.isComplete() &&
                (reply.getStatus() == AppConnection.AUTH_INPROGRESS ||
                 reply.getStatus() == AppConnection.SUCCESS)) {

                // Evaluate challenge to generate response
                challenge = reply.getBytes();
                response = clnt.evaluateChallenge(challenge);

                if (reply.getStatus() == AppConnection.SUCCESS) {
                    if (response != null) {
                        throw new Exception("Protocol error interacting with SASL");
                    }
                    break;
                }

                // Send response to server and read server's next challenge
                reply = conn.send(AppConnection.AUTH_CMD, response);
            }

            // Check status of authentication
            if (clnt.isComplete() && reply.getStatus() == AppConnection.SUCCESS) {
                System.out.println("Client authenticated.");
            } else {
                throw new Exception("Authentication failed: " +
                    " connection status? " + reply.getStatus());
            }

            String qop = (String) clnt.getNegotiatedProperty(Sasl.QOP);
            System.out.println("Negotiated QOP: " + qop);

            // Try out security layer
            boolean sl = (qop.equals("auth-conf") || qop.equals("auth-int"));

            byte[] msg = "Hello There!".getBytes("UTF-8");
            System.out.println("Sending: " + new String(msg, "UTF-8"));

            byte[] encrypted = (sl ? clnt.wrap(msg, 0, msg.length) : msg);

            reply = conn.send(AppConnection.DATA_CMD, encrypted);

            if (reply.getStatus() == AppConnection.SUCCESS) {
                byte[] encryptedReply = reply.getBytes();

                byte[] clearReply = (sl ? clnt.unwrap(encryptedReply,
                    0, encryptedReply.length) : encryptedReply);

                System.out.println("Received: " + new String(clearReply, "UTF-8"));
            } else {
                System.out.println("Failed exchange: " + reply.getStatus());
            }

            conn.close();

            return null;
        }
    }
}
