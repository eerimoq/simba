/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

static int test_read_write(struct harness_t *harness)
{
    uint32_t mask;
    struct event_t event;

    BTASSERT(event_init(&event) == 0);

    mask = 0x3;
    BTASSERT(chan_write(&event, &mask, sizeof(mask)) == 4);

    BTASSERT(chan_size(&event) == 1);

    mask = 0x1;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == 0x1);

    mask = 0x3;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == 0x2);

    return (0);
}

static int test_poll(struct harness_t *harness)
{
    uint32_t mask;
    struct event_t event;
    struct chan_list_t list;
    char workspace[64];

    BTASSERT(event_init(&event) == 0);

    /* Use a list with one chan.*/
    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);
    BTASSERT(chan_list_add(&list, &event) == 0);

    mask = 0xc;
    BTASSERT(chan_write(&event, &mask, sizeof(mask)) == 4);

    BTASSERT(chan_list_poll(&list) == &event);

    mask = 0x4;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == 0x4);

    mask = 0x8;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == 0x8);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { test_poll, "test_poll" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
