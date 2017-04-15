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
import com.sun.slp.Advertiser;
import com.sun.slp.ServiceURL;
import com.sun.slp.ServiceLocationAttribute;

import javax.management.*;
import javax.management.remote.*;
import javax.management.remote.rmi.*;

import java.util.Map;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Locale;
import java.util.Vector;
import java.io.IOException;
import java.io.Serializable;
import java.net.MalformedURLException;

import javax.naming.Context;
import javax.naming.NamingException;

/**
 * This class demonstrates how to use SLP as a lookup service for
 * JSR 160 connectors. It shows how to register a JMXConnectorServer
 * with the Service Location Protocol.
 * <p>
 * See README file and {@link #main(String[])} for more details.
 * <p>
 * Make sure to read the section "Binding with Lookup Services" of
 * the JMX Remote API 1.0 Specification before looking at this example.
 */
public class Server {

    // The Service URL will remain registered for 300 secs.
    // This is an intentionally long time for the purpose of this example.
    // In practice, a shorter lease, periodically refreshed, is preferable.
    //
    public final static int JMX_DEFAULT_LEASE = 300;

    // Default scope.
    //
    public final static String JMX_SCOPE = "DEFAULT";

    // The local MBeanServer.
    //
    private final MBeanServer mbs;

    private static boolean debug = false;

    /**
     * Constructs a Server object. Creates a new MBeanServer.
     */
    public Server() {
        mbs = MBeanServerFactory.createMBeanServer();
    }

    /**
     * Registers a JMX Connector URL with the SLP Lookup Service.
     *
     * @param jmxUrl A JMX Connector Server URL obtained from
     *               {@link JMXConnectorServer#getAddress()
     *               JMXConnectorServer.getAddress()}
     * @param name   The AgentName with which the URL will be
     *               registered in the SLP Lookup Service.
     */
    public static void register(JMXServiceURL jmxUrl, String name)
        throws ServiceLocationException {

        // Create the SLP service URL
        //
        // Note: It is recommended that the JMX Agents make use of the
        // leasing feature of SLP, and periodically renew their lease
        //
        ServiceURL serviceURL = new ServiceURL(jmxUrl.toString(),
                                               JMX_DEFAULT_LEASE);

        System.out.println("\nRegistering URL for " +  name + ": " + jmxUrl);
        debug("ServiceType is: " + serviceURL.getServiceType());

        // Prepare Lookup Attributes
        //
        Vector attributes = new Vector();
        Vector attrValues = new Vector();

        // Specify default SLP scope
        //
        attrValues.add(JMX_SCOPE);
        ServiceLocationAttribute attr1 =
            new ServiceLocationAttribute("SCOPE", attrValues);
        attributes.add(attr1);

        // Specify AgentName attribute (mandatory)
        //
        attrValues.removeAllElements();
        attrValues.add(name);
        ServiceLocationAttribute attr2 =
            new ServiceLocationAttribute("AgentName", attrValues);
        attributes.add(attr2);

        // Register with SLP
        // -----------------

        // Get SLP Advertiser
        //
        final Advertiser slpAdvertiser =
            ServiceLocationManager.getAdvertiser(Locale.US);

        // Register the service: URL
        //
        slpAdvertiser.register(serviceURL, attributes);
        System.out.println("\nRegistered URL: " + jmxUrl);
    }

