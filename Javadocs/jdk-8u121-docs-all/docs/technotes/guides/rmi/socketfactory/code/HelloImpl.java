/*
 * Copyright (c) 2002, Oracle and/or its affiliates. All rights reserved.
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
 * EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY
 * DAMAGES OR LIABILITIES  SUFFERED BY LICENSEE AS A RESULT OF OR
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

package examples.rmisocfac;

import java.io.*;
import java.rmi.*;
import java.rmi.server.*;
import java.rmi.registry.*;

public class HelloImpl implements Hello {

    /*
     * Constructs a HelloImpl remote object.
     */
    public HelloImpl() {
    }

    /*
     * Returns the string "Hello World!".
     */
    public String sayHello() {
        return  "Hello World!";
    }

    public static void main(String args[]) {

        /*
         * Create and install a security manager
         */
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        byte pattern = (byte) 0xAC;
        try {
            /*
             * Create remote object and export it to use
             * custom socket factories.
             */
            HelloImpl obj = new HelloImpl();
            RMIClientSocketFactory csf = new XorClientSocketFactory(pattern);
            RMIServerSocketFactory ssf = new XorServerSocketFactory(pattern);
            Hello stub =
                (Hello) UnicastRemoteObject.exportObject(obj, 0, csf, ssf);

            /*
             * Create a registry and bind stub in registry.
             */
            LocateRegistry.createRegistry(2002);
            Registry registry = LocateRegistry.getRegistry(2002);
            registry.rebind("Hello", stub);
            System.out.println("HelloImpl bound in registry");

        } catch (Exception e) {
            System.out.println("HelloImpl exception: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
