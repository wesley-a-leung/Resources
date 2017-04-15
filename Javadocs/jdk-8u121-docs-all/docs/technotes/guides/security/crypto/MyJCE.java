/*
 *
 * Copyright (c) 2002, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * -Redistributions of source code must retain the above copyright
 * notice, this  list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduct the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
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
 * DAMAGES OR LIABILITIES  SUFFERED BY LICENSEE AS A RESULT OF  OR
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

import java.io.*;
import java.net.*;
import java.security.cert.*;
import java.security.AccessController;
import java.security.CodeSource;
import java.security.PrivilegedAction;
import java.security.PrivilegedExceptionAction;
import java.security.PrivilegedActionException;
import java.security.Provider;
import java.security.PublicKey;
import java.util.*;
import java.util.jar.*;

public final class MyJCE extends Provider {

    // Flag for avoiding unnecessary self-integrity checking.
    private static boolean verifiedSelfIntegrity = false;

    // Provider's signing cert which is used to sign the jar.
    private static X509Certificate providerCert = null;

    // Raw bytes of provider's own code signing cert.
    // NOTE: YOU NEED TO CHANGE THIS TO YOUR OWN PROVIDER CERTIFICATE
    private static final byte[] bytesOfProviderCert = {
        (byte)0x30, (byte)0x82, (byte)0x03, (byte)0xB4, (byte)0x30, (byte)0x82,
        (byte)0x03, (byte)0x72, (byte)0xA0, (byte)0x03, (byte)0x02, (byte)0x01,
        (byte)0x02, (byte)0x02, (byte)0x02, (byte)0x01, (byte)0x04, (byte)0x30,
        (byte)0x0B, (byte)0x06, (byte)0x07, (byte)0x2A, (byte)0x86, (byte)0x48,
        (byte)0xCE, (byte)0x38, (byte)0x04, (byte)0x03, (byte)0x05, (byte)0x00,
        (byte)0x30, (byte)0x81, (byte)0x90, (byte)0x31, (byte)0x0B, (byte)0x30,
        (byte)0x09, (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x04, (byte)0x06,
        (byte)0x13, (byte)0x02, (byte)0x55, (byte)0x53, (byte)0x31, (byte)0x0B,
        (byte)0x30, (byte)0x09, (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x04,
        (byte)0x08, (byte)0x13, (byte)0x02, (byte)0x43, (byte)0x41, (byte)0x31,
        (byte)0x12, (byte)0x30, (byte)0x10, (byte)0x06, (byte)0x03, (byte)0x55,
        (byte)0x04, (byte)0x07, (byte)0x13, (byte)0x09, (byte)0x50, (byte)0x61,
        (byte)0x6C, (byte)0x6F, (byte)0x20, (byte)0x41, (byte)0x6C, (byte)0x74,
        (byte)0x6F, (byte)0x31, (byte)0x1D, (byte)0x30, (byte)0x1B, (byte)0x06,
        (byte)0x03, (byte)0x55, (byte)0x04, (byte)0x0A, (byte)0x13, (byte)0x14,
        (byte)0x53, (byte)0x75, (byte)0x6E, (byte)0x20, (byte)0x4D, (byte)0x69,
        (byte)0x63, (byte)0x72, (byte)0x6F, (byte)0x73, (byte)0x79, (byte)0x73,
        (byte)0x74, (byte)0x65, (byte)0x6D, (byte)0x73, (byte)0x20, (byte)0x49,
        (byte)0x6E, (byte)0x63, (byte)0x31, (byte)0x23, (byte)0x30, (byte)0x21,
        (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x04, (byte)0x0B, (byte)0x13,
        (byte)0x1A, (byte)0x4A, (byte)0x61, (byte)0x76, (byte)0x61, (byte)0x20,
        (byte)0x53, (byte)0x6F, (byte)0x66, (byte)0x74, (byte)0x77, (byte)0x61,
        (byte)0x72, (byte)0x65, (byte)0x20, (byte)0x43, (byte)0x6F, (byte)0x64,
        (byte)0x65, (byte)0x20, (byte)0x53, (byte)0x69, (byte)0x67, (byte)0x6E,
        (byte)0x69, (byte)0x6E, (byte)0x67, (byte)0x31, (byte)0x1C, (byte)0x30,
        (byte)0x1A, (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x04, (byte)0x03,
        (byte)0x13, (byte)0x13, (byte)0x4A, (byte)0x43, (byte)0x45, (byte)0x20,
        (byte)0x43, (byte)0x6F, (byte)0x64, (byte)0x65, (byte)0x20, (byte)0x53,
        (byte)0x69, (byte)0x67, (byte)0x6E, (byte)0x69, (byte)0x6E, (byte)0x67,
        (byte)0x20, (byte)0x43, (byte)0x41, (byte)0x30, (byte)0x1E, (byte)0x17,
        (byte)0x0D, (byte)0x30, (byte)0x31, (byte)0x31, (byte)0x30, (byte)0x31,
        (byte)0x39, (byte)0x32, (byte)0x33, (byte)0x30, (byte)0x34, (byte)0x33,
        (byte)0x31, (byte)0x5A, (byte)0x17, (byte)0x0D, (byte)0x30, (byte)0x36,
        (byte)0x31, (byte)0x30, (byte)0x32, (byte)0x33, (byte)0x32, (byte)0x33,
        (byte)0x30, (byte)0x34, (byte)0x33, (byte)0x31, (byte)0x5A, (byte)0x30,
        (byte)0x63, (byte)0x31, (byte)0x1D, (byte)0x30, (byte)0x1B, (byte)0x06,
        (byte)0x03, (byte)0x55, (byte)0x04, (byte)0x0A, (byte)0x0C, (byte)0x14,
        (byte)0x53, (byte)0x75, (byte)0x6E, (byte)0x20, (byte)0x4D, (byte)0x69,
        (byte)0x63, (byte)0x72, (byte)0x6F, (byte)0x73, (byte)0x79, (byte)0x73,
        (byte)0x74, (byte)0x65, (byte)0x6D, (byte)0x73, (byte)0x20, (byte)0x49,
        (byte)0x6E, (byte)0x63, (byte)0x31, (byte)0x23, (byte)0x30, (byte)0x21,
        (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x04, (byte)0x0B, (byte)0x0C,
        (byte)0x1A, (byte)0x4A, (byte)0x61, (byte)0x76, (byte)0x61, (byte)0x20,
        (byte)0x53, (byte)0x6F, (byte)0x66, (byte)0x74, (byte)0x77, (byte)0x61,
        (byte)0x72, (byte)0x65, (byte)0x20, (byte)0x43, (byte)0x6F, (byte)0x64,
        (byte)0x65, (byte)0x20, (byte)0x53, (byte)0x69, (byte)0x67, (byte)0x6E,
        (byte)0x69, (byte)0x6E, (byte)0x67, (byte)0x31, (byte)0x1D, (byte)0x30,
        (byte)0x1B, (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x04, (byte)0x03,
        (byte)0x0C, (byte)0x14, (byte)0x53, (byte)0x75, (byte)0x6E, (byte)0x20,
        (byte)0x4D, (byte)0x69, (byte)0x63, (byte)0x72, (byte)0x6F, (byte)0x73,
        (byte)0x79, (byte)0x73, (byte)0x74, (byte)0x65, (byte)0x6D, (byte)0x73,
        (byte)0x20, (byte)0x49, (byte)0x6E, (byte)0x63, (byte)0x30, (byte)0x82,
        (byte)0x01, (byte)0xB5, (byte)0x30, (byte)0x82, (byte)0x01, (byte)0x2A,
        (byte)0x06, (byte)0x05, (byte)0x2B, (byte)0x0E, (byte)0x03, (byte)0x02,
        (byte)0x0C, (byte)0x30, (byte)0x82, (byte)0x01, (byte)0x1F, (byte)0x02,
        (byte)0x81, (byte)0x81, (byte)0x00, (byte)0xFD, (byte)0x7F, (byte)0x53,
        (byte)0x81, (byte)0x1D, (byte)0x75, (byte)0x12, (byte)0x29, (byte)0x52,
        (byte)0xDF, (byte)0x4A, (byte)0x9C, (byte)0x2E, (byte)0xEC, (byte)0xE4,
        (byte)0xE7, (byte)0xF6, (byte)0x11, (byte)0xB7, (byte)0x52, (byte)0x3C,
        (byte)0xEF, (byte)0x44, (byte)0x00, (byte)0xC3, (byte)0x1E, (byte)0x3F,
        (byte)0x80, (byte)0xB6, (byte)0x51, (byte)0x26, (byte)0x69, (byte)0x45,
        (byte)0x5D, (byte)0x40, (byte)0x22, (byte)0x51, (byte)0xFB, (byte)0x59,
        (byte)0x3D, (byte)0x8D, (byte)0x58, (byte)0xFA, (byte)0xBF, (byte)0xC5,
        (byte)0xF5, (byte)0xBA, (byte)0x30, (byte)0xF6, (byte)0xCB, (byte)0x9B,
        (byte)0x55, (byte)0x6C, (byte)0xD7, (byte)0x81, (byte)0x3B, (byte)0x80,
        (byte)0x1D, (byte)0x34, (byte)0x6F, (byte)0xF2, (byte)0x66, (byte)0x60,
        (byte)0xB7, (byte)0x6B, (byte)0x99, (byte)0x50, (byte)0xA5, (byte)0xA4,
        (byte)0x9F, (byte)0x9F, (byte)0xE8, (byte)0x04, (byte)0x7B, (byte)0x10,
        (byte)0x22, (byte)0xC2, (byte)0x4F, (byte)0xBB, (byte)0xA9, (byte)0xD7,
        (byte)0xFE, (byte)0xB7, (byte)0xC6, (byte)0x1B, (byte)0xF8, (byte)0x3B,
        (byte)0x57, (byte)0xE7, (byte)0xC6, (byte)0xA8, (byte)0xA6, (byte)0x15,
        (byte)0x0F, (byte)0x04, (byte)0xFB, (byte)0x83, (byte)0xF6, (byte)0xD3,
        (byte)0xC5, (byte)0x1E, (byte)0xC3, (byte)0x02, (byte)0x35, (byte)0x54,
        (byte)0x13, (byte)0x5A, (byte)0x16, (byte)0x91, (byte)0x32, (byte)0xF6,
        (byte)0x75, (byte)0xF3, (byte)0xAE, (byte)0x2B, (byte)0x61, (byte)0xD7,
        (byte)0x2A, (byte)0xEF, (byte)0xF2, (byte)0x22, (byte)0x03, (byte)0x19,
        (byte)0x9D, (byte)0xD1, (byte)0x48, (byte)0x01, (byte)0xC7, (byte)0x02,
        (byte)0x15, (byte)0x00, (byte)0x97, (byte)0x60, (byte)0x50, (byte)0x8F,
        (byte)0x15, (byte)0x23, (byte)0x0B, (byte)0xCC, (byte)0xB2, (byte)0x92,
        (byte)0xB9, (byte)0x82, (byte)0xA2, (byte)0xEB, (byte)0x84, (byte)0x0B,
        (byte)0xF0, (byte)0x58, (byte)0x1C, (byte)0xF5, (byte)0x02, (byte)0x81,
        (byte)0x81, (byte)0x00, (byte)0xF7, (byte)0xE1, (byte)0xA0, (byte)0x85,
        (byte)0xD6, (byte)0x9B, (byte)0x3D, (byte)0xDE, (byte)0xCB, (byte)0xBC,
        (byte)0xAB, (byte)0x5C, (byte)0x36, (byte)0xB8, (byte)0x57, (byte)0xB9,
        (byte)0x79, (byte)0x94, (byte)0xAF, (byte)0xBB, (byte)0xFA, (byte)0x3A,
        (byte)0xEA, (byte)0x82, (byte)0xF9, (byte)0x57, (byte)0x4C, (byte)0x0B,
        (byte)0x3D, (byte)0x07, (byte)0x82, (byte)0x67, (byte)0x51, (byte)0x59,
        (byte)0x57, (byte)0x8E, (byte)0xBA, (byte)0xD4, (byte)0x59, (byte)0x4F,
        (byte)0xE6, (byte)0x71, (byte)0x07, (byte)0x10, (byte)0x81, (byte)0x80,
        (byte)0xB4, (byte)0x49, (byte)0x16, (byte)0x71, (byte)0x23, (byte)0xE8,
        (byte)0x4C, (byte)0x28, (byte)0x16, (byte)0x13, (byte)0xB7, (byte)0xCF,
        (byte)0x09, (byte)0x32, (byte)0x8C, (byte)0xC8, (byte)0xA6, (byte)0xE1,
        (byte)0x3C, (byte)0x16, (byte)0x7A, (byte)0x8B, (byte)0x54, (byte)0x7C,
        (byte)0x8D, (byte)0x28, (byte)0xE0, (byte)0xA3, (byte)0xAE, (byte)0x1E,
        (byte)0x2B, (byte)0xB3, (byte)0xA6, (byte)0x75, (byte)0x91, (byte)0x6E,
        (byte)0xA3, (byte)0x7F, (byte)0x0B, (byte)0xFA, (byte)0x21, (byte)0x35,
        (byte)0x62, (byte)0xF1, (byte)0xFB, (byte)0x62, (byte)0x7A, (byte)0x01,
        (byte)0x24, (byte)0x3B, (byte)0xCC, (byte)0xA4, (byte)0xF1, (byte)0xBE,
        (byte)0xA8, (byte)0x51, (byte)0x90, (byte)0x89, (byte)0xA8, (byte)0x83,
        (byte)0xDF, (byte)0xE1, (byte)0x5A, (byte)0xE5, (byte)0x9F, (byte)0x06,
        (byte)0x92, (byte)0x8B, (byte)0x66, (byte)0x5E, (byte)0x80, (byte)0x7B,
        (byte)0x55, (byte)0x25, (byte)0x64, (byte)0x01, (byte)0x4C, (byte)0x3B,
        (byte)0xFE, (byte)0xCF, (byte)0x49, (byte)0x2A, (byte)0x03, (byte)0x81,
        (byte)0x84, (byte)0x00, (byte)0x02, (byte)0x81, (byte)0x80, (byte)0x07,
        (byte)0xCC, (byte)0xF6, (byte)0x38, (byte)0x3A, (byte)0xCD, (byte)0xD3,
        (byte)0x58, (byte)0x99, (byte)0x90, (byte)0x0F, (byte)0x71, (byte)0xAF,
        (byte)0xAA, (byte)0xD0, (byte)0x03, (byte)0x27, (byte)0x3B, (byte)0x74,
        (byte)0xE1, (byte)0x64, (byte)0x38, (byte)0x11, (byte)0xBF, (byte)0xFA,
        (byte)0xB7, (byte)0xBF, (byte)0x2C, (byte)0xE7, (byte)0xBB, (byte)0xA7,
        (byte)0x92, (byte)0x2F, (byte)0x08, (byte)0xCE, (byte)0x27, (byte)0xF8,
        (byte)0xB4, (byte)0xFD, (byte)0xD8, (byte)0x14, (byte)0x1D, (byte)0xA3,
        (byte)0x95, (byte)0xBB, (byte)0x03, (byte)0x16, (byte)0xA6, (byte)0xBA,
        (byte)0xBC, (byte)0x35, (byte)0xC0, (byte)0xCD, (byte)0xF9, (byte)0xF5,
        (byte)0x6C, (byte)0xA7, (byte)0x94, (byte)0x5B, (byte)0x23, (byte)0x01,
        (byte)0xF9, (byte)0xAE, (byte)0xF5, (byte)0xC9, (byte)0xE0, (byte)0x81,
        (byte)0x7A, (byte)0xE8, (byte)0xE4, (byte)0x69, (byte)0xEB, (byte)0xF8,
        (byte)0xF5, (byte)0x80, (byte)0x25, (byte)0x04, (byte)0x2C, (byte)0x91,
        (byte)0x73, (byte)0x96, (byte)0x59, (byte)0xB4, (byte)0x06, (byte)0x83,
        (byte)0x17, (byte)0xB2, (byte)0x50, (byte)0xAC, (byte)0x4F, (byte)0xEB,
        (byte)0x9D, (byte)0x51, (byte)0x25, (byte)0x3D, (byte)0xF7, (byte)0xEE,
        (byte)0xB0, (byte)0x24, (byte)0x25, (byte)0x0E, (byte)0xFE, (byte)0xB4,
        (byte)0x32, (byte)0xA1, (byte)0xC4, (byte)0x0E, (byte)0xB3, (byte)0x66,
        (byte)0x41, (byte)0xE0, (byte)0x57, (byte)0xCE, (byte)0x9D, (byte)0xBE,
        (byte)0x33, (byte)0x2E, (byte)0x93, (byte)0x9A, (byte)0xC9, (byte)0x7A,
        (byte)0x57, (byte)0xDC, (byte)0xCD, (byte)0x88, (byte)0x60, (byte)0xA7,
        (byte)0xCE, (byte)0xA3, (byte)0x81, (byte)0x88, (byte)0x30, (byte)0x81,
        (byte)0x85, (byte)0x30, (byte)0x11, (byte)0x06, (byte)0x09, (byte)0x60,
        (byte)0x86, (byte)0x48, (byte)0x01, (byte)0x86, (byte)0xF8, (byte)0x42,
        (byte)0x01, (byte)0x01, (byte)0x04, (byte)0x04, (byte)0x03, (byte)0x02,
        (byte)0x04, (byte)0x10, (byte)0x30, (byte)0x0E, (byte)0x06, (byte)0x03,
        (byte)0x55, (byte)0x1D, (byte)0x0F, (byte)0x01, (byte)0x01, (byte)0xFF,
        (byte)0x04, (byte)0x04, (byte)0x03, (byte)0x02, (byte)0x05, (byte)0xE0,
        (byte)0x30, (byte)0x1D, (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x1D,
        (byte)0x0E, (byte)0x04, (byte)0x16, (byte)0x04, (byte)0x14, (byte)0x55,
        (byte)0x8D, (byte)0x1F, (byte)0x2A, (byte)0x05, (byte)0xAB, (byte)0x9B,
        (byte)0xCE, (byte)0x86, (byte)0x10, (byte)0xAE, (byte)0x3B, (byte)0x5D,
        (byte)0xF6, (byte)0xBA, (byte)0x3F, (byte)0x22, (byte)0xC5, (byte)0x6A,
        (byte)0xCA, (byte)0x30, (byte)0x1F, (byte)0x06, (byte)0x03, (byte)0x55,
        (byte)0x1D, (byte)0x23, (byte)0x04, (byte)0x18, (byte)0x30, (byte)0x16,
        (byte)0x80, (byte)0x14, (byte)0x65, (byte)0xE2, (byte)0xF4, (byte)0x86,
        (byte)0xC9, (byte)0xD3, (byte)0x4E, (byte)0xF0, (byte)0x91, (byte)0x4E,
        (byte)0x58, (byte)0xA2, (byte)0x6A, (byte)0xF5, (byte)0xD8, (byte)0x78,
        (byte)0x5A, (byte)0x9A, (byte)0xC1, (byte)0xA6, (byte)0x30, (byte)0x20,
        (byte)0x06, (byte)0x03, (byte)0x55, (byte)0x1D, (byte)0x11, (byte)0x04,
        (byte)0x19, (byte)0x30, (byte)0x17, (byte)0x81, (byte)0x15, (byte)0x79,
        (byte)0x75, (byte)0x2D, (byte)0x63, (byte)0x68, (byte)0x69, (byte)0x6E,
        (byte)0x67, (byte)0x2E, (byte)0x70, (byte)0x65, (byte)0x6E, (byte)0x67,
        (byte)0x40, (byte)0x73, (byte)0x75, (byte)0x6E, (byte)0x2E, (byte)0x63,
        (byte)0x6F, (byte)0x6D, (byte)0x30, (byte)0x0B, (byte)0x06, (byte)0x07,
        (byte)0x2A, (byte)0x86, (byte)0x48, (byte)0xCE, (byte)0x38, (byte)0x04,
        (byte)0x03, (byte)0x05, (byte)0x00, (byte)0x03, (byte)0x2F, (byte)0x00,
        (byte)0x30, (byte)0x2C, (byte)0x02, (byte)0x14, (byte)0x75, (byte)0x4B,
        (byte)0xE8, (byte)0x21, (byte)0x37, (byte)0x78, (byte)0x79, (byte)0x0A,
        (byte)0xD0, (byte)0xB5, (byte)0xDC, (byte)0x7E, (byte)0x36, (byte)0x75,
        (byte)0xB9, (byte)0xE4, (byte)0x14, (byte)0xB5, (byte)0xD0, (byte)0x46,
        (byte)0x02, (byte)0x14, (byte)0x6A, (byte)0x51, (byte)0xDC, (byte)0xBA,
        (byte)0x6D, (byte)0x1A, (byte)0x6B, (byte)0x5C, (byte)0x18, (byte)0x23,
        (byte)0x6A, (byte)0xF1, (byte)0xCA, (byte)0x21, (byte)0x8A, (byte)0x77,
        (byte)0xC2, (byte)0x05, (byte)0x16, (byte)0x42
    };

    // UNCOMMENT FOR TESTING AFTER YOU'VE REPLACED THE ABOVE CERT
    // RAW BYTES WITH YOUR OWN.
    // public static void main(String[] argv) {
    //     System.out.println("Integrity Checking? "
    //   + MyJCE.selfIntegrityChecking());
    // }

    public MyJCE() {
        // First, register provider name, version and description.
        super("MyJCE", 1.0, "sample provider which supports nothing");
        // Set up the provider properties here
        // For examples, reference the Appendix A and B of
        // JCE "How to Implement a Provider" Guide.
        //
        //    ...
        //
    }

    /**
     * Perform self-integrity checking. Call this method in all
     * the constructors of your SPI implementation classes.
     * NOTE: The following implementation assumes that all
     * your provider implementation is packaged inside ONE jar.
     */
    public static final synchronized boolean selfIntegrityChecking() {
        if (verifiedSelfIntegrity) {
            return true;
        }

        URL providerURL = AccessController.doPrivileged(
                                new PrivilegedAction<URL>() {
            public URL run() {
                CodeSource cs = MyJCE.class.getProtectionDomain().
                                            getCodeSource();
                return cs.getLocation();
            }
        });

        if (providerURL == null) {
            return false;
        }

        // Open a connnection to the provider JAR file
        JarVerifier jv = new JarVerifier(providerURL);

        // Make sure that the provider JAR file is signed with
        // provider's own signing certificate.
        try {
            if (providerCert == null) {
                providerCert = setupProviderCert();
            }
            jv.verify(providerCert);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        verifiedSelfIntegrity = true;
        return true;
    }

    /*
     * Set up 'providerCert' with the certificate bytes.
     */
    private static X509Certificate setupProviderCert()
              throws IOException, CertificateException {
        CertificateFactory cf = CertificateFactory.getInstance("X.509");
        ByteArrayInputStream inStream = new ByteArrayInputStream(
                                            bytesOfProviderCert);
        X509Certificate cert = (X509Certificate)
                                cf.generateCertificate(inStream);
        inStream.close();
        return cert;
    }

    static class JarVerifier {

        private URL jarURL = null;
        private JarFile jarFile = null;

        JarVerifier(URL jarURL) {
            this.jarURL = jarURL;
        }

        /**
         * Retrive the jar file from the specified url.
         */
        private JarFile retrieveJarFileFromURL(URL url)
            throws PrivilegedActionException, MalformedURLException {
            JarFile jf = null;

            // Prep the url with the appropriate protocol.
            jarURL =
                url.getProtocol().equalsIgnoreCase("jar") ?
                url :
                new URL("jar:" + url.toString() + "!/");
            // Retrieve the jar file using JarURLConnection
            jf = AccessController.doPrivileged(
                     new PrivilegedExceptionAction<JarFile>() {
                public JarFile run() throws Exception {
                    JarURLConnection conn =
                       (JarURLConnection) jarURL.openConnection();
                    // Always get a fresh copy, so we don't have to
                    // worry about the stale file handle when the
                    // cached jar is closed by some other application.
                    conn.setUseCaches(false);
                    return conn.getJarFile();
                }
            });
            return jf;
        }

        /**
         * First, retrieve the jar file from the URL passed in constructor.
         * Then, compare it to the expected X509Certificate.
         * If everything went well and the certificates are the same, no
         * exception is thrown.
         */
        public void verify(X509Certificate targetCert)
            throws IOException {
            // Sanity checking
            if (targetCert == null) {
                throw new SecurityException("Provider certificate is invalid");
            }

            try {
                if (jarFile == null) {
                    jarFile = retrieveJarFileFromURL(jarURL);
                }
            } catch (Exception ex) {
                SecurityException se = new SecurityException();
                se.initCause(ex);
                throw se;
            }

            Vector<JarEntry> entriesVec = new Vector<JarEntry>();

            // Ensure the jar file is signed.
            Manifest man = jarFile.getManifest();
            if (man == null) {
                throw new SecurityException("The provider is not signed");
            }

            // Ensure all the entries' signatures verify correctly
            byte[] buffer = new byte[8192];
            Enumeration entries = jarFile.entries();

            while (entries.hasMoreElements()) {
                JarEntry je = (JarEntry) entries.nextElement();

                // Skip directories.
                if (je.isDirectory()) continue;
                entriesVec.addElement(je);
                InputStream is = jarFile.getInputStream(je);

                // Read in each jar entry. A security exception will
                // be thrown if a signature/digest check fails.
                int n;
                while ((n = is.read(buffer, 0, buffer.length)) != -1) {
                    // Don't care
                }
                is.close();
            }

            // Get the list of signer certificates
            Enumeration e = entriesVec.elements();

            while (e.hasMoreElements()) {
                JarEntry je = (JarEntry) e.nextElement();

                // Every file must be signed except files in META-INF.
                Certificate[] certs = je.getCertificates();
                if ((certs == null) || (certs.length == 0)) {
                    if (!je.getName().startsWith("META-INF"))
                        throw new SecurityException("The provider " +
                                                    "has unsigned " +
                                                    "class files.");
                } else {
                    // Check whether the file is signed by the expected
                    // signer. The jar may be signed by multiple signers.
                    // See if one of the signers is 'targetCert'.
                    int startIndex = 0;
                    X509Certificate[] certChain;
                    boolean signedAsExpected = false;

                    while ((certChain = getAChain(certs, startIndex)) != null) {
                        if (certChain[0].equals(targetCert)) {
                            // Stop since one trusted signer is found.
                            signedAsExpected = true;
                            break;
                        }
                        // Proceed to the next chain.
                        startIndex += certChain.length;
                    }

                    if (!signedAsExpected) {
                        throw new SecurityException("The provider " +
                                                    "is not signed by a " +
                                                    "trusted signer");
                    }
                }
            }
        }

        /**
         * Extracts ONE certificate chain from the specified certificate array
         * which may contain multiple certificate chains, starting from index
         * 'startIndex'.
         */
        private static X509Certificate[] getAChain(Certificate[] certs,
                                                   int startIndex) {
            if (startIndex > certs.length - 1)
                return null;

            int i;
            // Keep going until the next certificate is not the
            // issuer of this certificate.
            for (i = startIndex; i < certs.length - 1; i++) {
                if (!((X509Certificate)certs[i + 1]).getSubjectDN().
                    equals(((X509Certificate)certs[i]).getIssuerDN())) {
                    break;
                }
            }
            // Construct and return the found certificate chain.
            int certChainSize = (i-startIndex) + 1;
            X509Certificate[] ret = new X509Certificate[certChainSize];
            for (int j = 0; j < certChainSize; j++ ) {
                ret[j] = (X509Certificate) certs[startIndex + j];
            }
            return ret;
        }

        // Close the jar file once this object is no longer needed.
        protected void finalize() throws Throwable {
            jarFile.close();
        }
    }
}
