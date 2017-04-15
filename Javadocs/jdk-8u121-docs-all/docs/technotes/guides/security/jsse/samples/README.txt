                Java(tm) Secure Socket Extension
            for the JavaTM 2 Standard Edition, v5.0

                        Sample Code

                          README

----------------------------------------------------------------------
CONTENTS
----------------------------------------------------------------------
	- Introduction
	- Sample KeyStores
	- Code Examples
	- Troubleshooting

----------------------------------------------------------------------
Introduction
----------------------------------------------------------------------
This JSSE sample code bundle provides some simple examples of how
the JSSE can be used to secure communications in the Java(tm) network
environment.

The samples do require some familiarity with Java and the JSSE API, so
please consult the appropriate documentation for more information.

JDK:	http://java.sun.com/doc/

JSSE:	Documentation for the JSSE API can be found in Sun's
	JDK implementation or documentation bundles, or at
	http://java.sun.com/j2se/1.5.0/docs/guide/security/jsse/JSSERefGuide.html

	If you use a JSSE implementation from a vendor other than Sun,
	also consult that JSSE implementation's documentation.  Follow any
	instructions given by the vendor as to how to configure the
	security provider, set the classpaths (if necessary), enable the
	https protocol handler, define HTTPS proxy servers, and so on.

----------------------------------------------------------------------
Sample Truststores
----------------------------------------------------------------------

This bundle includes sample KeyStore files that are used with
the sample code.  They are stored in the "JKS" KeyStore format,
which is the default format used by Sun's JDK implementation.  (If
another KeyStore format is desired, the JDK will need to be configured
to recognize the new default format.)

JSSE uses the following certificate KeyStore files to authenticate the
clients and servers.

*/testkeys      These files are used by the code samples as the source
		of public/private key and certificate material.  In the
		client program directories, the testkeys files contains
		the certificate entry for the Java mascot "Duke".  In
		the server program directories (./sockets/server and
		rmi), the file contains a certificate entry for the
		server "localhost".

		The sample code expects the testkeys file to be in the
		current working directory.

		NOTE:  These are very simple certificates and are not
		appropriate for a production environment, but they
		should be sufficient for running the samples here.

		The password for these keystores is:  passphrase

samplecacerts	This truststore file is very similar to the stock
		JDK cacerts file, in that it contains trust
		certificates from several vendors.  It also contains
		the trusted certificates from "Duke" and "localhost"
		above.

		The password for this keystore is the same as the
		JDK cacert's initial password:  changeit

		Please see your provider's documentation for
		how to configure the location of your trusted cert
		file.

		NOTES FOR RUNNING WITH SUN's JDK:
		---------------------------------

		    Users of the Sun JDK can specify the location of
		    the truststore by using one of the following
		    methods (and are described more fully in the Sun
		    JDK JSSE Reference Guide):

		    1.  System properties:

			java -Djavax.net.ssl.trustStore=samplecacerts \
			    -Djavax.net.ssl.trustStorePassword=changeit
			    Application

		    2.  Install the file into:

			<java-home>/lib/security/jssecacerts

		    3.  Install the file into:

			<java-home>/lib/security/cacerts

		    If you choose 2) or 3), be sure to replace this
		    file with a production cacerts file before
		    deployment.

The Sun JDK utility "keytool" can be used to generate alternate
certificates and keystore files.

*******************************************

IMPORTANT NOTE:  Verify Your cacerts File

Since you trust the CAs in the cacerts file as entities for signing and
issuing certificates to other entities, you must manage the cacerts
file carefully.  The cacerts file should contain only certificates of
the entities and CAs you trust.  It is your responsibility to verify the
trusted root CA certificates bundled in the cacerts file and make your
own trust decisions.  To remove an untrusted CA certificate from the
cacerts file, use the delete option of the keytool command. You can
find the cacerts file in the JRE installation directory. Contact your
system administrator if you do not have permission to edit this file.

----------------------------------------------------------------------
Code Examples
----------------------------------------------------------------------
The sample code bundle is broken into several directories, based on
the style of SSL connections.

urls

    URLReader.java

	This example illustrates using a URL to access resources on a
	secure site.  By default, this example connects to
	www.verisign.com, but it can be adapted to connect to the
	ClassFileServer below.  To do so, the URL will need to be
	modified to point to the correct address.  You may also need to
	update the server's certificate or provide a custom
	HostNameVerifier (see HttpsURLConnection) if the hostname in
	the server's certificate doesn't match the URL's hostname.

	SPECIAL NOTES:
	    If you are behind a firewall, you may need to set
	    the "https.proxyHost" and "https.proxyPort" System
	    properties to correctly specify the proxy.

	USAGE:
	    java URLReader


    URLReaderWithOptions.java

	This example is very similar to URLReader above, but
	allows you to set the system properties via arguments to the
	main method, rather than as -D options to the java runtime
	environment.

	USAGE:
	    java URLReaderWithOptions [-h proxyhost] [-p proxyport] \
		    [-k protocolhandlerpkgs] [-c ciphersarray]

		proxyHost = secure proxy server hostname (https.proxyHost)
		proxyPort = secure proxy server port (https.proxyPort)
		protocolhandlerpkgs = a "|" separated list of protocol handlers
			(java.protocol.handler.pkgs)
		ciphersarray = enabled cipher suites as a comma separated list
			(https.cipherSuites)

