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

static char thrd_stack[256];
static void *thrd(void *arg_p)
{
    thrd_resume(arg_p, 3);
    thrd_suspend(NULL);

    return (NULL);
}

static int test_suspend_resume(struct harness_t *harness_p)
{
    int err;

    thrd_spawn(thrd,
               thrd_self(),
               10,
               thrd_stack,
               sizeof(thrd_stack));

    err = thrd_suspend(NULL);
    BTASSERT(err == 3, "err = %d", err);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_suspend_resume, "test_suspend_resume" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
