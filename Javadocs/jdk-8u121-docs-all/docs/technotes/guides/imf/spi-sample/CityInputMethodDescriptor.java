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

package com.sun.demos.cityim.internal;

import java.awt.Image;
import java.awt.im.spi.InputMethod;
import java.awt.im.spi.InputMethodDescriptor;
import java.util.Locale;

/**
 * Provides sufficient information about an input method
 * to enable selection and loading of that input method.
 * The input method itself is only loaded when it is actually used.
 */

public class CityInputMethodDescriptor implements InputMethodDescriptor {

    private static Locale YOMI = new Locale("ja", "JP", "YOMI");

    public CityInputMethodDescriptor() {
    }

    /**
     * @see java.awt.im.spi.InputMethodDescriptor#getAvailableLocales
     */
    public Locale[] getAvailableLocales() {
        Locale[] locales = {Locale.ENGLISH,
                            Locale.GERMAN,
                            Locale.JAPANESE,
                            YOMI,
                            Locale.SIMPLIFIED_CHINESE,
                            Locale.TRADITIONAL_CHINESE};
        return locales;
    }

    /**
     * @see java.awt.im.spi.InputMethodDescriptor#hasDynamicLocaleList
     */
    public boolean hasDynamicLocaleList() {
        return false;
    }

    /**
     * @see java.awt.im.spi.InputMethodDescriptor#getInputMethodDisplayName
     */
    public synchronized String getInputMethodDisplayName(Locale inputLocale, Locale displayLanguage) {
        String localeName = null;
        if (inputLocale == Locale.ENGLISH) {
            localeName = "English";
        } else if (inputLocale == Locale.GERMAN) {
            localeName = "German";
        } else if (inputLocale == Locale.JAPANESE) {
            localeName = "Japanese";
        } else if (inputLocale == YOMI) {
            localeName = "Japanese Reading";
        } else if (inputLocale == Locale.SIMPLIFIED_CHINESE) {
            localeName = "Simplified Chinese";
        } else if (inputLocale == Locale.TRADITIONAL_CHINESE) {
            localeName = "Traditional Chinese";
        }
        if (localeName != null) {
            return "City Input Method - " + localeName;
        } else {
            return "City Input Method";
        }
    }

    /**
     * @see java.awt.im.spi.InputMethodDescriptor#getInputMethodIcon
     */
    public Image getInputMethodIcon(Locale inputLocale) {
        return null;
    }

    /**
     * @see java.awt.im.spi.InputMethodDescriptor#createInputMethod
     */
    public InputMethod createInputMethod() throws Exception {
        return new CityInputMethod();
    }

    public String toString() {
        Locale loc[] = getAvailableLocales();
        String locnames = null;

        for (int i = 0; i < loc.length; i++) {
            if (locnames == null) {
                locnames = loc[i].toString();
            } else {
                locnames += "," + loc[i];
            }
        }
        return getClass().getName() + "[" +
            "locales=" + locnames +
            ",localelist=" + (hasDynamicLocaleList() ? "dynamic" : "static") +
            "]";
    }
}
