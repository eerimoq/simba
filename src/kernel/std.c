/**
 * @file std.c
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
#include <stdarg.h>
#include <limits.h>

#define STD_OUTPUT_BUFFER_MAX 16

/* +7 for floating point decimal point and fraction. */
#define VALUE_BUF_MAX (3 * sizeof(long) + 7)

struct std_klog_entry_t {
    char *buf_p;
    int meta;
};

struct std_klog_t {
    char buf[STD_KLOG_BUFFER_MAX];
    char *next_p;
    uint16_t id;
    chan_t *chout_p;
};

struct output_t {
    int pos;
    char buffer[STD_OUTPUT_BUFFER_MAX];
};

FS_COMMAND_DEFINE("/kernel/std/klog_stdout", std_cmd_klog_stdout);
FS_COMMAND_DEFINE("/kernel/std/klog_read", std_cmd_klog_read);
FS_COMMAND_DEFINE("/kernel/std/klog_write", std_cmd_klog_write);

static struct std_klog_t std_klog = {
    .next_p = std_klog.buf + 1,
    .id = 0,
    .chout_p = NULL
};

/* Buffered output. */
static struct output_t output = {
    .pos = 0
};

int std_cmd_klog_stdout(int argc,
                        const char *argv[],
                        chan_t *chout_p,
                        chan_t *chin_p)
{
    long value;

    if (argc != 2) {
        std_fprintf(chout_p, FSTR("Usage: stdout {1,0}\r\n"));
        return (-EINVAL);
    }

    if (std_strtol(argv[1], &value)) {
        std_fprintf(chout_p, FSTR("bad value '%s'\r\n"), argv[1]);
        return (-EINVAL);
    }

    if (value == 1) {
        std_klog_set_output_channel(sys_get_stdout());
    } else {
        std_klog_set_output_channel(NULL);
    }

    return (0);
}

int std_cmd_klog_read(int argc,
                      const char *argv[],
                      chan_t *chout_p,
                      chan_t *chin_p)
{
    struct std_readk_t iter;
    char buf[64];

    STD_READK_ITER_INIT(&iter);

    while (std_readk(buf,
                     sizeof(buf),
                     &iter) == 1) {
        std_fprintf(chout_p, FSTR("%s"), buf);
    }

    return (0);
}

int std_cmd_klog_write(int argc,
                       const char *argv[],
                       chan_t *chout_p,
                       chan_t *chin_p)
{
    if (argc != 2) {
        return (-EINVAL);
    }

    std_printk(STD_LOG_NOTICE, FSTR("%s"), argv[1]);

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
    chan_t *chan_p = arg_p;

    output.buffer[output.pos++] = c;

    if (output.pos == membersof(output.buffer)) {
        chan_write(chan_p, output.buffer, output.pos);
        output.pos = 0;
    }
}

/**
 * Get character from kernel log.
 */
static char printk_getc(struct std_readk_t *iter_p)
{
    char c;

    c = *iter_p->buf_p++;

    if (iter_p->buf_p == &std_klog.buf[STD_KLOG_BUFFER_MAX]) {
        iter_p->buf_p = std_klog.buf;
    }

    return (c);
}

/**
 * Put characters to kernel log.
 */
static void printk_putc(char c, void *arg_p)
{
    struct std_klog_entry_t *entry_p = arg_p;

    *entry_p->buf_p++ = c;

    if (entry_p->buf_p == &std_klog.buf[STD_KLOG_BUFFER_MAX]) {
        entry_p->buf_p = std_klog.buf;
    }

    if ((std_klog.chout_p != NULL) && (entry_p->meta == 0)) {
        fprintf_putc(c, std_klog.chout_p);
    }
}

/**
 * Flush output buffer to channel.
 */
