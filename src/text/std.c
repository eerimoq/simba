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
#include <stdarg.h>
#include <limits.h>
#include <math.h>

/* +7 for floating point decimal point and fraction. */
#define VALUE_BUF_MAX (3 * sizeof(long) + 7)

struct buffered_output_t {
    void *chan_p;
    int pos;
    char buffer[CONFIG_STD_OUTPUT_BUFFER_MAX];
    size_t size;
};

struct snprintf_output_t {
    char *dst_p;
    size_t size;
    size_t size_max;
};

static char *skipwhite(const char *q_p)
{
    char *p_p = (char *)q_p;

    while (isspace((int)*p_p)) {
        p_p++;
    }

    return (p_p);
}

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
 * Put characters to buffer.
 */
static void snprintf_putc(char c, void *arg_p)
{
    struct snprintf_output_t *output_p;

    output_p = arg_p;

    if (output_p->size < output_p->size_max) {
        output_p->dst_p[output_p->size] = c;
    }

    output_p->size++;
}

/**
 * Put characters to standard output.
 */
static void fprintf_putc(char c, void *arg_p)
{
    struct buffered_output_t *output_p = arg_p;

    output_p->buffer[output_p->pos++] = c;
    output_p->size++;

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

static void formats(void (*std_putc)(char c, void *arg_p),
                    void *arg_p,
                    char *str_p,
                    char flags,
                    int width,
                    char negative_sign)
{
    char *s_p = str_p;

    while (*s_p++ != '\0') {
        width--;
    }

    /* Right justification. */
    if (flags != '-') {
        if ((negative_sign == 1) && (flags == '0')) {
            std_putc(*str_p++, arg_p);
        }

        while (width > 0) {
            std_putc(flags, arg_p);
            width--;
        }
    }

    /* Number */
    while (*str_p != '\0') {
        std_putc(*str_p++, arg_p);
    }

    /* Left justification. */
    while (width > 0) {
        std_putc(' ', arg_p);
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

    if ((c == 'i') || (c == 'd')) {
        if (value & (1UL << (sizeof(value) * CHAR_BIT - 1))) {
            value *= -1;
            *negative_sign_p = 1;
        }
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

#if CONFIG_FLOAT == 1

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
        value *= -1.0;
        *negative_sign_p = 1;
    }
#else
    if (value < 0.0) {
        value *= -1.0;
        *negative_sign_p = 1;
    }
#endif

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

#endif

static void vcprintf(void (*std_putc)(char c, void *arg_p),
                     void *arg_p,
                     far_string_t fmt_p,
                     va_list *ap_p)
{
    char c, flags, length, negative_sign, buf[VALUE_BUF_MAX], *s_p;
    int width;

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

            if (s_p == NULL) {
                s_p = "(null)";
            }

            break;

        case 'c':
            buf[sizeof(buf) - 2] = (char)va_arg(*ap_p, int);
            s_p = &buf[sizeof(buf) - 2];
            break;

        case 'i':
        case 'd':
        case 'u':
            s_p = formati(c, &buf[sizeof(buf) - 1], 10, ap_p, length, &negative_sign);
            break;

        case 'x':
            s_p = formati(c, &buf[sizeof(buf) - 1], 16, ap_p, length, &negative_sign);
            break;

#if CONFIG_FLOAT == 1
        case 'f':
            s_p = formatf(c, &buf[sizeof(buf) - 1], ap_p, length, &negative_sign);
            break;
#endif

        default:
            std_putc(c, arg_p);
            continue;
        }

        formats(std_putc, arg_p, s_p, flags, width, negative_sign);
    }
}

static void cvcprintf(struct buffered_output_t *output_p,
                      far_string_t fmt_p,
                      va_list *ap_p)
{
    chan_control(output_p->chan_p, CHAN_CONTROL_PRINTF_BEGIN);
    vcprintf(fprintf_putc, output_p, fmt_p, ap_p);
    output_flush(output_p);
    chan_control(output_p->chan_p, CHAN_CONTROL_PRINTF_END);
}

static void print_ascii(void *chan_p, const char *buf_p, size_t size)
{
    int i;

    for (i = 0; i < 16 - size; i++) {
        std_fprintf(chan_p, FSTR("   "));
    }

    std_fprintf(chan_p, FSTR("'"));

    for (i = 0; i < size; i++) {
        std_fprintf(chan_p,
                    FSTR("%c"),
                    isprint((int)buf_p[i]) ? buf_p[i] : '.');
    }

    std_fprintf(chan_p, FSTR("'"));
}

int std_module_init(void)
{
    return (0);
}

ssize_t std_sprintf(char *dst_p, far_string_t fmt_p, ...)
{
    va_list ap;
    ssize_t res;

    va_start(ap, fmt_p);
    res = std_vsprintf(dst_p, fmt_p, &ap);
    va_end(ap);

    return (res);
}

ssize_t std_snprintf(char *dst_p,
                     size_t size,
                     far_string_t fmt_p,
                     ...)
{
    va_list ap;
    ssize_t res;

    va_start(ap, fmt_p);
    res = std_vsnprintf(dst_p, size, fmt_p, &ap);
    va_end(ap);

    return (res);
}

ssize_t std_vsprintf(char *dst_p, far_string_t fmt_p, va_list *ap_p)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(fmt_p != NULL, EINVAL);

    char *d_p = dst_p;

    vcprintf(sprintf_putc, &d_p, fmt_p, ap_p);
    sprintf_putc('\0', &d_p);

    return (d_p - dst_p - 1);
}

