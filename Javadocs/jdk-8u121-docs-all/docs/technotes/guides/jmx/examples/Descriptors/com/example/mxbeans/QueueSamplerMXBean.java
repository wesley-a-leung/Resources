/*
 * Copyright (c) 2004, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * QueueSamplerMXBean.java - MXBean interface describing the management
 * operations and attributes for the QueueSampler MXBean. In this case
 * there is a read-only attribute "QueueSample" and an operation "clearQueue".
 */

package com.example.mxbeans;

@Author("Mr Bean")
@Version("1.0")
public interface QueueSamplerMXBean {
    @DisplayName("GETTER: QueueSample")
    public QueueSample getQueueSample();
    @DisplayName("OPERATION: clearQueue")
    public void clearQueue();
}
