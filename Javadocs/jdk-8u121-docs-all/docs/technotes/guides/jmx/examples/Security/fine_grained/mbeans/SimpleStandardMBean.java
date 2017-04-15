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

/**
 * This is the management interface explicitly defined for the
 * "SimpleStandard" standard MBean.
 *
 * The "SimpleStandard" standard MBean implements this interface
 * in order to be manageable through a JMX agent.
 *
 * The "SimpleStandardMBean" interface shows how to expose for management:
 * - a read/write attribute (named "State") through its getter and setter
 *   methods,
 * - a read-only attribute (named "NbChanges") through its getter method,
 * - an operation (named "reset").
 */
public interface SimpleStandardMBean {

    /**
     * Getter: set the "State" attribute of the "SimpleStandard" standard
     * MBean.
     *
     * @return the current value of the "State" attribute.
     */
    public String getState();

    /**
     * Setter: set the "State" attribute of the "SimpleStandard" standard
     * MBean.
     *
     * @param <VAR>s</VAR> the new value of the "State" attribute.
     */
    public void setState(String s);

    /**
     * Getter: get the "NbChanges" attribute of the "SimpleStandard" standard
     * MBean.
     *
     * @return the current value of the "NbChanges" attribute.
     */
    public int getNbChanges();

    /**
     * Operation: reset to their initial values the "State" and "NbChanges"
     * attributes of the "SimpleStandard" standard MBean.
     */
    public void reset();
}
