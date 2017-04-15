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
import java.net.*;
import java.nio.*;
import java.nio.channels.*;
import java.nio.charset.*;
import java.util.regex.*;

/* Ask a list of hosts what time it is.  Demonstrates NIO socket channels
 * (connection and reading), buffer handling, charsets, and regular
 * expressions.
 */

public class TimeQuery {

    // The standard daytime port
    private static int DAYTIME_PORT = 13;

    // The port we'll actually use
    private static int port = DAYTIME_PORT;

    // Charset and decoder for US-ASCII
    private static Charset charset = Charset.forName("US-ASCII");
    private static CharsetDecoder decoder = charset.newDecoder();

    // Direct byte buffer for reading
    private static ByteBuffer dbuf = ByteBuffer.allocateDirect(1024);

    // Ask the given host what time it is
    //
    private static void query(String host) throws IOException {
  InetSocketAddress isa
      = new InetSocketAddress(InetAddress.getByName(host), port);
  SocketChannel sc = null;

  try {

      // Connect
      sc = SocketChannel.open();
      sc.connect(isa);

      // Read the time from the remote host.  For simplicity we assume
      // that the time comes back to us in a single packet, so that we
      // only need to read once.
      dbuf.clear();
      sc.read(dbuf);

      // Print the remote address and the received time
      dbuf.flip();
      CharBuffer cb = decoder.decode(dbuf);
      System.out.print(isa + " : " + cb);

  } finally {
      // Make sure we close the channel (and hence the socket)
      if (sc != null)
    sc.close();
  }
    }

    public static void main(String[] args) {
  if (args.length < 1) {
      System.err.println("Usage: java TimeQuery [port] host...");
      return;
  }
  int firstArg = 0;

  // If the first argument is a string of digits then we take that
  // to be the port number
  if (Pattern.matches("[0-9]+", args[0])) {
      port = Integer.parseInt(args[0]);
      firstArg = 1;
  }

  for (int i = firstArg; i < args.length; i++) {
      String host = args[i];
      try {
    query(host);
      } catch (IOException x) {
    System.err.println(host + ": " + x);
      }
  }
    }

}