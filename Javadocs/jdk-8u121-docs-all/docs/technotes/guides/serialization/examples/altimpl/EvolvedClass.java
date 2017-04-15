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
import java.awt.*;

/**
 *
 * This example shows how to use the Serializable Fields API with
 * Serialization, demonstrating that the class can define fields
 * other than those already in the class to be serializable. This
 * differs from just rewriting the writeObject method to customize
 * the data format (see the Custom Data Format example) because,
 * in this example, versioning support still holds.
 *
 * Using the Serializable Fields API, this example specifically
 * changes the internal representation of a rectangle from
 * x1,y1,x2,y2 implementation (see Original Class) to Point(x1,y1),
 * Point(x2,y2) (see Evolved Class) while the external representation
 * still remains x1, y1, x2, y2. This ensures bidirectional compatibility
 * between the original and evolved representations.
 *
 * The original rectangle class (in OriginalClass.java) consists of four
 * integers (x1, y1, x2, y2). Instead of four integers, the evolved rectangle
 * class (in this file) has two Points as fields.  In order for this
 * evolved class to fulfill its contract with the original class, the
 * evolved class saves its fields as four integers (x1, y1, x2, y2)
 * instead of two points.  By doing this the evolved class ensures
 * bidirectional compatibility with the original class.
 *
 * To see how to run this: see OriginalClass.java
 *
 *  Compiled and Tested with JDK1.2
 */

public class EvolvedClass {

    /**
     *  There are two options: either a user can serialize an object or
     *  deserialize it. (using the -s or -d flag). These options allow
     *  for the demonstration of bidirection readability and writeability
     *  between the original and the evolved class. In other words,
     *  one can serialize an object here and deserialize it with the evolved
     *  class or vice versa.
     */
    public static void main(String args[]) {

        ARectangle orgClass = new ARectangle(100, 100, 102, 102);
        ARectangle newClass = null;

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

        /*
         * Serialize the original class if that's the option chosen
         */
        if (serialize) {
            try {
                FileOutputStream fo = new FileOutputStream("evolve.tmp");
                ObjectOutputStream so = new ObjectOutputStream(fo);
                so.writeObject(orgClass);
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
                newClass = (ARectangle) si.readObject();
            } catch (Exception e) {
                System.out.println(e);
                System.exit(1);
            }
            System.out.println("Now printing deserialized object: ");
            System.out.println();
            System.out.println(newClass);
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
 * The evolved Rectangle Class. Interally consists of two fields of type
 * Point but externally is still 4 integers (so that it is compatible
 * with the original rectangle class)
 *
 * In order to make this possible, we need to use the Serializable
 * Field API so that we can define serializable fields that are
 * not part of the implementation class.
 */
class ARectangle implements java.io.Serializable {

    // new rectangle representation

    /**
     * First of two points forming diagonal of rectangle.
     *
     * Note that this field is not a default serializable field
     * due to the use of serialPersistentFields member within this class.
     */
    Point point1;


    /**
     * Second of two points forming diagonal of rectangle.
     *
     * Note that this field is not a default serializable field
     * due to the use of serialPersistentFields member within this class.
     */
    Point point2;

    /*
     * mandatory SUID field for an evolved Serializable class.
     * serialVersionUID is gotten by doing the serialver command
     * on the original class:
     *                  serialver ARectangle (the original rectangle)
     */
    static final long serialVersionUID = 9030593813711490592L;


    /**
     * The special member, serialPeristentFields, explicitly declares
     * Serializable fields for this class. This allows for fields other
     * than the fields in the class to be persistent. Since we want to
     * save the state of the two Points point1 and point2, we declare
     * the 4 ints as the serial persistent fields
     *
     * @serialField x1  Integer
     *              X-coordinate of point 1 of diagonal points of rectangle.
     * @serialField y1  Integer
     *              Y-coordinate of point 1 of diagonal points of rectangle.
     * @serialField x2  Integer
     *              X-coordinate of point 2 of diagonal points of rectangle.
     * @serialField y2  Integer
     *              Y-coordinate of point 2 of diagonal points of rectangle.
     */
    private static final ObjectStreamField[] serialPersistentFields = {
        new ObjectStreamField("x1", Integer.TYPE),
        new ObjectStreamField("y1", Integer.TYPE),
        new ObjectStreamField("x2", Integer.TYPE),
        new ObjectStreamField("y2", Integer.TYPE)
        };

    ARectangle(int x1, int y1, int x2, int y2) {
        point1 = new Point(x1, y1);
        point2 = new Point(x2, y2);
    }

    /**
     * writeObject - Writes out the serializable fields
     * (the 4 integers, x1, y1, x2, y2) using the
     * Serializable Field API. (the methods putFields and
     * writeFields of the ObjectOutputStream Class and the method put
     * of the ObjectOutputStream.PutField inner class)
     *
     * @serialData Only the serializable fields of the class are written.
     *             No optional data written.
     */
    private void writeObject(ObjectOutputStream s)
        throws IOException {

            // set the values of the Serializable fields
            ObjectOutputStream.PutField fields = s.putFields();
            fields.put("x1", point1.x);
            fields.put("y1", point1.y);
            fields.put("x2", point2.x);
            fields.put("y2", point2.y);

            // save them
            s.writeFields();
  }

    /**
     * readsObject - Reads in the serializable fields
     * (the 4 integers, x1, y1, x2, y2) using the
     * Serializable Field API. (the methods getFields and
     * readFields of the ObjectInputStream Class and the method get
     * of the ObjectOutputStream.GetField inner class)
     *
     * @serialData No optional data is read.
     */
    private void readObject(ObjectInputStream s)
        throws IOException {

            // prepare to read the alternate persistent fields
            ObjectInputStream.GetField fields = null;
            try {
                fields = s.readFields();
            } catch (Exception ClassNotFoundException) {
                throw new IOException();
            }

            // read the alternate persistent fields
            int x1 = (int)fields.get("x1", 0);
            int y1 = (int)fields.get("y1", 0);
            int x2 = (int)fields.get("x2", 0);
            int y2 = (int)fields.get("y2", 0);

            // save them back as Points.
            point1 = new Point(x1, y1);
            point2 = new Point(x2, y2);
    }

    public String toString() {
        return("point1.x: " + point1.x + "\npoint1.y: " + point1.y + "\npoint2.x: " + point2.x + "\npoint2.y: " + point2.y);
    }
}
