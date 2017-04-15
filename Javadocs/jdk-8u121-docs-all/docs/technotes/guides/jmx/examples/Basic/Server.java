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
import javax.management.Attribute;
import javax.management.MBeanAttributeInfo;
import javax.management.MBeanConstructorInfo;
import javax.management.MBeanInfo;
import javax.management.MBeanNotificationInfo;
import javax.management.MBeanOperationInfo;
import javax.management.MBeanServer;
import javax.management.MBeanServerFactory;
import javax.management.ObjectName;
import javax.management.remote.JMXConnectorServer;
import javax.management.remote.JMXConnectorServerFactory;
import javax.management.remote.JMXServiceURL;

public class Server {

    public static void main(String[] args) {
        try {
            // Instantiate the MBean server
            //
            echo("\n>>> Create the MBean server");
            MBeanServer mbs = MBeanServerFactory.createMBeanServer();
            waitForEnterPressed();

            // Get default domain
            //
            echo("\n>>> Get the MBean server's default domain");
            String domain = mbs.getDefaultDomain();
            echo("\tDefault Domain = " + domain);
            waitForEnterPressed();

            // Create and register the SimpleStandard MBean
            //
            String mbeanClassName = "SimpleStandard";
            String mbeanObjectNameStr =
                domain + ":type=" + mbeanClassName + ",name=1";
            ObjectName mbeanObjectName =
                createSimpleMBean(mbs, mbeanClassName, mbeanObjectNameStr);
            waitForEnterPressed();

            // Get and display the management information exposed by the
            // SimpleStandard MBean
            //
            printMBeanInfo(mbs, mbeanObjectName, mbeanClassName);
            waitForEnterPressed();

            // Manage the SimpleStandard MBean
            //
            manageSimpleMBean(mbs, mbeanObjectName, mbeanClassName);
            waitForEnterPressed();

            // Create and register the SimpleDynamic MBean
            //
            mbeanClassName = "SimpleDynamic";
            mbeanObjectNameStr =
                domain + ":type=" + mbeanClassName + ",name=1";
            mbeanObjectName =
                createSimpleMBean(mbs, mbeanClassName, mbeanObjectNameStr);
            waitForEnterPressed();

            // Get and display the management information exposed by the
            // SimpleDynamic MBean
            //
            printMBeanInfo(mbs, mbeanObjectName, mbeanClassName);
            waitForEnterPressed();

            // Manage the SimpleDynamic MBean
            //
            manageSimpleMBean(mbs, mbeanObjectName, mbeanClassName);
            waitForEnterPressed();

            // Create an RMI connector server
            //
            echo("\nCreate an RMI connector server");
            JMXServiceURL url = new JMXServiceURL(
                      "service:jmx:rmi:///jndi/rmi://localhost:9999/server");
            JMXConnectorServer cs =
                JMXConnectorServerFactory.newJMXConnectorServer(url, null, mbs);

            // Start the RMI connector server
            //
            echo("\nStart the RMI connector server");
            cs.start();
            echo("\nThe RMI connector server successfully started");
            echo("and is ready to handle incoming connections");
            echo("\nStart the client on a different window and");
            echo("press <Enter> once the client has finished");
            waitForEnterPressed();

            // Stop the RMI connector server
            //
            echo("\nStop the RMI connector server");
            cs.stop();
            System.out.println("\nBye! Bye!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static ObjectName createSimpleMBean(MBeanServer mbs,
                                                String mbeanClassName,
                                                String mbeanObjectNameStr) {
        echo("\n>>> Create the " + mbeanClassName +
             " MBean within the MBeanServer");
        echo("\tObjectName = " + mbeanObjectNameStr);
        try {
            ObjectName mbeanObjectName =
                ObjectName.getInstance(mbeanObjectNameStr);
            mbs.createMBean(mbeanClassName, mbeanObjectName);
            return mbeanObjectName;
        } catch (Exception e) {
            echo("\t!!! Could not create the " + mbeanClassName + " MBean !!!");
            e.printStackTrace();
            echo("\nEXITING...\n");
            System.exit(1);
        }
        return null;
    }

    private static void printMBeanInfo(MBeanServer mbs,
                                       ObjectName mbeanObjectName,
                                       String mbeanClassName) {
        echo("\n>>> Retrieve the management information for the " +
             mbeanClassName);
        echo("    MBean using the getMBeanInfo() method of the MBeanServer");
        MBeanInfo info = null;
        try {
            info = mbs.getMBeanInfo(mbeanObjectName);
        } catch (Exception e) {
            echo("\t!!! Could not get MBeanInfo object for " +
                 mbeanClassName +" !!!");
            e.printStackTrace();
            return;
        }
        echo("\nCLASSNAME: \t" + info.getClassName());
        echo("\nDESCRIPTION: \t" + info.getDescription());
        echo("\nATTRIBUTES");
        MBeanAttributeInfo[] attrInfo = info.getAttributes();
        if (attrInfo.length > 0) {
            for (int i = 0; i < attrInfo.length; i++) {
                echo(" ** NAME: \t" + attrInfo[i].getName());
                echo("    DESCR: \t" + attrInfo[i].getDescription());
                echo("    TYPE: \t" + attrInfo[i].getType() +
                     "\tREAD: "+ attrInfo[i].isReadable() +
                     "\tWRITE: "+ attrInfo[i].isWritable());
            }
        } else echo(" ** No attributes **");
        echo("\nCONSTRUCTORS");
        MBeanConstructorInfo[] constrInfo = info.getConstructors();
        for (int i=0; i<constrInfo.length; i++) {
            echo(" ** NAME: \t" + constrInfo[i].getName());
            echo("    DESCR: \t" + constrInfo[i].getDescription());
            echo("    PARAM: \t" + constrInfo[i].getSignature().length +
                 " parameter(s)");
        }
        echo("\nOPERATIONS");
        MBeanOperationInfo[] opInfo = info.getOperations();
        if (opInfo.length > 0) {
            for (int i = 0; i < opInfo.length; i++) {
                echo(" ** NAME: \t" + opInfo[i].getName());
                echo("    DESCR: \t" + opInfo[i].getDescription());
                echo("    PARAM: \t" + opInfo[i].getSignature().length +
                     " parameter(s)");
            }
        } else echo(" ** No operations ** ");
        echo("\nNOTIFICATIONS");
        MBeanNotificationInfo[] notifInfo = info.getNotifications();
        if (notifInfo.length > 0) {
            for (int i = 0; i < notifInfo.length; i++) {
                echo(" ** NAME: \t" + notifInfo[i].getName());
                echo("    DESCR: \t" + notifInfo[i].getDescription());
                String notifTypes[] = notifInfo[i].getNotifTypes();
                for (int j = 0; j < notifTypes.length; j++) {
                    echo("    TYPE: \t" + notifTypes[j]);
                }
            }
        } else echo(" ** No notifications **");
    }

    private static void manageSimpleMBean(MBeanServer mbs,
                                          ObjectName mbeanObjectName,
                                          String mbeanClassName) {

        echo("\n>>> Manage the " + mbeanClassName +
             " MBean using its attributes ");
        echo("    and operations exposed for management");

        try {
            // Get attribute values
            printSimpleAttributes(mbs, mbeanObjectName);

            // Change State attribute
            echo("\n    Setting State attribute to value \"new state\"...");
            Attribute stateAttribute = new Attribute("State","new state");
            mbs.setAttribute(mbeanObjectName, stateAttribute);

            // Get attribute values
            printSimpleAttributes(mbs, mbeanObjectName);

            // Invoking reset operation
            echo("\n    Invoking reset operation...");
            mbs.invoke(mbeanObjectName, "reset", null, null);

            // Get attribute values
            printSimpleAttributes(mbs, mbeanObjectName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void printSimpleAttributes(MBeanServer mbs,
                                              ObjectName mbeanObjectName) {
        try {
            echo("\n    Getting attribute values:");
            String State = (String) mbs.getAttribute(mbeanObjectName, "State");
            Integer NbChanges =
                (Integer) mbs.getAttribute(mbeanObjectName,"NbChanges");
            echo("\tState     = \"" + State + "\"");
            echo("\tNbChanges = " + NbChanges);
        } catch (Exception e) {
            echo("\t!!! Could not read attributes !!!");
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
