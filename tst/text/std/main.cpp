/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"
#include <ctype.h>

static ssize_t test_vprintf_wrapper(far_string_t fmt_p, ...)
{
    ssize_t res;
    va_list ap;

    va_start(ap, fmt_p);
    res = std_vprintf(fmt_p, &ap);
    va_end(ap);

    return (res);
}

static ssize_t test_vfprintf_wrapper(void *chan_p,
                                     far_string_t fmt_p,
                                     ...)
{
    ssize_t res;
    va_list ap;

    va_start(ap, fmt_p);
    res = std_vfprintf(chan_p, fmt_p, &ap);
    va_end(ap);

    std_printf(FSTR("res = %d\r\n"), res);

    return (res);
}

static void on_printf_isr_expired(void *arg_p)
{
    std_printf_isr(FSTR("std_printf_isr(): %d\r\n"), 1);
    std_fprintf_isr(arg_p, FSTR("std_fprintf_isr(): %d\r\n"), 2);
}

static int test_sprintf(struct harness_t *harness_p)
{
    char buf[128];
    ssize_t size;

    size = std_sprintf(buf,
                       FSTR("Normal:                '%c' '%d' '%lu' '%s'"),
                       'b', -43, 0xffffffffUL, "foo");
    std_printf(FSTR("%s\r\n"), buf);
    BTASSERT(size == 51);
    BTASSERTM(&buf[0],
              "Normal:                'b' '-43' '4294967295' 'foo'",
              size + 1);

    size = std_sprintf(buf,
                       FSTR("Left justification:    '%-10c' '%-10d' '%-10lu' '%-10s'"),
                       'b', -43, 0xffffffffUL, "foo");
    BTASSERT(size == 74);
    BTASSERTM(&buf[0],
              "Left justification:    'b         ' '-43       ' "
              "'4294967295' 'foo       '",
              size + 1);

    size = std_sprintf(buf,
                       FSTR("Preceding with blanks: '%10c' '%10d' '%10lu' '%10s'"),
                       'b', -43, 0xffffffffUL, "foo");
    BTASSERT(size == 74);
    BTASSERTM(&buf[0],
              "Preceding with blanks: '         b' '       -43' "
              "'4294967295' '       foo'",
              size + 1);

    size = std_sprintf(buf,
                       FSTR("Preceding with zeros:  '%010c' '%010d' '%010lu' '%010s'"),
                       'b', -43, 0xffffffffUL, "foo");
    BTASSERT(size == 74);
    BTASSERTM(&buf[0],
              "Preceding with zeros:  '000000000b' '-000000043' "
              "'4294967295' '0000000foo'",
              size + 1);

    size = std_sprintf(buf, FSTR("Bad format: %g %"));
    BTASSERT(size == 14);
    BTASSERTM(&buf[0], "Bad format: g ", size + 1);

    size = std_sprintf(buf, FSTR("INT_MAX %%i: %i"), 0xffffffffL);
    BTASSERT(size == 14);
    BTASSERTM(&buf[0], "INT_MAX %i: -1", size + 1);

    size = std_sprintf(buf, FSTR("INT_MAX %%d: %d"), 0xffffffffL);
    BTASSERT(size == 14);
    BTASSERTM(&buf[0], "INT_MAX %d: -1", size + 1);

    size = std_sprintf(buf, FSTR("INT_MAX %%lu: %lu"), 0xffffffffL);
    BTASSERT(size == 23);
    BTASSERTM(&buf[0], "INT_MAX %lu: 4294967295", size + 1);

    size = std_sprintf(buf, FSTR("INT_MAX %%lx: %lx"), 0xffffffffL);
    BTASSERT(size == 21);
    BTASSERTM(&buf[0], "INT_MAX %lx: ffffffff", size + 1);

    size = std_sprintf(buf, FSTR("NULL string: %s"), NULL);
    BTASSERTM(&buf[0], "NULL string: (null)", size + 1);

#ifdef ARCH_LINUX
    size = std_sprintf(buf, FSTR("Big unsigned:          '%u'"), 0xffffffffU);
    BTASSERT(size == 35);
    BTASSERTM("Big unsigned:          '4294967295'", &buf[0], size + 1);
#else
    size = std_sprintf(buf, FSTR("Big unsigned:          '%u'"), 0xffffU);
    BTASSERT(size == 30);
    BTASSERTM("Big unsigned:          '65535'", &buf[0], size + 1);
#endif

    return (0);
}

