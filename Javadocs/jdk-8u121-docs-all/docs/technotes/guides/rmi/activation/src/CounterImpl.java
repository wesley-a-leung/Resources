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

package examples.activation;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.rmi.MarshalledObject;
import java.rmi.activation.Activatable;
import java.rmi.activation.ActivationID;

public class CounterImpl implements Counter {
    private RandomAccessFile raf;
    private int count;
    private final Object countLock = new Object();

    /**
     * Constructs a <code>CounterImpl</code> instance with the specified
     * activation ID and data.  This constructor is called during
     * activation to construct the object.  The data should contain the
     * name of a file used in keeping track of this object's invocation
     * count.
     **/
    public CounterImpl(ActivationID id, MarshalledObject data)
        throws Exception
    {
        /*
         * Obtain the file name from the supplied marshalled data, and
         * initialize the count value, read from the file.  Updating
         * the count value must be in a synchronized block because multiple
         * threads may need to access it.
         */
        if (data != null) {
            String filename = (String) data.get();
            synchronized (countLock) {
                count = openFile(filename);
            }
            System.err.println("count upon activation = " + count);
        }

        /*
         * Export the activatable object on an anonymous port.  The object
         * is exported after the count is read to prevent clients from
         * invoking the 'increment' method before initialization completes.
         */
        Activatable.exportObject(this, id, 0);
    }

    /**
     * Opens the specified file, and reads and returns the current count
     * value.  If the file does not exist, the file is created with a count
     * value of 0x00.  Any exceptions encountered while using the file are
     * propagated to the caller.
     **/
    private int openFile(String filename) throws IOException {

        if (filename != null && !filename.equals("")) {
            File file = new File(filename);
            boolean fileExists = file.exists();
            raf = new RandomAccessFile(file, "rws");
            return (fileExists) ? raf.readInt() : writeCount(0);

        } else {
            throw new IOException("invalid filename");
        }
    }

    /**
     * Writes the specified count value to the file, and returns the value
     * written.
     *
     * Implementation note: this simple implementation could lose the count
     * value if a crash occurs after the call to 'setLength' succeeds, but
     * before the 'writeInt' call completes.  A more robust implementation
     * would save the previous count value so that it could be recovered in
     * the event of a crash.
     **/
    private int writeCount(int value) throws IOException {
        raf.setLength(0);
        raf.writeInt(value);
        return value;
    }

    /**
     * Increments the count value, stores it persistently, and returns the
     * result.
     *
     * @throws IOException if an exception occurs while storing the count
     * value
     **/
    public int increment() throws IOException {
        synchronized (countLock) {
            return writeCount(++count);
        }
    }
}
