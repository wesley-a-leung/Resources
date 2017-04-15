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

// Implemented under 1.4.
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

// This example demonstrates adding drag and drop (DnD)
// support to a JLabel.
public class JLabelDragNDrop {
    JFrame aFrame;
    JPanel aPanel;
    JTextField tf;
    JLabel tl;

    // Constructor
    public JLabelDragNDrop() {
        // Create the frame and container.
        aFrame = new JFrame("JLabel Drag and Drop Demo");
        aFrame.setSize(100, 10);
        aPanel = new JPanel();
        aPanel.setLayout(new GridLayout(2, 2));

        // Add the widgets.
        addWidgets();

        // Add the panel to the frame.
        aFrame.getContentPane().add(aPanel, BorderLayout.CENTER);

        // Exit when the window is closed.
        aFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Show the panel.
        aFrame.pack();
        aFrame.setVisible(true);
    }

    // Create and add the widgets to the panel.
    private void addWidgets() {
        // Create widgets.
        tf = new JTextField(100);
        tf.setDragEnabled(true);
        tl = new JLabel("Drop Here", SwingConstants.LEFT);
        tl.setTransferHandler(new TransferHandler("text"));
        MouseListener ml = new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                JComponent c = (JComponent)e.getSource();
                TransferHandler th = c.getTransferHandler();
                th.exportAsDrag(c, e, TransferHandler.COPY);
            }
        };
        tl.addMouseListener(ml);

        // Add widgets to container.
        aPanel.add(tf);
        aPanel.add(tl);

        tl.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
    }

    // main method
    public static void main(String[] args) {
        // Set the look and feel.
        try {
            UIManager.setLookAndFeel(
                UIManager.getCrossPlatformLookAndFeelClassName());
        } catch(Exception e) {}

        JLabelDragNDrop example = new JLabelDragNDrop();
    }
}
