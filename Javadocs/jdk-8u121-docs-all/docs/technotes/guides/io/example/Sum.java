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

/* Compute 16-bit checksums for a list of files, in the style of the
 * BSD "sum" command.  Uses NIO mapped byte buffers for speed.
 */

public class Sum {

    // Compute a 16-bit checksum for all the remaining bytes
    // in the given byte buffer
    //
    private static int sum(ByteBuffer bb) {
  int sum = 0;
  while (bb.hasRemaining()) {
      if ((sum & 1) != 0)
    sum = (sum >> 1) + 0x8000;
      else
    sum >>= 1;
      sum += bb.get() & 0xff;
      sum &= 0xffff;
  }
  return sum;
    }

    // Compute and print a checksum for the given file
    //
    private static void sum(File f) throws IOException {

  // Open the file and then get a channel from the stream
  FileInputStream fis = new FileInputStream(f);
  FileChannel fc = fis.getChannel();

  // Get the file's size and then map it into memory
  int sz = (int)fc.size();
  MappedByteBuffer bb = fc.map(FileChannel.MapMode.READ_ONLY, 0, sz);

  // Compute and print the checksum
  int sum = sum(bb);
  int kb = (sz + 1023) / 1024;
  String s = Integer.toString(sum);
  System.out.println(s + "\t" + kb + "\t" + f);

  // Close the channel and the stream
  fc.close();
    }

    public static void main(String[] args) {
  if (args.length < 1) {
      System.err.println("Usage: java Sum file...");
      return;
  }
  for (int i = 0; i < args.length; i++) {
      File f = new File(args[i]);
      try {
    sum(f);
      } catch (IOException x) {
    System.err.println(f + ": " + x);
      }
  }
    }

}