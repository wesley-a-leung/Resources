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

import java.util.Collections;
import java.util.HashMap;
import javax.management.Attribute;
import javax.management.JMX;
import javax.management.MBeanServerConnection;
import javax.management.ObjectName;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXPrincipal;
import javax.management.remote.JMXServiceURL;
import javax.security.auth.Subject;

public class Client {

    public static void main(String[] args) {
        try {
            // Environment map
            //
            System.out.println("\nInitialize the environment map");
            HashMap env = new HashMap();

            // Provide the credentials required by the server to successfully
            // perform user authentication
            //
            String[] credentials = new String[] { "username" , "password" };
            env.put("jmx.remote.credentials", credentials);

            // Create an RMI connector client and
            // connect it to the RMI connector server
            //
            System.out.println("\nCreate an RMI connector client and " +
                               "connect it to the RMI connector server");
            JMXServiceURL url = new JMXServiceURL(
              "service:jmx:rmi:///jndi/rmi://localhost:9999/server");
            JMXConnector jmxc = JMXConnectorFactory.connect(url, env);

            // Create the delegation subject and retrieve an
            // MBeanServerConnection that uses that subject
            // when performing the operations on the remote
            // MBean server
            //
            // The connector server will check that the authentication identity
            // "username" has the right to execute operations on behalf of the
            // given authorization identity "delegate", i.e. the policy file
            // must contain the following grant clause:
            //
            // grant principal javax.management.remote.JMXPrincipal "username" {
            //   permission javax.management.remote.SubjectDelegationPermission
            //                  "javax.management.remote.JMXPrincipal.delegate";
            // };
            //
            System.out.println("\nCreate the delegation subject");
            Subject delegationSubject =
                new Subject(true,
                            Collections.singleton(new JMXPrincipal("delegate")),
                            Collections.EMPTY_SET,
                            Collections.EMPTY_SET);

            // Get an MBeanServerConnection for the given delegation subject
            //
            System.out.println("\nGet an MBeanServerConnection " +
                               "for the given delegation subject");
            MBeanServerConnection mbsc =
                jmxc.getMBeanServerConnection(delegationSubject);

            // Get domains from MBeanServer
            //
            System.out.println("\nDomains:");
            String domains[] = mbsc.getDomains();
            for (int i = 0; i < domains.length; i++) {
                System.out.println("\tDomain[" + i + "] = " + domains[i]);
            }

            // Create SimpleStandard MBean
            //
            ObjectName mbeanName = new ObjectName("MBeans:type=SimpleStandard");
            System.out.println("\nCreate SimpleStandard MBean...");
            mbsc.createMBean("SimpleStandard", mbeanName, null, null);

            // Get MBean count
            //
            System.out.println("\nMBean count = " + mbsc.getMBeanCount());

            // Get State attribute
            //
            System.out.println("\nState = " +
                               mbsc.getAttribute(mbeanName, "State"));

            // Set State attribute
            //
            mbsc.setAttribute(mbeanName,
                              new Attribute("State", "changed state"));

            // Get State attribute
            //
            // Another way of interacting with a given MBean is through a
            // dedicated proxy instead of going directly through the MBean
            // server connection
            //
            SimpleStandardMBean proxy = JMX.newMBeanProxy(
                    mbsc, mbeanName, SimpleStandardMBean.class);
            System.out.println("\nState = " + proxy.getState());

            // Add notification listener on SimpleStandard MBean
            //
            ClientListener listener = new ClientListener();
            System.out.println("\nAdd notification listener...");
            mbsc.addNotificationListener(mbeanName, listener, null, null);

            // Invoke "reset" in SimpleStandard MBean
            //
            // Calling "reset" makes the SimpleStandard MBean emit a
            // notification that will be received by the registered
            // ClientListener.
            //
            System.out.println("\nInvoke reset() in SimpleStandard MBean...");
            mbsc.invoke(mbeanName, "reset", null, null);

            // Sleep for 2 seconds in order to have time to receive the
            // notification before removing the notification listener.
            //
            System.out.println("\nWaiting for notification...");
            Thread.sleep(2000);

            // Remove notification listener on SimpleStandard MBean
            //
            System.out.println("\nRemove notification listener...");
            mbsc.removeNotificationListener(mbeanName, listener);

            // Unregister SimpleStandard MBean
            //
            System.out.println("\nUnregister SimpleStandard MBean...");
            mbsc.unregisterMBean(mbeanName);

            // Close MBeanServer connection
            //
            System.out.println("\nClose the connection to the server");
            jmxc.close();
            System.out.println("\nBye! Bye!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
