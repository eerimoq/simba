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

#ifndef __TEXT_STD_H__
#define __TEXT_STD_H__

#include "simba.h"
#include <stdarg.h>

/**
 * Initialize the std module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int std_module_init(void);

/**
 * Format and write data to destination buffer. The buffer must be big
 * enough to fit the formatted string. The output is null terminated.
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
 * * specifier: ``c``, ``s``, ``S``, ``d``, ``i``, ``u``, ``x`` or ``f``
 *
 * The ``S`` specifier expects a far string (``far_string_t``)
 * argument. Other specifiers have their usual definition.
 *
 * @param[out] dst_p Destination buffer. The formatted string is
 *                   written to this buffer.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Length of the string written to the destination buffer, not
 *         inclusing the null termination, or negative error code.
 */
ssize_t std_sprintf(char *dst_p, far_string_t fmt_p, ...);

/**
 * Format and write data to given buffer. The output is null
 * terminated.
 *
 * @param[out] dst_p Destination buffer. The formatted string is
 *                   written to this buffer.
 * @param[in] size Size of the destination buffer.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Length of the string written to the destination buffer, not
 *         inclusing the null termination, or negative error code.
 */
ssize_t std_snprintf(char *dst_p,
                     size_t size,
                     far_string_t fmt_p,
                     ...);

/**
 * Format and write data to given buffer. The output is null
 * terminated.
 *
 * @param[out] dst_p Destination buffer. The formatted string is
 *                   written to this buffer.
 * @param[in] fmt_p Format string.
 * @param[in] ap_p Variable arguments list.
 *
 * @return Length of the string written to the destination buffer, not
 *         inclusing the null termination, or negative error code.
 */
ssize_t std_vsprintf(char *dst_p, far_string_t fmt_p, va_list *ap_p);

/**
 * Format and write data to given buffer. The output is null
 * terminated.
 *
 * @param[out] dst_p Destination buffer. The formatted string is
 *                   written to this buffer.
 * @param[in] size Size of the destination buffer.
 * @param[in] fmt_p Format string.
 * @param[in] ap_p Variable arguments list.
 *
 * @return Length of the string written to the destination buffer, not
 *         inclusing the null termination, or negative error code.
 */
ssize_t std_vsnprintf(char *dst_p, size_t size, far_string_t fmt_p, va_list *ap_p);

/**
 * Format and print data to standard output. The output is not null
 * terminated.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Number of characters written to standard output, or
 *         negative error code.
 */
ssize_t std_printf(far_string_t fmt_p, ...);

/**
 * Format and print data to standard output. The output is not null
 * terminated.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] fmt_p Format string.
 * @param[in] ap_p Variable arguments list.
 *
 * @return Number of characters written to standard output, or
 *         negative error code.
 */
ssize_t std_vprintf(far_string_t fmt_p, va_list *ap_p);

/**
 * Format and print data to given channel. The output is not null
 * terminated.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] chan_p Output channel.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Number of characters written to given channel, or negative
 *         error code.
 */
ssize_t std_fprintf(void *chan_p, far_string_t fmt_p, ...);

/**
 * Format and print data to given channel. The output is not null
 * terminated.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] chan_p Output channel.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Number of characters written to given channel, or negative
 *         error code.
 */
ssize_t std_vfprintf(void *chan_p, far_string_t fmt_p, va_list *ap_p);

/**
 * Format and print data to standard output from interrupt context or
 * with the system lock taken. The output is not null terminated.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Number of characters written to standard output, or
 *         negative error code.
 */
ssize_t std_printf_isr(far_string_t fmt_p, ...);

/**
 * Format and print data to given channel from interrupt context or
 * with the system lock taken. The output is not null terminated.
 *
 * See `std_sprintf()` for the the format string specification.
 *
 * @param[in] chan_p Output channel.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguments list.
 *
 * @return Number of characters written to given channel, or negative
 *         error code.
 */
ssize_t std_fprintf_isr(void *chan_p, far_string_t fmt_p, ...);

/**
 * Convert given string to an integer in given base.
 *
 * @param[in] str_p Integer string.
 * @param[out] value_p Parsed integer.
 * @param[in] base The base of the value to parse. One of 16, 10, 8 or
 *                 2, or 0 to select base based on the string
 *                 prefix. Supported string prefixes are "0x" for
 *                 hexadecimal numbers, "0" for octal numbers and "0b"
 *                 for binary numbers.
 *
 * @return Pointer to the next byte, or NULL if no value was found.
 */
const char *std_strtolb(const char *str_p,
                        long *value_p,
                        int base);

/**
 * Convert given string to an integer with base selection based on the
 * string prefix.
 *
 * @param[in] str_p Integer string.
 * @param[out] value_p Parsed integer.
 *
 * @return Pointer to the next byte, or NULL if no value was found.
 */
const char *std_strtol(const char *str_p, long *value_p);

/**
 * Convert given string to a double.
 *
 * @param[in] str_p Double string.
 * @param[out] value_p Parsed value.
 *
 * @return Pointer to the next byte, or NULL if no value vas found.
 */
const char *std_strtod(const char *str_p, double *value_p);

/**
 * Convert string to decimal fixed point number with given precision.
 *
 * @param[in] str_p Double string.
 * @param[out] value_p Decimal fixed point number of given precision.
 * @param[in] precision Number precision, or decimal places.
 *
 * @return Pointer to the next byte, or NULL on failure.
 */
const char *std_strtodfp(const char *str_p,
                         long *value_p,
                         int precision);

/**
 * Copy string from far memory to memory.
 *
 * @param[in] dst_p Normal memory string.
 * @param[in] src_p Far memory string.
 *
 * @return String length or negative error code.
 */
int std_strcpy(char *dst_p, far_string_t src_p);

/**
 * Compare a string with a far string.
 *
 * @param[in] str_p Normal memory string.
 * @param[in] fstr_p Far memory string.
 *
 * @return zero(0) if match, otherwise the difference of
 *         the mismatched characters
 */
int std_strcmp(const char *str_p, far_string_t fstr_p);

/**
 * Compare two far strings.
 *
 * @param[in] fstr0_p Far memory string.
 * @param[in] fstr1_p Far memory string.
 *
 * @return zero(0) if match, otherwise the difference of the
 *         mismatched characters.
 */
int std_strcmp_f(far_string_t fstr0_p,
                 far_string_t fstr1_p);

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
int std_strncmp(far_string_t fstr_p,
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
int std_strncmp_f(far_string_t fstr0_p,
                  far_string_t fstr1_p,
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
int std_strlen(far_string_t fstr_p);

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

/**
 * Write a hex dump of given data to given channel.
 *
 * @param[in] chan_p Channel to write the hexdump to.
 * @param[in] buf_p Buffer to dump.
 * @param[in] size Size of buffer.
 *
 * @return Number of characters written to given channel, or negative
 *         error code.
 */
ssize_t std_hexdump(void *chan_p, const void *buf_p, size_t size);

#endif
