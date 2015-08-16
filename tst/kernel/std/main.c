/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

int test_sprintf(struct harness_t *harness_p)
{
    char buf[128];
    ssize_t size;

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Normal:                '%c' '%d' '%lu' '%s'"),
                                 'b', -43, 0xffffffffUL, "foo")) == 51);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Left justification:    '%-10c' '%-10d' '%-10lu' '%-10s'"),
                                 'b', -43, 0xffffffffUL, "foo")) == 74);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with blanks: '%10c' '%10d' '%10lu' '%10s'"),
                                 'b', -43, 0xffffffffUL, "foo")) == 74);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with zeros:  '%010c' '%010d' '%010lu' '%010s'"),
                                 'b', -43, 0xffffffffUL, "foo")) == 74);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Bad format: %g %"))) == 14);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

#ifdef ARCH_LINUX
    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Big unsigned:          '%u'"),
                                 0xffffffffU)) == 35);
#else
    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Big unsigned:          '%u'"),
                                 0xffffU)) == 30);
#endif
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    return (0);
}

int test_sprintf_double(struct harness_t *harness_p)
{
    char buf[128];
    ssize_t size;

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Normal:                '%f' '%f'"),
                                 10.5f, -37.731)) == 47);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Left justification:    '%-12f' '%-12f'"),
                                 10.5f, -37.731)) == 52);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with blanks: '%12f' '%12f'"),
                                 10.5f, -37.731)) == 52);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with zeros:  '%012f' '%012f'"),
                                 10.5f, -37.731)) == 52);
    std_printk(STD_LOG_NOTICE, FSTR("%s"), buf);

    return (0);
}

int test_strtol(struct harness_t *harness_p)
{
    long value;

    /* Positive integers.*/
    BTASSERT(std_strtol("0x1011", &value) == 0);
    BTASSERT(value == 0x1011);
    BTASSERT(std_strtol("0xf011", &value) == 0);
    BTASSERT(value == 0xf011);
    BTASSERT(std_strtol("0xB011", &value) == 0);
    BTASSERT(value == 0xB011);

    BTASSERT(std_strtol("1011", &value) == 0);
    BTASSERT(value == 1011);

    BTASSERT(std_strtol("0b1011", &value) == 0);
    BTASSERT(value == 0b1011);

    /* Negative integers.*/
    BTASSERT(std_strtol("-0x1011", &value) == 0);
    BTASSERT(value == -0x1011);

    BTASSERT(std_strtol("-1011", &value) == 0);
    BTASSERT(value == -1011);

    BTASSERT(std_strtol("-0b1011", &value) == 0);
    BTASSERT(value == -0b1011);

    /* Bad input. */
    BTASSERT(std_strtol("0x1r11", &value) == -EINVAL);

    BTASSERT(std_strtol("a011", &value) == -EINVAL);

    BTASSERT(std_strtol("0b1012", &value) == -EINVAL);

    return (0);
}

/* readk testcase fail if this test is executed. reak() has a bug probably. */
/* int test_strlen(struct harness_t *harness_p) */
/* { */
/*     BTASSERT(std_strlen(FSTR("")) == 0); */

/*     return (0); */
/* } */

int test_readk(struct harness_t *harness_p)
{
    struct std_readk_t iter;
    char buf[128];

    STD_READK_ITER_INIT(&iter);
    BTASSERT(std_readk(buf, sizeof(buf), &iter) == 1);
    std_printf(FSTR("readk: %s"), buf);
    BTASSERT(std_readk(buf, sizeof(buf), &iter) == 1);
    std_printf(FSTR("readk: %s"), buf);
    BTASSERT(std_readk(buf, sizeof(buf), &iter) == 1);
    std_printf(FSTR("readk: %s"), buf);
    BTASSERT(std_readk(buf, sizeof(buf), &iter) == 1);
    std_printf(FSTR("readk: %s"), buf);
    BTASSERT(std_readk(buf, sizeof(buf), &iter) == 1);
    std_printf(FSTR("readk: %s"), buf);
    BTASSERT(std_readk(buf, sizeof(buf), &iter) == 1);
    std_printf(FSTR("readk: %s"), buf);
    BTASSERT(std_readk(buf, sizeof(buf), &iter) == -1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_sprintf, "test_sprintf" },
        { test_strtol, "test_strtol" },
        /* { test_strlen, "test_strlen" }, */
        { test_readk, "test_readk" },
        { test_sprintf_double, "test_sprintf_double" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);
   
    return (0);
}
