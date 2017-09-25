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

#ifndef __DEBUG_HARNESS_H__
#define __DEBUG_HARNESS_H__

#include "simba.h"

#define _ASSERTFMT(fmt, ...) std_printf(FSTR(fmt "\n"), ##__VA_ARGS__);

#define _ASSERTHEX(actual_str, actual, expected_str, expected, size)    \
    std_printf(FSTR(":: \r\n"                                           \
                    "Memory buffer '" actual_str "'\r\n"));             \
    std_hexdump(sys_get_stdout(), actual, size);                        \
    std_printf(FSTR("is not equal to memory buffer '" expected_str "'\r\n")); \
    std_hexdump(sys_get_stdout(), expected, size);

/**
 * Assert given condition. Print an error message and return given
 * value ``res`` on error.
 */
#define BTASSERTRM(cond, cond_str, res, msg)                            \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":" STRINGIFY(__LINE__) ": BTASSERT: " \
                        cond_str " "));                                 \
        msg;                                                            \
        return (res);                                                   \
    }

/**
 * Assert given condition. Print an error message and return given
 * value ``res`` on error.
 */
#define BTASSERTR(cond, cond_str, res, ...) \
    BTASSERTRM(cond, cond_str, res, _ASSERTFMT(__VA_ARGS__));

/**
 * Assert given condition. Print an error message and return given
 * value on error.
 */
#define BTASSERTN(cond, ...) BTASSERTR(cond, #cond, NULL, __VA_ARGS__)

/**
 * Assert given condition. Print an error message and return.
 */
#define BTASSERT(cond, ...) BTASSERTR(cond, #cond, -1, __VA_ARGS__)

/**
 * Compare two integers `actual` and `expected` with given operator
 * `operator`. Print an error message if the condition is not true and
 * return.
 */
#define BTASSERTI(actual, operator, expected)                           \
    do {                                                                \
        long UNIQUE(_actual);                                           \
        long UNIQUE(_expected);                                         \
        UNIQUE(_actual) = (actual);                                     \
        UNIQUE(_expected) = (expected);                                 \
        BTASSERTR(UNIQUE(_actual) operator UNIQUE(_expected),           \
                  #actual " " #operator " " #expected,                  \
                  -1,                                                   \
                  ":: Condition '%ld " #operator " %ld' ('0x%lx "       \
                  #operator " 0x%lx') is not true.",                    \
                  UNIQUE(_actual),                                      \
                  UNIQUE(_expected),                                    \
                  UNIQUE(_actual),                                      \
                  UNIQUE(_expected));                                   \
    } while (0)

/**
 * Comapre two memory positions `actual` and `expected`. Print an
 * error message if they are not equal and return.
 */
#define BTASSERTM(actual, expected, size)                               \
    do {                                                                \
        const void *UNIQUE(_actual);                                    \
        const void *UNIQUE(_expected);                                  \
        UNIQUE(_actual) = (actual);                                     \
        UNIQUE(_expected) = (expected);                                 \
        BTASSERTRM(memcmp(UNIQUE(_actual), UNIQUE(_expected), size) == 0, \
                   "memcmp(" #actual ", " #expected ", " #size ") == 0", \
                   -1,                                                  \
                   _ASSERTHEX(#actual, UNIQUE(_actual),                 \
                              #expected, UNIQUE(_expected),             \
                              size));                                   \
    } while (0)

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

/**
 * Assert that given value is in given range. Print an error message
 * and return.
 */
#define BTASSERT_IN_RANGE(value, low, high)     \
    do {                                        \
        BTASSERT(value >= low);                 \
        BTASSERT(value <= high);                \
    } while (0)

/**
 * Stub given function. Used with the make variable STUB to preprocess
 * object file(s).
 */
#define STUB(function) __stub_ ## function

#define HARNESS_MOCK_READ_ALL                              -1

/**
 * The testcase function callback.
 *
 * @return zero(0) if the testcase passed, a negative error code if
 *         the testcase failed, and a positive value if the testcase
 *         was skipped.
 */
typedef int (*harness_testcase_cb_t)(void);