static int test_snprintf(struct harness_t *harness_p)
{
    char buf[128];

    BTASSERT(std_snprintf(buf, sizeof(buf), FSTR("foo")) == 3);
    BTASSERTM(&buf[0], "foo", 4);

    /* Destination buffer too small. */
    memset(buf, -1, sizeof(buf));
    BTASSERT(std_snprintf(buf, 2, FSTR("foo")) == 3);
    BTASSERTM(&buf[0], "fo\xff", 3);

    return (0);
}

static int test_vprintf(struct harness_t *harness_p)
{
    BTASSERT(test_vprintf_wrapper(FSTR("vprintf: %i\r\n"), 1) == 12);

    return (0);
}

static int test_vfprintf(struct harness_t *harness_p)
{
    BTASSERT(test_vfprintf_wrapper(sys_get_stdout(),
                                   FSTR("vprintf: %d\r\n"),
                                   1) == 12);

    return (0);
}

static int test_sprintf_double(struct harness_t *harness_p)
{
    char buf[128];
    ssize_t size;

    size = std_sprintf(&buf[0],
                       FSTR("Normal:                '%f' '%f'"),
                       10.5f, -37.731);
    BTASSERT(size == 47);
#if defined(ARCH_AVR)
    BTASSERTM(&buf[0],
              "Normal:                '10.500000' '-37.730999'",
              size + 1);
#else
    BTASSERTM(&buf[0],
              "Normal:                '10.500000' '-37.731000'",
              size + 1);
#endif

    BTASSERT((size = std_sprintf(buf,
                                 FSTR("Left justification:    '%-12f' '%-12f'"),
                                 10.5f, -37.731)) == 52);
    BTASSERT(size == 52);
#if defined(ARCH_AVR)
    BTASSERTM(&buf[0],
              "Left justification:    '10.500000   ' '-37.730999  '",
              size + 1);
#else
    BTASSERTM(&buf[0],
              "Left justification:    '10.500000   ' '-37.731000  '",
              size + 1);
#endif

    size = std_sprintf(buf,
                       FSTR("Preceding with blanks: '%12f' '%12f'"),
                       10.5f, -37.731);
    BTASSERT(size == 52);
#if defined(ARCH_AVR)
    BTASSERTM(&buf[0],
              "Preceding with blanks: '   10.500000' '  -37.730999'",
              size + 1);
#else
    BTASSERTM(&buf[0],
              "Preceding with blanks: '   10.500000' '  -37.731000'",
              size + 1);
#endif

    size = std_sprintf(buf,
                       FSTR("Preceding with zeros:  '%012f' '%012f'"),
                       10.5f, -37.731);
    BTASSERT(size == 52);
#if defined(ARCH_AVR)
    BTASSERTM(&buf[0],
              "Preceding with zeros:  '00010.500000' '-0037.730999'",
              size + 1);
#else
    BTASSERTM(&buf[0],
              "Preceding with zeros:  '00010.500000' '-0037.731000'",
              size + 1);
#endif

    return (0);
}

static int test_sprintf_unsigned(struct harness_t *harness_p)
{
    char buf[32];
    ssize_t size;

    size = std_sprintf(&buf[0], FSTR("%u"), 1);
    BTASSERT(size == 1);
    BTASSERTM(&buf[0], "1", size + 1);

    size = std_sprintf(&buf[0], FSTR("%03u"), 1);
    BTASSERT(size == 3);
    BTASSERTM(&buf[0], "001", size + 1);

    size = std_sprintf(&buf[0], FSTR("%03u"), 10);
    BTASSERT(size == 3);
    BTASSERTM(&buf[0], "010", size + 1);

    size = std_sprintf(&buf[0], FSTR("%03u"), 100);
    BTASSERT(size == 3);
    BTASSERTM(&buf[0], "100", size + 1);

    size = std_sprintf(&buf[0], FSTR("%03u"), 1000);
    BTASSERT(size == 4);
    BTASSERTM(&buf[0], "1000", size + 1);

    return (0);
}

