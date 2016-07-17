/**
 * @file std.c
 * @version 1.1.0
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
#include <stdarg.h>
#include <limits.h>

/* +7 for floating point decimal point and fraction. */
#define VALUE_BUF_MAX (3 * sizeof(long) + 7)

struct buffered_output_t {
    chan_t *chan_p;
    int pos;
    char buffer[CONFIG_STD_OUTPUT_BUFFER_MAX];
};

/**
 * @return true(1) if the character is part of the string, otherwise
 *         false(0).
 */
static int char_in_string(char c, const char *str_p)
{
    while (*str_p != '\0') {
        if (c == *str_p) {
            return (1);
        }

        str_p++;
    }

    return (0);
}

/**
 * Put characters to buffer.
 */
static void sprintf_putc(char c, void *arg_p)
{
    char **dst_pp = arg_p;

    *(*dst_pp)++ = c;
}

/**
 * Put characters to standard output.
 */
static void fprintf_putc(char c, void *arg_p)
{
    struct buffered_output_t *output_p = arg_p;

    output_p->buffer[output_p->pos++] = c;

    if (output_p->pos == membersof(output_p->buffer)) {
        chan_write(output_p->chan_p, output_p->buffer, output_p->pos);
        output_p->pos = 0;
    }
}

/**
 * Flush output buffer to channel.
 */
static void output_flush(struct buffered_output_t *output_p)
{
    if (output_p->pos > 0) {
        chan_write(output_p->chan_p, output_p->buffer, output_p->pos);
        output_p->pos = 0;
    }
}

static void formats(void (*std_putc)(char c, void *arg),
                    void *arg,
                    char *str,
                    char flags,
                    signed char width,
                    char negative_sign)
{
    char *s = str;

    while (*s++ != '\0') {
        width--;
    }

    /* Right justification. */
    if (flags != '-') {
        if ((negative_sign == 1) && (flags == '0')) {
            std_putc(*str++, arg);
        }

        while (width > 0) {
            std_putc(flags, arg);
            width--;
        }
    }

    /* Number */
    while (*str != '\0') {
        std_putc(*str++, arg);
    }

    /* Left justification. */
    while (width > 0) {
        std_putc(' ', arg);
        width--;
    }
}

static char *formati(char c,
                     char *str_p,
                     char radix,
                     va_list *ap_p,
                     char length,
                     char *negative_sign_p)
{
    unsigned long value;
    char digit;

    /* Get argument. */
    if (length == 0) {
        value = (unsigned long)va_arg(*ap_p, int);
    } else {
        value = (unsigned long)va_arg(*ap_p, long);
    }

    if ((c == 'd') && (value & (1UL << (sizeof(value) * CHAR_BIT - 1)))) {
        value *= -1;
        *negative_sign_p = 1;
    }

    if (length == 0) {
        value &= UINT_MAX;
    }

    /* Format number into buffer. */
    do {
        digit = (char)(value % radix);
        value /= radix;
        if (digit > 9) {
            digit += 39;
        }
        *--str_p = ('0' + digit);
    } while (value > 0);

    if (*negative_sign_p == 1) {
        *--str_p = '-';
    }

    return (str_p);
}

