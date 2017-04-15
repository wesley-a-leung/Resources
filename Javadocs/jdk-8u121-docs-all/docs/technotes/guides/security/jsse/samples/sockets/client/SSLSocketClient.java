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
 * This example demostrates how to use a SSLSocket as client to
 * send a HTTP request and get response from an HTTPS server.
 * It assumes that the client is not behind a firewall
 */

public class SSLSocketClient {

    public static void main(String[] args) throws Exception {
        try {
            SSLSocketFactory factory =
                (SSLSocketFactory)SSLSocketFactory.getDefault();
            SSLSocket socket =
                (SSLSocket)factory.createSocket("www.verisign.com", 443);

            /*
             * send http request
             *
             * Before any application data is sent or received, the
             * SSL socket will do SSL handshaking first to set up
             * the security attributes.
             *
             * SSL handshaking can be initiated by either flushing data
             * down the pipe, or by starting the handshaking by hand.
             *
             * Handshaking is started manually in this example because
             * PrintWriter catches all IOExceptions (including
             * SSLExceptions), sets an internal error flag, and then
             * returns without rethrowing the exception.
             *
             * Unfortunately, this means any error messages are lost,
             * which caused lots of confusion for others using this
             * code.  The only way to tell there was an error is to call
             * PrintWriter.checkError().
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

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