static int test_sprintf_far_string(struct harness_t *harness_p)
{
    char buf[8];
    ssize_t size;

    size = std_sprintf(&buf[0], FSTR("%S"), FSTR("far"));
    BTASSERT(size == 3);
    BTASSERTM(&buf[0], "far", size + 1);

    /* Left justification. */
    size = std_sprintf(&buf[0], FSTR("%-5S"), FSTR("far"));
    BTASSERT(size == 5);
    BTASSERTM(&buf[0], "far  ", size + 1);

    /* Right justification. */
    size = std_sprintf(&buf[0], FSTR("%5S"), FSTR("far"));
    BTASSERT(size == 5);
    BTASSERTM(&buf[0], "  far", size + 1);

    /* NULL pointer. */
    size = std_sprintf(&buf[0], FSTR("%S"), NULL);
    BTASSERT(size == 6);
    BTASSERTM(&buf[0], "(null)", size + 1);

    return (0);
}

static int test_strtol(struct harness_t *harness_p)
{
    long value;
    const char *next_p;

    /* Positive integers.*/

    /* Base 16. */
    BTASSERT(std_strtol("0x1011", &value) != NULL);
    BTASSERT(value == 0x1011);
    BTASSERT(std_strtol("0xf011", &value) != NULL);
    BTASSERT(value == 0xf011);
    BTASSERT(std_strtol("0xB011", &value) != NULL);
    BTASSERT(value == 0xb011);
    value = 57;
    BTASSERT(std_strtol("0x", &value) == NULL);
    BTASSERT(value == 57);
    BTASSERT(std_strtolb("0x", &value, 16) == NULL);
    BTASSERT(value == 57);
    BTASSERT(std_strtolb("g", &value, 16) == NULL);
    BTASSERT(value == 57);
    BTASSERT(std_strtolb("0xg", &value, 16) == NULL);
    BTASSERT(value == 57);

    /* Base 10. */
    BTASSERT(std_strtol("1011", &value) != NULL);
    BTASSERT(value == 1011);
    BTASSERT(std_strtol("0", &value) != NULL);
    BTASSERT(value == 0);
    BTASSERT(std_strtolb("1011", &value, 10) != NULL);
    BTASSERT(value == 1011);
    BTASSERT(std_strtolb("0x", &value, 10) != NULL);
    BTASSERT(value == 0);
    value = 57;
    BTASSERT(std_strtolb("a", &value, 10) == NULL);
    BTASSERT(value == 57);

    /* Base 8. */
    BTASSERT(std_strtol("01011", &value) != NULL);
    BTASSERT(value == 01011);
    BTASSERT(std_strtolb("01011", &value, 8) != NULL);
    BTASSERT(value == 01011);
    BTASSERT(std_strtolb("0x", &value, 8) != NULL);
    BTASSERT(value == 0);
    value = 57;
    BTASSERT(std_strtolb("8", &value, 8) == NULL);
    BTASSERT(value == 57);

    /* Base 2. */
    BTASSERT(std_strtol("0b1011", &value) != NULL);
    BTASSERT(value == 0b1011);
    BTASSERT(std_strtolb("0b1011", &value, 2) != NULL);
    BTASSERT(value == 0b1011);
    value = 57;
    BTASSERT(std_strtol("0b", &value) == NULL);
    BTASSERT(std_strtolb("0b", &value, 2) == NULL);
    BTASSERT(std_strtolb("2", &value, 2) == NULL);
    BTASSERT(std_strtolb("0b2", &value, 2) == NULL);
    BTASSERT(value == 57);

    /* Negative integers.*/
    BTASSERT(std_strtol("-0x1011", &value) != NULL);
    BTASSERT(value == -0x1011);

    BTASSERT(std_strtol("-1011", &value) != NULL);
    BTASSERT(value == -1011);

    BTASSERT(std_strtol("-0b1011", &value) != NULL);
    BTASSERT(value == -0b1011);

    /* Non-null termination. */
    BTASSERT(std_strtol("0x1r11", &value) != NULL);
    BTASSERT(value == 0x1);
    BTASSERT(std_strtol("0b1012", &value) != NULL);
    BTASSERT(value == 0b101);

    /* Bad input. No value found. */
    value = 57;
    BTASSERT(std_strtol("a", &value) == NULL);
    BTASSERT(value == 57);

    /* Next byte. */
    next_p = std_strtol("0x1011", &value);
    BTASSERT(next_p != NULL);
    BTASSERT(value == 0x1011);
    BTASSERT(*next_p == '\0');

    next_p = std_strtol("0x101.", &value);
    BTASSERT(next_p != NULL);
    BTASSERT(value == 0x101);
    BTASSERT(*next_p == '.');

    return (0);
}

