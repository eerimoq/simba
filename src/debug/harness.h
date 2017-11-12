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

#define _ASSERTHEX(actual_str, \
                   actual, \
                   expected_str, \
                   expected, \
                   actual_size, \
                   expected_size)                                       \
    std_printf(FSTR(":: \r\n"                                           \
                    "Memory buffer '" actual_str "'\r\n"));             \
    std_hexdump(sys_get_stdout(), actual, actual_size);                 \
    std_printf(FSTR("is not equal to memory buffer '" expected_str "'\r\n")); \
    std_hexdump(sys_get_stdout(), expected, expected_size);

/**
 * Assert given condition. Mark testcase as failed, print an error
 * message and return given value ``res`` on error.
 */
#define BTASSERTRM(cond, cond_str, res, msg)                            \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":" STRINGIFY(__LINE__) ": BTASSERT: " \
                        cond_str " "));                                 \
        msg;                                                            \
        harness_set_testcase_result(-1);                                \
        return (res);                                                   \
    }

/**
 * Assert given condition. Mark testcase as failed, print an error
 * message and return given value ``res`` on error.
 */
#define BTASSERTR(cond, cond_str, res, ...) \
    BTASSERTRM(cond, cond_str, res, _ASSERTFMT(__VA_ARGS__));

/**
 * Assert given condition. Mark testcase as failed, print an error
 * message and return given value on error.
 */
#define BTASSERTN(cond, ...) BTASSERTR(cond, #cond, NULL, __VA_ARGS__)

/**
 * Assert given condition. Mark testcase as failed, print an error
 * message and return -1 on error.
 */
#define BTASSERT(cond, ...) BTASSERTR(cond, #cond, -1, __VA_ARGS__)

/**
 * Compare two integers `actual` and `expected` with given operator
 * `operator`. Mark testcase as failed, print an error message if the
 * condition is not true and return -1 on error.
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
 * Comapre two memory positions `actual` and `expected`. Mark testcase
 * as failed, print an error message if they are not equal and return
 * -1 on error.
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
                              size,                                     \
                              size));                                   \
    } while (0)

/**
 * Assert given condition in a testcase. Mark testcase as failed,
 * print an error message and return on error.
 */
#define BTASSERTV(cond, ...)                                            \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":" STRINGIFY(__LINE__) ": BTASSERT: " #cond " ")); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        harness_set_testcase_result(-1);                                \
        return;                                                         \
    }

/**
 * Assert that given value is in given range. Mark testcase as failed,
 * print an error message and return.
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

/**
 * The testcase function callback.
 *
 * @return zero(0) if the testcase passed, a negative error code if
 *         the testcase failed, and a positive value if the testcase
 *         was skipped.
 */
typedef int (*harness_testcase_cb_t)(void);

/**
 * The read/assert callback function.
 *
 * @param[in] arg_p `arg_size` bytes copied from `arg_p` given to
 *                  `harness_mock_cwrite()`.
 * @param[in] buf_p Mock entry data buffer, equivalent to `buf_p`
 *                  given to `harness_mock_cwrite()`.
 *
 * @return true(1) if the mock entry shall be removed, otherwise
 *         false(0).
 */
typedef int (*harness_mock_cb_t)(void *arg_p,
                                 void *buf_p);

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
 * Write given data buffer to a mock entry with given id. The mock
 * entry can later be read `length` times with `harness_mock_read()`,
 * `harness_mock_try_read()` or `harness_mock_assert()`.
 *
 * @param[in] id_p Mock id string to write.
 *
 *                 NOTE: Only a reference to this string is stored in
 *                       the mock entry.
 * @param[in] buf_p Data for given mock id, or NULL if no data shall
 *                  be written.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 * @param[in] length Number of times this mock entry will be
 *                   read/asserted.
 *
 * @return Number of written words or negative error code.
 */
ssize_t harness_mock_mwrite(const char *id_p,
                            const void *buf_p,
                            size_t size,
                            int length);

/**
 * Write given data buffer to a mock entry with given id. The mock
 * entry can later be read with `harness_mock_read()`,
 * `harness_mock_try_read()` or `harness_mock_assert()` until the
 * callback `cb` returns true(1).
 *
 * @param[in] id_p Mock id string to write.
 *
 *                 NOTE: Only a reference to this string is stored in
 *                       the mock entry.
 * @param[in] buf_p Data for given mock id, or NULL if no data shall
 *                  be written.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 * @param[in] cb Callback function called on each read/assert of this
 *               mock entry. The mock entry will be removed once the
 *               callback returns true(1).
 * @param[in] arg_p Callback argument pointer.
 * @param[in] arg_size Callback argument size.
 *
 * @return Number of written words or negative error code.
 */
ssize_t harness_mock_cwrite(const char *id_p,
                            const void *buf_p,
                            size_t size,
                            harness_mock_cb_t cb,
                            void *arg_p,
                            size_t arg_size);

/**
 * Read data from mock entry with given id, and make the testcase fail
 * if the mock id is not found or if given size does not match the
 * size in the mock entry.
 *
 * @param[in] id_p Mock id string to read.
 * @param[out] buf_p Buffer to read into, or NULL if no data shall
 *                   be read.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 *
 * @return Number of read words or negative error code.
 */
ssize_t harness_mock_read(const char *id_p,
                          void *buf_p,
                          size_t size);

/**
 * Try to read data from mock entry with given id. The testcase does
 * not fail if the mock entry is missing. However, the test case fails
 * if the mock id is found and the data size does not match.
 *
 * @param[in] id_p Mock id string to read.
 * @param[out] buf_p Buffer to read into, or NULL if no data shall
 *                   be loaded.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 *
 * @return Number of read words, -ENOENT if no mock entry was found
 *         for given id, or negative error code.
 */
ssize_t harness_mock_try_read(const char *id_p,
                              void *buf_p,
                              size_t size);

/**
 * Find mock entry with given id and compare its data to given
 * buffer. The testcase fails if the mock id is not found or on data
 * mismatch.
 *
 * @param[in] id_p Mock id string to assert.
 * @param[in] buf_p Buffer with expected data, or NULL if no data
 *                  shall be compared.
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 *
 * @return zero(0) or negative error code.
 */
int harness_mock_assert(const char *id_p,
                        const void *buf_p,
                        size_t size);

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
 * @param[in] size Buffer size in words, or zero(0) if buf_p is NULL.
 * @param[in] timeout_p Read timeout.
 *
 * @return Number of read words or negative error code.
 */
ssize_t harness_mock_read_wait(const char *id_p,
                               void *buf_p,
                               size_t size,
                               struct time_t *timeout_p);

/**
 * Set currently executing testcase result to passed(0), skipped(1) or
 * failed(-1).
 *
 * @param[in] result Testcase result to set.
 *
 * @return zero(0) or negative error code.
 */
int harness_set_testcase_result(int result);

/**
 * Get currently executing testcase result.
 *
 * @return passed(0), skipped(1) or failed(-1).
 */
int harness_get_testcase_result(void);

#endif
