/**
 * @file main.c
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

static int write_filter_return_value;
static char buffer[8];

static ssize_t write_cb(void *self_p,
                        const void *buf_p,
                        size_t size)
{
    memcpy(buffer, buf_p, size);

    return (size);
}

static int write_filter(void *self_p, const void *buf_p, size_t size)
{
    return (write_filter_return_value);
}

static int test_filter(struct harness_t *harness_p)
{
    struct chan_t chan;
    char buf[2];

    BTASSERT(chan_init(&chan,
                       chan_read_null,
                       write_cb,
                       chan_size_null) == 0);

    /* Write without filter function. */
    buf[0] = 1;
    buf[1] = 2;
    BTASSERT(chan_write(&chan, &buf[0], 2) == 2);
    BTASSERT(memcmp(buffer, buf, 2) == 0);

    /* Filter out the message. */
    BTASSERT(chan_set_write_filter_cb(&chan, write_filter) == 0);
    buf[0] = 1;
    buf[1] = 2;
    write_filter_return_value = 1;
    memset(buffer, -1, sizeof(buffer));
    BTASSERT(chan_write(&chan, &buf[0], 2) == 2);
    buf[0] = -1;
    buf[1] = -1;
    BTASSERT(memcmp(buffer, buf, 2) == 0);
    BTASSERT(chan_set_write_filter_cb(&chan, NULL) == 0);

    /* Write isr without filter function. */
    BTASSERT(chan_set_write_isr_cb(&chan, write_cb) == 0);
    buf[0] = 1;
    buf[1] = 2;
    BTASSERT(chan_write_isr(&chan, &buf[0], 2) == 2);
    BTASSERT(memcmp(buffer, buf, 2) == 0);

    /* Filter out the message. */
    BTASSERT(chan_set_write_filter_isr_cb(&chan, write_filter) == 0);
    buf[0] = 1;
    buf[1] = 2;
    write_filter_return_value = 1;
    memset(buffer, -1, sizeof(buffer));
    BTASSERT(chan_write_isr(&chan, &buf[0], 2) == 2);
    buf[0] = -1;
    buf[1] = -1;
    BTASSERT(memcmp(buffer, buf, 2) == 0);
    BTASSERT(chan_set_write_filter_isr_cb(&chan, NULL) == 0);

    return (0);
}

static int test_null_channels(struct harness_t *harness_p)
{
    struct chan_t chan;
    char value;

    BTASSERT(chan_init(&chan,
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);

    BTASSERT(chan_read(&chan, &value, 1) == -1);
    BTASSERT(chan_write(&chan, &value, 1) == 1);
    BTASSERT(chan_size(&chan) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_filter, "test_filter" },
        { test_null_channels, "test_null_channels" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