static int test_strcpy(struct harness_t *harness_p)
{
    char buf[16];

    BTASSERT(std_strcpy(buf, FSTR("foo")) == 3);
    BTASSERT(buf[0] == 'f');
    BTASSERT(buf[1] == 'o');
    BTASSERT(buf[2] == 'o');
    BTASSERT(buf[3] == '\0');

    BTASSERT(std_strcpy(buf, FSTR("")) == 0);
    BTASSERT(buf[0] == '\0');

    return (0);
}

static int test_strcmp(struct harness_t *harness_p)
{
    /* Far + local. */
    BTASSERT(std_strncmp(FSTR("foo"), "foo", 3) == 0);
    BTASSERT(std_strncmp(FSTR("foo"), "foo", 0) == 0);
    BTASSERT(std_strncmp(FSTR("foo"), "foobar", 3) == 0);
    BTASSERT(std_strncmp(FSTR("foo"), "foobar", 4) == -'b');

    /* Far + far. */
    BTASSERT(std_strncmp_f(FSTR("foo"), FSTR("foo"), 3) == 0);
    BTASSERT(std_strncmp_f(FSTR("foo"), FSTR("foo"), 0) == 0);
    BTASSERT(std_strncmp_f(FSTR("foo"), FSTR("foobar"), 3) == 0);
    BTASSERT(std_strncmp_f(FSTR("foo"), FSTR("foobar"), 4) == -'b');

    return (0);
}

static int test_strlen(struct harness_t *harness_p)
{
    BTASSERT(std_strlen(FSTR("foo")) == 3);
    BTASSERT(std_strlen(FSTR("")) == 0);

    return (0);
}

static int test_strip(struct harness_t *harness_p)
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

static int test_libc(struct harness_t *harness_p)
{
    int c;
    const char *c_p;

    /* Blank. */
    c_p = " \t";

    while (*c_p != '\0') {
        c = *c_p++;
        BTASSERT(isblank(c) != 0);
    }

    /* Alpha-numeric. */
    c_p = "1aB";

    while (*c_p != '\0') {
        c = *c_p++;
        BTASSERT(isalnum(c) != 0);
    }

    /* atoi. */
    BTASSERT(atoi("1") == 1);

    return (0);
}

static int strtod_test(const char *str_p,
                       int expected_end_offset,
                       double expected_min,
                       double expected_max)
{
    const char *end_p;
    double value;

    end_p = std_strtod(str_p, &value);

    if (end_p == NULL) {
        if (expected_end_offset != 0) {
            std_printf(FSTR("End mismatch.\r\n"));
            return (-1);
        }
    } else if (end_p != &str_p[expected_end_offset]) {
        std_printf(FSTR("End mismatch.\r\n"));
        return (-1);
    }

    if (value < expected_min) {
        std_printf(FSTR("Value too small.\r\n"));
        return (-1);
    }

    if (value > expected_max) {
        std_printf(FSTR("Value too big.\r\n"));
        return (-1);
    }

    return (0);
}

