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
 * The subclass that implements Externalizable.. responsible for
 * saving the state of its non-Externalizable super class
 */
class Book extends ReadingMaterial implements Externalizable {

    int numpages;
    String name;
    boolean ishardcover;

    // other relevant information and methods
    // .
    // .
    // .

    /*
     * mandatory public no-arg constructor
     * if the superclass did not have a no-arg constructor, we would have to
     * give the regular constructor default values.
     */
    public Book() { super(); }

    Book(int pages, String n, boolean hardcover, String author,
         String subject, int yearwritten) {

        super(author, subject, yearwritten);
        numpages = pages;
        name = n;
        ishardcover = hardcover;
    }

    /**
     * Mandatory writeExernal method.
     *
     * @serialData Expicitly saves the supertype's fields and then saves this
     *             class fields. Writes superclass fields in following order:
     *             author as an object, subject as an object, yearwritten
     *             as an integer. Writes class fields in following order:
     *             numpages as an int, name as an Object and ishardcover
     *             as a boolean.
     */
    public void writeExternal(ObjectOutput out) throws IOException  {

        /*
         * Since the superclass does not implement the Serializable interface
         * we explicitly do the saving... Since these fields are not private
         * we can access them directly. If they were private, the superclass
         * would have to implement get methods that would allow the
         * subclass this necessary access for proper saving.
         */
        out.writeObject(author);
        out.writeObject(subject);
        out.writeInt(yearwritten);

        // now we take care of this subclass's fields
        out.writeInt(numpages);
        out.writeObject(name);
        out.writeBoolean(ishardcover);
    }

    /**
     * Mandatory readExternal method. Explicitly restores the supertype's
     * fields and then restores
     *  this class's fields
     */
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {

        /*
         * Since the superclass does not implement the Serializable interface
         * we explicitly do the restoring... Since these fields are not private
         * we can access them directly. If they were private, the superclass
         * would have to implement set methods that would allow the
         * subclass this necessary access for proper restoring.
         */
        author = (String) in.readObject();
        subject = (String) in.readObject();
        yearwritten = in.readInt();

        // now we take care of the subclass's fields
        numpages = in.readInt();
        name = (String) in.readObject();
        ishardcover = in.readBoolean();
    }

    /**
     * Prints out the fields. used for testing!
     */
    public String toString() {
        return("Name: " + name + "\n" + "Author: " + author + "\n" + "Pages: "
+ numpages + "\n" + "Subject: " + subject + "\n" + "Year: " + yearwritten + "\n");
    }
}
