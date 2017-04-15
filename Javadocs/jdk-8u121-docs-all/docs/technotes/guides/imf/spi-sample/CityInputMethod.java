/*
 * Copyright (c) 2005, Oracle and/or its affiliates. All rights reserved.
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

package com.sun.demos.cityim.internal;

import java.awt.AWTEvent;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.Window;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.InputEvent;
import java.awt.event.InputMethodEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.TextAttribute;
import java.awt.font.TextHitInfo;
import java.awt.im.InputMethodHighlight;
import java.awt.im.spi.InputMethod;
import java.awt.im.spi.InputMethodContext;
import java.io.InputStream;
import java.io.IOException;
import java.lang.reflect.Method;
import java.text.AttributedString;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Locale;
import java.util.Properties;
import java.util.Vector;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedExceptionAction;
import java.security.PrivilegedActionException;
import java.text.MessageFormat;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;


public class CityInputMethod implements InputMethod {

    private static Locale YOMI = new Locale("ja", "JP", "YOMI");
    private static Locale[] SUPPORTED_LOCALES = {
        Locale.ENGLISH, Locale.GERMAN,
        Locale.JAPANESE, YOMI,
        Locale.SIMPLIFIED_CHINESE, Locale.TRADITIONAL_CHINESE
    };
    private static Locale[] LOOKUP_LOCALES = {
        Locale.ENGLISH, Locale.GERMAN,
        Locale.JAPANESE, YOMI,
        Locale.CHINESE,
        Locale.SIMPLIFIED_CHINESE, Locale.TRADITIONAL_CHINESE
    };

    // lookup tables - shared by all instances of this input method
    private static Properties cityNames;
    private static Properties cityAliases;
    private static Properties cityLanguages;
    private static Properties templates;

    // windows - shared by all instances of this input method
    private static Window statusWindow;
    // current or last statusWindow owner instance
    private static CityInputMethod statusWindowOwner;
    // true if Solaris style; false if PC style
    private static boolean attachedStatusWindow = false;
    // remember the statusWindow location per instance
    private Rectangle clientWindowLocation;
    // status window location in PC style
    private static Point globalStatusWindowLocation;
    // keep live city input method instances (synchronized using statusWindow)
    private static HashSet cityInputMethodInstances = new HashSet(5);
    // non-null if Swing input method JFrame is available
    static Method methodCreateInputMethodJFrame = null;
    static {
        methodCreateInputMethodJFrame =
            (Method)AccessController.doPrivileged(new PrivilegedAction() {
            public Object run() {
                try {
                    Class[] params = new Class[2];
                    params[0] = String.class;
                    params[1] = Boolean.TYPE;
                    return InputMethodContext.class.getMethod("createInputMethodJFrame", params);
                }  catch (NoSuchMethodException e) {
                    return null;
                }
            }
        });
    }
    // input method window titles
    static final String statusWindowTitle = "City Input Method";
    static final String lookupWindowTitle = "Lookup Window";

    // lookup information - per instance
    private String[] lookupCandidates;
    private Locale[] lookupLocales;
    private int lookupCandidateCount;
    private LookupList lookupList;
    private int lookupSelection;

    // per-instance state
    InputMethodContext inputMethodContext;
    private boolean active;
    private boolean disposed;
    private Locale locale;
    private boolean converted;
    private StringBuffer rawText;
    private String convertedText;

    private int insertionPoint;
    private String[] rawTextSegs = null;
    private String[] convertedSegs = null;
    private String fmt = null;
    private int fieldPos[][] = null;
    private int segPos[][] = null;
    private int selectedSeg = 0;
    private int numSegs = 0;
    private int committedSeg = -1;
    private int previouslyCommittedCharacterCount = 0;

    public CityInputMethod() throws IOException {
        initializeTables();
        rawText = new StringBuffer();
    }

    public void setInputMethodContext(InputMethodContext context) {
        inputMethodContext = context;
        if (statusWindow == null) {
            Window sw;
            if (methodCreateInputMethodJFrame != null) {
                try {
                    Object[] params = new Object[2];
                    params[0] = statusWindowTitle;
                    params[1] = Boolean.FALSE;
                    sw = (Window)methodCreateInputMethodJFrame.invoke(context, params);
                } catch (Exception e) {
                    sw = context.createInputMethodWindow(statusWindowTitle, false);
                }
            } else {
                sw = context.createInputMethodWindow(statusWindowTitle, false);
            }
            JLabel label = new JLabel();
            label.setOpaque(true);
            label.setForeground(Color.black);
            label.setBackground(Color.white);
            synchronized (this.getClass()) {
                if (statusWindow == null) {
                    statusWindow = sw;
                    statusWindow.addComponentListener(new ComponentListener() {
                        public void componentResized(ComponentEvent e) {}
                        public void componentMoved(ComponentEvent e) {
                            synchronized (statusWindow) {
                                if (!attachedStatusWindow) {
                                    Component comp = e.getComponent();
                                    if (comp.isVisible()) {
                                        globalStatusWindowLocation = comp.getLocation();
                                    }
                                }
                            }
                        }
                        public void componentShown(ComponentEvent e) {}
                        public void componentHidden(ComponentEvent e) {}
                    });
                    label.addMouseListener(new MouseListener() {
                        public void mouseClicked(MouseEvent e) {
                            int count = e.getClickCount();
                            if (count >= 2) {
                                toggleStatusWindowStyle();
                            }
                        }
                        public void mousePressed(MouseEvent e) {}
                        public void mouseReleased(MouseEvent e) {}
                        public void mouseEntered(MouseEvent e) {}
                        public void mouseExited(MouseEvent e) {}
                    });
                    if (statusWindow instanceof JFrame) {
                        ((JFrame)statusWindow).getContentPane().add(label);
                    } else {
                        statusWindow.add(label);
                    }
                    statusWindowOwner = this;
                    updateStatusWindow(locale);
                    label.setSize(200, 50);
                    statusWindow.pack();
                }
            }
        }
        inputMethodContext.enableClientWindowNotification(this, attachedStatusWindow);
        synchronized (statusWindow) {
            cityInputMethodInstances.add(this);
        }
    }

    public boolean setLocale(Locale locale) {
        for (int i = 0; i < SUPPORTED_LOCALES.length; i++) {
            if (locale.equals(SUPPORTED_LOCALES[i])) {
                this.locale = locale;
                if (statusWindow != null) {
                    updateStatusWindow(locale);
                }
                return true;
            }
        }
        return false;
    }

    public Locale getLocale() {
        return locale;
    }

    void updateStatusWindow(Locale locale) {
        synchronized (statusWindow) {
            JLabel label;
            if (statusWindow instanceof JFrame) {
                label = (JLabel) ((JFrame)statusWindow).getContentPane().getComponent(0);
            } else {
                label = (JLabel) statusWindow.getComponent(0);
            }
            String localeName = locale == null ? "null" : locale.getDisplayName();
            String text = "Current locale: " + localeName;
            if (!label.getText().equals(text)) {
                label.setText(text);
                statusWindow.pack();
            }
            if (attachedStatusWindow) {
                if (clientWindowLocation != null) {
                    statusWindow.setLocation(clientWindowLocation.x,
                                             clientWindowLocation.y + clientWindowLocation.height);
                }
            } else {
                setPCStyleStatusWindow();
            }
        }
    }

    private void setPCStyleStatusWindow() {
        synchronized (statusWindow) {
            if (globalStatusWindowLocation == null) {
                Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
                globalStatusWindowLocation = new Point(d.width - statusWindow.getWidth(),
                                                       d.height - statusWindow.getHeight() - 25);
            }
            statusWindow.setLocation(globalStatusWindowLocation.x, globalStatusWindowLocation.y);
        }
    }

    private void setStatusWindowForeground(Color fg) {
        synchronized (statusWindow) {
            if (statusWindowOwner != this) {
                return;
            }
            JLabel label;
            if (statusWindow instanceof JFrame) {
                label = (JLabel) ((JFrame)statusWindow).getContentPane().getComponent(0);
            } else {
                label = (JLabel) statusWindow.getComponent(0);
            }
            label.setForeground(fg);
        }
    }

    private void toggleStatusWindowStyle() {
        synchronized (statusWindow) {
            if (attachedStatusWindow) {
                attachedStatusWindow = false;
                setPCStyleStatusWindow();
            } else {
                attachedStatusWindow = true;
            }
            Iterator itr = cityInputMethodInstances.iterator();
            while (itr.hasNext()) {
                CityInputMethod im = (CityInputMethod)itr.next();
                im.inputMethodContext.enableClientWindowNotification(im, attachedStatusWindow);
            }
        }
    }

    public void setCharacterSubsets(Character.Subset[] subsets) {
        // ignore
    }

    public void reconvert() {
        // not supported yet
        throw new UnsupportedOperationException();
    }

    public void dispatchEvent(AWTEvent event) {
        if (!active && (event instanceof KeyEvent)) {
            System.out.println("CityInputMethod.dispatch called with KeyEvent while not active");
        }
        if (disposed) {
            System.out.println("CityInputMethod.dispatch called after being disposed");
        }
        if (!(event instanceof InputEvent)) {
            System.out.println("CityInputMethod.dispatch called with event that's not an InputEvent");
        }

        /* In case the event has been already consumed or the component is already gone,
         * simply returns.
         */
        InputEvent inputEvent = (InputEvent) event;
        Component comp = inputEvent.getComponent();
        if (inputEvent.isConsumed() ||
            null == comp ||
            !comp.isShowing()) {
            return;
        }

        int eventType = event.getID();
        if (eventType == KeyEvent.KEY_RELEASED) {
            KeyEvent e = (KeyEvent) event;
            int keyCode = e.getKeyCode();
            if (lookupList != null) { // if candidate window is active
                if (e.isControlDown()) {
                    if (keyCode == KeyEvent.VK_DOWN) {
                        // Control + Arrow Down commits chunks
                        closeLookupWindow();
                        commit(selectedSeg);
                        e.consume();
                    }
                } else {
                    // select candidate by Arrow Up/Down
                    if (keyCode == KeyEvent.VK_DOWN) {
                        if (++lookupSelection == lookupCandidateCount) {
                            lookupSelection = 0;
                        }
                        selectCandidate(lookupSelection);
                        e.consume();
                    } else if (keyCode == KeyEvent.VK_UP) {
                        if (--lookupSelection < 0) {
                            lookupSelection = lookupCandidateCount - 1;
                        }
                        selectCandidate(lookupSelection);
                        e.consume();
                    }
                }
            } else {
                if (e.isControlDown()) {
                    if (keyCode == KeyEvent.VK_DOWN) {
                        // Control + Arrow Down commits chunks
                        commit(selectedSeg);
                        e.consume();
                    }
                } else {
                    // move selected segment by Arrow Right/Left
                    if ((keyCode == KeyEvent.VK_RIGHT) && (converted == true)) {
                        if (selectedSeg < (numSegs - 1)) {
                            selectedSeg++;
                            sendText(false);
                            e.consume();
                        } else {
                            Toolkit.getDefaultToolkit().beep();
                        }
                    } else if ((keyCode == KeyEvent.VK_LEFT) && (converted == true)) {
                        if (selectedSeg > (committedSeg + 1)) {
                            selectedSeg--;
                            sendText(false);
                            e.consume();
                        } else {
                            Toolkit.getDefaultToolkit().beep();
                        }
                    }
                }
            }
        } else if (eventType == MouseEvent.MOUSE_CLICKED) {
            MouseEvent e = (MouseEvent) event;
            Point pnt = comp.getLocationOnScreen();
            int x = (int)pnt.getX() + e.getX();
            int y = (int)pnt.getY() + e.getY();
            TextHitInfo hit = inputMethodContext.getLocationOffset(x,y);
            if (hit != null) {
                // within composed text
                if (converted) {
                    selectedSeg = findSegment(hit.getInsertionIndex());
                    sendText(false);
                    e.consume();
                } else {
                    insertionPoint = hit.getInsertionIndex();
                }
            } else {
                // if hit outside composition, simply commit all.
                commitAll();
            }
        } else if (eventType == KeyEvent.KEY_TYPED) {
            KeyEvent e = (KeyEvent) event;
            if (handleCharacter(e.getKeyChar())) {
                e.consume();
            }
        } else if (eventType == KeyEvent.KEY_PRESSED) {
            KeyEvent e = (KeyEvent) event;
            if ((e.getKeyCode() == KeyEvent.VK_ENTER) &&
                (lookupList != null || converted || rawText.length() != 0)) {
                e.consume();
            }
        }
    }

    /**
     * find segment at insertion point
     */
    int findSegment(int insertion) {
        for (int i = committedSeg + 1; i < numSegs; i++) {
            if ((segPos[i][0] < insertion) && (insertion < segPos[i][1])) {
                return i;
            }
        }
        return 0;
    }

    public void activate() {
        if (active) {
            System.out.println("CityInputMethod.activate called while active");
        }
        active = true;
        synchronized (statusWindow) {
            statusWindowOwner = this;
            updateStatusWindow(locale);
            if (!statusWindow.isVisible()) {
                statusWindow.setVisible(true);
            }
            setStatusWindowForeground(Color.black);
        }
    }

    public void deactivate(boolean isTemporary) {
        closeLookupWindow();
        if (!active) {
            System.out.println("CityInputMethod.deactivate called while not active");
        }
        setStatusWindowForeground(Color.lightGray);
        active = false;
    }

    public void hideWindows() {
        if (active) {
            System.out.println("CityInputMethod.hideWindows called while active");
        }
        closeLookupWindow();
        synchronized (statusWindow) {
            if (statusWindowOwner == this) {
                statusWindow.setVisible(false);
            }
        }
    }

    public void removeNotify() {
    }

    public void endComposition() {
        if (rawText.length() != 0) {
            commitAll();
        }
        closeLookupWindow();
    }

    public void notifyClientWindowChange(Rectangle location) {
        clientWindowLocation = location;
        synchronized (statusWindow) {
            if (!attachedStatusWindow || statusWindowOwner != this) {
                return;
            }
            if (location != null) {
                statusWindow.setLocation(location.x, location.y+location.height);
                if (!statusWindow.isVisible()) {
                    if (active) {
                        setStatusWindowForeground(Color.black);
                    } else {
                        setStatusWindowForeground(Color.lightGray);
                    }
                    statusWindow.setVisible(true);
                }
            } else {
                statusWindow.setVisible(false);
            }
        }
    }

    public void dispose() {
        if (active) {
            System.out.println("CityInputMethod.dispose called while active");
        }
        if (disposed) {
            System.out.println("CityInputMethod.disposed called repeatedly");
        }
        closeLookupWindow();
        synchronized (statusWindow) {
            cityInputMethodInstances.remove(this);
            if (cityInputMethodInstances.isEmpty()) {
                statusWindow.dispose();
            }
        }
        disposed = true;
    }

    public Object getControlObject() {
        return null;
    }

    public void setCompositionEnabled(boolean enable) {
        // not supported yet
        throw new UnsupportedOperationException();
    }

    public boolean isCompositionEnabled() {
        // always enabled
        return true;
    }

    private void initializeTables() throws IOException {
        synchronized (this.getClass()) {
            if (templates == null) {
                cityNames = loadProperties("names.properties");
                cityAliases = loadProperties("aliases.properties");
                cityLanguages = loadProperties("languages.properties");
                templates = loadProperties("templates.properties");
            }
        }
    }

    /** Use doPrivileged block to avoid the security exceptions
     *  since the user code which triggers this action, e.g.
     *  open IM selection menu from a ColorChooserDialog, may in the call stack.
     *  Since reading properties file is relatively safe since it is located in the
     *  jar file itself, not from user's local file system.
     *  @exception IOException when reading properties file fails.
     */
    private Properties loadProperties(final String fileName) throws IOException {
        try {
            return
                (Properties) AccessController.doPrivileged(new PrivilegedExceptionAction() {
                        public Object run() throws IOException {
                            Properties props = new Properties();
                            InputStream stream = this.getClass().getResourceAsStream(fileName);
                            props.load(stream);
                            stream.close();
                            return props;
                        }
                    });
        } catch(PrivilegedActionException pae) {
            throw (IOException)pae.getCause();
        }
    }

    /**
     * Attempts to handle a typed character.
     * @return whether the character was handled
     */
    private boolean handleCharacter(char ch) {
        if (lookupList != null) {
            if (ch == ' ') {
                if (++lookupSelection == lookupCandidateCount) {
                    lookupSelection = 0;
                }
                selectCandidate(lookupSelection);
                return true;
            } else if (ch == '\n') {
                commitAll();
                closeLookupWindow();
                return true;
            } else if ('1' <= ch && ch <= '0' + lookupCandidateCount) {
                selectCandidate(ch - '1');
                closeLookupWindow();
                return true;
            } else {
                Toolkit.getDefaultToolkit().beep();
                return true;
            }
        } else if (converted) {
            if (ch == ' ') {
                convertAgain();
                return true;
            } else if (ch == '\n') {
                commitAll();
                return true;
            } else {
                Toolkit.getDefaultToolkit().beep();
                return true;
            }
        } else {
            if (ch == ' ') {
                int length = rawText.length();
                if (length == 3 || length == 6 || length == 9) {
                    convertFirstTime();
                    return true;
                }
            } else if (ch == '\n') {
                if (rawText.length() != 0) {
                    commitAll();
                    return true;
                }
            } else if (ch == '\b') {
                if (insertionPoint > 0) {
                    rawText.deleteCharAt(insertionPoint - 1);
                    --insertionPoint;
                    sendText(false);
                    return true;
                }
            } else if ('a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z') {
                rawText.insert(insertionPoint++, ch);
                sendText(false);
                return true;
            }
            if (rawText.length() != 0) {
                Toolkit.getDefaultToolkit().beep();
                return true;
            }
        }
        return false;
    }

    /*
     * Looks up the entry for key in the given table, taken the
     * input method's locale into consideration.
     */
    String lookup(String lookupName, Properties table) {
        String result = null;
        String localeLookupName = lookupName + "_" + locale;
        while (true) {
            result = (String) table.get(localeLookupName);
            if (result != null) {
                break;
            }
            int index = localeLookupName.lastIndexOf("_");
            if (index == -1) {
                break;
            }
            localeLookupName = localeLookupName.substring(0, index);
        }
        return result;
    }

    String findAlias(String lookupName) {
        lookupName = lookupName.toUpperCase();
        return cityAliases.getProperty(lookupName, lookupName);
    }

    void convertFirstTime() {
        numSegs = rawText.length() / 3;
        rawTextSegs = new String[numSegs];
        convertedSegs = new String[numSegs];
        for (int i = 0; i < numSegs; ++i) {
            rawTextSegs[i] = rawText.substring(i * 3, (i + 1) *3);
            String alias = findAlias(rawTextSegs[i]);
            String result = lookup(alias, cityNames);
            if (result != null) {
                convertedSegs[i] = result;
            } else {
                convertedSegs[i] = rawText.substring(i * 3, (i + 1) * 3);
            }
        }
        converted = true;
        sendText(false);
    }

    void convertAgain() {
        String lookupName;
        lookupName = rawTextSegs[selectedSeg];
        // if converted string is same as original, it's not in word list. We skip
        // further conversion.
        if (!lookupName.equals(convertedSegs[selectedSeg])) {
            lookupName = findAlias(lookupName);
            lookupCandidates = new String[LOOKUP_LOCALES.length];
            lookupLocales = new Locale[LOOKUP_LOCALES.length];
            lookupCandidateCount = 0;
            lookupSelection = 0;
            for (int i = 0; i < LOOKUP_LOCALES.length; i++) {
                Locale iLocale = LOOKUP_LOCALES[i];
                String localeLookupName = lookupName + '_' + iLocale;
                String localeConvertedText = (String) cityNames.get(localeLookupName);
                if (localeConvertedText != null) {
                    lookupCandidates[lookupCandidateCount] = localeConvertedText;
                    lookupLocales[lookupCandidateCount] = iLocale;
                    lookupCandidateCount++;
                } else if (iLocale.equals(Locale.ENGLISH)) {
                    localeConvertedText = (String) cityNames.get(lookupName);
                    if (localeConvertedText != null) {
                        lookupCandidates[lookupCandidateCount] = localeConvertedText;
                        lookupLocales[lookupCandidateCount] = iLocale;
                        lookupCandidateCount++;
                    }
                }
                if (convertedSegs[selectedSeg].equals(localeConvertedText)) {
                    lookupSelection = lookupCandidateCount - 1;
                }
            }
            openLookupWindow();
        } else {
            Toolkit.getDefaultToolkit().beep();
        }
    }

    /* commits all chunks up to the specified index */
    private void commit(int index) {
        if (index >= (numSegs - 1)) {
            // if this is the last segment, commit all
            commitAll();
        } else {
            committedSeg = index;
            selectedSeg = committedSeg + 1;
            sendText(true);
        }
    }

    /* commits all chunks */
    void commitAll() {
        committedSeg = numSegs - 1;
        sendText(true);
        // once composed text is committed, reinitialize all variables
        rawText.setLength(0);
        convertedText = null;
        converted = false;

        rawTextSegs = null;
        convertedSegs = null;
        fmt = null;
        fieldPos = null;
        segPos = null;
        selectedSeg = 0;
        insertionPoint = rawText.length();
        numSegs = 0;
        committedSeg = -1;
        previouslyCommittedCharacterCount = 0;
    }

    void parseFormat() {
        Vector vec = new Vector();
        int[] elem = null;

        for(int i = 0; i < fmt.length(); i++) {
            if (fmt.charAt(i) == '{') {
                elem = new int[2];
                elem[0] = i;
            } else if (fmt.charAt(i) == '}') {
                elem[1] = i;
                vec.add(elem);
            }
        }
        if (vec.size() != 0) {
            fieldPos = new int[vec.size()][];
            vec.toArray(fieldPos);
        }
    }

    void formatOutput() {
        if (fmt == null) {
            String key = "Template" + Integer.toString(numSegs);
            fmt = lookup(key, templates);
            parseFormat();
        }
        convertedText = MessageFormat.format(fmt, (Object [])convertedSegs);
        // Figure out converted segment position
        int errors = 0;
        segPos = new int[fieldPos.length][2];
        for (int i = 0; i < fieldPos.length; i++) {
            int optLen = (fieldPos[i][1] - fieldPos[i][0]) + 1;
            int diffs = convertedSegs[i].length() - optLen;
            segPos[i][0] = fieldPos[i][0] + errors;
            segPos[i][1] = segPos[i][0] + convertedSegs[i].length();
            errors += diffs;
        }
    }

    void sendText(boolean committed) {
        AttributedString as = null;
        TextHitInfo caret = null;
        int committedCharacterCount = 0;
        int newTotalCommittedCharacterCount = previouslyCommittedCharacterCount;

        if (converted) {
            formatOutput();
            if (committed) {
                if (committedSeg == (numSegs - 1)) {
                    newTotalCommittedCharacterCount = convertedText.length();
                } else {
                    newTotalCommittedCharacterCount = segPos[committedSeg + 1][0];
                }
                committedCharacterCount = newTotalCommittedCharacterCount - previouslyCommittedCharacterCount;
            }
            as = new AttributedString(convertedText.substring(previouslyCommittedCharacterCount));
            for(int i = committedSeg + 1; i < numSegs; i++) {
                InputMethodHighlight highlight;
                if (i == selectedSeg) {
                    highlight = InputMethodHighlight.SELECTED_CONVERTED_TEXT_HIGHLIGHT;
                } else {
                    highlight = InputMethodHighlight.UNSELECTED_CONVERTED_TEXT_HIGHLIGHT;
                }
                as.addAttribute(TextAttribute.INPUT_METHOD_HIGHLIGHT, highlight,
                                segPos[i][0] - previouslyCommittedCharacterCount,
                                segPos[i][1] - previouslyCommittedCharacterCount);
            }
            previouslyCommittedCharacterCount = newTotalCommittedCharacterCount;
        } else {
            as = new AttributedString(rawText.toString());
            if (committed) {
                committedCharacterCount = rawText.length();
            } else if (rawText.length() != 0) {
                as.addAttribute(TextAttribute.INPUT_METHOD_HIGHLIGHT,
                                InputMethodHighlight.SELECTED_RAW_TEXT_HIGHLIGHT);
                caret = TextHitInfo.leading(insertionPoint);
            }
        }
        inputMethodContext.dispatchInputMethodEvent(
                                                    InputMethodEvent.INPUT_METHOD_TEXT_CHANGED,
                                                    as.getIterator(),
                                                    committedCharacterCount,
                                                    caret,
                                                    null);
    }

    void selectCandidate(int candidate) {
        lookupSelection = candidate;
        lookupList.selectCandidate(lookupSelection);
        convertedSegs[selectedSeg] = lookupCandidates[lookupSelection];
        sendText(false);
    }

    int getSelectedSegmentOffset() {
        return segPos[selectedSeg][0] - previouslyCommittedCharacterCount;
    }

    void openLookupWindow() {
        lookupList = new LookupList(this, inputMethodContext,
                                    lookupCandidates, lookupLocales, lookupCandidateCount);
        lookupList.selectCandidate(lookupSelection);
    }

    void closeLookupWindow() {
        if (lookupList != null) {
            lookupList.setVisible(false);
            lookupList = null;
        }
    }
}

