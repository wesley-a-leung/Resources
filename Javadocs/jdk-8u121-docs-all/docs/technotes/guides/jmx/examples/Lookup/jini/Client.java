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

package jini;

import net.jini.core.lookup.ServiceRegistrar;
import net.jini.admin.Administrable;
import net.jini.core.discovery.LookupLocator;
import net.jini.core.lookup.ServiceItem;
import net.jini.core.lookup.ServiceRegistration;
import net.jini.core.entry.Entry;
import net.jini.core.lease.Lease;
import net.jini.core.lookup.ServiceTemplate;
import net.jini.core.lookup.ServiceMatches;

import javax.management.remote.*;
import javax.management.*;

import java.net.MalformedURLException;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.Iterator;
import java.io.IOException;
import java.io.Serializable;
import java.rmi.RMISecurityManager;

/**
 * This class demonstrates how to use Jini as a lookup service for
 * JSR 160 connectors. It shows how to lookup a JMXConnector from
 * the Jini lookup service.
 * <p>
 * See README file and {@link #main(String[])} for more details.
 * <p>
 * Make sure to read the section "Binding with Lookup Services" of
 * the JMX Remote API 1.0 Specification before looking at this example.
 */
public class Client {

    private static boolean debug = false;

    /**
     * Get a pointer to the Jini Lookup Service.
     * (See Jini documentation for more info).
     * <p>
     * The Jini Lookup Service URL is determined as follows:
     * <p>
     * If the System property <code>"jini.lookup.url"</code> is provided,
     * its value is the Jini Lookup Service URL.
     * <p>
     * Otherwise, the default URL is assumed to be
     * <code>"jini://localhost"</code>
     *
     * @return a pointer to the Jini Lookup Service.
     */
    public static ServiceRegistrar getRegistrar()
        throws IOException, ClassNotFoundException, MalformedURLException {
        final String jurl =
            System.getProperty("jini.lookup.url","jini://localhost");
        final LookupLocator lookup = new LookupLocator(jurl);
        final ServiceRegistrar registrar = lookup.getRegistrar();
        if (registrar instanceof Administrable)
            debug("Registry is administrable.");
        return registrar;
    }

    /**
     * Lookup JMXConnectors in the Jini Lookup Service.
     *
     * @param registrar A pointer to the Jini Lookup Service,
     *        returned by {@link #getRegistrar()}.
     * @param name the AgentName of the JMXConnectors that should
     *        be returned. If <var>name</var> is null, then
     *        the JMXConnectors for all agents are returned
     *        (null is an equivalent for a wildcard).
     * @return The list of matching JMXConnectors retrieved from
     *         the Jini Lookup Service.
     */
    public static List lookup(ServiceRegistrar registrar,
                              String name) throws IOException {
        final ArrayList list = new ArrayList();

        // Returns only JMXConnectors. The filter could be made
        // more strict by suplying e.g. RMIConnector.class
        // (would only return RMIConnectors).
        //
        final Class[] classes = new Class[] {JMXConnector.class};

        // Will return only those services for which the Name
        // attribute was registered. Since JSR 160 specifies that
        // the Name attribute is mandatory, this makes it possible
        // to filter out all the services that do not conform
        // to the spec.
        // If <name> is null, then all services for which the
        // Name attribute was specified will match, regardless of
        // the value of that attribute. Otherwise, only those services
        // for which Name matches the specified name will be returned.
        //
        final Entry[] serviceAttrs = new Entry[] {
            // Add here the matching attributes.
            new net.jini.lookup.entry.Name(name)
        };

        // Create a ServiceTemplate to do the matching.
        //
        ServiceTemplate template =
            new ServiceTemplate(null,classes,serviceAttrs);

        // Lookup all matching services in the Jini Lookup Service.
        //
        ServiceMatches matches =
            registrar.lookup(template, Integer.MAX_VALUE);

        // Retrieve the matching JMX Connectors.
        //
        for (int i = 0; i < matches.totalMatches; i++) {

            debug("Found Service: " + matches.items[i].serviceID);
            if (debug) {
                // List the lookup attributes that where registered
                // for that service.
                if (matches.items[i].attributeSets != null) {
                    final Entry[] attrs = matches.items[i].attributeSets;
                    for (int j = 0; j < attrs.length ; j++) {
                        debug("\tAttribute["+j+"]=" + attrs[j]);
                    }
                }
            }

            if (matches.items[i].service != null) {
                // Service could be null if it can't be deserialized, because
                // e.g. the class was not found.
                // This will not happen with JSR 160 mandatory connectors
                // however.

                // Get the JMXConnector.
                //
                JMXConnector c = (JMXConnector)(matches.items[i].service);
                debug("Found a JMXConnector: " + c);

                // Add the connector to the result list.
                list.add(c);
            }
        }
        return list;
    }

