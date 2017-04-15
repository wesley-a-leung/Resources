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

import java.awt.Component;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.InputMethodEvent;
import java.awt.event.InputMethodListener;
import java.awt.im.InputMethodRequests;
import java.awt.font.TextAttribute;
import java.awt.font.TextHitInfo;
import java.awt.font.TextLayout;
import java.text.AttributedCharacterIterator;
import java.text.AttributedCharacterIterator.Attribute;
import java.text.AttributedString;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


/**
 * Implements an integrated text input user interface.
 * This class is an active client of the input method framework, that is,
 * actively uses its APIs to accomplish integration.
 * <p>
 * This class directly implements the two client interfaces of the input method
 * framework, InputMethodListener and InputMethodRequests.
 * This is not required. Especially in cases where the public
 * interface matters (such as in a class library), it may be more
 * appropriate to hide the implementations of these two interfaces
 * in separate classes.
 *
 */

public class ActiveClient extends LWTextComponent
        implements InputMethodListener, InputMethodRequests {

    // the composed text received from the input method
    // with font information added so we can easily create a TextLayout
    private AttributedString composedTextString = null;
    private AttributedCharacterIterator composedText = null;

    // the caret received from the input method; relative  to composed text
    private TextHitInfo caret = null;

    /**
     * Constructs an ActiveClient. Input methods are always enabled for
     * ActiveClient instances.
     * @param name the component name to be displayed above the text
     */
    public ActiveClient(String name) {
        super(name, true);
        enableInputMethods(true);
        addInputMethodListener(this);
    }

    /**
     * Implements getInputMethodRequests for ActiveClient by returning "this".
     * @return "this"
     */
    public InputMethodRequests getInputMethodRequests() {
        return this;
    }

    /**
     * Adjusts composed text for new font size.
     */
    public void setFontSize(int size) {
        super.setFontSize(size);
        if (composedTextString != null) {
            composedTextString.addAttribute(TextAttribute.FONT, getFont());
        }
    }

    /**
     * Returns the text that the user has entered.
     * This override returns the concatenation of committed text
     * and composed text.
     */
    public AttributedCharacterIterator getDisplayText() {
        if (composedText == null) {
            return super.getDisplayText();
        } else {
            // We don't want to copy all the text and attribute data here.
            // Instead, we return a CompositeIterator which iterates over
            // the concatenation of two iterators.
            return new CompositeIterator(super.getDisplayText(), composedText);
        }
    }

    /**
     * Returns a text hit info indicating the current caret (insertion point).
     * This override returns the caret provided by the input method while
     * there is composed text; otherwise it returns a caret at the end
     * of the committed text. The caret provided by the input method may be null.
     */
    public TextHitInfo getCaret() {
        if (composedText == null) {
            return super.getCaret();
        } else if (caret == null) {
            return null;
        } else {
            // the caret provided by the input method is relative
            // to the composed text, so translate it to the entire text
            return caret.getOffsetHit(getCommittedTextLength());
        }
    }

    // InputMethodListener implementation

    // constant attribute set for use when copying composed text
    private static final Attribute[] IM_ATTRIBUTES =
            { TextAttribute.INPUT_METHOD_HIGHLIGHT };

    /**
     * Handles changes to the text entered through an input method.
     * Committed text contained in the event is appended to the
     * committed text of the text component. Composed text contained
     * in the event replaces any existing composed text in the text
     * component.
     * The caret defined in the event is saved and will
     * be returned by getCaret if there is composed text. The
     * component is redrawn.
     * <p>
     * In this simple component, we only
     * keep input method highlight attributes. Smarter components may want to
     * keep language, reading, input method segment, and other
     * attributes as well.
     */
    public void inputMethodTextChanged(InputMethodEvent event) {
        int committedCharacterCount = event.getCommittedCharacterCount();
        AttributedCharacterIterator text = event.getText();
        composedText = null;
        char c;
        if (text != null) {
            // copy the committed text
            int toCopy = committedCharacterCount;
            c = text.first();
            while (toCopy-- > 0) {
                insertCharacter(c);
                c = text.next();
            }

            // copy the composed text
            if (text.getEndIndex() - (text.getBeginIndex() + committedCharacterCount) > 0) {
                composedTextString = new AttributedString(text,
                        text.getBeginIndex() + committedCharacterCount, // skip over committed text
                        text.getEndIndex(), IM_ATTRIBUTES);
                // add font information because TextLayout requires it
                composedTextString.addAttribute(TextAttribute.FONT, getFont());
                composedText = composedTextString.getIterator();
            }
        }
        event.consume();
        invalidateTextLayout();
        caret = event.getCaret();
        repaint();
    }

    /**
     * Handles changes to the caret within composed text.
     * The caret defined in the event is saved and will
     * be returned by getCaret if there is composed text. The
     * component is redrawn.
     */
    public void caretPositionChanged(InputMethodEvent event) {
        caret = event.getCaret();
        event.consume();
        repaint();
    }

    // InputMethodRequests implementation
    // Note that getCommittedTextLength is already implemented in LWTextComponent

    /**
     * Gets the location of a specified offset in the current composed text,
     * or of the selection in committed text.
     */
    public Rectangle getTextLocation(TextHitInfo offset) {

        // determine the text location in component coordinates
        Rectangle rectangle;
        if (offset == null) {
            // no composed text: return caret for committed text
            rectangle = getCaretRectangle();
        } else {
            // composed text: return caret within composed text
            TextHitInfo globalOffset = offset.getOffsetHit(getCommittedTextLength());
            rectangle = getCaretRectangle(globalOffset);
        }

        // translate to screen coordinates
        Point location = getLocationOnScreen();
        rectangle.translate(location.x, location.y);

        return rectangle;
    }

    /**
     * Gets the offset within the composed text for the specified absolute x
     * and y coordinates on the screen.
     */
    public TextHitInfo getLocationOffset(int x, int y) {

        // translate from screen coordinates to coordinates in the text layout
        Point location = getLocationOnScreen();
        Point textOrigin = getTextOrigin();
        x -= location.x + textOrigin.x;
        y -= location.y + textOrigin.y;

        // TextLayout maps locations far outside its bounds to locations within.
        // To avoid false hits, we use it only if it actually contains the location.
        // We also have to translate the TextHitInfo to be relative to composed text.
        TextLayout textLayout = getTextLayout();
        if (textLayout != null &&
                textLayout.getBounds().contains(x, y)) {
            return textLayout.hitTestChar(x, y).getOffsetHit(-getCommittedTextLength());
        } else {
            return null;
        }
    }

    /**
     * Gets the offset of the insert position in the committed text contained
     * in the text editing component. In this simple component, that's always
     * at the end of the committed text.
     */
    public int getInsertPositionOffset() {
        return getCommittedTextLength();
    }

    /**
     * Gets an iterator providing access to the entire text and attributes
     * contained in the text editing component except for uncommitted
     * text.
     */
    public AttributedCharacterIterator getCommittedText(int beginIndex,
                int endIndex, Attribute[] attributes) {
        return getCommittedText(beginIndex, endIndex);
    }

    /**
     * Returns null to indicate that the "Undo Commit" feature is not supported
     * by this simple text component.
     */
    public AttributedCharacterIterator cancelLatestCommittedText(Attribute[] attributes) {
        return null;
    }

    private static final AttributedCharacterIterator EMPTY_TEXT =
            (new AttributedString("")).getIterator();

    /**
     * Gets the currently selected text from the text editing component.
     * Since this simple text component doesn't support selections, this is
     * always an iterator over empty text.
     */
    public AttributedCharacterIterator getSelectedText(Attribute[] attributes) {
        return EMPTY_TEXT;
    }
}

