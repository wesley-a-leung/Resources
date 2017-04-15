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
 * The Externalizable subclass.
 * Will save the state of its superclass by calling the superclass's
 * writeExternal and readExternal Methods from within its own writeExternal
 * and readExternal methods
 */
public class Book extends ReadingMaterial implements Externalizable {

    private int numpages;
    private String name;
    private boolean ishardcover;

    // other relevant information and methods
    // .
    // .
    // .

    /**
     * mandatory public no-arg constructor
     */
    public Book() {
        super(); }

    public Book(int pages, String n, boolean hardcover, String author,
         String subject, int yearwritten) {

        super(author, subject, yearwritten);
        numpages = pages;
        name = n;
        ishardcover = hardcover;
    }

    /**
     * Mandatory writeExternal method.
     *
     * @serialData Saves state of its superclass by
     *             calling its writeExternal method and
     *             then save its own fields. Writes numpages field as int,
     *             name field as an object and ishardcover field as a boolean.
     *
     * @see ReadingMaterial#writeExternal(ObjectOutput)
     */
    public void writeExternal(ObjectOutput out) throws IOException {

        // first we call the writeExternal of the superclass as to write
        // all the superclass data fields
        super.writeExternal(out);

        // now we take care of this class's fields
        out.writeInt(numpages);
        out.writeObject(name);
        out.writeBoolean(ishardcover);
    }

    /**
     * Mandatory readExternal method. Will read in the data that we wrote out
     * in the writeExternal method. Restores the state of the superclass
     * first by calling the superclass's readExternal method. Then, restores
     * its own fields. These fields MUST BE IN THE SAME ORDER AND TYPE as we
     * wrote them out.
     * By the time, readExternal is called, an object of this class has already
     * been created using the public no-arg constructor, so this method is
     * used to restore the data to all of the fields of the newly created
     * object.
     */
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {

      // first call the superclass external method
    super.readExternal(in);

    // now take care of this subclass's fields
    numpages = in.readInt();
    name = (String)in.readObject();
    ishardcover= in.readBoolean();
  }

    /**
     * Prints out the fields. used for testing!
     */
    public String toString() {
        return("Name: " + name + "\n" + "Author: " + super.getAuthor() + "\n" + "Pages: "
+ numpages + "\n" + "Subject: " + super.getSubject() + "\n" + "Year: " + super.getYearwritten() + "\n" );
    }
}
