/**
 * @file main.c
 * @version 0.1.0
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

static int test_poll_timeout(struct harness_t *harness)
{
    struct event_t event;
    struct chan_list_t list;
    char workspace[64];
    struct time_t timeout;

    BTASSERT(event_init(&event) == 0);

    /* Use a list with one chan.*/
    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);
    BTASSERT(chan_list_add(&list, &event) == 0);

    /* Poll the list of channels and make sure the event channel has
       events set.*/
    timeout.seconds = 0;
    timeout.nanoseconds = 10000;
    BTASSERT(chan_list_poll(&list, &timeout) == NULL);

    /* Poll the list of channels and make sure the event channel has
       events set.*/
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
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
