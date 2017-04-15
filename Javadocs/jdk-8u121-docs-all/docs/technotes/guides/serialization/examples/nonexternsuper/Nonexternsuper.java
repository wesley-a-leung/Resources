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

/**
 * When using the Externalizable Interface, an externalizable object
 * must implement a writeExternal method to save the state of the object.
 * It must also explicitly coordinate with its supertype to save its state.

 * This simple example shows how to do this for an object whose supertype
 * is NOT externalizable.
 *
 *
 *   How to Run:
 *             Compile the file:  javac Nonexternsuper.java
 *             Then run: java Nonexternsuper.java
 *
 * This should print out a book object before and after serialization.
 *
 *
 * Tested and compiled on JDK 1.1 & Java 2 SDK, v1.2.
 */

public class Nonexternsuper {

    /**
     * Create an Book (subclass of reading material) object, serialize it,
     * deserialize it and see that they are the same. So, basically test that
     * this Externalizable example's works
     */
    public static void main(String args[]) {

        // create a Book object
        Book bookorg = new Book(100, "How to Serialize", true, "R.R", "Serialization", 97);
        Book booknew = null;

        //serialize the book
        try {
            FileOutputStream fo = new FileOutputStream("tmp");
            ObjectOutputStream so = new ObjectOutputStream(fo);
            so.writeObject(bookorg);
            so.flush();
        } catch (Exception e) {
            System.out.println(e);
            System.exit(1);
        }

        // de-serialize the Book
        try {
            FileInputStream fi = new FileInputStream("tmp");
            ObjectInputStream si = new ObjectInputStream(fi);
            booknew = (Book) si.readObject();
        }
        catch (Exception e) {
            System.out.println(e);
            System.exit(1);
        }

        /*
         * Print out the original and new book information
         * It should be the same if we did everything correctly!
         */
        System.out.println();
        System.out.println("Printing original book...");
        System.out.println(bookorg);
        System.out.println("Printing new book... ");
        System.out.println(booknew);
        System.out.println("Both original and new should be the same!");
        System.out.println();
    }
}