sockets

    server

	ClassServer.java
	ClassFileServer.java

	    This sample demonstrates the implementation of a
	    mini-webserver, which can service simple HTTP or HTTPS
	    requests (only the GET method is supported).

	    By default, the server does not use SSL/TLS.  However,
	    a command line option enables SSL/TLS.

	    Requests must be of the form:

		GET /<filename>

	    USAGE:
		java ClassFileServer port docroot [TLS [true]]

		    port = the port on which the server resides
		    docroot = the root of the local directory hierarchy
		    TLS = an optional flag which enables SSL/TLS
			services
		    true = an optional flag which requires that clients
			authenticate themselves.  This option requires
			that SSL/TLS support be enabled.

	    The secure server comes preinstalled with a certificate for
	    "localhost".  If server is on the same host as the client,
	    URLs of the form "https://localhost:port/file" should pass
	    hostname verification.  If you choose to run on separate
	    hosts, you should create a new host certificate for the
	    https hostname being used, otherwise there will be hostname
	    mismatch problems.  (Note:  in Java this can be corrected
	    in the HttpsURLConnection class by providing a custom
	    HostnameVerifier implementation, or in a browswer by
	    "Accept"ing the dialog box that describes the hostname
	    mismatch.)

	    If you are using the TLS variant (HTTPS), remember to
	    specify the https protocol:

		https://hostname:2001/dir1/file1

	    NOTE:  If you use a browser such as Microsoft's Internet
	    Explorer or Netscape's Navigator you will see a dialog
	    popup with the message that the application doesn't
	    recognize the "localhost" certificate.  This is normal
	    because the self-signed certificate being presented to the
	    browser is not initially trusted.  If desired, you could
	    import the "localhost" certificate into the browser's
	    truststore.

    client

	SSLSocketClient.java

	    This example demonstrates how to use a SSLSocket as a client to
	    send a HTTP request and get a response from an HTTPS server.
	    By default, this example connects to www.verisign.com, but
	    it can easily be adapted to connect to the ClassFileServer
	    above.  (Note:  The GET request must be slightly modified,
	    so that a file is specified.)

	    This application assumes the client is not behind a firewall.

	    USAGE:
		java SSLSocketClient

	SSLSocketClientWithClientAuth.java

	    This example is similar to SSLSocketClient above, but
	    this shows how to set up a key manager to do client
	    authentication if required by server.

	    This application also assumes the client is not behind a
	    firewall.

	    USAGE:
		java SSLSocketClientWithClientAuth host port requestedfilepath

	    NOTE:  If you are connecting to the ClassFileServer
	    application above, be sure that it can find "duke"'s
	    credentials.  See the "Sample Key Stores" section above.

	SSLSocketClientWithTunneling.java

	    This example illustrates how to do proxy Tunneling to access a
	    secure web server from behind a firewall.

	    The System properties "https.proxyHost" and "https.proxyPort"
	    are used to make a socket connection to the proxy host, and
	    then the SSLSocket is layered on top of that Socket.

	    USAGE:
		java SSLSocketClientWithTunneling

rmi

     Hello.java
     HelloImpl.java
     RMISSLClientSocketFactory.java
     RMISSLServerSocketFactory.java
     HelloClient.java

	This example illustrates how to use RMI over an SSL transport
	layer using JSSE.  The server runs HelloImpl, which sets up
	an internal RMI registry (rather than using the rmiregistry
	command).  The client runs HelloClient and communicates over
	a secured connection.

	Setting up this sample can be a little tricky, here are the
	necessary steps:

	    % javac *.java
	    % rmic HelloImpl
	    % java \
		-Djava.security.policy=policy \
		HelloImpl   (run in another window)
	    % java HelloClient (run in another window)
	
	For the server, the RMI security manager will be installed, and
	the supplied policy file grants permission to accept
	connections from any host.  Obviously, giving all permissions
	should not be done in a production environment.  You will need
	to give it the appropriate restrictive network privileges, such
	as:

	    permission java.net.SocketPermission \
		"hostname:1024-", "accept,resolve";

	In addition, this example can be easily updated to run with
	the new standard SSL/TLS-based RMI Socket Factories. To do this,
	modify the HelloImpl.java file to use:

	    javax.rmi.ssl.SslRMIClientSocketFactory
	    javax.rmi.ssl.SslRMIServerSocketFactory

	instead of:

	    RMISSLClientSocketFactory
	    RMISSLServerSocketFactory

	These new classes use SSLSocketFactory.getDefault() and
	SSLServerSocketFactory.getDefault(), so you will need to
	configure the system properly to locate your key and trust
	material.

	NOTES FOR RUNNING WITH SUN's JDK:
	---------------------------------

	    If you use the new classes mentioned above, you can specify
	    the key stores using the System properties:

		-Djavax.net.ssl.keyStore=testkeys
		-Djavax.net.ssl.keyStorePassword=passphrase

sslengine

    SSLEngineSimpleDemo.java

    This example demonstrates a very simple client/server
    application using SSLEngines.  It demonstrates the APIs without
    taking into account I/O or compute issues.

    USAGE:
	java SSLEngineSimpleDemo

NOTE:  There is a much more extensive SSLEngine sample available.
Please see the <jdk-home>/sample/nio/server directory for a
NIO/SSLEngine-based HTTP/HTTPS server.

----------------------------------------------------------------------
Troubleshooting
----------------------------------------------------------------------
One of the most common problems people have in using JSSE is when the
JSSE receives a certificate that is unknown to the mechanism that makes
trust decisions.  If an unknown certificate is received, the trust
mechanism will throw an exception saying that the certificate is
untrusted.  Make sure that the correct trust store is being used,
and that the JSSE is installed and configured correctly.

If you are using the "localhost" or "duke" credentials, be sure that
you have correctly specified the location of the samplecacerts file,
otherwise your application will not work.  (See "Sample Key Stores" for
more information.)

The SSL debug mechanism can be used to investigate such trust
problems.  See the implementation documentation for more information
about this subject.
