/*
 * Copyright (c) 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the
 *  distribution.
 *
 * Neither the name of Oracle nor the names of
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY
 * EXCLUDED. SUN MICROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL
 * NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF
 * USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR
 * ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT,
 * SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF
 * THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF SUN HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed, licensed or
 * intended for use in the design, construction, operation or
 * maintenance of any nuclear facility.
 */
package example.inetd;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketException;
import java.nio.channels.Channel;
import java.nio.channels.ServerSocketChannel;
import java.rmi.AlreadyBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.RMIServerSocketFactory;

/**
 * A utility to export a registry (using an inherited channel if launched
 * from <code>inetd</code>) and to bind a remote service's proxy in that
 * registry for clients to look up.
 **/
public class InitializeRegistry {

    private static final Object lock = new Object();
    private static boolean serviceAvailable = false;
    private static boolean initialized = false;

    /**
     * Prevents instantiation.
     */
    private InitializeRegistry() {
        throw new AssertionError();
    }

    /**
     * Creates and exports a registry (using an inherited channel, if any,
     * as specified below), and binds the specified name to the specified
     * proxy in that registry.
     *
     * First, the registry is exported as follows:
     * <ul>
     * <li>If the <code>System.inheritedChannel</code> method returns a
     * <code>ServerSocketChannel</code> instance, the registry is exported
     * with an <code>RMIServerSocketFactory</code> whose
     * <code>createServerSocket</code> method returns a
     * <code>ServerSocket</code> for the inherited
     * <code>ServerSocketChannel</code> that delays accepting
     * requests until the specified proxy is bound in the
     * registry.
     *
     * <li>If the <code>System.inheritedChannel</code> method returns
     * <code>null</code>, then the registry is exported with an
     * <code>RMIServerSocketFactory</code> whose
     * <code>createServerSocket</code> method returns a
     * <code>ServerSocket</code> constructed with the specified
     * port that delays accepting requests until the specified proxy is
     * bound in the registry.  In this case, if the port is <code>0</code>,
     * then an <code>IllegalArgumentException</code> is thrown.
     *
     * <li>Otherwise, if the <code>System.inheritedChannel</code> returns
     * an instance of any other type, an <code>IOException</code> is
     * thrown.
     * </ul>
     *
     * <p>Once the registry is exported, the registry's <code>bind</code>
     * method is invoked with the specified name and proxy as arguments.
     *
     * @param obj the proxy for a remote object
     * @param name the name for the remote object in the registry
     * @param port a port to export the registry on if there is no
     * inherited channel
     * @throws IllegalArgumentException if the inherited channel is
     * <code>null</code> and the port is <code>0</code>
     * @throws IllegalStateException if this method was called previously
     * @throws IOException if the inherited channel is not an instance
     * of <code>ServerSocketChannel</code> or <code>null</code>
     * @throws RemoteException if the registry could not be exported
     **/
    public static void initializeWithInheritedChannel(Remote proxy,
                                                      String name,
                                                      int port)
        throws IOException
    {
        /*
         * Only allow this method to be invoked once.
         */
        synchronized (InitializeRegistry.class) {
            if (initialized) {
                throw new IllegalStateException("already invoked");
            }
            initialized = true;
        }

        Channel channel = System.inheritedChannel();
        ServerSocket serverSocket = null;

        /*
         * Handle inherited channel, if any.
         */
        if (channel instanceof ServerSocketChannel) {
            /*
             * Service launched from inetd.  Get server socket from
             * inherited server socket channel.
             */
            serverSocket = ((ServerSocketChannel) channel).socket();

        } else if (channel == null) {
            /*
             * Service launched from the command line.  In this case, the
             * port specified for the registry must be nonzero
             */
            if  (port == 0) {
                throw new IllegalArgumentException("port must be nonzero");
            }
            serverSocket = new ServerSocket(port);

        } else {
            throw new IOException(
                "unexpected channel returned from inheritedChannel: " +
                channel.toString());
        }

        /*
         * Create server socket factory for registry to delay accepting
         * calls until a service is bound in the registry.
         */
        RMIServerSocketFactory ssf =
            new RegistryServerSocketFactory(serverSocket);

        /*
         * Create/export registry and bind name to proxy in registry.
         */
        Registry registry = LocateRegistry.createRegistry(port, null, ssf);
        try {
            registry.bind(name, proxy);
        } catch (RemoteException impossible) {
            throw new AssertionError(impossible);
        } catch (AlreadyBoundException impossible) {
            throw new AssertionError(impossible);
        }

        /*
         * Notify registry's socket factory that the service proxy is
         * bound in the registry, so that the registry can accept
         * incoming requests to look up the service.
         */
        synchronized (lock) {
            serviceAvailable = true;
            lock.notifyAll();
        }
    }