/**
 * Iterates over the combined text of two AttributedCharacterIterators.
 * Assumes that no annotation spans the two iterators.
 */

class CompositeIterator implements AttributedCharacterIterator {

    AttributedCharacterIterator iterator1;
    AttributedCharacterIterator iterator2;
    int begin1, end1;
    int begin2, end2;
    int endIndex;
    int currentIndex;
    AttributedCharacterIterator currentIterator;
    int currentIteratorDelta;

    /**
     * Constructs a CompositeIterator that iterates over the concatenation
     * of iterator1 and iterator2.
     * @param iterator1, iterator2 the base iterators that this composite iterator concatenates
     */
    CompositeIterator(AttributedCharacterIterator iterator1, AttributedCharacterIterator iterator2) {
        this.iterator1 = iterator1;
        this.iterator2 = iterator2;
        begin1 = iterator1.getBeginIndex();
        end1 = iterator1.getEndIndex();
        begin2 = iterator2.getBeginIndex();
        end2 = iterator2.getEndIndex();
        endIndex = (end1 - begin1) + (end2 - begin2);
        internalSetIndex(0);
    }

    // CharacterIterator implementation

    public char first() {
        return internalSetIndex(0);
    }

    public char last() {
        if (endIndex == 0) {
            return internalSetIndex(endIndex);
        } else {
            return internalSetIndex(endIndex - 1);
        }
    }

