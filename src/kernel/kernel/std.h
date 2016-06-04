/**
 * @file kernel/std.h
 * @version 0.6.0
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

#ifndef __KERNEL_STD_H__
#define __KERNEL_STD_H__

#include "simba.h"
#include <stdarg.h>

/**
 * Initialize module.
 *
 * @return zero(0) or negative error code.
 */
int std_module_init(void);

/**
 * Format and write data to destination buffer. The buffer must be big
 * enough to fit the formatted string.
 *
 * A format specifier has this format:
 *
 * %[flags][width][length]specifier
 *
 * where
 *
 * * flags: ``0`` or ``-``
 * * width: ``0``..``127``
 * * length: ``l`` for long or nothing
 * * specifier: ``c``, ``s``, ``d``, ``u``, ``x`` or ``f``
 *
 * @param[out] dst_p Destination buffer. The formatted string is
 *                   written to this buffer.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Number of bytes written to the destination buffer or
 *         negative error code.
 */
ssize_t std_sprintf(char *dst_p, FAR const char *fmt_p, ...);

/**
 * Format and print data to standard output.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguemnts list.
 *
 * @return void.
 */
void std_printf(FAR const char *fmt_p, ...);

/**
 * Format and print data to standard output.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] fmt_p Format string.
 * @param[in] ap_p Variable arguemnts list.
 *
 * @return void.
 */
void std_vprintf(FAR const char *fmt_p, va_list *ap_p);

/**
 * Format and print data to channel.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] chan_p Output channel.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguemnts list.
 *
 * @return void.
 */
void std_fprintf(chan_t *chan_p, FAR const char *fmt_p, ...);

/**
 * Format and print data to channel.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] chan_p Output channel.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguemnts list.
 *
 * @return void.
 */
void std_vfprintf(chan_t *chan_p, FAR const char *fmt_p, va_list *ap_p);

/**
 * Convert string to integer.
 *
 * @param[in] str_p Integer string.
 * @param[out] value_p Integer value.
 *
 * @return zero(0) or negative error code.
 */
int std_strtol(const char *str_p, long *value_p);

/**
 * Copy string from far memory to memory.
 *
 * @param[in] dst_p Normal memory string.
 * @param[in] src_p Far memory string.
 *
 * @return String length or negative error code.
 */
int std_strcpy(char *dst_p, FAR const char *src_p);

/**
 * Compare a string with a far string.
 *
 * @param[in] str_p Normal memory string.
 * @param[in] fstr_p Far memory string.
 *
 * @return zero(0) if match, otherwise the difference of
 *         the mismatched characters
 */
int std_strcmp(const char *str_p, FAR const char *fstr_p);

/**
 * Compare two far strings.
 *
 * @param[in] fstr0_p Far memory string.
 * @param[in] fstr1_p Far memory string.
 *
 * @return zero(0) if match, otherwise the difference of the
 *         mismatched characters.
 */
int std_strcmp_f(FAR const char *fstr0_p,
                 FAR const char *fstr1_p);

/**
 * Compare at most `size` bytes of one far string and one string.
 *
 * @param[in] fstr_p Far memory string.
 * @param[in] str_p String.
 * @param[in] size Compare at most size number of bytes.
 *
 * @return zero(0) if match, otherwise the difference of the
 *         mismatched characters.
 */
int std_strncmp(FAR const char *fstr_p,
                const char *str_p,
                size_t size);

/**
 * Compare at most `size` bytes of two far strings.
 *
 * @param[in] fstr0_p Far memory string.
 * @param[in] fstr1_p Far memory string.
 * @param[in] size Compare at most size number of bytes.
 *
 * @return zero(0) if match, otherwise the difference of the
 *         mismatched characters.
 */
int std_strncmp_f(FAR const char *fstr0_p,
                  FAR const char *fstr1_p,
                  size_t size);

/**
 * Get the length in bytes of given far string, not including null
 * termination.
 *
 * @param[in] fstr_p Far memory string.
 *
 * @return String length in number of bytes (not including the null
 *         termination).
 */
int std_strlen(FAR const char *fstr_p);

/**
 * Strip leading and trailing characters from a string. The characters
 * to strip are given by `strip_p`.
 *
 * @param[in] str_p String to strip characters from.
 * @param[in] strip_p Characters to strip or NULL for whitespace
 *                    characters. Must be null-terminated.
 *
 * @return Pointer to the stripped string.
 */
char *std_strip(char *str_p, const char *strip_p);

#endif
