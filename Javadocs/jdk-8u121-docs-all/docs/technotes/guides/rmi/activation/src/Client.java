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
package examples.activation;

import java.rmi.*;
import java.rmi.registry.*;

/**
 * This program looks up a remote object's stub (that implements the remote
 * interface <code>MyRemoteInterface</code>) in the registry on the host
 * supplied as the optional first argument, and then invokes the stub's
 * <code>remoteMethod</code> method.  When this client invokes a remote method
 * on the stub acquired from the registry, the remote object will activate
 * if not already active.
 *
 * <p>This program should be run as follows:
 * <pre>
 *      java -Djava.security.policy=<i><b>client.policy</b></i>        \
 *          -Dexamples.activation.client.codebase=<i><b>clientCodebase</b></i>      \
 *          -Dexamples.activation.name=<i><b>name</b></i>             \
 *          examples.activation.Client [<i><b>host</b></i>]
 * </pre>
 *
 * <p>where:<ul>
 * <li><i><b>client.policy</b></i> is the security policy file for this
 * program,
 * <li><i><b>clientCodebase</b></i> is the root directory for this program's
 * class (used in granting permissions to this program in the
 * <i><b>client.policy</b></i> file)
 * <li><i><b>name</b></i> is the name for the object's stub
 * in the registry
 * <li><i><b>host</b></i> is the server's host
 * </ul>
 *
 * <p>Note: <code>rmid</code> must be running on its default port,
 * and <code>rmiregistry</code> must be running on its default port
 * (both on the remote host) prior to running this program.
 **/
public class Client {

    public static void main(String args[])  throws Exception {

        /*
         * Obtain hostname supplied (optionally) as the first argument on
         * the command line.
         */
        String hostname = "localhost";
        if (args.length < 1) {
            System.err.println(
                "usage: java [options] examples.activation.Client <hostname>");
            System.exit(1);
        } else {
            hostname = args[0];
        }

        /*
         * Set a security manager, if one is not already set.
         */
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        /*
         * Obtain the stub for the activatable object from the
         * rmiregistry on port 1099 of the supplied host.  The name
         * to look up is specified by the system property
         * examples.activation.name.
         */
        String name = System.getProperty("examples.activation.name");
        Registry registry = LocateRegistry.getRegistry(hostname);
        MyRemoteInterface stub =
            (MyRemoteInterface) registry.lookup(name);
        System.err.println("Obtained stub from the registry.");

        /*
         * Invoke the remote method using the stub.
         */
        System.err.println("Invoking remote method...");

        String result = (String) stub.remoteMethod("hello there!");
        System.err.println("Returned from remote call.");
        System.err.println("Result: " + result);
    }
}
