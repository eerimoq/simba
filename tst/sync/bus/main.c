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

#define ID_FOO 0x0
#define ID_BAR 0x1

static int test_init(struct harness_t *harness)
{
    /* This function may be called multiple times. */
    BTASSERT(bus_module_init() == 0);
    BTASSERT(bus_module_init() == 0);
    
    return (0);
}

static int test_attach_detach(struct harness_t *harness)
{
    struct bus_t bus;
    struct bus_listener_t chan;
    struct queue_t queue;

    BTASSERT(bus_init(&bus) == 0);
    BTASSERT(bus_listener_init(&chan, ID_FOO, &queue) == 0);

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
    struct bus_listener_t chans[5];
    struct queue_t queues[2];
    struct queue_t dummy;
    char bufs[2][32];
    int foo;
    int value;

    /* Initiate. */
    BTASSERT(bus_init(&bus) == 0);
    BTASSERT(queue_init(&queues[0], bufs[0], sizeof(bufs[0])) == 0);
    BTASSERT(queue_init(&queues[1], bufs[1], sizeof(bufs[1])) == 0);
    BTASSERT(bus_listener_init(&chans[0], ID_FOO, &queues[0]) == 0);
    BTASSERT(bus_listener_init(&chans[1], ID_FOO, &queues[1]) == 0);
    BTASSERT(bus_listener_init(&chans[2], -1, &dummy) == 0);
    BTASSERT(bus_listener_init(&chans[3], -1, &dummy) == 0);
    BTASSERT(bus_listener_init(&chans[4], -1, &dummy) == 0);

    /* Write the message foo to the bus. No receiver is attached. */
    foo = 5;
    BTASSERT(bus_write(&bus, ID_FOO, &foo, sizeof(foo)) == 0);

    /* Attach two channels and write the foo message again. */
    BTASSERT(bus_attach(&bus, &chans[0]) == 0);
    BTASSERT(bus_attach(&bus, &chans[1]) == 0);
    BTASSERT(bus_attach(&bus, &chans[2]) == 0);
    BTASSERT(bus_attach(&bus, &chans[3]) == 0);
    BTASSERT(bus_attach(&bus, &chans[4]) == 0);
    BTASSERT(bus_write(&bus, ID_FOO, &foo, sizeof(foo)) == 2);

    /* Verify that the received message in queue 1 is correct. */
    value = 0;
    BTASSERT(queue_read(&queues[0], &value, sizeof(value)) == sizeof(value));
    BTASSERT(value == 5);

    /* Verify that the received message in queue 2 is correct. */
    value = 0;
    BTASSERT(queue_read(&queues[1], &value, sizeof(value)) == sizeof(value));
    BTASSERT(value == 5);

    /* Detach the channels with id ID_FOO. */
    BTASSERT(bus_detatch(&bus, &chans[0]) == 0);
    BTASSERT(bus_detatch(&bus, &chans[1]) == 0);

    /* Detach the channels with id -1. */
    BTASSERT(bus_detatch(&bus, &chans[3]) == 0);
    BTASSERT(bus_detatch(&bus, &chans[4]) == 0);
    BTASSERT(bus_detatch(&bus, &chans[2]) == 0);

    return (0);
}

static int test_multiple_ids(struct harness_t *harness)
{
    struct bus_t bus;
    struct bus_listener_t chans[2];
    struct queue_t queue;
    struct event_t event;
    char buf[32];
    int foo, value;
    uint32_t bar, mask;

    /* Initiate. */
    BTASSERT(bus_init(&bus) == 0);
    BTASSERT(queue_init(&queue, buf, sizeof(buf)) == 0);
    BTASSERT(event_init(&event) == 0);
    BTASSERT(bus_listener_init(&chans[0], ID_FOO, &queue) == 0);
    BTASSERT(bus_listener_init(&chans[1], ID_BAR, &event) == 0);

    /* Write the message foo to the bus. No receiver is attached. */
    foo = 5;
    BTASSERT(bus_write(&bus, ID_FOO, &foo, sizeof(foo)) == 0);

    /* Write the message bar to the bus. No receiver is attached. */
    bar = 0x80;
    BTASSERT(bus_write(&bus, ID_BAR, &bar, sizeof(bar)) == 0);

    /* Attach two channels and write the foo message again. */
    BTASSERT(bus_attach(&bus, &chans[0]) == 0);
    BTASSERT(bus_attach(&bus, &chans[1]) == 0);
    BTASSERT(bus_write(&bus, ID_FOO, &foo, sizeof(foo)) == 1);

    /* Verify that the received message in queue 1 is correct. */
    value = 0;
    BTASSERT(queue_read(&queue, &value, sizeof(value)) == sizeof(value));
    BTASSERT(value == 5);

    /* Write the bar message. */
    BTASSERT(bus_write(&bus, ID_BAR, &bar, sizeof(bar)) == 1);

    /* Verify that the received event is correct. */
    mask = 0xffffffff;
    BTASSERT(event_read(&event, &mask, sizeof(mask)) == sizeof(mask));
    BTASSERT(mask == 0x80);
    
    /* Detach the channels. */
    BTASSERT(bus_detatch(&bus, &chans[0]) == 0);
    BTASSERT(bus_detatch(&bus, &chans[1]) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_attach_detach, "test_attach_detach" },
        { test_write_read, "test_write_read" },
        { test_multiple_ids, "test_multiple_ids" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
