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
import java.awt.*;
import java.net.*;
import java.awt.image.*;
import java.awt.print.*;
import javax.print.*;
import javax.print.attribute.*;
import javax.print.attribute.standard.*;

public class Print2DPrinterJob implements Printable {

        public Print2DPrinterJob() {

                /* Construct the print request specification.
                * The print data is a Printable object.
                * the request additonally specifies a job name, 2 copies, and
                * landscape orientation of the media.
                */
                PrintRequestAttributeSet aset = new HashPrintRequestAttributeSet();
                aset.add(OrientationRequested.LANDSCAPE);
                aset.add(new Copies(2));
                aset.add(new JobName("My job", null));

                /* Create a print job */
                PrinterJob pj = PrinterJob.getPrinterJob();       
                pj.setPrintable(this);
                /* locate a print service that can handle the request */
                PrintService[] services =
                        PrinterJob.lookupPrintServices();

                if (services.length &gt; 0) {
                        System.out.println("selected printer " + services[0].getName());
                        try {
                                pj.setPrintService(services[0]);
                                pj.pageDialog(aset);
                                if(pj.printDialog(aset)) {
                                        pj.print(aset);
                                }
                        } catch (PrinterException pe) { 
                                System.err.println(pe);
                        }
                }
        }

        public int print(Graphics g,PageFormat pf,int pageIndex) {

                if (pageIndex == 0) {
                        Graphics2D g2d= (Graphics2D)g;
                        g2d.translate(pf.getImageableX(), pf.getImageableY()); 
                        g2d.setColor(Color.black);
                        g2d.drawString("example string", 250, 250);
                        g2d.fillRect(0, 0, 200, 200);
                        return Printable.PAGE_EXISTS;                                   
                } else {
                        return Printable.NO_SUCH_PAGE;
                }
        }

        public static void main(String arg[]) {

                Print2DPrinterJob sp = new Print2DPrinterJob();
        }
}
