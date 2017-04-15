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

package jndi;

import javax.naming.InitialContext;
import javax.naming.Context;
import javax.naming.NamingEnumeration;
import javax.naming.NameNotFoundException;
import javax.naming.NamingException;

import javax.naming.directory.DirContext;
import javax.naming.directory.Attribute;
import javax.naming.directory.BasicAttribute;
import javax.naming.directory.Attributes;
import javax.naming.directory.BasicAttributes;
import javax.naming.directory.DirContext;
import javax.naming.directory.Attribute;
import javax.naming.directory.Attributes;
import javax.naming.directory.SearchResult;
import javax.naming.directory.SearchControls;

import javax.naming.ldap.InitialLdapContext;

import javax.management.*;
import javax.management.remote.*;
import javax.management.remote.rmi.*;

import java.text.SimpleDateFormat;

import java.util.Date;
import java.util.Map;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.List;
import java.util.ArrayList;
import java.util.Locale;
import java.util.Vector;
import java.io.IOException;
import java.io.Serializable;
import java.net.InetAddress;
import java.net.MalformedURLException;

/**
 * This class demonstrates how to use an LDAP directory as a lookup
 * service for JSR 160 connectors. It shows how to register a
 * JMXConnectorServer with the LDAP directory through JNDI.
 * <p>
 * See README file and {@link #main(String[])} for more details.
 * <p>
 * Make sure to read the section "Binding with Lookup Services" of
 * the JMX Remote API 1.0 Specification before looking at this example.
 */
public class Server {

    // The URL will remain registered for 60 secs.
    //
    public final static int JMX_DEFAULT_LEASE = 60;

    private static boolean debug = false;

    /**
     * The local MBeanServer.
     */
    private final MBeanServer mbs;

    /**
     * Constructs a Server object. Creates a new MBeanServer.
     */
    public Server() {
        mbs = MBeanServerFactory.createMBeanServer();
    }

    /**
     * Get a pointer to the root context of the directory tree
     * under which this server is supposed to register itself.
     * All LDAP DNs will be considered to be relative to that root.
     * <p>
     * Note that this root is not part of the JSR 160 specification,
     * since the actual location where a JMX Agent will register
     * its connectors is left completely open by the specification.
     * The specification only discuss what the JMX Agent must/may
     * put in the directory - but not where.
     * <p>
     * This method assumes that the root of the directory is
     * will be passed in a the {@link Context#PROVIDER_URL
     * Context.PROVIDER_URL} System property.
     * <p>
     * This method will transfer a fixed set of System Properties to
     * the Hashtable given to the JNDI InitialContext:
     * <ul><li>{@link Context#INITIAL_CONTEXT_FACTORY
     *           Context.INITIAL_CONTEXT_FACTORY} - default is
     *         <code>"com.sun.jndi.ldap.LdapCtxFactory"</code></li>
     *     <li>{@link Context#PROVIDER_URL
     *           Context.PROVIDER_URL}</li>
     *     <li>{@link Context#SECURITY_PRINCIPAL
     *           Context.SECURITY_PRINCIPAL} - default is
     *         <code>"cn=Directory Manager"</code></li>
     *     <li>{@link Context#SECURITY_CREDENTIALS
     *           Context.SECURITY_CREDENTIALS}</li>
     * </ul>
     *
     * @return a pointer to the LDAP Directory.
     */
    public static DirContext getRootContext() throws NamingException {
        // Prepare environment
        //
        final Hashtable env = new Hashtable();

        // The Initial Context Factory must be provided, and
        // must point to an LDAP Context Factory
        //
        final String factory =
            System.getProperty(Context.INITIAL_CONTEXT_FACTORY,
                               "com.sun.jndi.ldap.LdapCtxFactory");

        // The LDAP Provider URL must be provided, and
        // must point to a running LDAP directory server
        //
        final String ldapServerUrl =
            System.getProperty(Context.PROVIDER_URL);

        // The LDAP user must be provided, and
        // must have write access to the subpart of the directory
        // where the agent will be registered.
        //
        final String ldapUser =
            System.getProperty(Context.SECURITY_PRINCIPAL,
                               "cn=Directory Manager");

        // Credentials must be provided, so that the user may
        // write to the directory.
        //
        final String ldapPasswd =
            System.getProperty(Context.SECURITY_CREDENTIALS);

        // Debug info: print provided values:
        //
        debug(Context.PROVIDER_URL + "=" + ldapServerUrl);
        debug(Context.SECURITY_PRINCIPAL + "=" + ldapUser);
        if (debug) {
            System.out.print(Context.SECURITY_CREDENTIALS + "=");
            final int len = (ldapPasswd==null)?0:ldapPasswd.length();
            for (int i=0;i<len;i++) System.out.print("*");
            System.out.println();
        }

        // Put provided value in the environment table.
        //
        env.put(Context.INITIAL_CONTEXT_FACTORY,factory);
        env.put(Context.SECURITY_PRINCIPAL, ldapUser);
        if (ldapServerUrl != null)
            env.put(Context.PROVIDER_URL, ldapServerUrl);
        if (ldapPasswd != null)
            env.put(Context.SECURITY_CREDENTIALS, ldapPasswd);

        // Create initial context
        //
        InitialContext root = new InitialLdapContext(env,null);

        // Now return the root directory context.
        //
        return (DirContext)(root.lookup(""));
    }