struct harness_testcase_t {
    harness_testcase_cb_t callback;
    const char *name_p;
};

/**
 * Run given testcases in the test harness.
 *
 * @param[in] testcases_p An array of testcases to run. The last
 *                        element in the array must have ``callback``
 *                        and ``name_p`` set to NULL.
 *
 * @return Never returns.
 */
int harness_run(struct harness_testcase_t *testcases_p);

/**
 * Continiously read from given channel and return when given pattern
 * has been read, or when given timeout occurs.
 *
 * @param[in] chan_p Channel to read from.
 * @param[in] pattern_p Pattern to wait for.
 * @param[in] timeout_p Timeout, or NULL to wait the default timeout
 *                      of one second.
 *
 * @return Number of bytes read from the channel when match occured,
 *         or negative error code.
 */
int harness_expect(void *chan_p,
                   const char *pattern_p,
                   const struct time_t *timeout_p);

/**
 * Write given data buffer to a mock entry with given id. The mock
 * entry can later be read with `harness_mock_read()` or
 * `harness_mock_try_read()`.
 *
 * @param[in] id_p Mock id string to write.
 *
 *                 NOTE: Only a reference to this string is stored in
 *                       the mock entry.
 * @param[in] buf_p Data for given mock id, or NULL if no data shall
 *                  be written.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 *
 * @return Number of written words or negative error code.
 */
ssize_t harness_mock_write(const char *id_p,
                           const void *buf_p,
                           size_t size);

/**
 * Read data from mock entry with given id, and make the testcase fail
 * if the mock id is not found.
 *
 * @param[in] id_p Mock id string to read.
 * @param[out] buf_p Buffer to read into, or NULL if no data shall
 *                   be read.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL,
 *                 or ``HARNESS_MOCK_READ_ALL`` to read all data
 *                 available.
 *
 * @return Number of read words or negative error code.
 */
ssize_t harness_mock_read(const char *id_p,
                          void *buf_p,
                          ssize_t size);

/**
 * Find mock entry with given id and compare its data to given
 * buffer. The testcase fails if the mock id is not found or on data
 * mismatch.
 *
 * @param[in] id_p Mock id string to assert.
 * @param[in] buf_p Buffer with expected data, or NULL if no data
 *                  shall be compared.
 *
 * @return zero(0) or negative error code.
 */
int harness_mock_assert(const char *id_p,
                        const void *buf_p);

/**
 * Try to read data from mock entry with given id.The testcase does
 * not fail if the mock entry is missing.
 *
 * @param[in] id_p Mock id string to read.
 * @param[out] buf_p Buffer to read into, or NULL if no data shall
 *                   be loaded.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL,
 *                 or ``HARNESS_MOCK_READ_ALL`` to read all data
 *                 available.
 *
 * @return Number of read words or negative error code.
 */
ssize_t harness_mock_try_read(const char *id_p,
                              void *buf_p,
                              ssize_t size);

/**
 * Write given data buffer to a mock entry with given id and notify
 * all raeders that data is available. The
 * `harness_mock_write_notify()` and `harness_mock_read_wait()`
 * functions are useful to mock communication interfaces between
 * threads.
 *
 * @param[in] id_p Mock id string to write.
 *
 *                 NOTE: Only a reference to this string is stored in
 *                       the mock entry.
 * @param[in] buf_p Data for given mock id, or NULL if no data shall
 *                  be written.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 *
 * @return Number of written words or negative error code.
 */
ssize_t harness_mock_write_notify(const char *id_p,
                                  const void *buf_p,
                                  size_t size);

/**
 * Read data from mock entry with given id. Suspends the current
 * thread if the mock id is not found.
 *
 * @param[in] id_p Mock id string to read.
 * @param[out] buf_p Buffer to read into, or NULL if no data shall
 *                   be read.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL,
 *                 or ``HARNESS_MOCK_READ_ALL`` to read all data
 *                 available.
 * @param[in] timeout_p Read timeout.
 *
 * @return Number of read words or negative error code.
 */
ssize_t harness_mock_read_wait(const char *id_p,
                               void *buf_p,
                               ssize_t size,
                               struct time_t *timeout_p);

#endif
