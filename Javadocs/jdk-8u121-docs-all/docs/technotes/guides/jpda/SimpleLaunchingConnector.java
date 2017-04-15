/*
 * Copyright (c) 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 
import com.sun.jdi.VirtualMachine;
import com.sun.jdi.Bootstrap;
import com.sun.jdi.connect.Connector;
import com.sun.jdi.connect.LaunchingConnector;
import com.sun.jdi.connect.Transport;
import com.sun.jdi.connect.IllegalConnectorArgumentsException;
import com.sun.jdi.connect.VMStartException;
import com.sun.jdi.connect.spi.TransportService;
import com.sun.jdi.connect.spi.Connection;
import java.io.IOException;
import java.io.File;
import java.util.Map;
import java.util.HashMap;

public class SimpleLaunchingConnector implements LaunchingConnector {
    TransportService ts;
    String ARG_NAME = "class";

    /*
     * Simple implementation of Connector.StringArgument
     */
    static class StringArgumentImpl implements Connector.StringArgument {
        String name;
        String label;
        String description;
        String value;

        StringArgumentImpl(String name, String label, String description, String value) {
            this.name = name;
            this.label = label;
            this.description = description;
            this.value = value;
        }

        public String name() {
            return name;
        }

        public String label() {
            return label;
        }

        public String description() {
            return description;
        }

        public String value() {
            return value;
        }

        public void setValue(String value) {
            this.value = value;
        }

        public boolean isValid(String value) {
            if (value.length() > 0) {
                return true;
            }
            return false;
        }

        public boolean mustSpecify() {
            return true;
        }
    }

    public SimpleLaunchingConnector() {
        try {
            Class c = Class.forName("com.sun.tools.jdi.SocketTransportService");
            ts = (TransportService)c.newInstance();
        } catch (Exception x) {
            throw new Error(x);
        }
    }

    public String name() {
        return "SimpleLaunchingConnector";
    }

    public String description() {
        return "SimpleLaunchingConnector";
    }

    public Transport transport() {
        return new Transport() {
            public String name() {
                return ts.name();
            }
        };
    }

    public Map<String, Connector.Argument> defaultArguments() {
        HashMap<String, Connector.Argument> map = new HashMap<String, Connector.Argument>();
        map.put(ARG_NAME,
                new StringArgumentImpl(ARG_NAME, "class name", "class name", ""));
        return map;
    }

    public VirtualMachine launch(Map<String, ? extends Connector.Argument> arguments) throws
                              IOException,
                              IllegalConnectorArgumentsException,
                              VMStartException {

        /*
         * Get the class name that we are to execute
         */
        String className = ((StringArgumentImpl)arguments.get(ARG_NAME)).value();
        if (className.length() == 0) {
            throw new IllegalConnectorArgumentsException("class name missing", ARG_NAME);
        }

        /*
         * Listen on an ephemeral port; launch the debuggee; wait
         * for the debuggee to connect; stop listening;
         */
        TransportService.ListenKey key = ts.startListening();

        String exe = System.getProperty("java.home") + File.separator + "bin" +
            File.separator;
        String arch = System.getProperty("os.arch");
        if (arch.equals("sparcv9")) {
            exe += "sparcv9/java";
        } else {
            exe += "java";
        }
        String cmd = exe + "-agentlib:jdwp=transport=dt_socket,timeout=15000,address=" +
            key.address() + "" + className;
        /*
         * If the debuggee VM might be a pre Java SE 5 VM, you
         * should use these options instead of the -agentlib option shown above:
         *  "-Xdebug -Xrunjdwp:transport=dt_socket,timeout=15000,address="
         */
        Process process = Runtime.getRuntime().exec(cmd);
        Connection conn = ts.accept(key, 30*1000, 9*1000);
        ts.stopListening(key);

        /*
         * Debugee is connected - return the virtual machine mirror
         */
        return Bootstrap.virtualMachineManager().createVirtualMachine(conn);
    }
}
