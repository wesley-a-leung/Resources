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

import java.io.IOException;
import java.util.Iterator;
import java.util.Set;
import javax.management.Attribute;
import javax.management.JMX;
import javax.management.MBeanServerConnection;
import javax.management.ObjectName;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXServiceURL;

public class Client {

    public static void main(String[] args) {
        try {
            // Create an RMI connector client and
            // connect it to the RMI connector server
            //
            echo("\nCreate an RMI connector client and " +
                 "connect it to the RMI connector server");
            JMXServiceURL url = new JMXServiceURL(
                      "service:jmx:rmi:///jndi/rmi://localhost:9999/server");
            JMXConnector jmxc = JMXConnectorFactory.connect(url, null);

            // Create listener
            //
            ClientListener listener = new ClientListener();

            // Get an MBeanServerConnection
            //
            echo("\nGet an MBeanServerConnection");
            MBeanServerConnection mbsc = jmxc.getMBeanServerConnection();
            waitForEnterPressed();

            // Get domains from MBeanServer
            //
            echo("\nDomains:");
            String domains[] = mbsc.getDomains();
            for (int i = 0; i < domains.length; i++) {
                echo("\tDomain[" + i + "] = " + domains[i]);
            }
            waitForEnterPressed();

            // Get MBeanServer's default domain
            //
            String domain = mbsc.getDefaultDomain();

            // Create SimpleStandard MBean
            //
            ObjectName stdMBeanName =
                new ObjectName(domain +":type=SimpleStandard,name=2");
            echo("\nCreate SimpleStandard MBean...");
            mbsc.createMBean("SimpleStandard", stdMBeanName, null, null);
            waitForEnterPressed();

            // Create SimpleDynamic MBean
            //
            ObjectName dynMBeanName =
                new ObjectName(domain +":type=SimpleDynamic,name=2");
            echo("\nCreate SimpleDynamic MBean...");
            mbsc.createMBean("SimpleDynamic", dynMBeanName, null, null);
            waitForEnterPressed();

            // Get MBean count
            //
            echo("\nMBean count = " + mbsc.getMBeanCount());

            // Query MBean names
            //
            echo("\nQuery MBeanServer MBeans:");
            Set names = mbsc.queryNames(null, null);
            for (Iterator i = names.iterator(); i.hasNext(); ) {
                echo("\tObjectName = " + (ObjectName) i.next());
            }
            waitForEnterPressed();

            // -------------------------------
            // Manage the SimpleStandard MBean
            // -------------------------------
            echo("\n>>> Perform operations on SimpleStandard MBean <<<");

            // Get State attribute in SimpleStandard MBean
            //
            echo("\nState = " + mbsc.getAttribute(stdMBeanName, "State"));

            // Set State attribute in SimpleStandard MBean
            //
            mbsc.setAttribute(stdMBeanName,
                              new Attribute("State", "changed state"));

            // Get State attribute in SimpleStandard MBean
            //
            // Another way of interacting with a given MBean is through a
            // dedicated proxy instead of going directly through the MBean
            // server connection
            //
            SimpleStandardMBean proxy = JMX.newMBeanProxy(
                    mbsc, stdMBeanName, SimpleStandardMBean.class, true);
            echo("\nState = " + proxy.getState());

            // Add notification listener on SimpleStandard MBean
            //
            echo("\nAdd notification listener...");
            mbsc.addNotificationListener(stdMBeanName, listener, null, null);

            // Invoke "reset" in SimpleStandard MBean
            //
            // Calling "reset" makes the SimpleStandard MBean emit a
            // notification that will be received by the registered
            // ClientListener.
            //
            echo("\nInvoke reset() in SimpleStandard MBean...");
            mbsc.invoke(stdMBeanName, "reset", null, null);

            // Sleep for 2 seconds in order to have time to receive the
            // notification before removing the notification listener.
            //
            echo("\nWaiting for notification...");
            sleep(2000);

            // Remove notification listener on SimpleStandard MBean
            //
            echo("\nRemove notification listener...");
            mbsc.removeNotificationListener(stdMBeanName, listener);

            // Unregister SimpleStandard MBean
            //
            echo("\nUnregister SimpleStandard MBean...");
            mbsc.unregisterMBean(stdMBeanName);
            waitForEnterPressed();

            // ------------------------------
            // Manage the SimpleDynamic MBean
            // ------------------------------
            echo("\n>>> Perform operations on SimpleDynamic MBean <<<");

            // Get State attribute in SimpleDynamic MBean
            //
            echo("\nState = " + mbsc.getAttribute(dynMBeanName, "State"));

            // Set State attribute in SimpleDynamic MBean
            //
            mbsc.setAttribute(dynMBeanName,
                              new Attribute("State", "changed state"));

            // Get State attribute in SimpleDynamic MBean
            //
            echo("\nState = " +
                               mbsc.getAttribute(dynMBeanName, "State"));

            // Add notification listener on SimpleDynamic MBean
            //
            echo("\nAdd notification listener...");
            mbsc.addNotificationListener(dynMBeanName, listener, null, null);

            // Invoke "reset" in SimpleDynamic MBean
            //
            // Calling "reset" makes the SimpleDynamic MBean emit a
            // notification that will be received by the registered
            // ClientListener.
            //
            echo("\nInvoke reset() in SimpleDynamic MBean...");
            mbsc.invoke(dynMBeanName, "reset", null, null);

            // Sleep for 2 seconds in order to have time to receive the
            // notification before removing the notification listener.
            //
            echo("\nWaiting for notification...");
            sleep(2000);

            // Remove notification listener on SimpleDynamic MBean
            //
            echo("\nRemove notification listener...");
            mbsc.removeNotificationListener(dynMBeanName, listener);

            // Unregister SimpleDynamic MBean
            //
            echo("\nUnregister SimpleDynamic MBean...");
            mbsc.unregisterMBean(dynMBeanName);
            waitForEnterPressed();

            // Close MBeanServer connection
            //
            echo("\nClose the connection to the server");
            jmxc.close();
            echo("\nBye! Bye!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void echo(String msg) {
        System.out.println(msg);
    }

    private static void sleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void waitForEnterPressed() {
        try {
            echo("\nPress <Enter> to continue...");
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