ssize_t std_vsnprintf(char *dst_p,
                      size_t size,
                      far_string_t fmt_p,
                      va_list *ap_p)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);
    ASSERTN(fmt_p != NULL, EINVAL);

    struct snprintf_output_t output;

    output.dst_p = dst_p;
    output.size = 0;
    output.size_max = size;

    vcprintf(snprintf_putc, &output, fmt_p, ap_p);
    snprintf_putc('\0', &output);

    return (output.size - 1);
}

ssize_t std_printf(far_string_t fmt_p, ...)
{
    ASSERTN(fmt_p != NULL, EINVAL);

    va_list ap;
    struct buffered_output_t output;

    output.pos = 0;
    output.size = 0;
    output.chan_p = sys_get_stdout();

    va_start(ap, fmt_p);
    cvcprintf(&output, fmt_p, &ap);
    va_end(ap);

    return (output.size);
}

ssize_t std_vprintf(far_string_t fmt_p, va_list *ap_p)
{
    ASSERTN(fmt_p != NULL, EINVAL);
    ASSERTN(ap_p != NULL, EINVAL);

    struct buffered_output_t output;

    output.pos = 0;
    output.size = 0;
    output.chan_p = sys_get_stdout();

    cvcprintf(&output, fmt_p, ap_p);

    return (output.size);
}

ssize_t std_fprintf(void *chan_p, far_string_t fmt_p, ...)
{
    ASSERTN(chan_p != NULL, EINVAL);
    ASSERTN(fmt_p != NULL, EINVAL);

    va_list ap;
    struct buffered_output_t output;

    output.pos = 0;
    output.size = 0;
    output.chan_p = chan_p;

    va_start(ap, fmt_p);
    cvcprintf(&output, fmt_p, &ap);
    va_end(ap);

    return (output.size);
}

ssize_t std_vfprintf(void *chan_p, far_string_t fmt_p, va_list *ap_p)
{
    ASSERTN(chan_p != NULL, EINVAL);
    ASSERTN(fmt_p != NULL, EINVAL);
    ASSERTN(ap_p != NULL, EINVAL);

    struct buffered_output_t output;

    output.pos = 0;
    output.size = 0;
    output.chan_p = chan_p;

    cvcprintf(&output, fmt_p, ap_p);

    return (output.size);
}