class LookupList extends JPanel {

    CityInputMethod inputMethod;
    InputMethodContext context;
    Window lookupWindow;
    String[] candidates;
    Locale[] locales;
    int candidateCount;
    int selected;

    final int INSIDE_INSET = 4;
    final int LINE_SPACING = 18;

    LookupList(CityInputMethod inputMethod, InputMethodContext context,
               String[] candidates, Locale[] locales, int candidateCount) {
        this.inputMethod = inputMethod;
        this.context = context;
        this.candidates = candidates;
        this.locales = locales;
        this.candidateCount = candidateCount;

        if (inputMethod.methodCreateInputMethodJFrame != null) {
            try {
                Object[] params = new Object[2];
                params[0] = inputMethod.lookupWindowTitle;
                params[1] = Boolean.TRUE;
                lookupWindow =
                    (Window)inputMethod.methodCreateInputMethodJFrame.invoke(context, params);
            } catch (Exception e) {
                lookupWindow =
                    context.createInputMethodWindow(inputMethod.lookupWindowTitle, true);
            }
        } else {
            lookupWindow = context.createInputMethodWindow(inputMethod.lookupWindowTitle, true);
        }
        setFont(new Font("Dialog", Font.PLAIN, 12));
        setPreferredSize(new Dimension(200, candidateCount * LINE_SPACING + 2 * INSIDE_INSET));
        setOpaque(true);
        setForeground(Color.black);
        setBackground(Color.white);

        enableEvents(AWTEvent.KEY_EVENT_MASK);
        enableEvents(AWTEvent.MOUSE_EVENT_MASK);

        if (lookupWindow instanceof JFrame) {
            ((JFrame)lookupWindow).getContentPane().add(this);
        } else {
            lookupWindow.add(this);
        }
        lookupWindow.pack();
        updateWindowLocation();
        lookupWindow.setVisible(true);
    }

