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
 * This example shows how to use versioning in serialization to evolve
 * the class to have a new superclass.
 *
 * For example if initially the class structure was:
 *
 * class A {...};
 * class C extends A {...};
 *
 * and the evolved clss structure looks like:
 *
 * class A {...};
 * class B extends A {...};
 * class C extends B {...};
 *
 * then it should be possible to read the old version of C with
 * the new version of C and vice versa.
 *
 * This example demonstrates this.
 *
 * NOTE: In this example, the superclasses (A and B in the above example)
 * implement the Serializable interface. If they did not, then it would
 * be the responsibility of the subclass C to save and restore the fields of
 * A and B. See the relevant example called:
 * "Serialization with a NonSerializable Superclass"
 *
 * Looking at the code/comments is sufficient but if you want to run:
 *
 * How to Run:
 *
 * Compile Original Class:
 *         javac OriginalClass.java
 * Run Original Class with serialization flag:
 *         java OriginalClass -s
 * Compile Evolved Class:
 *         javac EvolvedClass.java
 * Run Evolved Class with deserialization flag:
 *         java EvolvedClass -d
 *
 * This tests compatibility in one direction. Do the same in other
 * direction to see  bidirectional compatibility.
 *
 * Compiled and Tested with JDK1.2
 * This file contains the original class.
 * The original class is in file called EvolvedClass.java
 */

public class OriginalClass {

    /**
     * There are two options: either a user can serialize an object or
     *  deserialize it. (using the -s or -d flag). These options allow
     *  for the demonstration of bidirection readability and writeability
     *  between the original and the evolved class. In other words,
     *  one can serialize an object here and deserialize it with the evolved
     *  class or vice versa.
     */
    public static void main(String args[]) {
        ASubClass corg = new ASubClass(1, "SerializedByOriginalClass");
        ASubClass cnew = null;
        boolean serialize = false;
        boolean deserialize = false;

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

        if (serialize) {
            // Serialize the subclass
            try {
                FileOutputStream fo = new FileOutputStream("tmp");
                ObjectOutputStream so = new ObjectOutputStream(fo);
                so.writeObject(corg);
                so.flush();
            } catch (Exception e) {
                System.out.println(e);
                System.exit(1);
            }
        }


        if (deserialize) {
            // Deserialize the subclass
            try {
                FileInputStream fi = new FileInputStream("tmp");
                ObjectInputStream si = new ObjectInputStream(fi);
                cnew = (ASubClass) si.readObject();

            } catch (Exception e) {
                System.out.println(e);
                System.exit(1);
            }
            System.out.println();
            System.out.println("Printing deserialized class: ");
            System.out.println();
            System.out.println(cnew);
            System.out.println();
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
 * A simple superclass that implements Serializable
 */
class ASuperClass implements Serializable {

    /**
     * @serial
     */
    String name;

    ASuperClass(String name) {
        this.name = name;
    }

    public String toString() {
        return("Name: " + name);
    }
}

/**
 * A simple subclass that implements Serializable and extends
 * a serializable superclass.
 *
 * Again note, if the superclass was not serializable, then it would
 * be the responsibility of this subclass to save and restore the
 * superclass's fields. See example called "Serialization with a
 * NonSerializable Superclass" for more details.
 */
class ASubClass extends ASuperClass implements Serializable {

    /**
     * @serial
     */
    int num;

    ASubClass(int num, String name) {
        super(name);
        this.num = num;
    }

    public String toString() {
        return (super.toString() + "\nNum:  " + num);
    }
}