static void output_flush(chan_t *chan_p)
{
    if (output.pos > 0) {
        chan_write(chan_p, output.buffer, output.pos);
        output.pos = 0;
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
    unsigned int whole_number;
    unsigned int fraction_number;
    int i;

    /* Get argument. */
    value = va_arg(*ap_p, double);

    /* Convert a negative value to positive. */
    if (value < 0.0) {
        value *= -1.0;
        *negative_sign_p = 1;
    }

    /* Values bigger than 'unsigned int max' are not supported. */
    whole_number = (unsigned int)value;
    /* Always print 6 decimal places. */
    fraction_number = (unsigned int)((value - whole_number) * 1000000.0);

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

static void std_vprintf(void (*std_putc)(char c, void *arg_p),
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
    memset(std_klog.buf, -1, sizeof(std_klog.buf));
    /* First footer. */
    std_klog.buf[0] = '\0';

    return (0);
}

ssize_t std_sprintf(char *dst_p, FAR const char *fmt_p, ...)
{
    va_list ap;
    char *d_p = dst_p;

    va_start(ap, fmt_p);
    std_vprintf(sprintf_putc, &d_p, fmt_p, &ap);
    va_end(ap);
    sprintf_putc('\0', &d_p);

    return (d_p - dst_p - 1);
}

void std_printf(FAR const char *fmt_p, ...)
{
    va_list ap;
    chan_t *stdout_p;

    stdout_p = sys_get_stdout();

    if (stdout_p != NULL) {
        va_start(ap, fmt_p);
        std_vprintf(fprintf_putc, stdout_p, fmt_p, &ap);
        va_end(ap);
        output_flush(stdout_p);
    }
}

void std_fprintf(chan_t *chan_p, FAR const char *fmt_p, ...)
{
    va_list ap;

    va_start(ap, fmt_p);
    std_vprintf(fprintf_putc, chan_p, fmt_p, &ap);
    va_end(ap);
    output_flush(chan_p);
}

/**
 * Kernel buffer design:
 *                 next
 * >-|------e3------|--unused--|-------e0------|--e1--|---e2-_p->
 *
 * Each entry has a footer with information about where the entry
 * starts. An entry is valid if it's footer id and the previous entry's
 * footer id differs by one.
 */
void std_printk(char level, FAR const char *fmt_p, ...)
{
    va_list ap;
    char buf[32], *b_p = buf;
    struct time_t time;
    struct std_klog_entry_t entry;

    /* Check if severity level is set. */
    if ((thrd_get_log_mask() & (1 << level)) == 0) {
        return;
    }

    /* Add entry header. */
    entry.buf_p = std_klog.next_p;
    entry.meta = 1;
    printk_putc('\0', &entry);

    /* Fromat and print header. */
    time_get(&time);
    std_sprintf(buf, FSTR("%u:%lu: "), (unsigned int)std_klog.id++, time.seconds);
    entry.meta = 0;

    while (*b_p != '\0') {
        printk_putc(*b_p, &entry);
        b_p++;
    }

    /* Format and print user text. */
    va_start(ap, fmt_p);
    std_vprintf(printk_putc, &entry, fmt_p, &ap);
    va_end(ap);

    printk_putc('\r', &entry);
    printk_putc('\n', &entry);

    /* Add entry footer. */
    entry.meta = 1;
    printk_putc('\0', &entry);

    /* Flush buffered output. */
    if (std_klog.chout_p != NULL) {
        output_flush(std_klog.chout_p);
    }

    /* Update next entry pointer. */
    std_klog.next_p = entry.buf_p;
}

int std_readk(char *buf_p, size_t size, struct std_readk_t *iter_p)
{
    char c;

    /* Initialize. */
    if (iter_p->buf_p == NULL) {
        iter_p->buf_p = std_klog.next_p;

        /* Forward to first footer. */
        while (printk_getc(iter_p) != '\0') {
        }
    }

    /* Entry header found? */
    if (printk_getc(iter_p) != '\0') {
        return (-1);
    }

    /* Read entry. */
    while ((c = printk_getc(iter_p)) != '\0') {
        if (size-- > 1) {
            *buf_p++ = c;
        }
    }

    *buf_p = '\0';

    return (1);
}

void std_klog_set_output_channel(chan_t *chan_p)
{
    std_klog.chout_p = chan_p;
}

int std_strtol(const char *str_p, long *value_p)
{
    unsigned char base = 10;
    unsigned char c;
    long sign = 1;

    c = *str_p++;

    /* Find sign. */
    if (c == '-') {
        c = *str_p++;
        sign = -1;
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

    while (c != '\0') {
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
            return (-EINVAL);
        }

        *value_p += c;
        c = *str_p++;
    }

    *value_p *= sign;

    return (0);
}

int std_strcmp(const char *str_p, FAR const char *fstr_p)
{
    while (*str_p++ == *fstr_p++) {
        if (str_p[-1] == '\0') {
            return (0);
        }
    }

    return (1);
}

int std_strcmp_f(FAR const char *fstr0_p,
                 FAR const char *fstr1_p)
{
    while (*fstr0_p++ == *fstr1_p++) {
        if (fstr0_p[-1] == '\0') {
            return (0);
        }
    }

    return (fstr0_p[-1] - fstr1_p[-1]);
}

int std_strncmp_f(FAR const char *fstr0_p,
                  FAR const char *fstr1_p,
                  size_t size)
{
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
    FAR const char *fstr_start_p = fstr_p;

    while (*fstr_p++ != '\0') {
    }

    return (fstr_p - fstr_start_p - 1);
}