static char *formatf(char c,
                     char *str_p,
                     va_list *ap_p,
                     char length,
                     char *negative_sign_p)
{
    double value;
    unsigned long whole_number;
    unsigned long fraction_number;
    int i;

    /* Get argument. */
    value = va_arg(*ap_p, double);

    /* Convert a negative value a to positive. */
#if defined(ARCH_ESP)
    /* This will not work in all cases when the number is close to
     * zero. */
    if ((int)value < 0) {
#else
    if (value < 0.0) {
#endif
        value *= -1.0;
        *negative_sign_p = 1;
    }

    /* Values bigger than 'unsigned long max' are not supported. */
    whole_number = (unsigned long)value;
    /* Always print 6 decimal places. */
    fraction_number = (unsigned long)((value - whole_number) * 1000000.0);

    /* Write fraction number to output buffer. */
    for (i = 0; i < 6; i++) {
        *--str_p = '0' + (fraction_number % 10);
        fraction_number /= 10;
    }

    /* Write the decimal dot. */
    *--str_p = '.';

    /* Write whole number to output buffer. */
    while (whole_number != 0) {
        *--str_p = '0' + (whole_number % 10);
        whole_number /= 10;
    }

    /* Add negative sign if the number is negative. */
    if (*negative_sign_p == 1) {
        *--str_p = '-';
    }

    return (str_p);
}
    
static void vcprintf(void (*std_putc)(char c, void *arg_p),
                     void *arg_p,
                     FAR const char *fmt_p,
                     va_list *ap_p)
{
    char c, flags, length, negative_sign, buf[VALUE_BUF_MAX], *s_p;
    signed char width;

    buf[sizeof(buf) - 1] = '\0';

    while ((c = *fmt_p++) != '\0') {
        if (c != '%') {
            std_putc(c, arg_p);
            continue;
        }

        /* Prototype: %[flags][width][length]specifier  */

        /* Parse the flags. */
        flags = ' ';
        c = *fmt_p++;

        if ((c == '0') || (c == '-')) {
            flags = c;
            c = *fmt_p++;
        }

        /* Parse the width. */
        width = 0;

        while ((c >= '0') && (c <= '9')) {
            width *= 10;
            width += (c - '0');
            c = *fmt_p++;
        }

        /* Parse the length. */
        length = 0;

        if (c == 'l') {
            length = 1;
            c = *fmt_p++;
        }

        if (c == '\0') {
            break;
        }

        /* Parse the specifier. */
        negative_sign = 0;
        switch (c) {
        case 's':
            s_p = va_arg(*ap_p, char*);
            break;
        case 'c':
            buf[sizeof(buf) - 2] = (char)va_arg(*ap_p, int);
            s_p = &buf[sizeof(buf) - 2];
            break;
        case 'd':
        case 'u':
            s_p = formati(c, &buf[sizeof(buf) - 1], 10, ap_p, length, &negative_sign);
            break;
        case 'x':
            s_p = formati(c, &buf[sizeof(buf) - 1], 16, ap_p, length, &negative_sign);
            break;
        case 'f':
            s_p = formatf(c, &buf[sizeof(buf) - 1], ap_p, length, &negative_sign);
            break;
        default:
            std_putc(c, arg_p);
            continue;
        }

        formats(std_putc, arg_p, s_p, flags, width, negative_sign);
    }
}

int std_module_init(void)
{
    return (0);
}

ssize_t std_sprintf(char *dst_p, FAR const char *fmt_p, ...)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(fmt_p != NULL, EINVAL);

    va_list ap;
    char *d_p = dst_p;

    va_start(ap, fmt_p);
    vcprintf(sprintf_putc, &d_p, fmt_p, &ap);
    va_end(ap);
    sprintf_putc('\0', &d_p);

    return (d_p - dst_p - 1);
}

void std_printf(FAR const char *fmt_p, ...)
{
    ASSERTN(fmt_p != NULL, EINVAL);

    va_list ap;
    struct buffered_output_t output;

    output.pos = 0;
    output.chan_p = sys_get_stdout();

    if (output.chan_p != NULL) {
        va_start(ap, fmt_p);
        vcprintf(fprintf_putc, &output, fmt_p, &ap);
        va_end(ap);
        output_flush(&output);
    }
}

void std_vprintf(FAR const char *fmt_p, va_list *ap_p)
{
    ASSERTN(fmt_p != NULL, EINVAL);
    ASSERTN(ap_p != NULL, EINVAL);

    struct buffered_output_t output;

    output.pos = 0;
    output.chan_p = sys_get_stdout();

    if (output.chan_p != NULL) {
        vcprintf(fprintf_putc, &output, fmt_p, ap_p);
        output_flush(&output);
    }
}

void std_fprintf(chan_t *chan_p, FAR const char *fmt_p, ...)
{
    ASSERTN(chan_p != NULL, EINVAL);
    ASSERTN(fmt_p != NULL, EINVAL);

    va_list ap;
    struct buffered_output_t output;

    output.pos = 0;
    output.chan_p = chan_p;

    va_start(ap, fmt_p);
    vcprintf(fprintf_putc, &output, fmt_p, &ap);
    va_end(ap);
    output_flush(&output);
}

void std_vfprintf(chan_t *chan_p, FAR const char *fmt_p, va_list *ap_p)
{
    ASSERTN(chan_p != NULL, EINVAL);
    ASSERTN(fmt_p != NULL, EINVAL);
    ASSERTN(ap_p != NULL, EINVAL);

    struct buffered_output_t output;

    output.pos = 0;
    output.chan_p = chan_p;

    vcprintf(fprintf_putc, &output, fmt_p, ap_p);
    output_flush(&output);
}

