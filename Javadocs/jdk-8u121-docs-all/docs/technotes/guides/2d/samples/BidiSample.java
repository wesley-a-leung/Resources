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


import java.awt.*;
import java.awt.event.*;
import java.text.*;


public class BidiSample extends Frame {

    static String text = "This text has \u0634\u0627\u062f\u062c\u0645\u0647\u0648\u0631 123,456 \u0645\u0646 (Arabic)";

    static public void main(String[] args) {
        new BidiSample().setVisible(true);
    }

    BidiSample() {

        super("Bidi Sample");
        addWindowListener(new WindowAdapter() {
                public void windowClosing(WindowEvent e) {

                    System.exit(0);
                }
            });

        setBackground(Color.white);
        setFont(new Font("Lucida Sans", Font.PLAIN, 16));
        setSize(400, 300);
    }

    public void paint(Graphics g) {

        Insets insets = getInsets();
        int height = g.getFontMetrics().getHeight();
        int y = insets.top;
        int x = insets.left + 5;

        y += height;
        g.drawString(text, x, y);

        y += height * 2;
        g.drawString("run", x, y);
        g.drawString("level", x + 50, y);
        g.drawString("start", x + 100, y);
        g.drawString("limit", x + 150, y);
        g.drawString("text", x + 200, y);


        y += height;
        Bidi bidi = new Bidi(text, Bidi.FLAG_DIR_LTR);
        byte[] levels = new byte[bidi.getRunCount()];
        String[] strings = new String[levels.length];

        for (int i = 0; i < levels.length; ++i) {

            int start = bidi.getRunStart(i);
            int limit = bidi.getRunLimit(i);
            int level = bidi.getRunLevel(i);
            String str = text.substring(start, limit);

            g.drawString(Integer.toString(i), x, y);

            g.drawString(Integer.toString(level), x + 50, y);

            g.drawString(Integer.toString(start), x + 100, y);

            g.drawString(Integer.toString(limit), x + 150, y);

            g.drawString(str, x + 200, y);

            levels[i] = (byte)level;
            strings[i] = str;
            y += height;
        }

        y += height;
        Bidi.reorderVisually(levels, 0, strings, 0, levels.length);
        for (int i = 0; i < strings.length; ++i) {

            g.drawString(strings[i], x, y);
            y += height;
        }
    }

}
