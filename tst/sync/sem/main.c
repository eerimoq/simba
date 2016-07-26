/**
 * @file main.c
 * @version 3.1.0
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

static struct sem_t sem;
static struct sem_t sem2;

static THRD_STACK(t0_stack, 224);
static THRD_STACK(t1_stack, 224);

/* Defined in the linker script. */
extern char __simba_stack_begin;
extern char __simba_stack_size;

static void *sem_main(void *arg_p)
{
    /* 2. Give the second semaphore. */
    sem_give(&sem2, 1);
    sem_take(&sem, NULL);

    /* 7. Give the second semaphore. */
    sem_give(&sem2, 1);

    return (NULL);
}

static int test_all(struct harness_t *harness_p)
{
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 0
    };

    sem_init(&sem, 1, 1);
    sem_init(&sem2, 2, 2);
    
    /* 1. Take all resources. */
    BTASSERT(sem_take(&sem, &timeout) == -ETIMEDOUT);

    /* Create two thrds with higher priority than this thrd. */
    thrd_spawn(sem_main,
               NULL,
               -10,
               t0_stack,
               sizeof(t0_stack));
    thrd_spawn(sem_main,
               NULL,
               -10,
               t1_stack,
               sizeof(t1_stack));

    /* 3. Wait until both threads are waiting for sem. */
    sem_take(&sem2, NULL);
    sem_take(&sem2, NULL);

    /* 4. Start both threads. */
    sem_give(&sem, 2);

    /* 6. Wait for both threads. */
    sem_take(&sem2, NULL);
    sem_take(&sem2, NULL);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_all, "test_all" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
