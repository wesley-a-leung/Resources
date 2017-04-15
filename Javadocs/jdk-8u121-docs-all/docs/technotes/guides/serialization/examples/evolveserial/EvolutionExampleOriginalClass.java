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
 * When Java objects use serialization to store objects, the potential arises
 * that the version of the class reading the data is different from the version
 * of class that wrote that data

 *
 * This example demonstrates some of the compatible changes that Serialization
 * handles without class-specific methods

 * How to Run:
 *
 * Compile Original Class:
 *         javac EvolutionExampleOriginalClass.java
 * Run Original Class with serialization flag:
 *         java EvolutionExampleOriginalClass -s
 * Compile Evolved Class:
 *         javac EvolutionExampleEvolvedClass.java
 * Run Evolved Class with deserialization flag:
 *         java EvolutionExampleEvolvedClass -d
 *
 * This tests compatibility in one direction only. Perform the same steps in
 * the other direction to see bidirectional compatibility.
 *

 * Compiled and Tested with JDK1.1.4 & JDK1.2
 * This file contains the original class.
 * The evolved class is in file called EvolutionExampleEvolvedClass.java
 */
public class EvolutionExampleOriginalClass {


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

        AClass serializeclass = new AClass(10, "serializedByOriginalClass");
        AClass deserializeclass = null;

        /*
         * Serialize the original class if that's the option chosen
         */
        if (serialize) {
            try {
                FileOutputStream fo = new FileOutputStream("evolve.tmp");
                ObjectOutputStream so = new ObjectOutputStream(fo);
                so.writeObject(serializeclass);
                so.flush();
            } catch (Exception e) {
                System.out.println(e);
                System.exit(1);
            }
        }

        /*
         * Deserialize, if that's the option chosen and print the name
         * of the object, which will allow us to see who serialized the
         * object, the original class or the evolved class file
         */
        if (deserialize) {
            try {
                FileInputStream fi = new FileInputStream("evolve.tmp");
                ObjectInputStream si = new ObjectInputStream(fi);
                deserializeclass = (AClass) si.readObject();
            } catch (Exception e) {
                System.out.println(e);
                System.exit(1);
            }
            /*
             * Print out to see that it is indeed the same object as it was
             * when it was serialized (depending on whether it was the original
             * class that serialized it or the evolved class)
             */
            System.out.println("Now printing deserialized object's name: ");
            System.out.println();
            System.out.println("name: " + deserializeclass.name);
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

/*
 * Original Class
 * will evolve later (see the other file)
 */
class AClass implements Serializable {

    // some data fields.

    /**
     * @serial
     */
    private int num;

    /**
     * @serial
     */
    String name;

    static Hashtable ht = new Hashtable();

    // ...
    // ...
    // ...

    AClass(int n, String s) {
        num = n;
        name = s;
    }

    // some methods...
    // ...
    // ...

    /**
     * These writeObject and readObject merely defaultwriteObject and
     * defaultreadObject - so they don't do anything.. but they are placed
     * here so that we can show that we can remove them in the evolved
     * class without any effect.
     *
     * @serialData Write serializable fields. No optional data written.
     */
    private void writeObject(ObjectOutputStream s)
        throws IOException {
        s.defaultWriteObject();
    }

    /**
     * readObject - just calls defaultreadObject()
     *
     * @serialData Read serializable fields. No optional data read.
     */
    private void readObject(ObjectInputStream s)
        throws IOException, ClassNotFoundException {
            s.defaultReadObject();
    }
}
