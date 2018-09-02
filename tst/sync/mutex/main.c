/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

#define ITERATIONS 100

static struct mutex_t mutex;
static int global_counter = 0;
static int t0_counter = 0;
static int t1_counter = 0;
static int is_locked = 0;

#if defined(ARCH_ESP32) || defined(ARCH_PPC)
static THRD_STACK(t0_stack, 512);
static THRD_STACK(t1_stack, 512);
#elif defined(ARCH_ARM64) || defined(ARCH_MIPS)
static THRD_STACK(t0_stack, 1024);
static THRD_STACK(t1_stack, 1024);
#else
static THRD_STACK(t0_stack, 224);
static THRD_STACK(t1_stack, 224);
#endif

static void *mutex_main(void *arg_p)
{
    int i;
    int *counter_p;

    counter_p = arg_p;

    for (i = 0; i < ITERATIONS; i++) {
        mutex_lock(&mutex);

        if (is_locked == 1) {
            (*counter_p) = ITERATIONS;
            break;
        }

        is_locked = 1;
        thrd_sleep_ms(1);
        is_locked = 0;
        global_counter++;
        mutex_unlock(&mutex);
        (*counter_p)++;
    }

    thrd_suspend(NULL);

    return (NULL);
}

static int test_multi_thread(void)
{
    int done;

    BTASSERT(mutex_module_init() == 0);
    BTASSERT(mutex_module_init() == 0);
    BTASSERT(mutex_init(&mutex) == 0);

    thrd_spawn(mutex_main,
               &t0_counter,
               -10,
               t0_stack,
               sizeof(t0_stack));
    thrd_spawn(mutex_main,
               &t1_counter,
               -10,
               t1_stack,
               sizeof(t1_stack));

    done = 0;

    while (done == 0) {
        sys_lock();
        done = ((t0_counter == ITERATIONS) && (t1_counter == ITERATIONS));
        sys_unlock();
        thrd_yield();
    }

    BTASSERTI(t0_counter, ==, ITERATIONS);
    BTASSERTI(t1_counter, ==, ITERATIONS);
    BTASSERTI(global_counter, ==, t0_counter + t1_counter);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_multi_thread, "test_multi_thread" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
