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

#include "simba.h"

static int asserti(int actual, int expected)
{
    BTASSERTI(actual, ==, expected);

    return (0);
}

static int test_asserti(struct harness_t *harness_p)
{
    BTASSERT(asserti(1, 2) == -1);
    BTASSERT(asserti(1, 1) == 0);

    return (0);
}

static int assertm(const void *actual_p,
                   const void *expected_p,
                   size_t size)
{
    BTASSERTM(actual_p, expected_p, size);

    return (0);
}

static int test_assertm(struct harness_t *harness_p)
{
    BTASSERT(assertm("foo", "bar", 3) == -1);
    BTASSERT(assertm("foo", "foo", 3) == 0);

    return (0);
}

static ssize_t my_memcpy(char *dst_p,
                         const char *src_p,
                         size_t size)
{
    ssize_t res;

    /* Save input. */
    harness_mock_write("my_memcpy(src_p)", src_p, strlen(src_p) + 1);
    harness_mock_write("my_memcpy(size)", &size, sizeof(size));

    /* Get output. */
    harness_mock_read("my_memcpy(dst_p)", dst_p, 16);
    harness_mock_read("my_memcpy: return (res)", &res, sizeof(res));

    return (res);
}

static int test_mock(struct harness_t *harness_p)
{
    char buf[16];
    size_t size;
    ssize_t res;

    /* Prepare function output. */
    harness_mock_write("my_memcpy(dst_p)", "bar", sizeof("bar"));
    res = 4;
    harness_mock_write("my_memcpy: return (res)", &res, sizeof(res));

    /* Call the function and validate its output. */
    BTASSERTI(my_memcpy(&buf[0], "foo", 4), ==, 4);
    BTASSERTM(&buf[0], "bar", 4);

    /* Validate function input. */
    harness_mock_read("my_memcpy(src_p)", &buf[0], sizeof("foo"));
    harness_mock_read("my_memcpy(size)", &size, sizeof(size));
    BTASSERTM(&buf[0], "foo", sizeof("foo"));
    BTASSERTI(size, ==, 4);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_asserti, "test_asserti" },
        { test_assertm, "test_assertm" },
        { test_mock, "test_mock" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
