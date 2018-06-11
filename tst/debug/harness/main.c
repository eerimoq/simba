/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "my_module.h"
#include "my_module_mock.h"

static THRD_STACK(mock_thread_stack, 512);

static int asserti(int actual, int expected)
{
    BTASSERTI(actual, ==, expected);

    return (0);
}

static int assertm(const void *actual_p,
                   const void *expected_p,
                   size_t size)
{
    BTASSERTM(actual_p, expected_p, size);

    return (0);
}

static void *mock_thread(void *arg_p)
{
    int value;

    while (1) {
        harness_mock_read_wait("chan_request",
                               &value,
                               sizeof(value),
                               NULL);
        value++;
        harness_mock_write_notify("chan_response",
                                  &value,
                                  sizeof(value));
    }

    return (NULL);
}

static int cwrite_cb(void *arg_p, void *buf_p)
{
    int *length_p;
    int *value_p;

    length_p = arg_p;
    value_p = buf_p;

    (*length_p)--;
    *value_p += 2;

    return (*length_p == 0);
}

static int test_asserti(void)
{
    BTASSERT(asserti(1, 2) == -1);
    BTASSERT(harness_get_testcase_result() == -1);
    BTASSERT(harness_set_testcase_result(0) == 0);
    BTASSERT(asserti(1, 1) == 0);

    return (0);
}

static int test_assertm(void)
{
    BTASSERT(assertm("foo", "bar", 3) == -1);
    BTASSERT(harness_get_testcase_result() == -1);
    BTASSERT(harness_set_testcase_result(0) == 0);
    BTASSERT(assertm("foo", "foo", 3) == 0);

    return (0);
}

static int test_mock(void)
{
    char buf[16];

    mock_write_my_memcpy("bar", "foo", 4, 4);
    BTASSERTI(my_memcpy(&buf[0], "foo", 4), ==, 4);
    BTASSERTM(&buf[0], "bar", 4);

    mock_write_my_memcpy("bar", "foo", 4, -1);
    BTASSERTI(my_memcpy(&buf[0], "foo", 4), ==, -1);
    BTASSERTM(&buf[0], "bar", 4);

    harness_mock_write("foo", NULL, 0);
    harness_mock_read("foo", NULL, 0);

    buf[0] = 1;
    harness_mock_write("foo", &buf[0], 1);
    buf[0] = 0;
    harness_mock_read("foo", &buf[0], 1);
    BTASSERT(buf[0] == 1);

    harness_mock_try_read("foo", NULL, 0);

    return (0);
}

static int test_mock_assert(void)
{
    char buf[16];

    /* Without data. */
    harness_mock_write("foo", NULL, 0);
    harness_mock_assert("foo", NULL, 0);

    /* With data. */
    buf[0] = 1;
    harness_mock_write("foo", &buf[0], 1);
    harness_mock_assert("foo", &buf[0], 1);

    /* Wrong data. */
    buf[1] = 0;
    harness_mock_write("foo", &buf[0], 1);
    buf[0] = 0;
    harness_mock_assert("foo", &buf[0], 1);
    BTASSERT(harness_get_testcase_result() == -1);
    BTASSERT(harness_set_testcase_result(0) == 0);

    /* Mock id not found. */
    harness_mock_assert("foo", &buf[0], 1);
    BTASSERT(harness_get_testcase_result() == -1);
    BTASSERT(harness_set_testcase_result(0) == 0);

    return (0);
}

static int test_mock_wait_notify(void)
{
    int value;
    int i;

    thrd_spawn(mock_thread,
               NULL,
               0,
               mock_thread_stack,
               sizeof(mock_thread_stack));

    for (i = 0; i < 5; i++) {
        value = i;
        BTASSERT(harness_mock_write_notify("chan_request",
                                           &value,
                                           sizeof(value)) == sizeof(value));
        BTASSERTI(harness_mock_read_wait("chan_response",
                                         &value,
                                         sizeof(value),
                                         NULL), ==, sizeof(value));
        BTASSERTI(value, ==, i + 1);
    }

    return (0);
}

static int test_mock_mwrite(void)
{
    int value;
    int i;

    value = 1;

    BTASSERT(harness_mock_mwrite("mwrite(value)",
                                 &value,
                                 sizeof(value),
                                 3) == sizeof(value));

    for (i = 0; i < 3; i++) {
        value = 0;
        BTASSERT(harness_mock_read("mwrite(value)",
                                   &value,
                                   sizeof(value)) == sizeof(value));
        BTASSERT(value == 1);
    }

    BTASSERT(harness_mock_read("mwrite(value)",
                               &value,
                               sizeof(value)) == -1);

    harness_set_testcase_result(0);

    return (0);
}

static int test_mock_cwrite(void)
{
    int value;
    int length;

    value = 1;
    length = 2;

    BTASSERT(harness_mock_cwrite("cwrite(value)",
                                 &value,
                                 sizeof(value),
                                 cwrite_cb,
                                 &length,
                                 sizeof(length)) == sizeof(value));

    value = 0;
    BTASSERT(harness_mock_read("cwrite(value)",
                               &value,
                               sizeof(value)) == sizeof(value));
    BTASSERT(value == 1);

    BTASSERT(harness_mock_read("cwrite(value)",
                               &value,
                               sizeof(value)) == sizeof(value));
    BTASSERT(value == 3);

    BTASSERT(harness_mock_read("cwrite(value)",
                               &value,
                               sizeof(value)) == -1);

    harness_set_testcase_result(0);

    return (0);
}

static int test_stub(void)
{
    mock_write_foo(0);
    BTASSERT(foo() == 0);

    /* Override the weak generated function with the stub function
       defined in this file. */
    BTASSERT(bar() == 1);

    return (0);
}

/* Overrides the weak definition in my_module_mock.c. */
int STUB(bar)()
{
    return (1);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_asserti, "test_asserti" },
        { test_assertm, "test_assertm" },
        { test_mock, "test_mock" },
        { test_mock_assert, "test_mock_assert" },
        { test_mock_wait_notify, "test_mock_wait_notify" },
        { test_mock_mwrite, "test_mock_mwrite" },
        { test_mock_cwrite, "test_mock_cwrite" },
        { test_stub, "test_stub" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