    /**
     * A server socket factory to use when exporting a registry launched
     * from 'inetd' with 'wait' status.  This socket factory's
     * 'createServerSocket' method returns a server socket that wraps the
     * server socket specified during construction and is specialized to
     * delay accepting requests until a remote service is bound in the
     * registry (when the 'serviceAvailable' flag is 'true').  The server
     * socket supplied to the constructor should be the server socket
     * obtained from the 'System.inheritedChannel' method.
     *
     * Note that only a single instance of this class should be created.
     **/
    private static class RegistryServerSocketFactory
        implements RMIServerSocketFactory
    {
        private final ServerSocket serverSocket;

        /**
         * Constructs a 'RegistryServerSocketFactory' with the specified
         * 'serverSocket'.
         **/
        RegistryServerSocketFactory(ServerSocket serverSocket) {
            this.serverSocket = serverSocket;
        }

        /**
         * Returns the server socket specified during construction wrapped
         * in a 'DelayedAcceptServerSocket'.  The port argument is ignored.
         **/
        public ServerSocket createServerSocket(int port)
            throws IOException
        {
            return new DelayedAcceptServerSocket(serverSocket);
        }

    }

    /**
     * A server socket that delegates all public methods to the underlying
     * server socket specified at construction.  The accept method is
     * overridden to delay calling accept on the underlying server socket
     * until a remote service is bound in the registry (when the
     * 'serviceAvailable' flag is 'true').
     **/
    private static class DelayedAcceptServerSocket extends ServerSocket {

        private final ServerSocket serverSocket;

        /**
         * Constructs a 'DelayedAcceptServerSocket' with the specified
         * 'serverSocket'.
         */
        DelayedAcceptServerSocket(ServerSocket serverSocket)
            throws IOException
        {
            this.serverSocket = serverSocket;
        }

        public void bind(SocketAddress endpoint) throws IOException {
            serverSocket.bind(endpoint);
        }

        public void bind(SocketAddress endpoint, int backlog)
                throws IOException
        {
            serverSocket.bind(endpoint, backlog);
        }

        public InetAddress getInetAddress() {
            return serverSocket.getInetAddress();
        }

        public int getLocalPort() {
            return serverSocket.getLocalPort();
        }

        public SocketAddress getLocalSocketAddress() {
            return serverSocket.getLocalSocketAddress();
        }

        /**
         * Delays calling accept on the underlying server socket until the
         * remote service is bound in the registry.
         **/
        public Socket accept() throws IOException {
            synchronized (lock) {
                try {
                    while (!serviceAvailable) {
                        lock.wait();
                    }
                } catch (InterruptedException e) {
                    throw (IOException)
                        (new InterruptedIOException()).initCause(e);
                }
            }
            return serverSocket.accept();
        }

        public void close() throws IOException {
            serverSocket.close();
        }

        public ServerSocketChannel getChannel() {
            return serverSocket.getChannel();
        }

        public boolean isBound() {
            return serverSocket.isBound();
        }

        public boolean isClosed() {
            return serverSocket.isClosed();
        }

        public void setSoTimeout(int timeout)
            throws SocketException
        {
            serverSocket.setSoTimeout(timeout);
        }

        public int getSoTimeout() throws IOException {
            return serverSocket.getSoTimeout();
        }

        public void setReuseAddress(boolean on) throws SocketException {
            serverSocket.setReuseAddress(on);
        }

        public boolean getReuseAddress() throws SocketException {
            return serverSocket.getReuseAddress();
        }

        public String toString() {
            return serverSocket.toString();
        }

        public void setReceiveBufferSize(int size)
            throws SocketException
        {
            serverSocket.setReceiveBufferSize(size);
        }

        public int getReceiveBufferSize()
            throws SocketException
        {
            return serverSocket.getReceiveBufferSize();
        }
    }
}
