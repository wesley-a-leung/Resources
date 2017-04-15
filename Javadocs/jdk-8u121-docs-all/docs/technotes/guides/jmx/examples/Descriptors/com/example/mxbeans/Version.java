/*
 * Copyright (c) 2004, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * Version.java - This annotation allows to supply
 * the current version of the MBean interface.
 */

package com.example.mxbeans;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import javax.management.DescriptorKey;

@Documented
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface Version {
    @DescriptorKey("version")
    String value();
}
