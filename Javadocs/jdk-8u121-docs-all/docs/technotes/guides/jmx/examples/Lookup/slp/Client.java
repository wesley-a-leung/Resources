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

package slp;

import com.sun.slp.ServiceLocationManager;
import com.sun.slp.ServiceLocationException;
import com.sun.slp.Locator;
import com.sun.slp.ServiceURL;
import com.sun.slp.ServiceLocationAttribute;
import com.sun.slp.ServiceType;
import com.sun.slp.ServiceLocationEnumeration;

import javax.management.*;
import javax.management.remote.*;

import javax.naming.Context;

import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Iterator;
import java.util.Locale;
import java.util.Vector;

import java.io.IOException;
import java.io.Serializable;

/**
 * This class demonstrates how to use SLP as a lookup service for
 * JSR 160 connectors. It shows how to lookup a JMXServiceURL
 * from the SLP lookup service.
 * <p>
 * See README file and {@link #main(String[])} for more details.
 * <p>
 * Make sure to read the section "Binding with Lookup Services" of
 * the JMX Remote API 1.0 Specification before looking at this example.
 */
public class Client {

    // Default scope.
    //
    public final static String JMX_SCOPE = "DEFAULT";

    private static boolean debug = false;

    /**
     * Get a pointer to the SLP Lookup Service.
     * (See RFC 2614 for more info).
     * @return a pointer to the SLP Lookup Service.
     */
    public static Locator getLocator() throws ServiceLocationException {
        // Getting the Locator (for lookup purposes)
        //
        final Locator slpLocator = ServiceLocationManager.getLocator(Locale.US);
        return slpLocator;
    }