static int test_strtod(struct harness_t *harness_p)
{
    BTASSERT(strtod_test(".1", 2, .1, .1000001) == 0);
    BTASSERT(strtod_test("  .", 0, .0, .0) == 0);
    BTASSERT(strtod_test("--1.2e3.5", 0, .0, .0) == 0);
    BTASSERT(strtod_test("--1-.2e3.5", 0, .0, .0) == 0);
    BTASSERT(strtod_test("-a", 0, .0, .0) == 0);
    BTASSERT(strtod_test("a", 0, .0, .0) == 0);
    BTASSERT(strtod_test(".1e", 2, .1, .1000001) == 0);
    BTASSERT(strtod_test(".1e-", 2, .1, .1000001) == 0);
    BTASSERT(strtod_test(" .e-", 0, .0, .0) == 0);
    BTASSERT(strtod_test(" .e", 0, .0, .0) == 0);
    BTASSERT(strtod_test(" e", 0, .0, .0) == 0);
    BTASSERT(strtod_test(" e0", 0, .0, .0) == 0);
    BTASSERT(strtod_test(" ee", 0, .0, .0) == 0);
    BTASSERT(strtod_test(" -e", 0, .0, .0) == 0);
    BTASSERT(strtod_test(" ..9", 0, .0, .0) == 0);

    /* Unknown why the ESP and AVR only gives approximate values. */
#if defined(ARCH_ESP)
    BTASSERT(strtod_test("  1.2e3", 7, 1.2e3, 1.2000001e3) == 0);
    BTASSERT(strtod_test(" +1.2e3", 7, +1.2e3, +1.2000001e3) == 0);
    BTASSERT(strtod_test("1.2e3", 5, 1.2e3, 1.2000001e3) == 0);
    BTASSERT(strtod_test("+1.2e3", 6, +1.2e3, +1.2000001e3) == 0);
    BTASSERT(strtod_test("+1.e3", 5, +1.e3, +1.e3) == 0);
    BTASSERT(strtod_test("-1.2e3", 6, -1.200001e3, -1.2e3) == 0);
    BTASSERT(strtod_test("-1.2e3.5", 6, -1.200001e3, -1.2e3) == 0);
    BTASSERT(strtod_test("-1.2e", 4, -1.2000001, -1.19999) == 0);
    BTASSERT(strtod_test(".1e3", 4, .1e3, .100001e3) == 0);
    BTASSERT(strtod_test(".1e-3", 5, .1e-3, .100001e-3) == 0);
    BTASSERT(strtod_test(" .9", 3, .9, .900001) == 0);
#elif defined(ARCH_AVR)
    BTASSERT(strtod_test("  1.2e3", 7, 1.1999999e3, 1.2e3) == 0);
    BTASSERT(strtod_test(" +1.2e3", 7, +1.1999999e3, +1.2e3) == 0);
    BTASSERT(strtod_test("1.2e3", 5, 1.1999999e3, 1.2e3) == 0);
    BTASSERT(strtod_test("+1.2e3", 6, +1.1999999e3, +1.2e3) == 0);
    BTASSERT(strtod_test("+1.e3", 5, +.999999e3, +1.e3) == 0);
    BTASSERT(strtod_test("-1.2e3", 6, -1.2e3, -1.199999e3) == 0);
    BTASSERT(strtod_test("-1.2e3.5", 6, -1.2e3, -1.199999e3) == 0);
    BTASSERT(strtod_test("-1.2e", 4, -1.2, -1.2) == 0);
    BTASSERT(strtod_test(".1e3", 4, .0999999e3, .1e3) == 0);
    BTASSERT(strtod_test(".1e-3", 5, .1e-3, .1000001e-3) == 0);
    BTASSERT(strtod_test(" .9", 3, .9, .9000001) == 0);
#else
    BTASSERT(strtod_test("  1.2e3", 7, 1.2e3, 1.2e3) == 0);
    BTASSERT(strtod_test(" +1.2e3", 7, +1.2e3, +1.2e3) == 0);
    BTASSERT(strtod_test("1.2e3", 5, 1.2e3, 1.2e3) == 0);
    BTASSERT(strtod_test("+1.2e3", 6, +1.2e3, +1.2e3) == 0);
    BTASSERT(strtod_test("+1.e3", 5, +1.e3, +1.e3) == 0);
    BTASSERT(strtod_test("-1.2e3", 6, -1.2e3, -1.2e3) == 0);
    BTASSERT(strtod_test("-1.2e3.5", 6, -1.2e3, -1.2e3) == 0);
    BTASSERT(strtod_test("-1.2e", 4, -1.2, -1.2) == 0);
    BTASSERT(strtod_test(".1e3", 4, .1e3, .1e3) == 0);
    BTASSERT(strtod_test(".1e-3", 5, .1e-3, .1e-3) == 0);
    BTASSERT(strtod_test(" .9", 3, .9, .9) == 0);
    BTASSERT(strtod_test("009", 3, 9.0, 9.0) == 0);
    BTASSERT(strtod_test("0.09e02", 7, 9.0, 9.0) == 0);
    /* http://thread.gmane.org/gmane.editors.vim.devel/19268/ */
    BTASSERT(strtod_test("0.9999999999999999999999999999999999", 36, 1.0, 1.0) == 0);
    BTASSERT(strtod_test("2.2250738585072010e-308", 23, .0, .0) == 0); // BUG
    /* PHP (slashdot.jp): http://opensource.slashdot.jp/story/11/01/08/0527259/PHP%E3%81%AE%E6%B5%AE%E5%8B%95%E5%B0%8F%E6%95%B0%E7%82%B9%E5%87%A6%E7%90%86%E3%81%AB%E7%84%A1%E9%99%90%E3%83%AB%E3%83%BC%E3%83%97%E3%81%AE%E3%83%90%E3%82%B0 */
    BTASSERT(strtod_test("2.2250738585072011e-308", 23, .0, .0) == 0);
    /* Gauche: http://blog.practical-scheme.net/gauche/20110203-bitten-by-floating-point-numbers-again */
    BTASSERT(strtod_test("2.2250738585072012e-308",
                         23,
                         2.2250738585072012e-308l,
                         2.2250738585072012e-308l) == 0);
    BTASSERT(strtod_test("2.2250738585072013e-308",
                         23,
                         2.2250738585072013e-308l,
                         2.2250738585072013e-308l) == 0);
    BTASSERT(strtod_test("2.2250738585072014e-308",
                         23,
                         2.2250738585072014e-308l,
                         2.2250738585072014e-308l) == 0);
#endif

    return (0);
}

