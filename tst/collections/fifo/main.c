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
#include "fifos.h"

static int test_put_get(void)
{
    struct fifo_t fifo;

    BTASSERT(fifo_init(&fifo, 4) == 0);

    /* Get from am empty fifo. */
    BTASSERT(fifo_get(&fifo) == -1);

    /* Put and get an element. */
    BTASSERT(fifo_put(&fifo) == 0);
    BTASSERT(fifo_get(&fifo) == 0);

    /* Put and get an element. */
    BTASSERT(fifo_put(&fifo) == 1);
    BTASSERT(fifo_get(&fifo) == 1);

    /* Put until the fifo is full. */
    BTASSERT(fifo_put(&fifo) == 2);
    BTASSERT(fifo_put(&fifo) == 3);
    BTASSERT(fifo_put(&fifo) == 0);
    BTASSERT(fifo_put(&fifo) == -1);

    /* Get all elements. */
    BTASSERT(fifo_get(&fifo) == 2);
    BTASSERT(fifo_get(&fifo) == 3);
    BTASSERT(fifo_get(&fifo) == 0);
    BTASSERT(fifo_get(&fifo) == -1);

    return (0);
}

static int test_put_get_int(void)
{
    struct fifo_int_t fifo;
    int buf[4];
    int value;

    BTASSERT(fifo_init_int(&fifo, buf, membersof(buf)) == 0);

    /* Put into the fifo. */
    value = 10;
    BTASSERT(fifo_put_int(&fifo, &value) == 0);
    value = 20;
    BTASSERT(fifo_put_int(&fifo, &value) == 0);
    value = 30;
    BTASSERT(fifo_put_int(&fifo, &value) == 0);
    value = 40;
    BTASSERT(fifo_put_int(&fifo, &value) == -1);

    /* Get from the fifo. */
    BTASSERT(fifo_get_int(&fifo, &value) == 0);
    BTASSERT(value == 10);
    BTASSERT(fifo_get_int(&fifo, &value) == 0);
    BTASSERT(value == 20);
    BTASSERT(fifo_get_int(&fifo, &value) == 0);
    BTASSERT(value == 30);
    BTASSERT(fifo_get_int(&fifo, &value) == -1);

    return (0);
}

static int test_put_get_foo(void)
{
    struct fifo_foo_t fifo;
    struct foo_t buf[4];
    struct foo_t value;

    BTASSERT(fifo_init_foo(&fifo, buf, membersof(buf)) == 0);

    /* Put into the fifo. */
    value.a = 10;
    value.b = 11;
    BTASSERT(fifo_put_foo(&fifo, &value) == 0);
    value.a = 20;
    value.b = 21;
    BTASSERT(fifo_put_foo(&fifo, &value) == 0);
    value.a = 30;
    value.b = 31;
    BTASSERT(fifo_put_foo(&fifo, &value) == 0);
    value.a = 40;
    value.b = 41;
    BTASSERT(fifo_put_foo(&fifo, &value) == -1);

    /* Get from the fifo. */
    BTASSERT(fifo_get_foo(&fifo, &value) == 0);
    BTASSERT(value.a == 10);
    BTASSERT(value.b == 11);
    BTASSERT(fifo_get_foo(&fifo, &value) == 0);
    BTASSERT(value.a == 20);
    BTASSERT(value.b == 21);
    BTASSERT(fifo_get_foo(&fifo, &value) == 0);
    BTASSERT(value.a == 30);
    BTASSERT(value.b == 31);
    BTASSERT(fifo_get_foo(&fifo, &value) == -1);

    return (0);
}

static int test_put_get_strings(void)
{
    struct fifo_strings_t fifo;
    string_t buf[4];
    string_t value;

    BTASSERT(fifo_init_strings(&fifo, buf, membersof(buf)) == 0);

    /* Put into the fifo. */
    strcpy(&value[0], "Hello!");
    BTASSERT(fifo_put_strings(&fifo, &value) == 0);

    /* Get from the fifo. */
    value[0] = '\0';
    BTASSERT(fifo_get_strings(&fifo, &value) == 0);
    BTASSERT(strcmp(&value[0], "Hello!") == 0);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_put_get, "test_put_get" },
        { test_put_get_int, "test_put_get_int" },
        { test_put_get_foo, "test_put_get_foo" },
        { test_put_get_strings, "test_put_get_strings" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
