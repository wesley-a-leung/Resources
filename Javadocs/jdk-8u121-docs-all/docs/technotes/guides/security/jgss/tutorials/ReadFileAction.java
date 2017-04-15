/*
 *
 * Copyright (c) 2001, 2002, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistributions of source code must retain the above copyright
 * notice, this  list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduct the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
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
 * DAMAGES OR LIABILITIES  SUFFERED BY LICENSEE AS A RESULT OF  OR
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

import java.security.PrivilegedAction;
import java.io.*;

/**
 * This class implements the PrivilegedAction interface to demonstrate the
 * reading of a file that belongs to the client. This code will be
 * executed by the server while impersonating the client principal.
 */
public class ReadFileAction implements PrivilegedAction {

    private String fileName;

    /**
     * Contructs a ReadFileAction instance.
     *
     * @param kerberosPrincipalName the name of the Kerberos principal
     * who owns the file that will be read. The filename is constructed
     * from the name of the principal.
     */
    public ReadFileAction(String kerberosPrincipalName) {
        /*
         * Separate the realm component from the name and use the rest of
         * it for constructing the filename. If the principal name is
         * "joe@REALM" then the file that will be read is
         * "data/joe_info.txt". The path separator "/" might be "\" in the
         * case of Windows.
         */
        int realmSeparatorPos = kerberosPrincipalName.lastIndexOf('@');
        fileName = "data" + File.separatorChar
            + kerberosPrincipalName.substring(0, realmSeparatorPos)
            + "_info.txt";
    }

    /**
     * Does the actual reading of the file. It displays the text contained
     * in the file.
     */
    public Object run() {
        System.out.println("===============================================");
        System.out.println("Reading file: " + fileName);
        try {
            BufferedReader reader = new BufferedReader(new FileReader(fileName));
            String str = reader.readLine();
            while (str != null) {
                System.out.println(str);
                str = reader.readLine();
            }
        } catch (IOException e) {
            System.err.println(e);
        }
        System.out.println("===============================================");
        return null;
    }
}
