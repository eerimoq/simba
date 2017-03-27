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

/* Define the integer fifo. */
FIFO_DEFINE_TEMPLATE(int);

static int test_put_get(struct harness_t *harness)
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

static int test_put_get_int(struct harness_t *harness)
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

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_put_get, "test_put_get" },
        { test_put_get_int, "test_put_get_int" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
