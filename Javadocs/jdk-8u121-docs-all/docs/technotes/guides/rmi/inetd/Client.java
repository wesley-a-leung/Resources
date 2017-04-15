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

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

/**
 * A client that looks up a service by name in a registry on the port,
 * supplied as a command-line argument, and invokes a method on the
 * service.
 *
 * <p>The service may be configured to be launched by <code>inetd</code> so
 * executing this client may start the service running.
 */
public class Client {

    public static void main(String[] args) throws Exception {

        int port = 0;
        String host = "";
        String message = "";

        /*
         * Parse port argument.
         */
        if (args.length > 2) {
            host = args[0];

            try {
                port = Integer.parseInt(args[1]);
                if (port == 0) {
                    goodbye("nonzero port argument required", null);
                }

            } catch (NumberFormatException e) {
                goodbye("malformed port argument", e);

            }
            message = args[2];

        } else {
            usage();
        }

        /*
         * Lookup service by name in the registry on the specified host and
         * port.
         */
        Registry registry = LocateRegistry.getRegistry(host, port);
        ServiceInterface proxy =
            (ServiceInterface) registry.lookup("ServiceInterface");

        /*
         * Invoke the 'sendMessage' method on the service passing the
         * specified message.
         */
        System.out.println("sending message: " + message);
        System.out.println("received message from proxy: " +
                           proxy.sendMessage(message));

        System.out.println("done.");
    }

    /**
     * Prints message and exception (if any), then exits.
     */
    private static void goodbye(String message, Exception e) {
        System.err.println("Client: " + message + (e != null ? ": " : ""));
        if (e != null) {
            e.printStackTrace();
        }
        System.exit(1);
    }

    /**
     * Prints usage message, then exits.
     */
    private static void usage() {
        System.err.println("Client <host> <port> <message>");
        System.exit(1);
    }
}
