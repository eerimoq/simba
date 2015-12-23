/**
 * @file kernel/log.h
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

#ifndef __KERNEL_LOG_H__
#define __KERNEL_LOG_H__

#include "simba.h"

/* Severity levels' log masks. */

/** System unusable. */
#define LOG_EMERG   0
/** Immediate action required. */
#define LOG_ALERT   1
/** Critical condition. */
#define LOG_CRIT    2
/** Error conditions. */
#define LOG_ERR     3
/** Warning conditions. */
#define LOG_WARNING 4
/** Normal but significant conditions. */
#define LOG_NOTICE  5
/** Informational messages. */
#define LOG_INFO    6
/** Debugging messages. */
#define LOG_DEBUG   7

/** Craete log mask from level. */
#define LOG_MASK(level) (1 << (LOG_ ## level))

/** Set all levels up to and including mask. */
#define LOG_UPTO(level) ((1 << (LOG_ ## level + 1)) - 1)

#if !defined(LOG_BUFFER_SIZE)
#    define LOG_BUFFER_SIZE 256
#endif

/** No logging. */
#define LOG_MODE_OFF      0
/** Circular logging. This is the default behaviour. */
#define LOG_MODE_CIRCULAR 1
/** Turn off logging when the buffer is full. */
#define LOG_MODE_CAPTURE  2

#define LOG_NAME TOKENPASTE(log_, UNIQUE(MODULE_NAME))
#define LOG_NAME_WRITE TOKENPASTE(LOG_NAME, _write)

/**
 * Write a log entry to the log.
 *
 * Example that writes a log entry to the log if the log level
 * `LOG_INFO` is enabled.
 *
 * @rst
 *     .. code-block:: c
 *
 *        void foo()
 *        {
 *            LOG(INFO, "one = %d", 1);
 *        }
 * @endrst
 *
 * @param[in] level Log entry level.
 * @param[in] format Log entry format string.
 * @param[in] ... Variable argument list of values to log.
 */
#if defined(__SIMBA_GEN__)
#    define LOG(level, format, ...) ..log-begin.. LOG_NAME format ..log-end..
#else
#    define LOG(level, format, ...)             \
    extern int LOG_NAME_WRITE(char, ...);       \
    LOG_NAME_WRITE(LOG_ ## level, ## __VA_ARGS__)
#endif

struct log_entry_header_t {
    size_t size;
    unsigned long number;
    unsigned long time;
    char level;
    int id;
};

struct log_entry_footer_t {
    size_t size;
};

/**
 * Initialize the logging module.
 *
 * @return zero(0) or negative error code.
 */
int log_module_init(void);

/**
 * Reset the log module. Clears the log and sets the log mode to
 * circular.
 *
 * @return zero(0) or negative error code.
 */
int log_reset(void);

/**
 * Set the log mode to off or circular.
 *
 * @param[in] mode Mode to set. One of `LOG_MODE_OFF`,
 *                 `LOG_MODE_CIRCULAR` or `LOG_MODE_CAPTURE`.
 *
 * @return The old log mode.
 */
int log_set_mode(int mode);

/**
 * Get the current log mode.
 *
 * @return Current log mode.
 */
int log_get_mode(void);

/**
 * Add log entry to quick log buffer.
 *
 * @param[in] level Log level.
 * @param[in] id Log point identity.
 * @param[in] buf_p Buffer to log.
 * @param[in] size Buffer size.
 *
 * @return true(1) if the entry was written to the log and false(0)
 *         otherwise.
 */
int log_write(char level, int id, void *buf_p, size_t size);

/**
 * Format all entries in the log buffer and write them to given
 * channel.
 *
 * @param[in] chout_p Output channel.
 *
 * @return zero(0) or negative error code.
 */
int log_format(chan_t *chout_p);

#endif
