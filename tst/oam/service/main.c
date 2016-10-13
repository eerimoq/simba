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

static int service_foo_cb(struct service_t *self_p,
                          struct service_server_t *server_p,
                          void *chan_p);

static int service_bar_cb(struct service_t *self_p,
                          struct service_server_t *server_p,
                          void *chan_p);

static struct service_server_t server;
static int workspace[32];
static struct event_t test_event;
static struct event_t foo_event;
static struct event_t bar_event;
static THRD_STACK(server_stack, 1024);

/**
 * The services and their channels.
 */
static void *foo_channels[2] = { &foo_event, NULL };
static void *bar_channels[2] = { &bar_event, NULL };
static struct service_t services[3] = {
    {
        .name_p = "foo",
        .callback = service_foo_cb,
        .channels_p = &foo_channels[0]
    },
    {
        .name_p = "bar",
        .callback = service_bar_cb,
        .channels_p = &bar_channels[0]
    },
    {
        .name_p = NULL
    }
};

/**
 * Foo service callback.
 */
static int service_foo_cb(struct service_t *self_p,
                          struct service_server_t *server_p,
                          void *chan_p)
{
    uint32_t mask;

    std_printf(FSTR("service foo\r\n"));

    BTASSERT(self_p == &services[0]);
    BTASSERT(chan_p == &foo_event);

    /* Read the event and write it back to the test thread. */
    mask = 0xffffffff;
    event_read(chan_p, &mask, sizeof(mask));
    chan_write(&test_event, &mask, sizeof(mask));

    return (0);
}

/**
 * Bar service callback.
 */
static int service_bar_cb(struct service_t *self_p,
                          struct service_server_t *server_p,
                          void *chan_p)
{
    uint32_t mask;

    std_printf(FSTR("service bar\r\n"));

    BTASSERT(self_p == &services[1]);
    BTASSERT(chan_p == &bar_event);

    /* Read the event and write it back to the test thread. */
    mask = 0xffffffff;
    event_read(chan_p, &mask, sizeof(mask));
    chan_write(&test_event, &mask, sizeof(mask));

    return (0);
}

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(event_init(&test_event) == 0);
    BTASSERT(event_init(&foo_event) == 0);
    BTASSERT(event_init(&bar_event) == 0);

    BTASSERT(service_module_init() == 0);

    /* Initialize the service server. */
    BTASSERT(service_server_init(&server,
                                 "my_server",
                                 &services[0],
                                 &workspace[0],
                                 sizeof(workspace)) == 0);

    BTASSERT(thrd_spawn(service_server_main,
                        &server,
                        0,
                        server_stack,
                        sizeof(server_stack)) != NULL);

    return (0);
}

static int test_list(struct harness_t *harness_p)
{
    char buf[64];

    strcpy(buf, "/sync/service/list");
    BTASSERT(fs_call(buf, NULL, sys_get_stdout(), NULL) == 0);

    return (0);
}

static int test_service_foo(struct harness_t *harness_p)
{
    uint32_t mask;

    /* Write an event for the foo service. */
    mask = 0x1;
    chan_write(&foo_event, &mask, sizeof(mask));

    /* Wait for the foo service to signal the test thread. */
    event_read(&test_event, &mask, sizeof(mask));

    return (0);
}

static int test_service_bar(struct harness_t *harness_p)
{
    uint32_t mask;

    /* Write an event for the foo service. */
    mask = 0x1;
    chan_write(&bar_event, &mask, sizeof(mask));

    /* Wait for the bar service to signal the test thread. */
    event_read(&test_event, &mask, sizeof(mask));

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_list, "test_list" },
        { test_service_foo, "test_service_foo" },
        { test_service_bar, "test_service_bar" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
