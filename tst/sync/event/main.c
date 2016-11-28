/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
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

#define EVENT_BIT_0 0x1
#define EVENT_BIT_1 0x2
#define EVENT_BIT_2 0x4
#define EVENT_BIT_3 0x8

static int test_read_write(struct harness_t *harness)
{
    uint32_t mask;
    struct event_t event;

    BTASSERT(event_init(&event) == 0);

    /* Write two events. */
    mask = (EVENT_BIT_1 | EVENT_BIT_0);
    BTASSERT(chan_write(&event, &mask, sizeof(mask)) == 4);

    BTASSERT(chan_size(&event) == 1);

    /* Read first event. */
    mask = EVENT_BIT_0;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == EVENT_BIT_0);

    /* Read second event. */
    mask = (EVENT_BIT_1 | EVENT_BIT_0);
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == EVENT_BIT_1);

    /* Write second and third events. */
    mask = (EVENT_BIT_2 | EVENT_BIT_1);
    BTASSERT(chan_write(&event, &mask, sizeof(mask)) == 4);

    /* Write first event. */
    mask = EVENT_BIT_0;
    BTASSERT(chan_write(&event, &mask, sizeof(mask)) == 4);

    /* Read first and second events. */
    mask = (EVENT_BIT_1 | EVENT_BIT_0);
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == (EVENT_BIT_1 | EVENT_BIT_0));

    /* Read third events. */
    mask = EVENT_BIT_2;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == EVENT_BIT_2);

    BTASSERT(chan_size(&event) == 0);

    return (0);
}

static int test_poll(struct harness_t *harness)
{
    uint32_t mask;
    struct event_t event;

    BTASSERT(event_init(&event) == 0);

    mask = (EVENT_BIT_0);
    BTASSERT(chan_write(&event, &mask, sizeof(mask)) == 4);

    /* Poll the channel and make sure the event channel has an event
       set.*/
    BTASSERT(chan_poll(&event, NULL) == &event);
    BTASSERT(chan_size(&event) == 1);

    return (0);
}

static int test_poll_timeout(struct harness_t *harness)
{
    struct event_t event;
    struct time_t timeout;

    BTASSERT(event_init(&event) == 0);

    /* Poll the channel and make sure a timeout occured.*/
    timeout.seconds = 0;
    timeout.nanoseconds = 10000;
    BTASSERT(chan_poll(&event, &timeout) == NULL);
    BTASSERT(chan_size(&event) == 0);

    return (0);
}

static int test_poll_list(struct harness_t *harness)
{
    uint32_t mask;
    struct event_t event;
    struct event_t event_dummy;
    struct chan_list_t list;
    char workspace[64];

    BTASSERT(event_init(&event) == 0);
    BTASSERT(event_init(&event_dummy) == 0);

    /* Use a list with one chan.*/
    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);
    BTASSERT(chan_list_add(&list, &event) == 0);
    BTASSERT(chan_list_add(&list, &event_dummy) == 0);

    mask = (EVENT_BIT_3 | EVENT_BIT_2);
    BTASSERT(chan_write(&event, &mask, sizeof(mask)) == 4);

    /* Poll the list of channels and make sure the event channel has
       events set.*/
    BTASSERT(chan_list_poll(&list, NULL) == &event);

    BTASSERT(chan_size(&event) == 1);

    mask = EVENT_BIT_2;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == EVENT_BIT_2);

    BTASSERT(chan_size(&event) == 1);

    mask = EVENT_BIT_3;
    BTASSERT(chan_read(&event, &mask, sizeof(mask)) == 4);
    BTASSERT(mask == EVENT_BIT_3);

    BTASSERT(chan_size(&event) == 0);

    return (0);
}

static int test_poll_list_timeout(struct harness_t *harness)
{
    struct event_t event;
    struct chan_list_t list;
    char workspace[64];
    struct time_t timeout;
    int is_polled;
    
    BTASSERT(event_init(&event) == 0);

    /* Use a list with one chan.*/
    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);
    BTASSERT(chan_list_add(&list, &event) == 0);

    /* Poll the list of channels and make sure an timeout occured. */
    timeout.seconds = 0;
    timeout.nanoseconds = 10000;
    BTASSERT(chan_list_poll(&list, &timeout) == NULL);

    sys_lock();
    is_polled = chan_is_polled_isr(&event.base);
    sys_unlock();
    
    BTASSERT(is_polled == 0);

    /* Poll the list of channels and make sure an timeout occured. */
    timeout.seconds = 0;
    timeout.nanoseconds = 0;
    BTASSERT(chan_list_poll(&list, &timeout) == NULL);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { test_poll, "test_poll" },
        { test_poll_timeout, "test_poll_timeout" },
        { test_poll_list, "test_poll_list" },
        { test_poll_list_timeout, "test_poll_list_timeout" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