    /**
     * List all MBeans and their attributes.
     */
    public static void listMBeans(MBeanServerConnection server)
        throws IOException {

        final Set names = server.queryNames(null,null);
        for (final Iterator i=names.iterator(); i.hasNext(); ) {
            ObjectName name = (ObjectName)i.next();
            System.out.println("Got MBean: "+name);
            try {
                MBeanInfo info =
                    server.getMBeanInfo((ObjectName)name);
                MBeanAttributeInfo[] attrs = info.getAttributes();
                if (attrs == null) continue;
                for (int j=0; j<attrs.length; j++) {
                    if (attrs[j].isReadable()) {
                        try {
                            Object o =
                                server.getAttribute(name,attrs[j].getName());
                            System.out.println("\t\t" + attrs[j].getName() +
                                               " = "+o);
                        } catch (Exception x) {
                            System.err.println("JmxClient failed to get " +
                                               attrs[j].getName());
                            x.printStackTrace(System.err);
                        }
                    }
                }
            } catch (Exception x) {
                System.err.println("JmxClient failed to get MBeanInfo: "  + x);
                x.printStackTrace(System.err);
            }
        }
    }

    /**
     * Trace a debug message.
     */
    private static void debug(String msg) {
        if (debug) System.out.println(msg);
    }

    /**
     * Program Main
     * <p>
     * Lookup all JMX agents in the Jini Lookup Service and list
     * their MBeans and attributes.
     * <p>
     * You may wish to use the following properties on the Java command line:
     * <ul>
     * <li><code>-Dagent.name=&lt;AgentName&gt;</code>: specifies an
     *     AgentName to lookup (default is null, meaning any agent).</li>
     * <li><code>-Djini.lookup.url=&lt;jini-url&gt;</code>:
     *     the Jini Lookup Service URL (default is "jini://localhost"),
     *     see {@link #getRegistrar()}.</li>
     * <li><code>-Ddebug="true|false"</code>: switch the Client debug flag
     *     on/off (default is "false").</li>
     * </ul>
     */
    public static void main(String[] args) {
        try {
            // Jini requires a security manager.
            //
            if (System.getSecurityManager() == null)
                System.setSecurityManager(new RMISecurityManager());

            // Get the value of the debug flag.
            //
            debug = (Boolean.valueOf(System.getProperty("debug","false"))).
                booleanValue();

            // Get AgentName to lookup. If not defined, all agents
            // are looked up.
            //
            final String agentName = System.getProperty("agent.name");

            // Get a pointer to the Jini Lookup Service.
            //
            final ServiceRegistrar registrar = getRegistrar();
            debug("registrar is: " + registrar);

            // Lookup all matching agents in the Jini Lookup Service.
            //
            List l = lookup(registrar,agentName);

            // Attempt to connect to retrieved agents
            //
            System.out.println("Number of agents found : " + l.size());
            int j = 1;
            for (Iterator i=l.iterator();i.hasNext();j++) {
                JMXConnector c1 = (JMXConnector) i.next();
                if (c1 != null) {

                    // Connect
                    //
                    System.out.println(
                      "----------------------------------------------------");
                    System.out.println("\tConnecting to agent number "+j);
                    System.out.println(
                      "----------------------------------------------------");
                    debug("JMXConnector is: " + c1);

                    try {
                        c1.connect(null);
                    } catch (IOException x) {
                        System.err.println("Connection failed: " + x);
                        if (debug) x.printStackTrace(System.err);
                        continue;
                    }

                    // Get MBeanServerConnection
                    //
                    MBeanServerConnection conn =
                        c1.getMBeanServerConnection();
                    debug("Connection is:" + conn);
                    System.out.println("Server domain is: " +
                                       conn.getDefaultDomain());

                    // List all MBeans
                    //
                    try {
                        listMBeans(conn);
                    } catch (IOException x) {
                        System.err.println("Failed to list MBeans: " + x);
                        if (debug) x.printStackTrace(System.err);
                    }

                    // Close connector
                    //
                    try {
                        c1.close();
                    } catch (IOException x) {
                        System.err.println("Failed to close connection: " + x);
                        if (debug) x.printStackTrace(System.err);
                    }
                }
            }
        } catch (Exception x) {
            System.err.println("Unexpected exception caught in main: " + x);
            x.printStackTrace(System.err);
        }
    }
}
