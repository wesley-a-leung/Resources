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

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.font.FontRenderContext;
import java.awt.font.TextAttribute;
import java.awt.font.TextHitInfo;
import java.awt.font.TextLayout;
import java.text.AttributedCharacterIterator;
import java.text.AttributedString;

/**
 * Implements a very simple lightweight text editing component.
 * It lets the user edit a single line of text using the keyboard.
 * The only special character that it knows about is backspace; all other
 * characters are added to the text. Selections are not supported, so
 * there's only a simple caret indicating the insertion point.
 * The component also displays a component name above the editable
 * text line, and draws a black frame whose thickness indicates whether
 * the component has the focus.
 * <p>
 * The component can be initialized to enable or disable input
 * through input methods. Other than that, it doesn't do anything
 * to support input methods, so input method interaction (if any)
 * will occur in a separate composition window. However, the
 * component is designed to be easily extended with full input
 * method support. It distinguishes between "displayed text" and
 * "committed text" - here, they're the same, but in a subclass
 * that supports on-the-spot input, the displayed text would be the
 * combination of committed text and composed text. The component
 * also uses TextLayout to draw the text, so it can be easily
 * extended to handle input method highlights.
 */

public class LWTextComponent extends Component implements KeyListener, FocusListener {

    // whether the component currently has the focus
    private transient boolean haveFocus;

    // the component name that's displayed at the top of the component's area
    private String name;

    // The text the user has entered. The term "committed text"
    // follows the usage in the input method framework.
    private StringBuffer committedText = new StringBuffer();

    // We use a text layout for drawing and measuring. Since they
    // are expensive to create, we cache it and invalidate it when
    // the text is modified.
    private transient TextLayout textLayout = null;
    private transient boolean validTextLayout = false;

    // members that determine where the text is drawn
    private static final int LINE_OFFSET = 8;
    private int textOriginX;
    private int nameOriginY;
    private int textOriginY;

    /**
     * Constructs a LWTextComponent.
     * @param name the component name to be displayed above the text
     * @param enableInputMethods whether to enable input methods for this component
     */
    public LWTextComponent(String name, boolean enableInputMethods) {
        super();
        this.name = name;
        setSize(300, 80);
        // we have to set the foreground color because otherwise
        // text may not display correctly when we use an input
        // method highlight that swaps background and foreground
        // colors
        setForeground(Color.black);
        setBackground(Color.white);
        setFontSize(12);
        setVisible(true);
        setEnabled(true);
        addKeyListener(this);
        addFocusListener(this);
        addMouseListener(new MouseFocusListener(this));
        enableInputMethods(enableInputMethods);
    }

    public void setFontSize(int size) {
        setFont(new Font("Dialog", Font.PLAIN, size));
        nameOriginY = LINE_OFFSET + size;
        textOriginX = 10;
        textOriginY = 2 * (LINE_OFFSET + size);
    }

    /**
     * Draws the component. The following items are drawn:
     * <ul>
     * <li>the component's background
     * <li>a frame, thicker if the component has the focus
     * <li>the component name
     * <li>the text that the user has entered
     * <li>the caret, if the component has the focus
     * </ul>
     */
    public synchronized void paint(Graphics g) {

        // draw the background
        g.setColor(getBackground());
        Dimension size = getSize();
        g.fillRect(0, 0, size.width, size.height);

        // draw the frame, thicker if the component has the focus
        g.setColor(Color.black);
        g.drawRect(0, 0, size.width - 1, size.height - 1);
        if (haveFocus) {
            g.drawRect(1, 1, size.width - 3, size.height - 3);
        }

        // draw the component name
        g.setColor(getForeground());
        g.drawString(name, textOriginX, nameOriginY);

        // draw the text that the user has entered
        TextLayout textLayout = getTextLayout();
        if (textLayout != null) {
            textLayout.draw((Graphics2D) g, textOriginX, textOriginY);
        }

        // draw the caret, if the component has the focus
        Rectangle rectangle = getCaretRectangle();
        if (haveFocus && rectangle != null) {
            g.setXORMode(getBackground());
            g.fillRect(rectangle.x, rectangle.y, 1, rectangle.height);
            g.setPaintMode();
        }
    }

    /**
     * Returns the text that the user has entered and committed.
     * Since this component does not support on-the-spot input, there's no
     * composed text, so all text that has been entered is committed.
     * @return an AttributedCharacterIterator for the text that the user has entered and committed
     */
    public AttributedCharacterIterator getCommittedText() {
        AttributedString string = new AttributedString(committedText.toString());
        return string.getIterator();
    }

    /**
     * Returns a subrange of the text that the user has entered and committed.
     * Since this component does not support on-the-spot input, there's no
     * composed text, so all text that has been entered is committed.
     * @param beginIndex the index of the first character of the subrange
     * @param endIndex the index of the character following the subrange
     * @return an AttributedCharacterIterator for a subrange of the text that the user has entered and committed
     */
    public AttributedCharacterIterator getCommittedText(int beginIndex, int endIndex) {
        AttributedString string = new AttributedString(committedText.toString());
        return string.getIterator(null, beginIndex, endIndex);
    }

