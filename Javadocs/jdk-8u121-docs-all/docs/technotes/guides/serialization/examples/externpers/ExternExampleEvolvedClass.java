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
import java.util.*;

/**
 * This is a continuation of the Externalizable Persistence Example.
 * This file contains the evolved version of the ExternVersioningClass.
 * Of course, normally, the evolved class would actually take place of the
 * original class but we just keep both of them to demonstrate the evolution
 *
 * For information on how to run, see file ExternExampleOriginalClass.java
 */
public class ExternExampleEvolvedClass {

    /**
     *  There are two options: either a user can serialize an object or
     *  deserialize it. (using the -s or -d flag). These options allow
     *  for the demonstration of bidirection readability and writeability
     *  between the original and the evolved class. In other words,
     *  one can serialize an object here and deserialize it with the evolved
     *  class or vice versa.
     */
    public static void main(String args[]) {

        boolean serialize = false;
        boolean deserialize = false;

        ExternVersioningClass wobj = new ExternVersioningClass
            (1, "evolvedclass", true, 1.23);
        ExternVersioningClass robj = null;
        /*
         * see if we are serializing or deserializing.
         * The ability to deserialize or serialize allows
         * us to see the bidirectional readability and writeability
         */
        if (args.length == 1) {
            if (args[0].equals("-d")) {
                deserialize = true;
                } else if (args[0].equals("-s")) {
                    serialize = true;
                } else {
                    usage();
                    System.exit(0);
                }
        } else {
            usage();
            System.exit(0);
        }

        /*
         * serialize, if that's the chosen option
         */
        if (serialize) {
            try {
                FileOutputStream fo = new FileOutputStream("evolve.tmp");
                ObjectOutputStream so = new ObjectOutputStream(fo);
                so.writeObject(wobj);
                so.flush();
            } catch (Exception e) {
                System.out.println(e);
                System.exit(1);
            }
        }

        /*
         * deserialize, if that's the chosen option
         */
        if (deserialize) {
            try {
                FileInputStream fi = new FileInputStream("evolve.tmp");
                ObjectInputStream si = new ObjectInputStream(fi);
                robj = (ExternVersioningClass) si.readObject();
            } catch (Exception e) {
                System.out.println(e);
                System.exit(1);
            }
        }
    }

    /**
     * Prints out the usage
     */
    static void usage() {
        System.out.println("Usage:");
        System.out.println("      -s (in order to serialize)");
        System.out.println("      -d (in order to deserialize)");
    }
}


/**
 * The evolved class!
 */
class ExternVersioningClass implements Externalizable  {

    /*
     * Each versioned class must identify the original class version from which
     * it evolved. This SUID is obtained using the command serialver on the
     * original class
     */
    static final long serialVersionUID = -6527577423406625824L;
    public static final int version = 2;

    // the data that was part of the original class
    int dimension;
    int array[];
    String name;

    // new data
    boolean b;
    double d;

    // ***need to have a public-no-arg constructor***
    public ExternVersioningClass() {}

    ExternVersioningClass(int dim, String n, boolean boo, double dou){
        // initialize
        dimension = dim;
        array = new int[dimension];
        name = n;
        b = boo;
        d = dou;
    }

    /**
     * the mandatory writeExternal method. First writes out the version number
     * (ie. 2 because this is the evolved class.) Then writes out the fields
     * that were a part of the original class and then the fields that are
     * in this class. (since the original class assumes that the new data
     * fields are appended)
     *
     * @serialData Writes out the int "class version number" 2. Next, the
     *             dimension field is written as an int. Followed by
     *             the array field written as an object. The
     *             name field is written as an object. The new b field is
     *             written as a boolean. Lastly, the new d field is written
     *             as a double.
     */
    public void writeExternal(ObjectOutput out) throws IOException {

        // we first write out the class version number (ie. # 2)
        out.writeInt(version);

        // We have to first write out the data of the original class because
        // it assumes that we appended the new data.
        out.writeInt(dimension);
        out.writeObject(array);
        out.writeObject(name);

        // the original class will ignore this data.
        out.writeBoolean(b);
        out.writeDouble(d);
    }

    /**
     * mandatory readExternal method.
     *
     * @serialData First reads in the version number and
     *             then if the version number suggests that the object
     *             was written as an original class, gives the new fields
     *             default values but if the object was written by this
     *             evolved class, then reads in all the fields accordingly.
     */
    public void readExternal(ObjectInput in)
        throws IOException, java.lang.ClassNotFoundException
    {
        int version = in.readInt();
        System.out.println
            ("Reading an Object written by Version #: " + version);

        dimension = in.readInt();

        // need to allocate memory for the array we will read in
        array = (int[]) in.readObject();
        name = (String) in.readObject();


        /*
         * only if the object was written out by the evolved class, do we
         * read in the rest of the data.
         */
        if (version == 2) {
            b = in.readBoolean();
            d = in.readDouble();
        }

        // otherwise, we give it default values
        else if(version == 1) {
            b = true;
            d = 1.00;
        }
    }
}
