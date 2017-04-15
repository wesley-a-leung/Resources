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

import javax.management.remote.*;
import javax.management.*;

import java.text.SimpleDateFormat;

import java.util.Date;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Set;
import java.util.Iterator;
import java.util.Locale;
import java.util.Vector;

import java.io.IOException;
import java.io.Serializable;

/**
 * This class demonstrates how to use an LDAP directory as a lookup service
 * for JSR 160 connectors. It shows how to lookup a JMXServiceURL
 * from the LDAP directory.
 * <p>
 * See README file and {@link #main(String[])} for more details.
 * <p>
 * Make sure to read the section "Binding with Lookup Services" of
 * the JMX Remote API 1.0 Specification before looking at this example.
 */
public class Client {

    private static boolean debug = false;

    /**
     * List all the attributes of an LDAP node.
     *
     * @param root The root DirContext.
     * @param dn   The DN of the node, relative to the root DirContext.
     */
    public static void listAttributes(DirContext root, String dn)
        throws NamingException {
        final Attributes attrs = root.getAttributes(dn);
        System.out.println("dn: " + dn);
        System.out.println("attributes: " + attrs);
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
     * Parses the expirationDate in order to determined whether
     * the associated URL has expired.
     *
     * @param expirationDate an X.208 GeneralizedTime, local or GMT.
     *        Only yyyyMMddHHmmss.S (local time) and  yyyyMMddHHmmss.SZ
     *        (GMT time) formats are recognized.
     * @return true if the expirationDate could be parsed and is past,
     *         false otherwise.
     */
    public static boolean hasExpired(String expirationDate) {
        if (expirationDate == null) return false;
        try {
            final Date localExpDate = getLocalDate(expirationDate);
            final Date now = new Date();
            if (localExpDate.before(now)) return true;
        } catch (java.text.ParseException x) {
            x.printStackTrace(System.out);
        }
        return false;
    }

    /**
     * Returns a date in the local time zone parsed from an X.208
     * formatted date. Only yyyyMMddHHmmss.S (local time) and
     * yyyyMMddHHmmss.SZ (GMT time) formats are recognized.
     *
     * @param expirationDate an X.208 GeneralizedTime, local or GMT.
     * @return the corresponding Date in the local time zone.
     */
    public static Date getLocalDate(String expirationDate)
        throws java.text.ParseException {
        final SimpleDateFormat fmt = new SimpleDateFormat("yyyyMMddHHmmss.S");
        Date localDate = fmt.parse(expirationDate);
        if (expirationDate.endsWith("Z")) {
            final Date date = new Date();
            if (fmt.getCalendar().getTimeZone().inDaylightTime(date))
                localDate =
                    new Date(localDate.getTime() +
                             fmt.getCalendar().getTimeZone().getRawOffset() +
                             fmt.getCalendar().getTimeZone().getDSTSavings());
            else
                localDate =
                    new Date(localDate.getTime() +
                             fmt.getCalendar().getTimeZone().getRawOffset());
        }
        return localDate;
    }

    /**
     * Lookup JMXConnectors in the LDAP directory.
     *
     * @param root A pointer to the LDAP directory,
     *        returned by {@link #getRootContext()}.
     * @param protocolType The protocol type of the JMX Connectors
     *        we want to retrieve. If <var>protocolType</var> is null,
     *        then the jmxProtocolType attribute is ignored. Otherwise,
     *        only those agents that have registered a matching
     *        jmxProtocolType attribute will be returned.
     * @param name the AgentName of the JMXConnectors that should
     *        be returned. If <var>name</var> is null, then
     *        the JMXConnectors for all agents are returned
     *        (null is an equivalent for a wildcard).
     * @return The list of matching JMXConnectors retrieved from
     *         the LDAP directory.
     */
    public static List lookup(DirContext root, String protocolType, String name)
        throws IOException, NamingException {

        final ArrayList list = new ArrayList();

        // If protocolType is not null, include it in the filter.
        //
        String queryProtocol =
            (protocolType==null)?"":"(jmxProtocolType="+protocolType+")";

        // Set the LDAPv3 query string
        //
        // Only those node that have the jmxConnector object class are
        // of interest to us, so we specify (objectClass=jmxConnector)
        // in the filter.
        //
        // We specify the jmxAgentName attribute in the filter so that the
        // query will return only those services for which the AgentName
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
        // We also specify (jmxServiceURL=*) so that only those node
        // for which the jmxServiceURL attribute is present will be
        // returned. Thus, we filter out all those node corresponding
        // to agents that are not currently available.
        //
        String query =
            "(&" + "(objectClass=jmxConnector) " +
            "(jmxServiceURL=*) " +
            queryProtocol +
            "(jmxAgentName=" + ((name!=null)?name:"*") + "))";

        System.out.println("Looking up JMX Agents with filter: " + query );

        SearchControls ctrls = new SearchControls();

        // Want to get all jmxConnector objects, wherever they've been
        // registered.
        //
        ctrls.setSearchScope(SearchControls.SUBTREE_SCOPE);

        // Want to get only the jmxServiceUrl and jmxExpirationDate
        // (comment these lines and all attributes will be returned).
        //
        // ctrls.setReturningAttributes(new String[] {
        //           "jmxServiceURL",
        //           "jmxExpirationDate"
        //           });

        // Search...
        //
        final NamingEnumeration results = root.search("", query, ctrls);

        // Get the URL...
        //
        while (results.hasMore()) {

            // Get node...
            //
            final SearchResult r = (SearchResult) results.nextElement();
            debug("Found node: " + r.getName());

            // Get attributes
            //
            final Attributes attrs = r.getAttributes();

            // Get jmxServiceURL attribute
            //
            final Attribute  attr = attrs.get("jmxServiceURL");
            if (attr == null) continue;

            // Get jmxExpirationDate
            //
            final Attribute  exp = attrs.get("jmxExpirationDate");

            // Check that URL has not expired.
            //
            if ((exp != null) && hasExpired((String)exp.get())) {
                System.out.print(r.getName() + ": ");
                System.out.println("URL expired since: " + exp.get());
                continue;
            }

            // Get the URL string
            //
            final String urlStr = (String)attr.get();
            if (urlStr.length() == 0) continue;

            debug("Found URL: " + urlStr);

            // Create a JMXServiceURL
            //
            final JMXServiceURL url  = new JMXServiceURL(urlStr);

            // Create a JMXConnector
            //
            final JMXConnector conn =
                JMXConnectorFactory.newJMXConnector(url,null);

            // Add the connector to the result list
            //
            list.add(conn);
            if (debug) listAttributes(root,r.getName());
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
     * Lookup all JMX agents in the LDAP Directory and list
     * their MBeans and attributes.
     * <p>
     * You may wish to use the following properties on the Java command line:
     * <ul>
     * <li><code>-Dagent.name=&lt;AgentName&gt;</code>: specifies an
     *     AgentName to lookup (default is null, meaning any agent).</li>
     * <li><code>-Dprotocol=&lt;ProtocolType&gt;</code>: restrains the client
     *     to lookup for a specific protocol type (default is null,
     *     meaning any type).</li>
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

            // Get a pointer to the LDAP Directory.
            //
            final DirContext root = getRootContext();
            debug("root is: " + root.getNameInNamespace());

            final String protocolType=System.getProperty("protocol");
            final String agentName=System.getProperty("agent.name");

            // Lookup all matching agents in the LDAP Directory.
            //
            List l = lookup(root,protocolType,agentName);

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
