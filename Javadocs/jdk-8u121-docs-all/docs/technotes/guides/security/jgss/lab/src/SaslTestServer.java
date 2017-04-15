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
import java.util.HashMap;
import java.net.*;
import java.util.Date;

/**
 * A sample server application that uses SASL to authenticate clients
 * using Kerberos as the underlying mechanism. It then
 * exchanges data securely with the client.
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
 * Usage:  java <options> SaslTestServer service serverName
 *
 * Example: java -Djava.security.auth.login.config=jaas-krb5.conf \
 *               SaslTestServer host machine.imc.org
 *
 * Add -Djava.security.krb5.conf=krb5.conf to specify application-specific
 * Kerberos configuration (different from operating system's Kerberos
 * configuration).
 */


public class SaslTestServer {
    private static final String MECH = "GSSAPI"; // SASL name for GSS-API/Kerberos
    private static final int PORT = 4568;
    private static final int LOOP_LIMIT = 1;
    private static int loopCount = 0;

    public static void main(String[] args) throws Exception {
        // Obtain the command-line arguments and parse the server's principal

        if (args.length < 2) {
            System.err.println(
                "Usage: java <options> SaslTestServer <service> <host>");
            System.exit(-1);
        }

        PrivilegedExceptionAction action =
            new SaslServerAction(args[0], args[1], PORT);

        Jaas.loginAndAction("server", action);
    }

    static class SaslServerAction implements PrivilegedExceptionAction {
        private String service;      // used for SASL authentication
        private String serverName;   // named used for SASL authentication
        private int localPort;
        private CallbackHandler cbh = new TestCallbackHandler();

        SaslServerAction(String service, String serverName, int port) {
            this.service = service;
            this.serverName = serverName;
            this.localPort = port;
        }

        public Object run() throws Exception {
            ServerSocket ss = new ServerSocket(localPort);

            HashMap<String,Object> props = new HashMap<String,Object>();
            props.put(Sasl.QOP, "auth-conf,auth-int,auth");

            // Loop, accepting requests from any client
            while (loopCount++ < LOOP_LIMIT) {
                System.out.println("Waiting for incoming connection...");
                Socket socket = ss.accept();

                // Create application-level connection to handle request
                AppConnection conn = new AppConnection(socket);

                // Normally, the application protocol will negotiate which
                // SASL mechanism to use. In this simplified example, we
                // will always use "GSSAPI", the name of the mechanism that does
                // Kerberos via GSS-API

                // Create SaslServer to perform authentication
                SaslServer srv = Sasl.createSaslServer(MECH,
                    service, serverName, props, cbh);

                if (srv == null) {
                    throw new Exception(
                        "Unable to find server implementation for " + MECH);
                }

                boolean auth = false;

                // Read initial response from client
                byte[] response = conn.receive(AppConnection.AUTH_CMD);
                AppConnection.AppReply clientMsg;

                while (!srv.isComplete()) {
                    try {
                        // Generate challenge based on response
                        byte[] challenge = srv.evaluateResponse(response);

                        if (srv.isComplete()) {
                            conn.send(AppConnection.SUCCESS, challenge);
                            auth = true;
                        } else {
                            clientMsg = conn.send(AppConnection.AUTH_INPROGRESS,
                                challenge);
                            response = clientMsg.getBytes();
                        }
                    } catch (SaslException e) {
                        // e.printStackTrace();
                        // Send failure notification to client
                        conn.send(AppConnection.FAILURE, null);
                        break;
                    }
                }

                // Check status of authentication
                if (srv.isComplete() && auth) {
                    System.out.print("Client authenticated; ");
                    System.out.println("authorized client is: " +
                        srv.getAuthorizationID());
                } else {
                    // Go get another client
                    System.out.println("Authentication failed. ");
                    continue;
                }

                String qop = (String) srv.getNegotiatedProperty(Sasl.QOP);
                System.out.println("Negotiated QOP: " + qop);

                // Now try to use security layer
                boolean sl = (qop.equals("auth-conf") || qop.equals("auth-int"));

                byte[] msg = conn.receive(AppConnection.DATA_CMD);
                byte[] realMsg = (sl ? srv.unwrap(msg, 0, msg.length) : msg);

                System.out.println("Received: " + new String(realMsg, "UTF-8"));

                // Construct reply to send to client
                String now = new Date().toString();
                byte[] nowBytes = now.getBytes("UTF-8");
                int len = realMsg.length + 1 + nowBytes.length;
                byte[] reply = new byte[len];
                System.arraycopy(realMsg, 0, reply, 0, realMsg.length);
                reply[realMsg.length] = ' ';
                System.arraycopy(nowBytes, 0, reply, realMsg.length+1,
                    nowBytes.length);

                System.out.println("Sending: " + new String(reply, "UTF-8"));

                byte[] realReply = (sl ? srv.wrap(reply, 0, reply.length) : reply);

                conn.send(AppConnection.SUCCESS, realReply);
            }
            return null;
        }
    }

    static class TestCallbackHandler implements CallbackHandler {

        public void handle(Callback[] callbacks)
            throws UnsupportedCallbackException {

            AuthorizeCallback acb = null;

            for (int i = 0; i < callbacks.length; i++) {
                if (callbacks[i] instanceof AuthorizeCallback) {
                    acb = (AuthorizeCallback) callbacks[i];
                } else {
                    throw new UnsupportedCallbackException(callbacks[i]);
                }
            }

            if (acb != null) {
                String authid = acb.getAuthenticationID();
                String authzid = acb.getAuthorizationID();
                if (authid.equals(authzid)) {
                    // Self is always authorized
                    acb.setAuthorized(true);

                } else {
                    // Should check some database for mapping and decide.
                    // Current simplified policy is to reject authzids that
                    // don't match authid

                    acb.setAuthorized(false);
                }

                if (acb.isAuthorized()) {
                    // Set canonicalized name.
                    // Should look up database for canonical names

                    acb.setAuthorizedID(authzid);
                }
            }
        }
    }
}
