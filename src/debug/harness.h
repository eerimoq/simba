/**
 * @file debug/harness.h
 * @version 7.0.0
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

#ifndef __DEBUG_HARNESS_H__
#define __DEBUG_HARNESS_H__

#include "simba.h"

/**
 * Assert given condition. Print an error message and return given
 * value ``res`` on error.
 */
#define BTASSERTN(cond, res, ...)                                       \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":%d: BTASSERT: %s "), __LINE__, #cond); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        return (res);                                                   \
    }

/**
 * Assert given condition in a testcase. Print an error message and
 * return -1 on error.
 */
#define BTASSERT(cond, ...)                                             \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":%d: BTASSERT: %s "), __LINE__, #cond); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        return (-1);                                                    \
    }

struct harness_t;

/**
 * The testcase function callback.
 *
 * @param[in] harness_t The harness object.
 *
 * @return zero(0) if the testcase passed, a negative error code if
 *         the testcase failed, and a positive value if the testcase
 *         was skipped.
 */
typedef int (*harness_testcase_cb_t)(struct harness_t *harness_p);

struct harness_testcase_t {
    harness_testcase_cb_t callback;
    const char *name_p;
};

struct harness_t {
    struct uart_driver_t uart;
};

/**
 * Initialize given test harness.
 *
 * @param[in] self_p Test harness to initialize.
 *
 * @return zero(0) or negative error code.
 */
int harness_init(struct harness_t *self_p);

/**
 * Run given testcases in given test harness.
 *
 * @param[in] self_p Test harness.
 * @param[in] testcases_p An array of testcases to run. The last
 *                        element in the array must have ``callback``
 *                        and ``name_p`` set to NULL.
 *
 * @return zero(0) or negative error code.
 */
int harness_run(struct harness_t *self_p,
                struct harness_testcase_t *testcases_p);

#endif
