/*
 * Copyright (c) 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 

import java.io.File;
import java.util.HashMap;
import javax.management.MBeanServer;
import javax.management.MBeanServerFactory;
import javax.management.remote.JMXConnectorServer;
import javax.management.remote.JMXConnectorServerFactory;
import javax.management.remote.JMXServiceURL;

public class Server {

    public static void main(String[] args) {
        try {
            // Instantiate the MBean server
            //
            System.out.println("\nCreate the MBean server");
            MBeanServer mbs = MBeanServerFactory.createMBeanServer();

            // Environment map
            //
            System.out.println("\nInitialize the environment map");
            HashMap env = new HashMap();

            // Provide the password file used by the connector server to
            // perform user authentication. The password file is a properties
            // based text file specifying username/password pairs. This
            // properties based password authenticator has been implemented
            // using the JMXAuthenticator interface and is passed to the
            // connector through the "jmx.remote.authenticator" property
            // in the map.
            //
            // This property is implementation-dependent and might not be
            // supported by all implementations of the JMX Remote API.
            //
            env.put("jmx.remote.x.password.file",
                    "config" + File.separator + "password.properties");

            // Provide the access level file used by the connector server to
            // perform user authorization. The access level file is a properties
            // based text file specifying username/access level pairs where
            // access level is either "readonly" or "readwrite" access to the
            // MBeanServer operations. This properties based access control
            // checker has been implemented using the MBeanServerForwarder
            // interface which wraps the real MBean server inside an access
            // controller MBean server which performs the access control checks
            // before forwarding the requests to the real MBean server.
            //
            // This property is implementation-dependent and might not be
            // supported by all implementations of the JMX Remote API.
            //
            env.put("jmx.remote.x.access.file",
                    "config" + File.separator + "access.properties");

            // Create an RMI connector server
            //
            System.out.println("\nCreate an RMI connector server");
            JMXServiceURL url = new JMXServiceURL(
              "service:jmx:rmi:///jndi/rmi://localhost:9999/server");
            JMXConnectorServer cs =
                JMXConnectorServerFactory.newJMXConnectorServer(url, env, mbs);

            // Start the RMI connector server
            //
            System.out.println("\nStart the RMI connector server");
            cs.start();
            System.out.println("\nRMI connector server successfully started");
            System.out.println("\nWaiting for incoming connections...");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
