/*
 * Copyright (c) 2002, Oracle and/or its affiliates. All rights reserved.
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
 * EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY
 * DAMAGES OR LIABILITIES  SUFFERED BY LICENSEE AS A RESULT OF OR
 * RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR
 * ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE
 * FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT,
 * SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF
 * THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed, licensed or
 * intended for use in the design, construction, operation or
 * maintenance of any nuclear facility.
 */

import java.io.*;
import java.net.*;
import java.util.*;


/**
 * This example shows how to use sockets to send and receive objects.
 * This file contains the class Server, which does the receiving of objects
 * from class WriteSocket in file WriteSocket.java
 * The Server has to run first and wait for the WriteSocket
 * to send the information.
 *
 * Compiled and Tested with JDK1.1 & JDK1.2
 */
public class Server {

    /**
     * Create the serversocket and use its stream to receive serialized objects
     */
    public static void main(String args[]) {

      ServerSocket ser = null;
      Socket soc = null;
      String str = null;
      Date d = null;

      try {
        ser = new ServerSocket(8020);
        /*
         * This will wait for a connection to be made to this socket.
         */
        soc = ser.accept();
        InputStream o = soc.getInputStream();
        ObjectInput s = new ObjectInputStream(o);
        str = (String) s.readObject();
        d = (Date) s.readObject();
        s.close();

        // print out what we just received
        System.out.println(str);
        System.out.println(d);
      } catch (Exception e) {
          System.out.println(e.getMessage());
          System.out.println("Error during serialization");
          System.exit(1);
      }
    }
}
