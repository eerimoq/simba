/**
 * @file kernel/std.h
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

#ifndef __KERNEL_STD_H__
#define __KERNEL_STD_H__

#include "simba.h"

#if !defined(STD_KLOG_BUFFER_MAX)
#    define STD_KLOG_BUFFER_MAX 256
#endif

/* Severity levels' log masks. */
#define STD_LOG_EMERG   0        /* System unusable. */
#define STD_LOG_ALERT   1        /* Immediate action required. */
#define STD_LOG_CRIT    2        /* Critical condition. */
#define STD_LOG_ERR     3        /* Error conditions. */
#define STD_LOG_WARNING 4        /* Warning conditions. */
#define STD_LOG_NOTICE  5        /* Normal but significant conditions. */
#define STD_LOG_INFO    6        /* Informational messages. */
#define STD_LOG_DEBUG   7        /* Debugging messages. */

/* Craete log mask from level. */
#define STD_LOG_MASK(level) (1 << (STD_LOG_ ## level))

/* Set all levels up to and including mask. */
#define STD_LOG_UPTO(level) ((1 << (STD_LOG_ ## level + 1)) - 1)

/* Initialize readk iterator. */
#define STD_READK_ITER_INIT(iter) ((iter)->buf_p = NULL)

struct std_readk_t {
    char *buf_p;
};

/**
 * Initialize module.
 * @return zero(0) or negative error code.
 */
int std_module_init(void);

/**
 * Format and write data to buffer.
 *
 * Supported format specifiers are:
 *
 * %[flags][width][length]specifier
 *
 * flags: '0' or '-'
 * width: 0..127
 * length: 'l' for long or nothing
 * specifier: c, s, d, u, x
 *
 * @param[out] dst Destination buffer.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguemnts list.
 */
ssize_t std_sprintf(char *dst_p, FAR const char *fmt_p, ...);

/**
 * Format and print data to standard output.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguemnts list.
 * @return void.
 */
void std_printf(FAR const char *fmt_p, ...);

/**
 * Format and print data to channel.
 * @param[in] chan_p Output channel.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguemnts list.
 * @return void.
 */
void std_fprintf(chan_t *chan_p, FAR const char *fmt_p, ...);

/**
 * Format and print data to kernel log.
 * @param[in] level Log level.
 * @param[in] fmt_p Format string.
 * @param[in] ... Variable arguemnts list.
 * @return void.
 */
void std_printk(char level, FAR const char *fmt_p, ...);

/**
 * Read a kernel log entry. The entry is trunkated if it does
 * not fit into buffer.
 * @param[in] buf Buffer to read into.
 * @param[in] size Size of buffer.
 * @param[in] state Initialized read iterator.
 * @return zero(0) or negative error code.
 */
int std_readk(char *buf_p, size_t size, struct std_readk_t *iter);

/**
 * Set output channel for kernel log.
 * @param[in] chan Output channel.
 * @return void.
 */
void std_klog_set_output_channel(chan_t *chan);

/**
 * Convert string to integer.
 * @param[in] str_p Integer string.
 * @param[out] value_p Integer value.
 * @return zero(0) or negative error code.
 */
int std_strtol(const char *str_p, long *value_p);

/**
 * Compare two strings.
 * @param[in] str_p Normal memory string.
 * @param[in] fstr_p Far memory string.
 * @return zero(0) if match, otherwise the difference of
 *         the mismatched characters
 */
int std_strcmp(const char *str_p, FAR const char *fstr_p);

/**
 * Compare two strings.
 * @param[in] fstr0_p Far memory string.
 * @param[in] fstr1_p Far memory string.
 * @return zero(0) if match, otherwise the difference of
 *         the mismatched characters
 */
int std_strcmp_f(FAR const char *fstr0_p,
                 FAR const char *fstr1_p);

/**
 * Compare two strings.
 * @param[in] fstr0_p Far memory string.
 * @param[in] fstr1_p Far memory string.
 * @param[in] size Compare at most size number of bytes.
 * @return zero(0) if match, otherwise the difference of
 *         the mismatched characters
 */
int std_strncmp_f(FAR const char *fstr0_p,
                  FAR const char *fstr1_p,
                  size_t size);

/**
 * Get length of string, not including null termination.
 * @param[in] fstr_p Far memory string.
 * @return string length
 */
int std_strlen(FAR const char *fstr_p);

#endif