    /**
     * Positions the lookup window near (usually below) the
     * insertion point in the component where composition occurs.
     */
    private void updateWindowLocation() {
        Point windowLocation = new Point();
        int textOffset = inputMethod.getSelectedSegmentOffset();
        Rectangle caretRect = context.getTextLocation(TextHitInfo.leading(textOffset));
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension windowSize = lookupWindow.getSize();
        final int SPACING = 2;

        if (caretRect.x + windowSize.width > screenSize.width) {
            windowLocation.x = screenSize.width - windowSize.width;
        } else {
            windowLocation.x = caretRect.x;
        }

        if (caretRect.y + caretRect.height + SPACING + windowSize.height > screenSize.height) {
            windowLocation.y = caretRect.y - SPACING - windowSize.height;
        } else {
            windowLocation.y = caretRect.y + caretRect.height + SPACING;
        }

        lookupWindow.setLocation(windowLocation);
    }

    void selectCandidate(int candidate) {
        selected = candidate;
        repaint();
    }

    public void paint(Graphics g) {
        super.paint(g);
        FontMetrics metrics = g.getFontMetrics();
        int descent = metrics.getDescent();
        int ascent = metrics.getAscent();
        for (int i = 0; i < candidateCount; i++) {
            g.drawString((i + 1) + "   " + candidates[i] +
                    "  (" + locales[i].getDisplayName() + ")",
                    INSIDE_INSET, LINE_SPACING * (i + 1) + INSIDE_INSET - descent);
        }
        Dimension size = getSize();
        g.drawRect(INSIDE_INSET / 2,
                LINE_SPACING * ( selected + 1) + INSIDE_INSET - (descent + ascent + 1),
                size.width - INSIDE_INSET,
                descent + ascent + 2);
        g.drawRect(0, 0, size.width - 1, size.height - 1);
    }

    public void setVisible(boolean visible) {
        if (!visible) {
            lookupWindow.setVisible(false);
            lookupWindow.dispose();
            lookupWindow = null;
        }
        super.setVisible(visible);
    }

    protected void processKeyEvent(KeyEvent event) {
        inputMethod.dispatchEvent(event);
    }

    protected void processMouseEvent(MouseEvent event) {
        if (event.getID() == MouseEvent.MOUSE_PRESSED) {
            int y = event.getY();
            if (y >= INSIDE_INSET && y < INSIDE_INSET + candidateCount * LINE_SPACING) {
                inputMethod.selectCandidate((y - INSIDE_INSET) / LINE_SPACING);
                inputMethod.closeLookupWindow();
            }
        }
    }
}
