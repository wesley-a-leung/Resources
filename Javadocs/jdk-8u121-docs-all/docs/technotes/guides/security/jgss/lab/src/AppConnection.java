/*
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistributions of source code must retain the above copyright
 * notice, this  list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduct the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
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
 * DAMAGES OR LIABILITIES  SUFFERED BY LICENSEE AS A RESULT OF  OR
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
import java.net.Socket;

class AppConnection {
    public static final int AUTH_CMD = 100;
    public static final int DATA_CMD = 200;

    public static final int SUCCESS = 0;
    public static final int AUTH_INPROGRESS = 1;
    public static final int FAILURE = 2;

    private DataInputStream inStream;
    private DataOutputStream outStream;
    private Socket socket;

    // Client application
    AppConnection(String hostName, int port) throws IOException {
        socket = new Socket(hostName, port);

        inStream = new DataInputStream(socket.getInputStream());
        outStream = new DataOutputStream(socket.getOutputStream());

        System.out.println("Connected to address " +
            socket.getInetAddress());
    }

    // Server side application
    AppConnection(Socket socket) throws IOException {
        this.socket = socket;
        inStream = new DataInputStream(socket.getInputStream());
        outStream = new DataOutputStream(socket.getOutputStream());

        System.out.println("Got connection from client " +
            socket.getInetAddress());
    }

    byte[] receive(int expected) throws IOException {
        if (expected != -1) {
            int cmd = inStream.readInt();
            if (expected != cmd) {
                throw new IOException("Received unexpected code: " + cmd);
            }
            //System.out.println("Read cmd: " + cmd);
        }

        byte[] reply = null;
        int len;
        try {
            len = inStream.readInt();
            //System.out.println("Read length: " + len);

        } catch (IOException e) {
            len = 0;
        }
        if (len > 0) {
            reply = new byte[len];
            inStream.readFully(reply);
        } else {
            reply = new byte[0];
        }
        return reply;
    }

    AppReply send(int cmd, byte[] bytes) throws IOException {
        //System.out.println("Write cmd: " + cmd);
        outStream.writeInt(cmd);
        if (bytes != null) {
            //System.out.println("Write length: " + bytes.length);
            outStream.writeInt(bytes.length);
            if (bytes.length > 0) {
                outStream.write(bytes);
            }
        } else {
            //System.out.println("Write length: " + 0);
            outStream.writeInt(0);
        }

        outStream.flush();

        if (cmd == SUCCESS || cmd == FAILURE) {
            return null;   // Done
        }

        int returnCode = inStream.readInt();
        //System.out.println("Read cmd: " + returnCode);

        byte[] reply = null;
        if (returnCode != FAILURE) {
            reply = receive(-1);
        }
        return new AppReply(returnCode, reply);
    }

    static class AppReply {
        private int code;
        private byte[] bytes;

        AppReply(int code, byte[] bytes) {
            this.bytes = bytes;
            this.code = code;
        }

        int getStatus() {
            return code;
        }

        byte[] getBytes() {
            return bytes;
        }
    }

    void close() {
        try {
            socket.close();
        } catch (IOException e) {
        }
    }
}
