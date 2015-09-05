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

static struct thrd_t *thrd_p;

static void callback(void *arg_p)
{
    thrd_resume_irq(thrd_p, 0);
}

int test_timer(struct harness_t *harness_p)
{
    int i;
    struct timer_t timer;
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 100000000
    };

    sys_start();

    thrd_p = thrd_self();

    /* Single shot timer. */
    timer_set(&timer, &timeout, callback, NULL, 0);
    thrd_suspend(NULL);

    thrd_usleep(100000);

    /* Periodic timer. */
    std_printf(FSTR("timer_set(PERIODIC)\r\n"));
    timer_set(&timer, &timeout, callback, NULL, TIMER_PERIODIC);

    for (i = 0; i < 5; i++) {
        thrd_suspend(NULL);
        std_printf(FSTR("Timeout %d.\r\n"), i);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_timer, "test_timer" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
