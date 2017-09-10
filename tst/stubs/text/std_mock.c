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
#include "std_mock.h"

int mock_write_std_module_init(int res)
{
    harness_mock_write("std_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(std_module_init)()
{
    int res;

    harness_mock_read("std_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_sprintf(char *dst_p,
                           far_string_t fmt_p,
                           ssize_t res)
{
    harness_mock_write("std_sprintf(): return (dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("std_sprintf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_sprintf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_sprintf)(char *dst_p,
                                                 far_string_t fmt_p)
{
    ssize_t res;

    harness_mock_read("std_sprintf(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("std_sprintf(fmt_p)",
                        &fmt_p);

    harness_mock_read("std_sprintf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_snprintf(char *dst_p,
                            size_t size,
                            far_string_t fmt_p,
                            ssize_t res)
{
    harness_mock_write("std_snprintf(): return (dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("std_snprintf(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("std_snprintf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_snprintf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_snprintf)(char *dst_p,
                                                  size_t size,
                                                  far_string_t fmt_p)
{
    ssize_t res;

    harness_mock_read("std_snprintf(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("std_snprintf(size)",
                        &size);

    harness_mock_assert("std_snprintf(fmt_p)",
                        &fmt_p);

    harness_mock_read("std_snprintf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_vsprintf(char *dst_p,
                            far_string_t fmt_p,
                            va_list *ap_p,
                            ssize_t res)
{
    harness_mock_write("std_vsprintf(): return (dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("std_vsprintf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_vsprintf(ap_p)",
                       ap_p,
                       sizeof(*ap_p));

    harness_mock_write("std_vsprintf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_vsprintf)(char *dst_p,
                                                  far_string_t fmt_p,
                                                  va_list *ap_p)
{
    ssize_t res;

    harness_mock_read("std_vsprintf(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("std_vsprintf(fmt_p)",
                        &fmt_p);

    harness_mock_assert("std_vsprintf(ap_p)",
                        ap_p);

    harness_mock_read("std_vsprintf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_vsnprintf(char *dst_p,
                             size_t size,
                             far_string_t fmt_p,
                             va_list *ap_p,
                             ssize_t res)
{
    harness_mock_write("std_vsnprintf(): return (dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("std_vsnprintf(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("std_vsnprintf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_vsnprintf(ap_p)",
                       ap_p,
                       sizeof(*ap_p));

    harness_mock_write("std_vsnprintf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_vsnprintf)(char *dst_p,
                                                   size_t size,
                                                   far_string_t fmt_p,
                                                   va_list *ap_p)
{
    ssize_t res;

    harness_mock_read("std_vsnprintf(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("std_vsnprintf(size)",
                        &size);

    harness_mock_assert("std_vsnprintf(fmt_p)",
                        &fmt_p);

    harness_mock_assert("std_vsnprintf(ap_p)",
                        ap_p);

    harness_mock_read("std_vsnprintf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_printf(far_string_t fmt_p,
                          ssize_t res)
{
    harness_mock_write("std_printf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_printf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_printf)(far_string_t fmt_p)
{
    ssize_t res;

    harness_mock_assert("std_printf(fmt_p)",
                        &fmt_p);

    harness_mock_read("std_printf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_vprintf(far_string_t fmt_p,
                           va_list *ap_p,
                           ssize_t res)
{
    harness_mock_write("std_vprintf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_vprintf(ap_p)",
                       ap_p,
                       sizeof(*ap_p));

    harness_mock_write("std_vprintf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_vprintf)(far_string_t fmt_p,
                                                 va_list *ap_p)
{
    ssize_t res;

    harness_mock_assert("std_vprintf(fmt_p)",
                        &fmt_p);

    harness_mock_assert("std_vprintf(ap_p)",
                        ap_p);

    harness_mock_read("std_vprintf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_fprintf(void *chan_p,
                           far_string_t fmt_p,
                           ssize_t res)
{
    harness_mock_write("std_fprintf(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("std_fprintf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_fprintf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_fprintf)(void *chan_p,
                                                 far_string_t fmt_p)
{
    ssize_t res;

    harness_mock_assert("std_fprintf(chan_p)",
                        chan_p);

    harness_mock_assert("std_fprintf(fmt_p)",
                        &fmt_p);

    harness_mock_read("std_fprintf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_vfprintf(void *chan_p,
                            far_string_t fmt_p,
                            va_list *ap_p,
                            ssize_t res)
{
    harness_mock_write("std_vfprintf(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("std_vfprintf(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_vfprintf(): return (ap_p)",
                       ap_p,
                       sizeof(*ap_p));

    harness_mock_write("std_vfprintf(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_vfprintf)(void *chan_p,
                                                  far_string_t fmt_p,
                                                  va_list *ap_p)
{
    ssize_t res;

    harness_mock_assert("std_vfprintf(chan_p)",
                        chan_p);

    harness_mock_assert("std_vfprintf(fmt_p)",
                        &fmt_p);

    harness_mock_read("std_vfprintf(): return (ap_p)",
                      ap_p,
                      -1);

    harness_mock_read("std_vfprintf(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_printf_isr(far_string_t fmt_p,
                              ssize_t res)
{
    harness_mock_write("std_printf_isr(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_printf_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_printf_isr)(far_string_t fmt_p)
{
    ssize_t res;

    harness_mock_assert("std_printf_isr(fmt_p)",
                        &fmt_p);

    harness_mock_read("std_printf_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_fprintf_isr(void *chan_p,
                               far_string_t fmt_p,
                               ssize_t res)
{
    harness_mock_write("std_fprintf_isr(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("std_fprintf_isr(fmt_p)",
                       &fmt_p,
                       sizeof(fmt_p));

    harness_mock_write("std_fprintf_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_fprintf_isr)(void *chan_p,
                                                     far_string_t fmt_p)
{
    ssize_t res;

    harness_mock_assert("std_fprintf_isr(chan_p)",
                        chan_p);

    harness_mock_assert("std_fprintf_isr(fmt_p)",
                        &fmt_p);

    harness_mock_read("std_fprintf_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strtolb(const char *str_p,
                           long *value_p,
                           int base,
                           const char *res)
{
    harness_mock_write("std_strtolb(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("std_strtolb(): return (value_p)",
                       value_p,
                       sizeof(*value_p));

    harness_mock_write("std_strtolb(base)",
                       &base,
                       sizeof(base));

    harness_mock_write("std_strtolb(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(std_strtolb)(const char *str_p,
                                                     long *value_p,
                                                     int base)
{
    const char *res;

    harness_mock_assert("std_strtolb(str_p)",
                        str_p);

    harness_mock_read("std_strtolb(): return (value_p)",
                      value_p,
                      -1);

    harness_mock_assert("std_strtolb(base)",
                        &base);

    harness_mock_read("std_strtolb(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strtol(const char *str_p,
                          long *value_p,
                          const char *res)
{
    harness_mock_write("std_strtol(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("std_strtol(): return (value_p)",
                       value_p,
                       sizeof(*value_p));

    harness_mock_write("std_strtol(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(std_strtol)(const char *str_p,
                                                    long *value_p)
{
    const char *res;

    harness_mock_assert("std_strtol(str_p)",
                        str_p);

    harness_mock_read("std_strtol(): return (value_p)",
                      value_p,
                      -1);

    harness_mock_read("std_strtol(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strtod(const char *str_p,
                          double *value_p,
                          const char *res)
{
    harness_mock_write("std_strtod(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("std_strtod(): return (value_p)",
                       value_p,
                       sizeof(*value_p));

    harness_mock_write("std_strtod(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(std_strtod)(const char *str_p,
                                                    double *value_p)
{
    const char *res;

    harness_mock_assert("std_strtod(str_p)",
                        str_p);

    harness_mock_read("std_strtod(): return (value_p)",
                      value_p,
                      -1);

    harness_mock_read("std_strtod(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strtodfp(const char *str_p,
                            long *value_p,
                            int precision,
                            const char *res)
{
    harness_mock_write("std_strtodfp(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("std_strtodfp(): return (value_p)",
                       value_p,
                       sizeof(*value_p));

    harness_mock_write("std_strtodfp(precision)",
                       &precision,
                       sizeof(precision));

    harness_mock_write("std_strtodfp(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(std_strtodfp)(const char *str_p,
                                                      long *value_p,
                                                      int precision)
{
    const char *res;

    harness_mock_assert("std_strtodfp(str_p)",
                        str_p);

    harness_mock_read("std_strtodfp(): return (value_p)",
                      value_p,
                      -1);

    harness_mock_assert("std_strtodfp(precision)",
                        &precision);

    harness_mock_read("std_strtodfp(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strcpy(char *dst_p,
                          far_string_t src_p,
                          int res)
{
    harness_mock_write("std_strcpy(dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("std_strcpy(src_p)",
                       &src_p,
                       sizeof(src_p));

    harness_mock_write("std_strcpy(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(std_strcpy)(char *dst_p,
                                            far_string_t src_p)
{
    int res;

    harness_mock_assert("std_strcpy(dst_p)",
                        dst_p);

    harness_mock_assert("std_strcpy(src_p)",
                        &src_p);

    harness_mock_read("std_strcpy(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strcmp(const char *str_p,
                          far_string_t fstr_p,
                          int res)
{
    harness_mock_write("std_strcmp(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("std_strcmp(fstr_p)",
                       &fstr_p,
                       sizeof(fstr_p));

    harness_mock_write("std_strcmp(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(std_strcmp)(const char *str_p,
                                            far_string_t fstr_p)
{
    int res;

    harness_mock_assert("std_strcmp(str_p)",
                        str_p);

    harness_mock_assert("std_strcmp(fstr_p)",
                        &fstr_p);

    harness_mock_read("std_strcmp(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strcmp_f(far_string_t fstr0_p,
                            far_string_t fstr1_p,
                            int res)
{
    harness_mock_write("std_strcmp_f(fstr0_p)",
                       &fstr0_p,
                       sizeof(fstr0_p));

    harness_mock_write("std_strcmp_f(fstr1_p)",
                       &fstr1_p,
                       sizeof(fstr1_p));

    harness_mock_write("std_strcmp_f(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(std_strcmp_f)(far_string_t fstr0_p,
                                              far_string_t fstr1_p)
{
    int res;

    harness_mock_assert("std_strcmp_f(fstr0_p)",
                        &fstr0_p);

    harness_mock_assert("std_strcmp_f(fstr1_p)",
                        &fstr1_p);

    harness_mock_read("std_strcmp_f(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strncmp(far_string_t fstr_p,
                           const char *str_p,
                           size_t size,
                           int res)
{
    harness_mock_write("std_strncmp(fstr_p)",
                       &fstr_p,
                       sizeof(fstr_p));

    harness_mock_write("std_strncmp(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("std_strncmp(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("std_strncmp(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(std_strncmp)(far_string_t fstr_p,
                                             const char *str_p,
                                             size_t size)
{
    int res;

    harness_mock_assert("std_strncmp(fstr_p)",
                        &fstr_p);

    harness_mock_assert("std_strncmp(str_p)",
                        str_p);

    harness_mock_assert("std_strncmp(size)",
                        &size);

    harness_mock_read("std_strncmp(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strncmp_f(far_string_t fstr0_p,
                             far_string_t fstr1_p,
                             size_t size,
                             int res)
{
    harness_mock_write("std_strncmp_f(fstr0_p)",
                       &fstr0_p,
                       sizeof(fstr0_p));

    harness_mock_write("std_strncmp_f(fstr1_p)",
                       &fstr1_p,
                       sizeof(fstr1_p));

    harness_mock_write("std_strncmp_f(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("std_strncmp_f(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(std_strncmp_f)(far_string_t fstr0_p,
                                               far_string_t fstr1_p,
                                               size_t size)
{
    int res;

    harness_mock_assert("std_strncmp_f(fstr0_p)",
                        &fstr0_p);

    harness_mock_assert("std_strncmp_f(fstr1_p)",
                        &fstr1_p);

    harness_mock_assert("std_strncmp_f(size)",
                        &size);

    harness_mock_read("std_strncmp_f(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strlen(far_string_t fstr_p,
                          int res)
{
    harness_mock_write("std_strlen(fstr_p)",
                       &fstr_p,
                       sizeof(fstr_p));

    harness_mock_write("std_strlen(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(std_strlen)(far_string_t fstr_p)
{
    int res;

    harness_mock_assert("std_strlen(fstr_p)",
                        &fstr_p);

    harness_mock_read("std_strlen(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_strip(char *str_p,
                         const char *strip_p,
                         char *res)
{
    harness_mock_write("std_strip(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("std_strip(strip_p)",
                       strip_p,
                       strlen(strip_p) + 1);

    harness_mock_write("std_strip(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

char *__attribute__ ((weak)) STUB(std_strip)(char *str_p,
                                             const char *strip_p)
{
    char *res;

    harness_mock_assert("std_strip(str_p)",
                        str_p);

    harness_mock_assert("std_strip(strip_p)",
                        strip_p);

    harness_mock_read("std_strip(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_std_hexdump(void *chan_p,
                           const void *buf_p,
                           size_t size,
                           ssize_t res)
{
    harness_mock_write("std_hexdump(chan_p)",
                       chan_p,
                       size);

    harness_mock_write("std_hexdump(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("std_hexdump(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("std_hexdump(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(std_hexdump)(void *chan_p,
                                                 const void *buf_p,
                                                 size_t size)
{
    ssize_t res;

    harness_mock_assert("std_hexdump(chan_p)",
                        chan_p);

    harness_mock_assert("std_hexdump(buf_p)",
                        buf_p);

    harness_mock_assert("std_hexdump(size)",
                        &size);

    harness_mock_read("std_hexdump(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
