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
 * The Externalizable Superclass:
 *  When the Externalizable Subclass Book implements its writeExternal and
 *  readExternal Methods, it needs to save the state of its superclass using
 * the superclass's writeExternal and readExternal Methods
 */
public class ReadingMaterial implements Externalizable {

    /*
     * In order for us to be able to serialize this data, these must be
     * either serializable/externalizable objects or primitive data types.
     */
    private  String author;
    private  String subject;
    private  int yearwritten;

    // other relevant data and methods
    // .
    // .
    // .

    /*
     * Must have a public no-arg constructor when implementing Externalizable
     */
    public ReadingMaterial() {}

    /**
     * Initialize the fields
     *
     */
    public ReadingMaterial(String auth, String sub, int year) {
        author = auth;
        subject = sub;
        yearwritten = year;
    }

    /**
     * A public field access method, since the data fields are private and
     * will need to be accessed by the subclass to print them or use them
     * otherwise.
     */
    public String getAuthor() {
        return author; }
    /**
     * A field access method, since the data fields are private and will need
     * to be accessed by the subclass to print them or use them otherwise.
     */
    public String getSubject() {
        return subject; }
    /**
     * A field access method, since the data fields are private and will need
     * to be accessed by the subclass to print them or use them otherwise.
     */
    public int getYearwritten() {
        return yearwritten; }

    /**
     * Mandatory writeExternal method.
     * @serialData Write author and subject field as objects and then write
     *             yearwritten field as an integer.
     */
    public void writeExternal(ObjectOutput out) throws IOException {

        out.writeObject(author);
        out.writeObject(subject);
        out.writeInt(yearwritten);
  }

    /**
     * Mandatory readExternal method. Will read in the data that we wrote out
     * in the writeExternal method. MUST BE IN THE SAME ORDER and type as we
     * wrote it out. By the time, readExternal is called, an object of this
     * class has already been created using the public no-arg constructor,
     * so this method is used to restore the data to all of the fields of the
     * newly created object.
     */
  public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {

    author = (String)in.readObject();
    subject = (String)in.readObject();
    yearwritten = in.readInt();
  }
}
