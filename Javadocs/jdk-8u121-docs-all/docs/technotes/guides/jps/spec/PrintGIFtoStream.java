/*
 * Copyright (c) 2001, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 

import java.io.*;
import javax.print.*;
import javax.print.attribute.*;
import javax.print.attribute.standard.*;

/*
* Use the Java Print Service API to locate a service which can export
* a GIF-encoded image to a stream as Postscript. This may be spooled to a
* Postscript printer, or used in a postscript viewer.
*/
public class PrintGIFtoStream {

        public static void main(String args[]) {

                /* Use the pre-defined flavor for a GIF from an InputStream */
                DocFlavor flavor = DocFlavor.INPUT_STREAM.GIF;

                /* Specify the type of the output stream */
                String psMimeType = DocFlavor.BYTE_ARRAY.POSTSCRIPT.getMimeType();

                /* Locate factory which can export a GIF image stream as Postscript */
                StreamPrintServiceFactory[] factories =
                        StreamPrintServiceFactory.lookupStreamPrintServiceFactories(
                                        flavor, psMimeType);
                if (factories.length == 0) {
                        System.err.println("No suitable factories");
                        System.exit(0);
                }

                try {
                        /* Load the file */
                        FileInputStream fis = new FileInputStream("java2dlogo.gif");
                        /* Create a file for the exported postscript */
                        String filename = "newfile.ps";
                        FileOutputStream fos = new FileOutputStream(filename);

                        /* Create a Stream printer for Postscript */
                        StreamPrintService sps = factories[0].getPrintService(fos);

                        /* Create and call a Print Job for the GIF image */
                        DocPrintJob pj = sps.createPrintJob();
                        PrintRequestAttributeSet aset = new HashPrintRequestAttributeSet();
                        aset.add(new Copies(2));
                        aset.add(MediaSizeName.ISO_A4);
                        aset.add(Sides.TWO_SIDED_LONG_EDGE);
                        aset.add(Finishings.STAPLE);

                        Doc doc = new SimpleDoc(fis, flavor, null);

                        pj.print(doc, aset);
                        fos.close();

                } catch (PrintException pe) { 
                        System.err.println(pe);
                } catch (IOException ie) { 
                        System.err.println(ie);
                }
        }
}
