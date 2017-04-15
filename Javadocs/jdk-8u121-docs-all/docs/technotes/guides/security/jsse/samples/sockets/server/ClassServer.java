/*
 *
 * Copyright (c) 1994, 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistribution of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistribution in binary form must reproduce the above copyright
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
 * EXCLUDED. SUN MICROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL
 * NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT
 * OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR
 * ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT,
 * SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF
 * THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF SUN HAS
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that this software is not designed, licensed or
 * intended for use in the design, construction, operation or
 * maintenance of any nuclear facility.
 */

import java.io.*;
import java.net.*;
import javax.net.*;

/*
 * ClassServer.java -- a simple file server that can serve
 * Http get request in both clear and secure channel
 */

/**
 * Based on ClassServer.java in tutorial/rmi
 */
public abstract class ClassServer implements Runnable {

    private ServerSocket server = null;
    /**
     * Constructs a ClassServer based on <b>ss</b> and
     * obtains a file's bytecodes using the method <b>getBytes</b>.
     *
     */
    protected ClassServer(ServerSocket ss)
    {
        server = ss;
        newListener();
    }

    /**
     * Returns an array of bytes containing the bytes for
     * the file represented by the argument <b>path</b>.
     *
     * @return the bytes for the file
     * @exception FileNotFoundException if the file corresponding
     * to <b>path</b> could not be loaded.
     * @exception IOException if error occurs reading the class
     */
    public abstract byte[] getBytes(String path)
        throws IOException, FileNotFoundException;

    /**
     * The "listen" thread that accepts a connection to the
     * server, parses the header to obtain the file name
     * and sends back the bytes for the file (or error
     * if the file is not found or the response was malformed).
     */
    public void run()
    {
        Socket socket;

        // accept a connection
        try {
            socket = server.accept();
        } catch (IOException e) {
            System.out.println("Class Server died: " + e.getMessage());
            e.printStackTrace();
            return;
        }

        // create a new thread to accept the next connection
        newListener();

        try {
            OutputStream rawOut = socket.getOutputStream();

            PrintWriter out = new PrintWriter(
                                new BufferedWriter(
                                new OutputStreamWriter(
                                rawOut)));
            try {
                // get path to class file from header
                BufferedReader in =
                    new BufferedReader(
                        new InputStreamReader(socket.getInputStream()));
                String path = getPath(in);
                // retrieve bytecodes
                byte[] bytecodes = getBytes(path);
                // send bytecodes in response (assumes HTTP/1.0 or later)
                try {
                    out.print("HTTP/1.0 200 OK\r\n");
                    out.print("Content-Length: " + bytecodes.length +
                                   "\r\n");
                    out.print("Content-Type: text/html\r\n\r\n");
                    out.flush();
                    rawOut.write(bytecodes);
                    rawOut.flush();
                } catch (IOException ie) {
                    ie.printStackTrace();
                    return;
                }

            } catch (Exception e) {
                e.printStackTrace();
                // write out error response
                out.println("HTTP/1.0 400 " + e.getMessage() + "\r\n");
                out.println("Content-Type: text/html\r\n\r\n");
                out.flush();
            }

        } catch (IOException ex) {
            // eat exception (could log error to log file, but
            // write out to stdout for now).
            System.out.println("error writing response: " + ex.getMessage());
            ex.printStackTrace();

        } finally {
            try {
                socket.close();
            } catch (IOException e) {
            }
        }
    }

    /**
     * Create a new thread to listen.
     */
    private void newListener()
    {
        (new Thread(this)).start();
    }

    /**
     * Returns the path to the file obtained from
     * parsing the HTML header.
     */
    private static String getPath(BufferedReader in)
        throws IOException
    {
        String line = in.readLine();
        String path = "";
        // extract class from GET line
        if (line.startsWith("GET /")) {
            line = line.substring(5, line.length()-1).trim();
            int index = line.indexOf(' ');
            if (index != -1) {
                path = line.substring(0, index);
            }
        }

        // eat the rest of header
        do {
            line = in.readLine();
        } while ((line.length() != 0) &&
                 (line.charAt(0) != '\r') && (line.charAt(0) != '\n'));

        if (path.length() != 0) {
            return path;
        } else {
            throw new IOException("Malformed Header");
        }
    }
}
