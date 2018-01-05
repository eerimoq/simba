/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#ifndef __STD_MOCK_H__
#define __STD_MOCK_H__

#include "simba.h"

int mock_write_std_module_init(int res);

int mock_write_std_sprintf(char *dst_p,
                           far_string_t fmt_p,
                           ssize_t res);

int mock_write_std_snprintf(char *dst_p,
                            size_t size,
                            far_string_t fmt_p,
                            ssize_t res);

int mock_write_std_vsprintf(char *dst_p,
                            far_string_t fmt_p,
                            va_list *ap_p,
                            ssize_t res);

int mock_write_std_vsnprintf(char *dst_p,
                             size_t size,
                             far_string_t fmt_p,
                             va_list *ap_p,
                             ssize_t res);

int mock_write_std_printf(far_string_t fmt_p,
                          ssize_t res);

int mock_write_std_vprintf(far_string_t fmt_p,
                           va_list *ap_p,
                           ssize_t res);

int mock_write_std_fprintf(void *chan_p,
                           far_string_t fmt_p,
                           ssize_t res);

int mock_write_std_vfprintf(void *chan_p,
                            far_string_t fmt_p,
                            va_list *ap_p,
                            ssize_t res);

int mock_write_std_printf_isr(far_string_t fmt_p,
                              ssize_t res);

int mock_write_std_fprintf_isr(void *chan_p,
                               far_string_t fmt_p,
                               ssize_t res);

int mock_write_std_strtolb(const char *str_p,
                           long *value_p,
                           int base,
                           const char *res);

int mock_write_std_strtol(const char *str_p,
                          long *value_p,
                          const char *res);

int mock_write_std_strtod(const char *str_p,
                          double *value_p,
                          const char *res);

int mock_write_std_strtodfp(const char *str_p,
                            long *value_p,
                            int precision,
                            const char *res);

int mock_write_std_strcpy(char *dst_p,
                          far_string_t src_p,
                          int res);

int mock_write_std_strcmp(const char *str_p,
                          far_string_t fstr_p,
                          int res);

int mock_write_std_strcmp_f(far_string_t fstr0_p,
                            far_string_t fstr1_p,
                            int res);

int mock_write_std_strncmp(far_string_t fstr_p,
                           const char *str_p,
                           size_t size,
                           int res);

int mock_write_std_strncmp_f(far_string_t fstr0_p,
                             far_string_t fstr1_p,
                             size_t size,
                             int res);

int mock_write_std_strlen(far_string_t fstr_p,
                          int res);

int mock_write_std_strip(char *str_p,
                         const char *strip_p,
                         char *res);

int mock_write_std_hexdump(void *chan_p,
                           const void *buf_p,
                           size_t size,
                           ssize_t res);

#endif
