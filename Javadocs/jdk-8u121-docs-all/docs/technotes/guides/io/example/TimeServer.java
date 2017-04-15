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
import java.util.*;
import java.util.regex.*;

/* Listen for connections and tell callers what time it is.
 * Demonstrates NIO socket channels (accepting and writing),
 * buffer handling, charsets, and regular expressions.
 */

public class TimeServer {

    // We can't use the normal daytime port (unless we're running as root,
    // which is unlikely), so we use this one instead
    private static int PORT = 8013;

    // The port we'll actually use
    private static int port = PORT;

    // Charset and encoder for US-ASCII
    private static Charset charset = Charset.forName("US-ASCII");
    private static CharsetEncoder encoder = charset.newEncoder();

    // Direct byte buffer for writing
    private static ByteBuffer dbuf = ByteBuffer.allocateDirect(1024);


    // Open and bind the server-socket channel
    //
    private static ServerSocketChannel setup() throws IOException {
  ServerSocketChannel ssc = ServerSocketChannel.open();
  InetSocketAddress isa
      = new InetSocketAddress(InetAddress.getLocalHost(), port);
  ssc.socket().bind(isa);
  return ssc;
    }

    // Service the next request to come in on the given channel
    //
    private static void serve(ServerSocketChannel ssc) throws IOException {
  SocketChannel sc = ssc.accept();
  try {
      String now = new Date().toString();
      sc.write(encoder.encode(CharBuffer.wrap(now + "\r\n")));
      System.out.println(sc.socket().getInetAddress() + " : " + now);
      sc.close();
  } finally {
      // Make sure we close the channel (and hence the socket)
      sc.close();
  }
    }

    public static void main(String[] args) throws IOException {
  if (args.length > 1) {
      System.err.println("Usage: java TimeServer [port]");
      return;
  }

  // If the first argument is a string of digits then we take that
  // to be the port number
  if ((args.length == 1) && Pattern.matches("[0-9]+", args[0]))
      port = Integer.parseInt(args[0]);

  ServerSocketChannel ssc = setup();
  for (;;)
      serve(ssc);

    }

}