    /**
     * Returns the length of the text that the user has entered and committed.
     * Since this component does not support on-the-spot input, there's no
     * composed text, so all text that has been entered is committed.
     * @return the length of the text that the user has entered and committed
     */
    public int getCommittedTextLength() {
        return committedText.length();
    }

    /**
     * Returns the text that the user has entered.
     * As TextLayout requires a font to be defined for each character,
     * the default font is applied to the entire text.
     * A subclass that supports on-the-spot input must override this
     * method to include composed text.
     * @return the text that the user has entered
     */
    public AttributedCharacterIterator getDisplayText() {
        AttributedString string = new AttributedString(committedText.toString());
        if (committedText.length() > 0) {
            string.addAttribute(TextAttribute.FONT, getFont());
        }
        return string.getIterator();
    }

    /**
     * Returns a text layout for the text that the user has entered.
     * This text layout is created from the text returned by getDisplayText.
     * The text layout is cached until invalidateTextLayout is called.
     * @see #invalidateTextLayout
     * @see #getDisplayText
     * @return a text layout for the text that the user has entered, or null
     */
    public synchronized TextLayout getTextLayout() {
        if (!validTextLayout) {
            textLayout = null;
            AttributedCharacterIterator text = getDisplayText();
            if (text.getEndIndex() > text.getBeginIndex()) {
                FontRenderContext context = ((Graphics2D) getGraphics()).getFontRenderContext();
                textLayout = new TextLayout(text, context);
            }
        }
        validTextLayout = true;
        return textLayout;
    }

    /**
     * Invalidates the cached text layout. This must be called whenever
     * the component's text is modified.
     * @see #getTextLayout
     */
    public synchronized void invalidateTextLayout() {
        validTextLayout = false;
    }

    /**
     * Returns the origin of the text. This is the leftmost point
     * on the baseline of the text.
     * @return the origin of the text
     */
    public Point getTextOrigin() {
        return new Point(textOriginX, textOriginY);
    }

    /**
     * Returns a 0-width caret rectangle. This rectangle is derived from
     * the caret returned by getCaret. getCaretRectangle returns
     * null iff getCaret does.
     * @see #getCaret
     * @return the caret rectangle, or null
     */
    public Rectangle getCaretRectangle() {
        TextHitInfo caret = getCaret();
        if (caret == null) {
            return null;
        }
        return getCaretRectangle(caret);
    }

    /**
     * Returns a 0-width caret rectangle for the given text index.
     * It is calculated based on the text layout returned by getTextLayout,
     * so this method can be used for the entire displayed text.
     * @param caret the text index for which to calculate a caret rectangle
     * @return the caret rectangle
     */
    public Rectangle getCaretRectangle(TextHitInfo caret) {
        TextLayout textLayout = getTextLayout();
        int caretLocation;
        if (textLayout != null) {
            caretLocation = Math.round(textLayout.getCaretInfo(caret)[0]);
        } else {
            caretLocation = 0;
        }
        FontMetrics metrics = getGraphics().getFontMetrics();
        return new Rectangle(textOriginX + caretLocation,
                             textOriginY - metrics.getAscent(),
                             0, metrics.getAscent() + metrics.getDescent());
    }

    /**
     * Returns a text hit info indicating the current caret (insertion point).
     * This class always returns a caret at the end of the text that the user
     * has entered. Subclasses may return a different caret or null.
     * @return the caret, or null
     */
    public TextHitInfo getCaret() {
        return TextHitInfo.trailing(committedText.length() - 1);
    }

    /**
     * Inserts the given character at the end of the text.
     * @param c the character to be inserted
     */
    public void insertCharacter(char c) {
        committedText.append(c);
        invalidateTextLayout();
    }

    /**
     * Handles the key typed event. If the character is backspace,
     * the last character is removed from the text that the user
     * has entered. Otherwise, the character is appended to the text.
     * Then, the text is redrawn.
     * @event the key event to be handled
     */
    public void keyTyped(KeyEvent event) {
        char keyChar = event.getKeyChar();
        if (keyChar == '\b') {
            int len = committedText.length();
            if (len > 0) {
                committedText.setLength(len - 1);
            invalidateTextLayout();
            }
        } else {
            insertCharacter(keyChar);
        }
        event.consume();
        repaint();
    }

    /** Ignores key pressed events. */
    public void keyPressed(KeyEvent event) {}

    /** Ignores key released events. */
    public void keyReleased(KeyEvent event) {}

    /** Turns on drawing of the component's thicker frame and the caret. */
    public void focusGained(FocusEvent event) {
        haveFocus = true;
        repaint();
    }

    /** Turns off drawing of the component's thicker frame and the caret. */
    public void focusLost(FocusEvent event) {
        haveFocus = false;
        repaint();
    }

}

class MouseFocusListener extends MouseAdapter {

    private Component target;

    MouseFocusListener(Component target) {
        this.target = target;
    }

    public void mouseClicked(MouseEvent e) {
        target.requestFocus();
    }

}
