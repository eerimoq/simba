/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __DEBUG_HARNESS_H__
#define __DEBUG_HARNESS_H__

#include "simba.h"

#define _ASSERTFMT(fmt, ...) std_printf(FSTR(fmt "\n"), ##__VA_ARGS__);

/**
 * Assert given condition. Print an error message and return given
 * value ``res`` on error.
 */
#define BTASSERTR(cond, res, ...)                                       \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":" STRINGIFY(__LINE__) ": BTASSERT: " #cond " ")); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        return (res);                                                   \
    }

/**
 * Assert given condition. Print an error message and return given
 * value on error.
 */
#define BTASSERTN(cond, ...) BTASSERTR(cond, NULL, __VA_ARGS__)

/**
 * Assert given condition. Print an error message and return.
 */
#define BTASSERT(cond, ...) BTASSERTR(cond, -1, __VA_ARGS__)

/**
 * Assert given condition in a testcase. Print an error message and
 * return -1 on error.
 */
#define BTASSERTV(cond, ...)                                            \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":" STRINGIFY(__LINE__) ": BTASSERT: " #cond " ")); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        return;                                                         \
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
