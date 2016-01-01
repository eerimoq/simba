/**
 * @file main.c
 * @version 0.1.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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
    std_printf(FSTR("%s\r\n"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Left justification:    '%-10c' '%-10d' '%-10lu' '%-10s'"),
                                 'b', -43, 0xffffffffUL, "foo")) == 74);
    std_printf(FSTR("%s\r\n"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with blanks: '%10c' '%10d' '%10lu' '%10s'"),
                                 'b', -43, 0xffffffffUL, "foo")) == 74);
    std_printf(FSTR("%s\r\n"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with zeros:  '%010c' '%010d' '%010lu' '%010s'"),
                                 'b', -43, 0xffffffffUL, "foo")) == 74);
    std_printf(FSTR("%s\r\n"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Bad format: %g %"))) == 14);
    std_printf(FSTR("%s\r\n"), buf);

#ifdef ARCH_LINUX
    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Big unsigned:          '%u'"),
                                 0xffffffffU)) == 35);
#else
    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Big unsigned:          '%u'"),
                                 0xffffU)) == 30);
#endif
    std_printf(FSTR("%s\r\n"), buf);

    return (0);
}

int test_sprintf_double(struct harness_t *harness_p)
{
    char buf[128];
    ssize_t size;

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Normal:                '%f' '%f'"),
                                 10.5f, -37.731)) == 47);
    std_printf(FSTR("%s\r\n"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Left justification:    '%-12f' '%-12f'"),
                                 10.5f, -37.731)) == 52);
    std_printf(FSTR("%s\r\n"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with blanks: '%12f' '%12f'"),
                                 10.5f, -37.731)) == 52);
    std_printf(FSTR("%s\r\n"), buf);

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Preceding with zeros:  '%012f' '%012f'"),
                                 10.5f, -37.731)) == 52);
    std_printf(FSTR("%s\r\n"), buf);

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

int test_strip(struct harness_t *harness_p)
{
    char leading_whitespace[] = "\t hej";
    char trailing_whitespace[] = "hej \r\n";
    char empty_string[] = "";
    char leading_and_trailing_abc_string[] = "abc123abc";
    char buf[32];
    char *expected_p;

    /* Leading whitespace characters. */
    strcpy(buf, leading_whitespace);
    expected_p = &buf[2];
    BTASSERT(std_strip(buf, NULL) == expected_p);

    /* Trailing whitespace characters. */
    strcpy(buf, trailing_whitespace);
    expected_p = buf;
    BTASSERT(std_strip(buf, NULL) == expected_p);
    BTASSERT(buf[3] == '\0');

    /* Empty string. */
    strcpy(buf, empty_string);
    expected_p = buf;
    BTASSERT(std_strip(buf, NULL) == expected_p);
    BTASSERT(buf[0] == '\0');

    /* Strip chosen character set. */
    strcpy(buf, leading_and_trailing_abc_string);
    expected_p = &buf[3];
    BTASSERT(std_strip(buf, "xyzabc") == expected_p);
    BTASSERT(buf[6] == '\0');

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_sprintf, "test_sprintf" },
        { test_strtol, "test_strtol" },
        { test_sprintf_double, "test_sprintf_double" },
        { test_strip, "test_strip" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);
   
    return (0);
}