    /**
     * Registers a JMX Connector URL with the LDAP directory.
     * <p>
     * This method expects to find the LDAP DN where it will register
     * the JMX Connector URL in the "dn" System property. If that
     * property is not set, then "cn=<var>name</var>" is assumed.
     * <p>
     * If the given DN does not point to an existing node in the
     * directory, then this method will attempt to create it. Yet,
     * the parent node must already exist in that case.
     * <p>
     * If the DN points to a node that is already of the <var>jmxConnector</var>
     * class, then this method will simply override its <var>jmxServiceURL</var>
     * ,<var>jmxAgentName</var>, <var>jmxProtocolType</var>,
     * <var>jmxAgentHost</var> and <var>jmxExpirationDate</var> attributes.
     *
     * @param root      A pointer to the root context we are using,
     *                  as returned by {@link #getRootContext()}.
     * @param jmxUrl    A JMX Connector Server URL, that should have
     *                  been obtained from
     *                  {@link JMXConnectorServer#getAddress()
     *                  JMXConnectorServer.getAddress()};
     * @param name      The AgentName with which the URL must be registered
     *                  in the LDAP directory.
     */
    public static void register(DirContext root,
                                JMXServiceURL jmxUrl,
                                String name)
        throws NamingException, IOException {

        // Get the LDAP DN where to register
        //
        final String mydn = System.getProperty("dn","cn="+name);

        debug("dn: " + mydn );

        // First check whether <mydn> already exists
        //
        Object o = null;
        try {
            o = root.lookup(mydn);
            // There is already a node at <mydn>
            //
        } catch (NameNotFoundException n) {
            // <mydn> does not exist! attempt to create it.
            //

            // Prepare attributes for creating a javaContainer
            // with the auxiliary class jmxConnector.
            //
            Attributes attrs = new BasicAttributes();

            // Prepare objectClass attribute: we're going to create
            // a javaContainer with the jmxConnector auxiliary class.
            //
            Attribute objclass = new BasicAttribute("objectClass");
            objclass.add("top");
            objclass.add("javaContainer");
            objclass.add("jmxConnector");
            attrs.put(objclass);
            attrs.put("jmxAgentName", name);
            o = root.createSubcontext(mydn,attrs);
        }

        // That's not supposed to happen but who knows...
        //
        if (o == null) throw new NameNotFoundException();

        // Check that the entry contains the jmxConnector objectClass
        // before modifying the attributes.
        //
        final Attributes attrs = root.getAttributes(mydn);
        final Attribute oc = attrs.get("objectClass");
        if (!oc.contains("jmxConnector")) {
            // The node does not have the jmxConnector class.
            //
            final String msg = "The supplied node [" + mydn + "] does not " +
                "contain the jmxConnector objectclass";
            throw new NamingException(msg);
        }

        // Now need to replace jmxConnector attributes.
        //
        final Attributes newattrs = new BasicAttributes();
        newattrs.put("jmxAgentName",name);
        newattrs.put("jmxServiceURL",jmxUrl.toString());
        newattrs.put("jmxAgentHost",InetAddress.getLocalHost().getHostName());
        newattrs.put("jmxProtocolType",jmxUrl.getProtocol());
        newattrs.put("jmxExpirationDate",
                     getExpirationDate(JMX_DEFAULT_LEASE));
        root.modifyAttributes(mydn,DirContext.REPLACE_ATTRIBUTE,newattrs);
    }

    /**
     * Creates an RMI Connector Server, starts it, and registers it
     * with the LDAP directory.
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
     * @return the created RMIConnectorServer.
     */
    public JMXConnectorServer rmi(String url)
        throws IOException, JMException,
               NamingException, ClassNotFoundException {

        // Make a JMXServiceURL from the url string.
        //
        JMXServiceURL jurl = new JMXServiceURL(url);

        // Prepare the environment Map
        //
        final HashMap env = new HashMap();
        final String rprop = RMIConnectorServer.JNDI_REBIND_ATTRIBUTE;
        final String rebind=System.getProperty(rprop,"true");
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

        // Start the connector and register it in the LDAP directory.
        //
        start(rmis,env,agentName);

        return rmis;
    }

