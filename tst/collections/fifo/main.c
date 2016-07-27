/**
 * @file main.c
 * @version 3.1.1
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
