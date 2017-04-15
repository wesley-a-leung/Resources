/*
 *
 * Copyright (c) 2001, 2002, Oracle and/or its affiliates. All rights reserved.
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
import java.lang.reflect.*;
import java.util.Arrays;

import javax.security.auth.callback.*;
import javax.security.auth.login.*;
import javax.security.auth.Subject;
import com.sun.security.auth.callback.TextCallbackHandler;

/**
 * <p> This class authenticates a <code>Subject</code> and then
 * executes a specified application as that <code>Subject</code>.
 * To use this class, the java interpreter would typically be invoked as:
 *
 * <pre>
 *    % java -Djava.security.manager \
 *        Login \
 *        <applicationclass> <applicationClass_args>
 * </pre>
 *
 * <p> <i>applicationClass</i> represents the application to be executed
 * as the authenticated <code>Subject</code>,
 * and <i>applicationClass_args</i> are passed as arguments to
 * <i>applicationClass</i>.
 *
 * <p> To perform the authentication, <code>Login</code> uses a
 * <code>LoginContext</code>.  A <code>LoginContext</code> relies on a
 * <code>Configuration</code> to determine the modules that should be used
 * to perform the actual authentication.  The location of the Configuration
 * is dependent upon each Configuration implementation.
 * The default Configuration implementation
 * (<code>com.sun.security.auth.login.ConfigFile</code>)
 * allows the Configuration location to be specified (among other ways)
 * via the <code>java.security.auth.login.config</code> system property.
 * Therefore, the <code>Login</code> class can also be invoked as:
 *
 * <pre>
 *    % java -Djava.security.manager \
 *        -Djava.security.auth.login.config=<configuration_url> \
 *        Login \
 *        <your_application_class> <your_application_class_args>
 * </pre>
 */

public class Login {

    /**
     * <p> Instantate a <code>LoginContext</code> using the
     * provided application classname as the index for the login
     * <code>Configuration</code>.  Authenticate the <code>Subject</code>
     * (three retries are allowed) and invoke
     * <code>Subject.doAsPrivileged</code>
     * with the authenticated <code>Subject</code> and a
     * <code>PrivilegedExceptionAction</code>.
     * The <code>PrivilegedExceptionAction</code>
     * loads the provided application class, and then invokes
     * its public static <code>main</code> method, passing it
     * the application arguments.
     *
     * <p>
     *
     * @param args the arguments for <code>Login</code>.  The first
     *          argument must be the class name of the application to be
     *          invoked once authentication has completed, and the
     *          subsequent arguments are the arguments to be passed
     *          to that application's public static <code>main</code> method.
     */
    public static void main(String[] args) {

        // check for the application's main class
        if (args == null || args.length == 0) {
            System.err.println("Invalid arguments: " +
                "Did not provide name of application class.");
            System.exit(-1);
        }

        LoginContext lc = null;
        try {
            lc = new LoginContext(args[0], new TextCallbackHandler());
        } catch (LoginException le) {
            System.err.println("Cannot create LoginContext. "
                + le.getMessage());
            System.exit(-1);
        } catch (SecurityException se) {
            System.err .println("Cannot create LoginContext. "
                + se.getMessage());
            System.exit(-1);
        }

        // the user has 3 attempts to authenticate successfully
        int i;
        for (i = 0; i < 3; i++) {
            try {

                // attempt authentication
                lc.login();

                // if we return with no exception, authentication succeeded
                break;

            } catch (AccountExpiredException aee) {

                System.err.println("Your account has expired.  " +
                                "Please notify your administrator.");
                System.exit(-1);

            } catch (CredentialExpiredException cee) {

                System.err.println("Your credentials have expired.");
                System.exit(-1);

            } catch (FailedLoginException fle) {

                System.err.println("Authentication Failed");
                try {
                      Thread.currentThread().sleep(3000);
                } catch (Exception e) {
                      // ignore
                }

            } catch (Exception e) {

                System.err.println("Unexpected Exception - unable to continue");
                e.printStackTrace();
                System.exit(-1);
            }
        }

        // did they fail three times?
        if (i == 3) {
            System.err.println("Sorry");
            System.exit(-1);
        }

        // push the subject into the current ACC
        try {
            Subject.doAsPrivileged(lc.getSubject(),
                                   new  MyAction(args),
                                   null);
        } catch (java.security.PrivilegedActionException pae) {
            pae.printStackTrace();
            System.exit(-1);
        }

        System.exit(0);
    }
}

class MyAction implements java.security.PrivilegedExceptionAction {

    String[] origArgs;

    public MyAction(String[] origArgs) {
        this.origArgs = (String[])origArgs.clone();
    }

    public Object run() throws Exception {

        // get the ContextClassLoader
        ClassLoader cl = Thread.currentThread().getContextClassLoader();

        try {
            // get the application class's main method
            Class c = Class.forName(origArgs[0], true, cl);
            Class[] PARAMS = { origArgs.getClass() };
            java.lang.reflect.Method mainMethod = c.getMethod("main", PARAMS);

            // invoke the main method with the remaining args
            String[] appArgs = new String[origArgs.length - 1];
            System.arraycopy(origArgs, 1, appArgs, 0, origArgs.length - 1);
            Object[] args = { appArgs };
            mainMethod.invoke(null /*ignored*/, args);
        } catch (Exception e) {
            throw new java.security.PrivilegedActionException(e);
        }

        // successful completion
        return null;
    }
}
