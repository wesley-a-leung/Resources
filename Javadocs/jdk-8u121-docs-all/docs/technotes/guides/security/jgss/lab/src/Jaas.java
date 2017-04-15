/*
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
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

import javax.security.auth.Subject;
import javax.security.auth.login.*;
import javax.security.auth.callback.CallbackHandler;
import java.security.*;
import com.sun.security.auth.callback.TextCallbackHandler;
import java.io.File;

public class Jaas {
    private static String name;
    private static final boolean verbose = false;

    public static void main(String[] args) throws Exception {
        if (args.length > 0) {
            name = args[0];
        } else {
            name = "client";
        }

        // Create action to perform
        PrivilegedExceptionAction action = new MyAction();

        loginAndAction(name, action);
    }

    static void loginAndAction(String name, PrivilegedExceptionAction action)
        throws LoginException, PrivilegedActionException {

        // Create a callback handler
        CallbackHandler callbackHandler = new TextCallbackHandler();

        LoginContext context = null;

        try {
            // Create a LoginContext with a callback handler
            context = new LoginContext(name, callbackHandler);

            // Perform authentication
            context.login();
        } catch (LoginException e) {
            System.err.println("Login failed");
            e.printStackTrace();
            System.exit(-1);
        }

        // Perform action as authenticated user
        Subject subject = context.getSubject();
        if (verbose) {
            System.out.println(subject.toString());
        } else {
            System.out.println("Authenticated principal: " +
                subject.getPrincipals());
        }

        Subject.doAs(subject, action);

        context.logout();
    }

    // Action to perform
    static class MyAction implements PrivilegedExceptionAction {
        MyAction() {
        }

        public Object run() throws Exception {
            // Replace the following with an action to be performed
            // by authenticated user
            System.out.println("Performing secure action ...");
            return null;
        }
    }
}
