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
import javax.print.event.*;

public class PrintPS {

        public static void main(String args[]) {
                PrintPS ps = new PrintPS();
        }
        public PrintPS() {
                /* Construct the print request specification.
                 * The print data is Postscript which will be 
                 * supplied as a stream.  The media size 
                 * required is A4, and 2 copies are to be printed
                 */
                DocFlavor flavor = DocFlavor.INPUT_STREAM.POSTSCRIPT;
                PrintRequestAttributeSet aset = 
                        new HashPrintRequestAttributeSet();
                aset.add(MediaSizeName.ISO_A4);
                aset.add(new Copies(2));
                aset.add(Sides.TWO_SIDED_LONG_EDGE);
                aset.add(Finishings.STAPLE);

                /* locate a print service that can handle it */
                PrintService[] pservices =
                        PrintServiceLookup.lookupPrintServices(flavor, aset);
                if (pservices.length &gt; 0) {
                        System.out.println("selected printer " + pservices[0].getName());

                        /* create a print job for the chosen service */
                        DocPrintJob pj = pservices[0].createPrintJob();
                        try {
                                /* 
                                * Create a Doc object to hold the print data.
                                * Since the data is postscript located in a disk file,
                                * an input stream needs to be obtained
                                * BasicDoc is a useful implementation that will if requested
                                * close the stream when printing is completed.
                                */
                                FileInputStream fis = new FileInputStream("example.ps");
                                Doc doc = new SimpleDoc(fis, flavor, null);

                                /* print the doc as specified */
                                pj.print(doc, aset);

                                /*
                                * Do not explicitly call System.exit() when print returns.
                                * Printing can be asynchronous so may be executing in a
                                * separate thread.
                                * If you want to explicitly exit the VM, use a print job 
                                * listener to be notified when it is safe to do so.
                                */

                        } catch (IOException ie) { 
                                System.err.println(ie);
                        } catch (PrintException e) { 
                                System.err.println(e);
                        }
                }
        }
}