const char *std_strtol(const char *str_p, long *value_p)
{
    ASSERTNRN(str_p != NULL, EINVAL);
    ASSERTNRN(value_p != NULL, EINVAL);

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

/*
 * strtod implementation.
 * author: Yasuhiro Matsumoto
 * license: public domain
 */
const char *std_strtod(const char *str, double *value_p)
{
    double d = 0.0;
    int sign;
    int n = 0;
    const char *p, *a;

    a = p = str;
    p = skipwhite(p);

    /* decimal part */
    sign = 1;

    if (*p == '-') {
        sign = -1;
        ++p;
    } else if (*p == '+') {
        ++p;
    }

    if (isdigit((int)*p)) {
        d = (double)(*p++ - '0');

        while (*p && isdigit((int)*p)) {
            d = d * 10.0 + (double)(*p - '0');
            ++p;
            ++n;
        }

        a = p;
    } else if (*p != '.') {
        goto done;
    }

    d *= sign;

    /* fraction part */
    if (*p == '.') {
        double f = 0.0;
        double base = 0.1;
        ++p;

        if (isdigit((int)*p)) {
            while (*p && isdigit((int)*p)) {
                f += base * (*p - '0') ;
                base /= 10.0;
                ++p;
                ++n;
            }
        }

        d += f * sign;
        a = p;
    }

    /* exponential part */
    if ((*p == 'E') || (*p == 'e')) {
        int e = 0;
        ++p;

        sign = 1;

        if (*p == '-') {
            sign = -1;
            ++p;
        } else if (*p == '+') {
            ++p;
        }

        if (isdigit((int)*p)) {
            while (*p == '0') {
                ++p;
            }

            e = (int)(*p++ - '0');

            while (*p && isdigit((int)*p)) {
                e = e * 10 + (int)(*p - '0');
                ++p;
            }

            e *= sign;
        } else if (!isdigit((int)*(a-1))) {
            a = str;
            goto done;
        } else if (*p == 0) {
            goto done;
        }

        if (d == 2.2250738585072011 && e == -308) {
            d = 0.0;
            a = p;
            goto done;
        }

        if (d == 2.2250738585072012 && e <= -308) {
#if __DBL_MANT_DIG__ > 24
            d *= 1.0e-308l;
            a = p;
#else
            d = 0.0;
            a = str;
#endif
            goto done;
        }

        d *= pow(10.0, (double) e);
        a = p;
    } else if (p > str && !isdigit((int)*(p-1))) {
        a = str;
        goto done;
    }

 done:
    *value_p = d;

    return (a);
}

int std_strcpy(char *dst_p, far_string_t fsrc_p)
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

int std_strcmp(const char *str_p, far_string_t fstr_p)
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

int std_strcmp_f(far_string_t fstr0_p,
                 far_string_t fstr1_p)
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

int std_strncmp(far_string_t fstr_p,
                const char *str_p,
                size_t size)
{
    ASSERTN(fstr_p != NULL, EINVAL);
    ASSERTN(str_p != NULL, EINVAL);

    far_string_t fstr_end_p = (fstr_p + size);

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

int std_strncmp_f(far_string_t fstr0_p,
                  far_string_t fstr1_p,
                  size_t size)
{
    ASSERTN(fstr0_p != NULL, EINVAL);
    ASSERTN(fstr1_p != NULL, EINVAL);

    far_string_t fstr_end_p = (fstr0_p + size);

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

int std_strlen(far_string_t fstr_p)
{
    ASSERTN(fstr_p != NULL, EINVAL);

    far_string_t fstr_start_p = fstr_p;

    while (*fstr_p++ != '\0') {
    }

    return (fstr_p - fstr_start_p - 1);
}

char *std_strip(char *str_p, const char *strip_p)
{
    ASSERTNRN(str_p != NULL, EINVAL);

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

ssize_t std_hexdump(void *chan_p, const void *buf_p, size_t size)
{
    const char *b_p;
    int pos;
    ssize_t res;

    res = 0;
    pos = 0;
    b_p = buf_p;

    while (size > 0) {
        if ((pos % 16) == 0) {
            std_fprintf(chan_p, FSTR("%08x: "), pos);
        }

        std_fprintf(chan_p, FSTR("%02x "), b_p[pos] & 0xff);

        if ((pos % 16) == 15) {
            print_ascii(chan_p, &b_p[pos - 15], 16);
            std_fprintf(chan_p, FSTR("\r\n"));
            res += 78;
        }

        pos++;
        size--;
    }

    print_ascii(chan_p, &b_p[pos - (pos % 16)], pos % 16);
    std_fprintf(chan_p, FSTR("\r\n"));
    res += (60 + (pos % 16));

    return (res);
}