    /**
     * Start a JMXConnectorServer and register it with the LDAP directory.
     *
     * @param server the JMXConnectorServer to start and register.
     * @param env   the environment Map.
     * @param agentName the AgentName with which the URL must be registered
     *                  in the LDAP Directory. This is not a LDAP DN, but
     *                  the value of the jmxAgentName attribute.
     */
    public void start(JMXConnectorServer server, Map env, String agentName)
        throws IOException, NamingException {

        // Start the JMXConnectorServer
        //
        server.start();

        // Get a pointer to the LDAP directory.
        //
        final DirContext root = getRootContext();

        // Create a JMX Service URL to register in the LDAP directory
        //
        final JMXServiceURL address = server.getAddress();

        // Register the URL in the LDAP directory
        //
        register(root,address,agentName);
    }

    /**
     * Returns a X.208 string representing the GMT date at now + sec.
     *
     * @param sec Number of seconds from now.
     * @return an X.208 GMT GeneralizedTime (ending with Z).
     */
    public static String getExpirationDate(long sec) {
        final SimpleDateFormat fmt =  new SimpleDateFormat("yyyyMMddHHmmss.S");
        final Date date = new Date();
        final Date gmtDate;
        if (fmt.getCalendar().getTimeZone().inDaylightTime(date))
            gmtDate = new Date(System.currentTimeMillis() -
                               fmt.getCalendar().getTimeZone().getRawOffset() -
                               fmt.getCalendar().getTimeZone().getDSTSavings() +
                               1000*sec);
        else
            gmtDate =
                new Date(System.currentTimeMillis() -
                         fmt.getCalendar().getTimeZone().getRawOffset() +
                         1000*sec);
        return ((fmt.format(gmtDate))+"Z");
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
     * the method that will create and register the appropriate
     * JMX Connector Server for that URL.
     * <p>
     * You may wish to use the following properties on the Java command line:
     * <ul>
     * <li><code>-Durl=&lt;jmxServiceURL&gt;</code>: specifies the URL of
     *     the JMX Connector Server you wish to use. See README file for more
     *     details.</li>
     * <li><code>-Dagent.name=&lt;AgentName&gt;</code>: specifies an
     *     AgentName to register with.</li>
     * <li><code>-Djava.naming.factory.initial=&lt;initial-context-factory&gt;
     *     </code>: The initial context factory to use for accessing the
     *     LDAP directory (see {@link Context#INITIAL_CONTEXT_FACTORY
     *     Context.INITIAL_CONTEXT_FACTORY}) - default is
     *     <code>"com.sun.jndi.ldap.LdapCtxFactory"</code>.</li>
     * <li><code>-Djava.naming.provider.url=&lt;provider-url&gt;</code>:
     *     The LDAP Provider URL (see {@link Context#PROVIDER_URL
     *     Context.PROVIDER_URL}).</li>
     * <li><code>-Djava.naming.security.principal=&lt;ldap-principal&gt;
     *     </code>: The security principal (login) to use to connect with
     *     the LDAP directory (see {@link Context#SECURITY_PRINCIPAL
     *     Context.SECURITY_PRINCIPAL} - default is
     *     <code>"cn=Directory Manager"</code>.</li>
     * <li><code>-Djava.naming.security.credentials=&lt;ldap-credentials&gt;
     *     </code>: The security credentials (password) to use to
     *     connect with the LDAP directory (see
     *     {@link Context#SECURITY_CREDENTIALS
     *     Context.SECURITY_CREDENTIALS}).</li>
     * <li><code>-Ddebug="true|false"</code>: switch the Server debug flag
     *     on/off (default is "false")</li>
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
            final String url =
                System.getProperty("url", "service:jmx:rmi://");

            // Build a JMXServiceURL
            //
            final JMXServiceURL jurl = new JMXServiceURL(url);

            // Creates a JMX Connector Server
            //
            final JMXConnectorServer server;
            debug("Creating Connector: " + jurl);

            final String p = jurl.getProtocol();
            if (p.equals("rmi"))        // Create an RMI Connector
                s.rmi(url);
            else if (p.equals("iiop"))  // Create an RMI/IIOP Connector
                s.rmi(url);
            else                        // Unsupported protocol
                throw new MalformedURLException("Unsupported protocol: " + p);

            System.out.println("\nService URL successfully registered " +
                               "in the LDAP Lookup Service");

        } catch (Exception x) {
            System.err.println("Unexpected exception caught in main: " + x);
            x.printStackTrace(System.err);
        }
    }
}
