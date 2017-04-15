/*
 * Copyright (c) 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the
 *  distribution.
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
 * NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF
 * USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR
 * ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT,
 * SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF
 * THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF SUN HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed, licensed or
 * intended for use in the design, construction, operation or
 * maintenance of any nuclear facility.
 */
package example.inetd;

import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;


/**
 * Used to launch a server from <code>inetd</code>.  The <code>main</code>
 * method of this server implementation exports the service object (an
 * instance of <code>ServiceInterface</code>) and then uses the utility
 * method <code>InitializeRegistry.initializeWithInheritedChannel</code> to
 * create and export a registry using an inherited channel (if any) and
 * to bind a name to the service proxy in that registry for clients to look
 * up.
 *
 * <p>This program, when configured to be started from <code>inetd</code>,
 * will start the server when a client attempts to look up the server by
 * name in the registry on the port configured with <code>inetd</code>.
 *
 * <p>To configure <code>inetd</code> to launch this program, the following
 * entry should be added to the configuration file
 * <code>/etc/inetd.conf</code>:
 *
 * <pre>
 *    example-server stream tcp wait nobody <b><i>jrehome</i></b>/bin/java \
 *        java -classpath <b><i>classpath</i></b> example.inetd.Server
 * </pre>
 * where <b><i>jrehome</i></b> is the path to the installed JRE, and
 * <b><i>classpath</i></b> is the class path for the example.
 *
 * <p>Additionally, the <code>example-server</code> name needs to be
 * configured as a service in the <code>/etc/services</code> file as
 * follows:
 *
 * <pre>
 *    example-server    <b><i>port</i></b>/tcp
 * </pre>
 * where <b><i>port</i></b> is the port the registry will accept requests
 * on.  Note that the service name--
 * here we used <code>example-server</code>-- can be another name, as long
 * as the chosen name it is not already configured in /etc/services.
 *
 * <p>This program also supports being run from the command line.  In this
 * case, there is no inherited channel, and the program will simply
 * create/export a remote service, create/export a registry on the port
 * number supplied as the command line argument, and then bind the
 * service's proxy in the registry for clients to look up.

 * <p>This program can be run from the command line as follows:
 *
 * <pre>
 *     java example.inetd.Server <b><i>port</i></b>
 * </pre>
 * where <b><i>port</i></b> is a nonzero port that the registry accepts
 * requests on.
 **/
public class Server implements ServiceInterface {

    public Server() {}

    public String sendMessage(String message) {
        System.out.println(message);
        return "received message: " + message;
    }

    public static void main(String[] args) {

        /*
         * Redirect System.err output to a file.
         */
        redirectErrorOutput();

        /*
         * Parse port argument.
         */
        int port = 0;
        if (args.length > 0) {
            try {
                port = Integer.parseInt(args[0]);

            } catch (NumberFormatException e) {
                goodbye("malformed port argument", e);
            }
        }

        /*
         * Create and export service, and then bind in local registry using
         * inherited channel, if any.
         */
        ServiceInterface server = new Server();
        Remote proxy = null;
        try {
            /*
             * Use two argument exportObject method to make sure that
             * dynamic proxy can be used.
             */
            proxy = UnicastRemoteObject.exportObject(server, 0);

        } catch (RemoteException e) {
            goodbye("exception exporting service", e);
        }

        try {
            InitializeRegistry.initializeWithInheritedChannel(
                proxy, "ServiceInterface", port);

        } catch (IllegalArgumentException e) {
            goodbye("invalid port argument", e);
        } catch (Exception e) {
            goodbye("exception occurred", e);
        }

        System.err.println("ready");

        /*
         * Wait forever.
         */
        try {
            Thread.sleep(Long.MAX_VALUE);
        } catch (InterruptedException e) {
            throw new AssertionError(e);
        }
    }

    /**
     * Redirects System.err output to a file.
     */
    private static void redirectErrorOutput() {
        try {
            File file = File.createTempFile("example-server-err", null, null);
            PrintStream errStream =
                new PrintStream(new FileOutputStream(file));
            System.setErr(errStream);

        } catch (IOException e) {
            goodbye("exception setting up error output file", e);
        }
    }

    /**
     * Prints message and exception, then exits.
     */
    private static void goodbye(String message, Exception e) {
        System.err.println("Server: " + message + ": ");
        e.printStackTrace();
        System.exit(1);
    }
}