    /**
     * Creates an RMI Connector Server, starts it, and registers it
     * with the SLP Lookup Service.
     * <p>
     * This method will transfer a fixed set of System Properties to
     * the Map given to the RMIConnectorServer constructor. Some
     * JNDI properties, if defined, are transfered to the Map so
     * that they may be used when LDAP is used as external directory
     * to register the RMI Stub (see {@link javax.management.remote.rmi}
     * Javadoc). Note that even if LDAP is used as external directory
     * the {@link Context#INITIAL_CONTEXT_FACTORY
     *            Context.INITIAL_CONTEXT_FACTORY} and
     * {@link Context#PROVIDER_URL Context.PROVIDER_URL} properties
     * usually don't need to be passed.
     * <p>
     * The following System properties, if defined, are transfered to
     * the Map given to the RMIConnectorServer constructor.
     * <ul><li>{@link Context#INITIAL_CONTEXT_FACTORY
     *           Context.INITIAL_CONTEXT_FACTORY}</li>
     *     <li>{@link Context#PROVIDER_URL
     *           Context.PROVIDER_URL}</li>
     *     <li>{@link Context#SECURITY_PRINCIPAL
     *           Context.SECURITY_PRINCIPAL}</li>
     *     <li>{@link Context#SECURITY_CREDENTIALS
     *           Context.SECURITY_CREDENTIALS}</li>
     *     <li>{@link RMIConnectorServer#JNDI_REBIND_ATTRIBUTE
     *           RMIConnectorServer.JNDI_REBIND_ATTRIBUTE} - default
     *           is <code>true</code>.</li>
     * </ul>
     *
     * @param url A string representation of the JMXServiceURL.
     *
     * @return the created RMIConnectorServer.
     */
    public JMXConnectorServer rmi(String url) throws
        IOException,
        JMException,
        NamingException,
        ClassNotFoundException,
        ServiceLocationException {

        // Make a JMXServiceURL from the url string.
        //
        JMXServiceURL jurl = new JMXServiceURL(url);

        // Prepare the environment Map
        //
        final HashMap env = new HashMap();
        final String rprop = RMIConnectorServer.JNDI_REBIND_ATTRIBUTE;
        final String rebind = System.getProperty(rprop,"true");
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
        env.put(rprop,rebind); // default is true.

        // Create an RMIConnectorServer
        //
        System.out.println("Creating RMI Connector: " + jurl);
        JMXConnectorServer rmis =
            JMXConnectorServerFactory.newJMXConnectorServer(jurl, env, mbs);

        // Get the AgentName for registering the Connector in the Lookup Service
        //
        final String agentName = System.getProperty("agent.name",
                                                    "DefaultAgent");

        // Start the connector and register it with SLP Lookup Service
        //
        start(rmis, agentName);

        return rmis;
    }

    /**
     * Start a JMXConnectorServer and register it with SLP Lookup Service.
     *
     * @param server the JMXConnectorServer to start and register.
     * @param agentName the AgentName with which the URL must be registered
     *                  in the SLP Lookup Service.
     */
    public void start(JMXConnectorServer server, String agentName)
        throws IOException, ServiceLocationException {

        // Start the JMXConnectorServer
        //
        server.start();

        // Create a JMX Service URL to register with SLP
        //
        final JMXServiceURL address = server.getAddress();

        // Register the URL with the SLP Lookup Service.
        //
        register(address, agentName);
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
     * Creates a server object, gets the JMX Service URL, and calls
     * the method that will create and register the appropriate JMX
     * Connector Server for that URL.
     * <p>
     * You may wish to use the following properties on the Java command line:
     * <ul>
     * <li><code>-Durl=&lt;jmxServiceURL&gt;</code>: specifies the URL of
     *     the JMX Connector Server you wish to use. See README file for more
     *     details.</li>
     * <li><code>-Dagent.name=&lt;AgentName&gt;</code>: specifies the
     *     AgentName to register with.</li>
     * <li><code>-Ddebug="true|false"</code>: switch the Server debug flag
     *     on/off (default is "false").</li>
     * </ul>
     */
    public static void main(String[] args) {
        try {
            // Get the value of the debug flag.
            //
            debug = (Boolean.valueOf(System.getProperty("debug","false"))).
                booleanValue();

            // Create a new Server object.
            //
            final Server s = new Server();

            // Get the JMXConnector URL
            //
            final String url = System.getProperty("url", "service:jmx:rmi://");

            // Build a JMXServiceURL
            //
            final JMXServiceURL jurl = new JMXServiceURL(url);

            // Creates a JMX Connector Server
            //
            debug("Creating Connector: " + jurl);
            final String p = jurl.getProtocol();
            if (p.equals("rmi"))         // Create an RMI Connector
                s.rmi(url);
            else if (p.equals("iiop"))   // Create an RMI/IIOP Connector
                s.rmi(url);
            else                         // Unsupported protocol
                throw new MalformedURLException("Unsupported protocol: " + p);

            System.out.println("\nService URL successfully registered " +
                               "in the SLP Lookup Service");

        } catch (Exception x) {
            System.err.println("Unexpected exception caught in main: " + x);
            x.printStackTrace(System.err);
        }
    }
}