const char *std_strtol(const char *str_p, long *value_p)
{
    ASSERTN(str_p != NULL, EINVAL);
    ASSERTN(value_p != NULL, EINVAL);

    unsigned char base = 10;
    unsigned char c;
    long sign = 1;

    c = *str_p++;

    /* Find sign. */
    if (c == '-') {
        c = *str_p++;
        sign = -1;
    }

    /* The number must start with a digit. */
    if (isdigit(c) == 0) {
        return (NULL);
    }

    /* Find base based on prefix. */
    if (c == '0') {
        c = *str_p++;

        if (c == 'x') {
            base = 16;
            c = *str_p++;
        } else if (c == 'b') {
            base = 2;
            c = *str_p++;
        } else {
            base = 8;
        }
    }

    /* Get number. */
    *value_p = 0;

    while (((base == 16) && isxdigit(c))
           || ((base == 10) && isdigit(c))
           || ((base == 8) && (c >= '0') && (c < '8'))
           || ((base == 2) && (c >= '0') && (c < '2'))) {
        *value_p *= base;

        /* Special handling of base 16. */
        if (base == 16) {
            if (c >= 'a') {
                c -= ('a' - '0' - 10);
            } else if (c >= 'A') {
                c -= ('A' - '0' - 10);
            }
        }

        /* Bad value in string. */
        c -= '0';

        if (c >= base) {
            return (NULL);
        }

        *value_p += c;
        c = *str_p++;
    }

    *value_p *= sign;

    return (str_p - 1);
}

int std_strcpy(char *dst_p, FAR const char *fsrc_p)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(fsrc_p != NULL, EINVAL);

    size_t length = 0;

    while (*fsrc_p != '\0') {
        *dst_p++ = *fsrc_p++;
        length++;
    }

    *dst_p = '\0';

    return (length);
}

int std_strcmp(const char *str_p, FAR const char *fstr_p)
{
    ASSERTN(str_p != NULL, EINVAL);
    ASSERTN(fstr_p != NULL, EINVAL);

    while (*str_p++ == *fstr_p++) {
        if (str_p[-1] == '\0') {
            return (0);
        }
    }

    return (str_p[-1] - fstr_p[-1]);
}

int std_strcmp_f(FAR const char *fstr0_p,
                 FAR const char *fstr1_p)
{
    ASSERTN(fstr0_p != NULL, EINVAL);
    ASSERTN(fstr1_p != NULL, EINVAL);

    while (*fstr0_p++ == *fstr1_p++) {
        if (fstr0_p[-1] == '\0') {
            return (0);
        }
    }

    return (fstr0_p[-1] - fstr1_p[-1]);
}

int std_strncmp(FAR const char *fstr_p,
                const char *str_p,
                size_t size)
{
    ASSERTN(fstr_p != NULL, EINVAL);
    ASSERTN(str_p != NULL, EINVAL);

    FAR const char *fstr_end_p = (fstr_p + size);

    /* Match if no characters are compared. */
    if (size == 0) {
        return (0);
    }

    while (*fstr_p++ == *str_p++) {
        if ((fstr_p[-1] == '\0') || (fstr_p == fstr_end_p)) {
            return (0);
        }
    }

    return (fstr_p[-1] - str_p[-1]);
}

int std_strncmp_f(FAR const char *fstr0_p,
                  FAR const char *fstr1_p,
                  size_t size)
{
    ASSERTN(fstr0_p != NULL, EINVAL);
    ASSERTN(fstr1_p != NULL, EINVAL);

    FAR const char *fstr_end_p = (fstr0_p + size);

    /* Match if no characters are compared. */
    if (size == 0) {
        return (0);
    }

    while (*fstr0_p++ == *fstr1_p++) {
        if ((fstr0_p[-1] == '\0') || (fstr0_p == fstr_end_p)) {
            return (0);
        }
    }

    return (fstr0_p[-1] - fstr1_p[-1]);
}

int std_strlen(FAR const char *fstr_p)
{
    ASSERTN(fstr_p != NULL, EINVAL);

    FAR const char *fstr_start_p = fstr_p;

    while (*fstr_p++ != '\0') {
    }

    return (fstr_p - fstr_start_p - 1);
}

char *std_strip(char *str_p, const char *strip_p)
{
    ASSERTN(str_p != NULL, EINVAL);

    char *begin_p;
    size_t length;

    /* Strip whitespace characters by default. */
    if (strip_p == NULL) {
        strip_p = "\t\n\x0b\x0c\r ";
    }

    /* String leading characters. */
    while ((*str_p != '\0') && char_in_string(*str_p, strip_p)) {
        str_p++;
    }

    begin_p = str_p;

    /* Strip training characters. */
    length = strlen(str_p);
    str_p += (length - 1);

    while ((str_p >= begin_p) && char_in_string(*str_p, strip_p)) {
        *str_p = '\0';
        str_p--;
    }

    return (begin_p);
}

int std_strin(const char *str_p, const char *pattern_p)
{
    size_t offset;
    size_t str_len, pattern_len;

    offset = 0;
    str_len = strlen(str_p);
    pattern_len = strlen(pattern_p);

    while ((offset + pattern_len) <= str_len) {
        if (strncmp(pattern_p, str_p + offset, pattern_len) == 0) {
            return (1);
        }

        offset++;
    }

    return (0);
}
