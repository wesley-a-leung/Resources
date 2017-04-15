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
import java.applet.Applet;
import java.awt.image.*;

public class BufferedShapeMover extends Applet{

        static protected Label label;

        public void init(){
                //Initialize the layout.
                setLayout(new BorderLayout());
                add(new BSMCanvas());
                label = new Label("Drag rectangle around within the area");
                add("South", label);
        }

    public static void main(String s[]) {
        Frame f = new Frame("BufferedShapeMover");
        f.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {System.exit(0);}
        });
        Applet applet = new BufferedShapeMover();
        f.add("Center", applet);
        applet.init();
        f.pack();
        f.setSize(new Dimension(550,250));
        f.show();
    }

}

class BSMCanvas extends Canvas implements MouseListener, MouseMotionListener{

        Rectangle rect = new Rectangle(0, 0, 100, 50);
        BufferedImage bi;
        Graphics2D big;

        // Holds the coordinates of the user's last mousePressed event.
        int last_x, last_y;
        boolean firstTime = true;
        TexturePaint fillPolka, strokePolka;
        Rectangle area;

        // True if the user pressed, dragged or released the mouse outside of the rectangle; false otherwise.
        boolean pressOut = false;

        public BSMCanvas(){
                setBackground(Color.white);
                addMouseMotionListener(this);
                addMouseListener(this);

                // Creates the fill texture paint pattern.
                bi = new BufferedImage(5, 5, BufferedImage.TYPE_INT_RGB);
                big = bi.createGraphics();
                big.setColor(Color.pink);
                big.fillRect(0, 0, 7, 7);
                big.setColor(Color.cyan);
                big.fillOval(0, 0, 3, 3);
                Rectangle r = new Rectangle(0,0,5,5);
                fillPolka = new TexturePaint(bi, r);
                big.dispose();

                //Creates the stroke texture paint pattern.
                bi = new BufferedImage(5, 5, BufferedImage.TYPE_INT_RGB);
                big = bi.createGraphics();
                big.setColor(Color.cyan);
                big.fillRect(0, 0, 7, 7);
                big.setColor(Color.pink);
                big.fillOval(0, 0, 3, 3);
                r = new Rectangle(0,0,5,5);
                strokePolka = new TexturePaint(bi, r);
                big.dispose();
        }

        // Handles the event of the user pressing down the mouse button.
        public void mousePressed(MouseEvent e){

                last_x = rect.x - e.getX();
                last_y = rect.y - e.getY();

                // Checks whether or not the cursor is inside of the rectangle while the user is pressing themouse.
                if(rect.contains(e.getX(), e.getY())){
                        updateLocation(e);
                } else {
                BufferedShapeMover.label.setText("First position the cursor on the rectangle and then drag.");
                        pressOut = true;
                }
        }

        // Handles the event of a user dragging the mouse while holding down the mouse button.
        public void mouseDragged(MouseEvent e){

                if(!pressOut){
                         updateLocation(e);
                } else {
                BufferedShapeMover.label.setText("First position the cursor on the rectangle and then drag.");
                }
        }

        // Handles the event of a user releasing the mouse button.
        public void mouseReleased(MouseEvent e){

                // Checks whether or not the cursor is inside of the rectangle when the user releases the
                // mouse button.
                if(rect.contains(e.getX(), e.getY())){
                        updateLocation(e);
                } else {
                BufferedShapeMover.label.setText("First position the cursor on the rectangle and then drag.");
                        pressOut = false;
                }
        }

        // This method required by MouseListener.
        public void mouseMoved(MouseEvent e){}

        // These methods are required by MouseMotionListener.
        public void mouseClicked(MouseEvent e){}
        public void mouseExited(MouseEvent e){}
        public void mouseEntered(MouseEvent e){}
        public void updateLocation(MouseEvent e){

                rect.setLocation(last_x + e.getX(), last_y + e.getY());
                /*
                 * Updates the label to reflect the location of the
                 * current rectangle
                 * if checkRect returns true; otherwise, returns error message.
                 */
                if (checkRect()) {
                    BufferedShapeMover.label.setText("Rectangle located at " +
                                                     rect.getX() + ", " +
                                                     rect.getY());
                } else {
                    BufferedShapeMover.label.setText("Please don't try to "+
                                                     " drag outside the area.");
                }
                repaint();
        }

        public void paint(Graphics g){
                update(g);
        }

        public void update(Graphics g){
                Graphics2D g2 = (Graphics2D)g;

                if(firstTime){
                    Dimension dim = getSize();
                    int w = dim.width;
                    int h = dim.height;
                    area = new Rectangle(dim);
                        bi = (BufferedImage)createImage(w, h);
                        big = bi.createGraphics();
                        rect.setLocation(w/2-50, h/2-25);
                        big.setStroke(new BasicStroke(8.0f));
                        firstTime = false;
                }

                // Clears the rectangle that was previously drawn.
                big.setColor(Color.white);
                big.clearRect(0, 0, area.width, area.height);

                // Draws and fills the newly positioned rectangle to the buffer.
                big.setPaint(strokePolka);
                big.draw(rect);
                big.setPaint(fillPolka);
                big.fill(rect);

                // Draws the buffered image to the screen.
                g2.drawImage(bi, 0, 0, this);
        }
        /*
         * Checks if the rectangle is contained within the applet window.  If the rectangle
         * is not contained withing the applet window, it is redrawn so that it is adjacent
         * to the edge of the window and just inside the window.
         */
        boolean checkRect(){
            if (area == null) {
                return false;
            }
                if(area.contains(rect.x, rect.y, 100, 50)){
                        return true;
                }
                int new_x = rect.x;
                int new_y = rect.y;

                if((rect.x+100)>area.width){
                        new_x = area.width-99;
                }
                if(rect.x < 0){
                        new_x = -1;
                }
                if((rect.y+50)>area.height){
                        new_y = area.height-49;
                }
                if(rect.y < 0){
                        new_y = -1;
                }
                rect.setLocation(new_x, new_y);
                return false;
        }

}
