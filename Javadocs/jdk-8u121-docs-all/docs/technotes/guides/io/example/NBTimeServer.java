/*
 * Copyright (c) 2001, 2014, Oracle and/or its affiliates. All rights reserved.
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

import java.io.*;
import java.nio.*;
import java.nio.channels.*;
import java.nio.channels.spi.*;
import java.net.*;
import java.util.*;

/* A non-blocking Internet time server implemented using
 * the I/O (NIO) facilities added to Java SE 1.4.
 */

// Listen on a port for connections and write back the current time.
public class NBTimeServer {
    private static final int DEFAULT_TIME_PORT = 8900;

    // Constructor with no arguments creates a time server on default port.
    public NBTimeServer() throws Exception {
  acceptConnections(this.DEFAULT_TIME_PORT);
    }

    // Constructor with port argument creates a time server on specified port.
    public NBTimeServer(int port) throws Exception {
  acceptConnections(port);
    }

    // Accept connections for current time. Lazy Exception thrown.
    private static void acceptConnections(int port) throws Exception {
  // Selector for incoming time requests
  Selector acceptSelector = SelectorProvider.provider().openSelector();

  // Create a new server socket and set to non blocking mode
  ServerSocketChannel ssc = ServerSocketChannel.open();
  ssc.configureBlocking(false);

  // Bind the server socket to the local host and port

  InetAddress lh = InetAddress.getLocalHost();
  InetSocketAddress isa = new InetSocketAddress(lh, port);
  ssc.socket().bind(isa);

  // Register accepts on the server socket with the selector. This
  // step tells the selector that the socket wants to be put on the
  // ready list when accept operations occur, so allowing multiplexed
  // non-blocking I/O to take place.
  SelectionKey acceptKey = ssc.register(acceptSelector,
                SelectionKey.OP_ACCEPT);

  int keysAdded = 0;

  // Here's where everything happens. The select method will
  // return when any operations registered above have occurred, the
  // thread has been interrupted, etc.
  while ((keysAdded = acceptSelector.select()) > 0) {
      // Someone is ready for I/O, get the ready keys
      Set readyKeys = acceptSelector.selectedKeys();
      Iterator i = readyKeys.iterator();

      // Walk through the ready keys collection and process date requests.
      while (i.hasNext()) {
    SelectionKey sk = (SelectionKey)i.next();
    i.remove();
    // The key indexes into the selector so you
    // can retrieve the socket that's ready for I/O
    ServerSocketChannel nextReady =
        (ServerSocketChannel)sk.channel();
    // Accept the date request and send back the date string
    Socket s = nextReady.accept().socket();
    // Write the current time to the socket
                PrintWriter out = new PrintWriter(s.getOutputStream(), true);
    Date now = new Date();
    out.println(now);
    out.close();
      }
  }
    }

    // Entry point.
    public static void main(String[] args) {
  // Parse command line arguments and
  // create a new time server (no arguments yet)
  try {
    NBTimeServer nbt = new NBTimeServer();
  } catch(Exception e) {
    e.printStackTrace();
  }
    }
}