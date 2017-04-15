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
 * The superclass DOES NOT implement externalizable
 */
class ReadingMaterial  {

    /*
     * In order for us to be able to serialize this data, these must be
     * either serializable/externalizable objects or primitive data types.
     *
     *
     * We do not make these private because we need the subclass Book to be
     * able to save the state of this superclass.
     * Alternatively, we could make these private and create set and get
     * functions that would allow access to the Book subclass.
     * If we failed to do that with private fields, then fields would not
     * be saveable!
     */
    String author;
    String subject;
    int yearwritten;

    // other relevant data and methods
    // .
    // .
    // .

    /*
     * A public no-arg constructor so that an externalizable sub-class can
     * use it. If this did not exist, then the subclass would have to call
     * the regular argument constructor with default arguments in its own
     * public no-arg constructor
     */
    public ReadingMaterial() {}

    ReadingMaterial(String auth, String sub, int year) {
        author = auth;
        subject = sub;
        yearwritten = year;
    }
}
