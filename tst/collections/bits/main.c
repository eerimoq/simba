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

static int test_insert(struct harness_t *harness)
{
    uint32_t dst;
    uint32_t src;
    int pos;
    int size;
 
    /* Insert a one at LSB. */
    dst = 0xffffffff;
    src = 0x0;
    pos = 0;
    size = 1;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0xfffffffe);
 
    /* Insert a one at MSB. */
    dst = 0xffffffff;
    src = 0x0;
    pos = 31;
    size = 1;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0x7fffffff);
 
    /* Insert in the middle. */
    dst = 0xffffffff;
    src = 0x34;
    pos = 17;
    size = 5;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0xffe9ffff);
 
    /* Insert the maximum 31 bits. */
    dst = 0xffffffff;
    src = 0x12345678;
    pos = 0;
    size = 31;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0x92345678);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_insert, "test_insert" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