    /**
     * Lookup JMXConnectors in the SLP Lookup Service.
     *
     * @param slpLocator A pointer to the SLP Lookup Service,
     *        returned by {@link #getLocator()}.
     * @param name the AgentName of the JMXConnectors that should
     *        be returned. If <var>name</var> is null, then
     *        the JMXConnectors for all agents are returned
     *        (null is an equivalent for a wildcard).
     * @return The list of matching JMXConnectors retrieved from
     *         the SLP Lookup Service.
     */
    public static List lookup(Locator slpLocator, String name)
        throws IOException, ServiceLocationException {

        final ArrayList list = new ArrayList();

        // Set the lookup SCOPE.
        //
        Vector scopes = new Vector();
        scopes.add(JMX_SCOPE);

        // Set the LDAPv3 query string
        //
        // Will return only those services for which the AgentName
        // attribute was registered. Since JSR 160 specifies that
        // the AgentName attribute is mandatory, this makes it possible
        // to filter out all the services that do not conform
        // to the spec.
        // If <name> is null, it is replaced by "*", so that all
        // services for which the AgentName attribute was specified match,
        // regardless of the value of that attribute.
        // Otherwise, only those services for which AgentName matches the
        // name or pattern specified by <name> will be returned.
        //
        String query = "(&(AgentName=" + ((name!=null)?name:"*") + "))";

        debug("Looking up JMX Agents with filter: " + query );

        // Lookup the JMX agents....
        //
        ServiceLocationEnumeration result =
            slpLocator.findServices(new ServiceType("service:jmx"),
                                    scopes, query);

        debug("... Got service enumeration.");

        // Build the JMXConnector list
        //
        while (result.hasMoreElements()) {
            final ServiceURL surl = (ServiceURL) result.next();
            debug("\nFound Service URL: " + surl);

            // Some debug info:
            //
            if (debug) {
                // Retrieve the Lookup Attributes that were registered
                // with this URL
                //
                debug("Getting attributes...");
                final ServiceLocationEnumeration slpAttributes =
                    slpLocator.findAttributes(surl, scopes, new Vector());
                debug("... Got attribute enumeration.");
                while (slpAttributes.hasMoreElements()) {
                    final ServiceLocationAttribute slpAttribute =
                        (ServiceLocationAttribute) slpAttributes.nextElement();
                    debug("\tAttribute: " + slpAttribute);
                }
            }

            // Create a JMXConnector
            // ---------------------

            // Create a JMX Service URL
            //
            JMXServiceURL jmxUrl = new JMXServiceURL(surl.toString());
            debug("JMX Service URL: " + jmxUrl);

            // Obtain a JMXConnector from the factory
            //
            try {
                JMXConnector client =
                    JMXConnectorFactory.newJMXConnector(jmxUrl,null);
                debug("JMX Connector: " + client);

                // Add the connector to the result list.
                //
                if (client != null) list.add(client);
            } catch (IOException x ) {
                System.err.println("Failed to create JMXConnector for " +
                                   jmxUrl);
                System.err.println("Error is: " + x);
                System.err.println("Skipping...");
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
     * Program Main.
     * <p>
     * Lookup all JMX agents in the SLP Lookup Service and list
     * their MBeans and attributes.
     * <p>
     * You may wish to use the following properties on the Java command line:
     * <ul>
     * <li><code>-Dagent.name=&lt;AgentName&gt;</code>: specifies an
     *     AgentName to lookup (default is null, meaning any agent).</li>
     * <li><code>-Ddebug="true|false"</code>: switch the Client debug flag
     *     on/off (default is "false").</li>
     * </ul>
     */
    public static void main(String[] args) {
        try {
            // Get the value of the debug flag.
            //
            debug = (Boolean.valueOf(System.getProperty("debug","false"))).
                booleanValue();

            // Get AgentName to lookup.
            // If not defined, all agents are taken into account.
            //
            final String agentName = System.getProperty("agent.name");

            // Get a pointer to the SLP Lookup Service.
            //
            final Locator slpLocator = getLocator();
            debug("slpLocator is: " + slpLocator);

            // Lookup all matching agents in the SLP Lookup Service.
            //
            List l = lookup(slpLocator,agentName);

            // Attempt to connect to retrieved agents
            //
            System.out.println("\nNumber of agents found : " + l.size());
            int j = 1;
            for (Iterator i=l.iterator();i.hasNext();j++) {
                JMXConnector c1 = (JMXConnector) i.next();
                if (c1 != null) {

                    // Connect
                    //
                    System.out.println(
                      "\n----------------------------------------------------");
                    System.out.println("\tConnecting to agent number "+j);
                    System.out.println(
                      "----------------------------------------------------");
                    debug("JMXConnector is: " + c1);

                    // Prepare the environment Map
                    //
                    final HashMap env = new HashMap();
                    final String factory =
                        System.getProperty(Context.INITIAL_CONTEXT_FACTORY);
                    final String ldapServerUrl =
                        System.getProperty(Context.PROVIDER_URL);
                    final String ldapUser =
                        System.getProperty(Context.SECURITY_PRINCIPAL);
                    final String ldapPasswd =
                        System.getProperty(Context.SECURITY_CREDENTIALS);

                    // Transfer some system properties to the Map
                    //
                    if (factory!= null) // this should not be needed
                        env.put(Context.INITIAL_CONTEXT_FACTORY,factory);
                    if (ldapServerUrl!=null) // this should not be needed
                        env.put(Context.PROVIDER_URL, ldapServerUrl);
                    if (ldapUser!=null) // this is needed when LDAP is used
                        env.put(Context.SECURITY_PRINCIPAL, ldapUser);
                    if (ldapPasswd != null) // this is needed when LDAP is used
                        env.put(Context.SECURITY_CREDENTIALS, ldapPasswd);

                    try {
                        c1.connect(env);
                    } catch (IOException x) {
                        System.err.println("Connection failed: " + x);
                        x.printStackTrace(System.err);
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
                        x.printStackTrace(System.err);
                    }

                    // Close connector
                    //
                    try {
                        c1.close();
                    } catch (IOException x) {
                        System.err.println("Failed to close connection: " + x);
                        x.printStackTrace(System.err);
                    }
                }
            }
        } catch (Exception x) {
            System.err.println("Unexpected exception caught in main: " + x);
            x.printStackTrace(System.err);
        }
    }
}