static int test_strtodfp(struct harness_t *harness_p)
{
    long value;

    const char value_1[] = "1.";
    BTASSERT(std_strtodfp(&value_1[0], &value, 2) == &value_1[2]);
    BTASSERTI(value, ==, 100);

    const char value_2[] = "1.1";
    BTASSERT(std_strtodfp(&value_2[0], &value, 6) == &value_2[3]);
    BTASSERTI(value, ==, 1100000);

    const char value_3[] = "1.01";
    BTASSERT(std_strtodfp(&value_3[0], &value, 6) == &value_3[4]);
    BTASSERTI(value, ==, 1010000);

    const char value_4[] = "0.000012";
    BTASSERT(std_strtodfp(&value_4[0], &value, 6) == &value_4[8]);
    BTASSERTI(value, ==, 12);

    const char value_5[] = "1234.00010";
    BTASSERT(std_strtodfp(&value_5[0], &value, 6) == &value_5[10]);
    BTASSERTI(value, ==, 1234000100);

    const char value_6[] = "1234.00010";
    BTASSERT(std_strtodfp(&value_6[0], &value, 4) == &value_6[9]);
    BTASSERTI(value, ==, 12340001);

    const char value_7[] = "1234.00010";
    BTASSERT(std_strtodfp(&value_7[0], &value, 0) == &value_7[5]);
    BTASSERTI(value, ==, 1234);

    const char value_8[] = "01234.00010";
    BTASSERT(std_strtodfp(&value_8[0], &value, 0) == &value_8[6]);
    BTASSERTI(value, ==, 1234);

    const char value_9[] = "1.y";
    BTASSERT(std_strtodfp(&value_9[0], &value, 0) == &value_9[2]);
    BTASSERTI(value, ==, 1);

    const char value_10[] = "077.077D";
    BTASSERT(std_strtodfp(&value_10[0], &value, 4) == &value_10[7]);
    BTASSERTI(value, ==, 770770);

    const char value_11[] = "1";
    BTASSERT(std_strtodfp(&value_11[0], &value, 6) == &value_11[1]);
    BTASSERTI(value, ==, 1000000);

    const char value_12[] = "x.123";
    BTASSERT(std_strtodfp(&value_12[0], &value, 6) == NULL);

    return (0);
}

static int test_hexdump(struct harness_t *harness_p)
{
    BTASSERT(std_hexdump(sys_get_stdout(),
                         "1234567890abcdefghijkl\xff\r\n",
                         26) == 148);

    return (0);
}

static int test_printf_isr(struct harness_t *harness_p)
{
    struct time_t timeout;
    struct timer_t timer;
    struct queue_t queue;
    uint8_t buf[64];
    
    queue_init(&queue, &buf[0], sizeof(buf));
    
    timeout.seconds = 0;
    timeout.nanoseconds = 1;

    timer_init(&timer,
               &timeout,
               on_printf_isr_expired,
               &queue,
               0);
    timer_start(&timer);

    BTASSERTI(harness_expect(&queue,
                             "std_fprintf_isr(): 2\r\n",
                             NULL), ==, 22);
    
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_sprintf, "test_sprintf" },
        { test_snprintf, "test_snprintf" },
        { test_vprintf, "test_vprintf" },
        { test_vfprintf, "test_vfprintf" },
        { test_strtol, "test_strtol" },
        { test_strcpy, "test_strcpy" },
        { test_strcmp, "test_strcmp" },
        { test_strlen, "test_strlen" },
        { test_sprintf_double, "test_sprintf_double" },
        { test_sprintf_unsigned, "test_sprintf_unsigned" },
        { test_sprintf_far_string, "test_sprintf_far_string" },
        { test_strip, "test_strip" },
        { test_libc, "test_libc" },
        { test_strtod, "test_strtod" },
        { test_strtodfp, "test_strtodfp" },
        { test_hexdump, "test_hexdump" },
        { test_printf_isr, "test_printf_isr" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
