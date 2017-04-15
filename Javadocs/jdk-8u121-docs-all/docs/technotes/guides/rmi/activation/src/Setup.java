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
import java.rmi.activation.*;
import java.rmi.registry.*;
import java.util.Properties;

/**
 * This program registers information about a remote object
 * with the activation system (<code>rmid</code>), and then binds a stub
 * for the remote object in the <code>rmiregistry</code> so that clients
 * can look it up.  The remote object's implementation class must be
 * supplied on the command line. When a client invokes a remote method on
 * the stub acquired from the registry, the remote object will activate.
 *
 * <p>This program should be run as follows:
 * <pre>
 *      java -Djava.security.policy=<i><b>setup.policy</b></i>        \
 *          -Djava.rmi.server.codebase=<i><b>codebase</b></i>         \
 *          -Dexamples.activation.setup.codebase=<i><b>setupCodebase</b></i>  \
 *          -Dexamples.activation.impl.codebase=<i><b>implCodebase</b></i> \
 *          -Dexamples.activation.name=<i><b>name</b></i>             \
 *          [-Dexamples.activation.file=<i><b>filename</b></i>]       \
 *          [-Dexamples.activation.policy=<i><b>group.policy</b></i>] \
 *          examples.activation.Setup <i><b>implClass</b></i>
 * </pre>
 *
 * <p>where:<ul>
 * <li><i><b>setup.policy</b></i> is the security policy file for the
 * <code>Setup</code> program,
 * <li><i><b>codebase</b></i> is the location (URL) of the remote interface
 * class(es)
 * <li><i><b>setupCodebase</b></i> is the root directory for the Setup program
 * class (used in granting permissions to the Setup program in the
 * <i><b>setup.policy</b></i> file)
 * <li><i><b>implCodebase</b></i> is the location (URL) of the implementation
 * class(es), used in the activation descriptor and also used in the group
 * policy file as the codebase to grant permissions to
 * <li><i><b>name</b></i> is the name for the object's stub
 * to be bound in the registry
 * <li><i><b>file</b></i> (optional) is the name of a file containing
 * the object's persistent state which is registered as the "data" in the
 * object's activation descriptor (no default),
 * <li><i><b>group.policy</b></i> (optional) is the security policy
 * file for the activation group (defaults to "group.policy"), and
 * <li><i><b>implClass</b></i> is the fully-qualified name of the
 * implementation class.
 * </ul>
 *
 * <p>Note: <code>rmid</code> must be running on its default port, and
 * <code>rmiregistry</code> must be running on its default port prior to
 * running this program.
 **/
public class Setup {

    /**
     * Prevents instantiation.
     */
    private Setup() {}

    public static void main(String[] args) throws Exception {

        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        /*
         * Get the impl class argument from the command line.
         */
        String implClass = "";
        if (args.length < 1) {
            System.err.println(
                "usage: java [options] examples.activation.Setup <implClass>");
            System.exit(1);
        } else {
            implClass = args[0];
        }

        /*
         * Construct an activation group descriptor.
         *
         * First, create a properties map containing overrides for system
         * properties in the activation group VM.  Add a value for the
         * "java.security.policy" system property to the properties map.
         * The value for the security policy file is specified by the
         * "examples.activation.policy" system property which defaults to
         * the file named "group.policy" in the working directory.
         *
         * Also add the value for the "examples.activation.impl.codebase" system
         * property to the map, which is specified by the
         * "examples.activation.impl.codebase" system property.  This property
         * is used in the group's policy file.
         **/
        String policy =
            System.getProperty("examples.activation.policy", "group.policy");
        String implCodebase =
            System.getProperty("examples.activation.impl.codebase");
        String filename =
            System.getProperty("examples.activation.file", "");

        Properties props = new Properties();
        props.put("java.security.policy", policy);
        props.put("java.class.path", "no_classpath");
        props.put("examples.activation.impl.codebase", implCodebase);
        if (filename != null && !filename.equals("")) {
            props.put("examples.activation.file", filename);
        }

        ActivationGroupDesc groupDesc = new ActivationGroupDesc(props, null);

        /*
         * Register the activation group descriptor with the activation
         * system to obtain a group ID.
         */
        ActivationGroupID groupID =
           ActivationGroup.getSystem().registerGroup(groupDesc);
        System.err.println("Activation group descriptor registered.");

        /*
         * Construct an activation descriptor for the activatable object.
         *
         * The location specifies a URL from where the implementation class
         * can be loaded when this object is activated.  The value of the
         * URL is specified by the "examples.activation.impl.codebase" system
         * property.
         *
         * The data (optional) specifies initialization data passed to the
         * remote object when being constructed as part of the activation
         * process. Here, the value of "data" is specified by the
         * "examples.activation.filename" system property and represents
         * the name of a file containing the object's persistent state (to
         * be read in when the object is activated).
         */
        MarshalledObject data = null;
        if (filename != null && !filename.equals("")) {
            data = new MarshalledObject(filename);
        }

        ActivationDesc desc =
            new ActivationDesc(groupID, implClass, implCodebase, data);

        /*
         * Register the activation descriptor with the activation system.
         * Use the convenience method 'Activatable.register' which returns
         * a constructed stub for the activatable object after registering
         * the activation descriptor.
         */
        Remote stub = Activatable.register(desc);
        System.err.println("Activation descriptor registered.");

        /*
         * Bind the activatable object's stub to a name in the registry
         * running on port 1099.  The name is specified by the system
         * property "examples.activation.name".
         */
        String name = System.getProperty("examples.activation.name");
        LocateRegistry.getRegistry().rebind(name, stub);
        System.err.println("Stub bound in registry.");
    }
}
