/**
 * @file main.c
 * @version 0.2.0
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

#define ID_FOO 0x0
#define ID_BAR 0x1

struct foo_t {
    struct bus_message_header_t header;
    int value;
};

struct bar_t {
    struct bus_message_header_t header;
    int value;
};

static int test_attach_detach(struct harness_t *harness)
{
    struct bus_t bus;
    struct bus_chan_t chan;
    struct queue_t queue;

    BTASSERT(bus_init(&bus) == 0);
    BTASSERT(bus_chan_init(&chan, &queue, ID_FOO) == 0);

    /* Attach-detach a channel. */
    BTASSERT(bus_attach(&bus, &chan) == 0);
    BTASSERT(bus_detatch(&bus, &chan) == 0);

    /* Detach already detached channel fails. */
    BTASSERT(bus_detatch(&bus, &chan) == -1);

    return (0);
}

static int test_write_read(struct harness_t *harness)
{
    struct bus_t bus;
    struct bus_chan_t chans[2];
    struct queue_t queues[2];
    struct foo_t foo;
    char bufs[2][32];

    /* Initiate. */
    BTASSERT(bus_init(&bus) == 0);
    BTASSERT(queue_init(&queues[0], bufs[0], sizeof(bufs[0])) == 0);
    BTASSERT(queue_init(&queues[1], bufs[1], sizeof(bufs[1])) == 0);
    BTASSERT(bus_chan_init(&chans[0], &queues[0], ID_FOO) == 0);
    BTASSERT(bus_chan_init(&chans[1], &queues[1], ID_FOO) == 0);

    /* Write the message foo to the bus. No receiver is attached. */
    foo.header.id = ID_FOO;
    foo.header.size = sizeof(foo);
    foo.value = 5;
    BTASSERT(bus_write(&bus, &foo.header) == 0);

    /* Attach two channels and write the foo message again. */
    BTASSERT(bus_attach(&bus, &chans[0]) == 0);
    BTASSERT(bus_attach(&bus, &chans[1]) == 0);
    BTASSERT(bus_write(&bus, &foo.header) == 2);

    /* Verify that the received message in queue 1 is correct. */
    memset(&foo, 0, sizeof(foo));
    BTASSERT(queue_read(&queues[0], &foo, sizeof(foo)) == sizeof(foo));
    BTASSERT(foo.header.id == ID_FOO);
    BTASSERT(foo.header.size == sizeof(foo));
    BTASSERT(foo.value == 5);

    /* Verify that the received message in queue 2 is correct. */
    memset(&foo, 0, sizeof(foo));
    BTASSERT(queue_read(&queues[1], &foo, sizeof(foo)) == sizeof(foo));
    BTASSERT(foo.header.id == ID_FOO);
    BTASSERT(foo.header.size == sizeof(foo));
    BTASSERT(foo.value == 5);

    /* Detach the channels. */
    BTASSERT(bus_detatch(&bus, &chans[0]) == 0);
    BTASSERT(bus_detatch(&bus, &chans[1]) == 0);

    return (0);
}

static int test_multiple_ids(struct harness_t *harness)
{
    struct bus_t bus;
    struct bus_chan_t chans[2];
    struct queue_t queues[2];
    struct foo_t foo;
    struct bar_t bar;
    char bufs[2][32];

    /* Initiate. */
    BTASSERT(bus_init(&bus) == 0);
    BTASSERT(queue_init(&queues[0], bufs[0], sizeof(bufs[0])) == 0);
    BTASSERT(queue_init(&queues[1], bufs[1], sizeof(bufs[1])) == 0);
    BTASSERT(bus_chan_init(&chans[0], &queues[0], ID_FOO) == 0);
    BTASSERT(bus_chan_init(&chans[1], &queues[1], ID_BAR) == 0);

    /* Write the message foo to the bus. No receiver is attached. */
    foo.header.id = ID_FOO;
    foo.header.size = sizeof(foo);
    foo.value = 5;
    BTASSERT(bus_write(&bus, &foo.header) == 0);

    /* Write the message bar to the bus. No receiver is attached. */
    bar.header.id = ID_BAR;
    bar.header.size = sizeof(bar);
    bar.value = 3;
    BTASSERT(bus_write(&bus, &bar.header) == 0);

    /* Attach two channels and write the foo message again. */
    BTASSERT(bus_attach(&bus, &chans[0]) == 0);
    BTASSERT(bus_attach(&bus, &chans[1]) == 0);
    BTASSERT(bus_write(&bus, &foo.header) == 1);

    /* Verify that the received message in queue 1 is correct. */
    memset(&foo, 0, sizeof(foo));
    BTASSERT(queue_read(&queues[0], &foo, sizeof(foo)) == sizeof(foo));
    BTASSERT(foo.header.id == ID_FOO);
    BTASSERT(foo.header.size == sizeof(foo));
    BTASSERT(foo.value == 5);

    /* Write the bar message. */
    BTASSERT(bus_write(&bus, &bar.header) == 1);

    /* Verify that the received message in queue 2 is correct. */
    memset(&bar, 0, sizeof(bar));
    BTASSERT(queue_read(&queues[1], &bar, sizeof(bar)) == sizeof(bar));
    BTASSERT(bar.header.id == ID_BAR);
    BTASSERT(bar.header.size == sizeof(bar));
    BTASSERT(bar.value == 3);
    
    /* Detach the channels. */
    BTASSERT(bus_detatch(&bus, &chans[0]) == 0);
    BTASSERT(bus_detatch(&bus, &chans[1]) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_attach_detach, "test_attach_detach" },
        { test_write_read, "test_write_read" },
        { test_multiple_ids, "test_multiple_ids" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();
    bus_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
