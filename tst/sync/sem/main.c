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

static struct sem_t sem;
static struct sem_t sem2;

#if defined(ARCH_ESP32) || defined(ARCH_PPC)
static THRD_STACK(t0_stack, 512);
static THRD_STACK(t1_stack, 512);
#else
static THRD_STACK(t0_stack, 224);
static THRD_STACK(t1_stack, 224);
#endif

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

static int test_multi_thread(struct harness_t *harness_p)
{
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 0
    };

    sem_init(&sem, 2, 2);
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

static int test_binary(struct harness_t *harness_p)
{
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 1
    };

    BTASSERT(sem_init(&sem, 1, 1) == 0);

    BTASSERT(sem_take(&sem, &timeout) == -ETIMEDOUT);

    /* Give twice to test that the resource count stays at zero(0). */
    BTASSERT(sem_give(&sem, 1) == 0);
    BTASSERT(sem_give(&sem, 1) == 0);

    /* Can be taken once, not twice. */
    BTASSERT(sem_take(&sem, &timeout) == 0);
    BTASSERT(sem_take(&sem, &timeout) == -ETIMEDOUT);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_multi_thread, "test_multi_thread" },
        { test_binary, "test_binary" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