    public char next() {
        if (currentIndex < endIndex) {
            return internalSetIndex(currentIndex + 1);
        } else {
            return DONE;
        }
    }

    public char previous() {
        if (currentIndex > 0) {
            return internalSetIndex(currentIndex - 1);
        } else {
            return DONE;
        }
    }

    public char current() {
        return currentIterator.setIndex(currentIndex + currentIteratorDelta);
    }

    public char setIndex(int position) {
        if (position < 0 || position > endIndex) {
            throw new IllegalArgumentException("invalid index");
        }
        return internalSetIndex(position);
    }

    private char internalSetIndex(int position) {
        currentIndex = position;
        if (currentIndex < end1 - begin1) {
            currentIterator = iterator1;
            currentIteratorDelta = begin1;
        } else {
            currentIterator = iterator2;
            currentIteratorDelta = begin2 - (end1 - begin1);
        }
        return currentIterator.setIndex(currentIndex + currentIteratorDelta);
    }

    public int getBeginIndex() {
        return 0;
    }

    public int getEndIndex() {
        return endIndex;
    }

    public int getIndex() {
        return currentIndex;
    }

    // AttributedCharacterIterator implementation

    public int getRunStart() {
        return currentIterator.getRunStart() - currentIteratorDelta;
    }

    public int getRunLimit() {
        return currentIterator.getRunLimit() - currentIteratorDelta;
    }

    public int getRunStart(Attribute attribute) {
        return currentIterator.getRunStart(attribute) - currentIteratorDelta;
    }

    public int getRunLimit(Attribute attribute) {
        return currentIterator.getRunLimit(attribute) - currentIteratorDelta;
    }

    public int getRunStart(Set attributes) {
        return currentIterator.getRunStart(attributes) - currentIteratorDelta;
    }

    public int getRunLimit(Set attributes) {
        return currentIterator.getRunLimit(attributes) - currentIteratorDelta;
    }

    public Map getAttributes() {
        return currentIterator.getAttributes();
    }

    public Set getAllAttributeKeys() {
        Set keys = new HashSet(iterator1.getAllAttributeKeys());
        keys.addAll(iterator2.getAllAttributeKeys());
        return keys;
    }

    public Object getAttribute(Attribute attribute) {
        return currentIterator.getAttribute(attribute);
    }

    // Object overrides

    public Object clone() {
        try {
            CompositeIterator other = (CompositeIterator) super.clone();
            return other;
        } catch (CloneNotSupportedException e) {
            throw new InternalError();
        }
    }
}
