/**
 * @file kernel/qlog.h
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

#ifndef __KERNEL_QLOG_H__
#define __KERNEL_QLOG_H__

#include "simba.h"

#if !defined(QLOG_ENTRIES_MAX)
#    define QLOG_ENTRIES_MAX 32
#endif

/* Qlog modes. */
#define QLOG_MODE_OFF      0
#define QLOG_MODE_CIRCULAR 1
#define QLOG_MODE_TRIGGER  2
#define QLOG_MODE_CAPTURE  3

#if defined(__SIMBA_GEN__)
#    define QLOG_DEFINE(level, name, format) ..qlog.. #level #name format
#else
#    define QLOG_DEFINE(level, name, format)    \
    extern qlog_id_t qlog_id_ ## name;
#endif

#define QLOG_ID(name) qlog_id_ ## name

/* Logging macros. */
#define QLOG4(name, v0, v1, v2, v3) qlog_write(QLOG_ID(name), v0, v1, v2, v3)
#define QLOG3(name, v0, v1, v2)     QLOG4(name, v0, v1, v2, 0)
#define QLOG2(name, v0, v1)         QLOG3(name, v0, v1, 0)
#define QLOG1(name, v0)             QLOG2(name, v0, 0)
#define QLOG0(name)                 QLOG1(name, 0)

typedef int32_t qlog_id_t;

struct qlog_entry_t {
    unsigned long number;
    unsigned long time;
    qlog_id_t qlog_id;
    long args[4];
};

/**
 * Initialize module.
 * @return zero(0) or negative error code.
 */
int qlog_module_init(void);

/**
 * Reset qlog module. Clears the log and sets the log mode to
 * circular.
 * @return zero(0) or negative error code.
 */
int qlog_reset();

/**
 * Set qlog mode to off, circular or trigger.
 * @param[in] mode Mode to set.
 * @return Overwritten mode.
 */
int qlog_set_mode(int mode);

/**
 * Get current qlog mode.
 * @return Current qlog mode.
 */
int qlog_get_mode();

/**
 * Set a trigger.
 * @param[in] qlog_id Qlog identity.
 * @param[in] mask Value mask.
 * @param[in] v0 Value 0.
 * @param[in] v1 Value 1.
 * @param[in] v2 Value 2.
 * @param[in] v3 Value 3.
 * @return zero(0) or negative error code.
 */
int qlog_set_trigger(qlog_id_t qlog_id,
                     uint8_t mask,
                     long v0,
                     long v1,
                     long v2,
                     long v3);

/**
 * Add log entry to quick log buffer.
 * @param[in] qlog_id Log point identity.
 * @param[in] v0 Value 0.
 * @param[in] v1 Value 1.
 * @param[in] v2 Value 2.
 * @param[in] v3 Value 3.
 * @return true(1) if the entry was written to the log and false(0)
 *         otherwise.
 */
int qlog_write(qlog_id_t qlog_id,
               long v0,
               long v1,
               long v2,
               long v3);

/**
 * Format all entries in the qlog buffer and write them to given
 * channel.
 * @param[in] chout_p Output channel.
 * @return zero(0) or negative error code.
 */
int qlog_format(chan_t *chout_p);

#endif
