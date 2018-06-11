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

static volatile int data0;
static volatile int data1;
static volatile int data2;
static struct cond_t cond0;
static struct cond_t cond1;
static struct cond_t cond2;
static struct mutex_t mutex0;
static struct mutex_t mutex1;
static struct mutex_t mutex2;

#if defined(ARCH_ESP32) || defined(ARCH_PPC)
static THRD_STACK(t0_stack, 512);
static THRD_STACK(t1_stack, 512);
#elif defined(ARCH_ARM64)
static THRD_STACK(t0_stack, 1024);
static THRD_STACK(t1_stack, 1024);
#else
static THRD_STACK(t0_stack, 224);
static THRD_STACK(t1_stack, 224);
#endif

static void *cond_main_0(void *arg_p)
{
    thrd_set_name("cond_0");

    /* test_signal */
    mutex_lock(&mutex0);

    if (data0 == 0) {
        cond_wait(&cond0, &mutex0, NULL);
    }

    BTASSERTN(data0 == 1);
    data0 = 0;
    mutex_unlock(&mutex0);

    thrd_sleep_ms(1);

    mutex_lock(&mutex1);
    std_printf(OSTR("Thread 0 signalling thread 1.\r\n"));
    data1 = 1;
    cond_signal(&cond1);
    mutex_unlock(&mutex1);

    /* test_broadcast */
    mutex_lock(&mutex0);

    if (data0 == 0) {
        cond_wait(&cond0, &mutex0, NULL);
    }

    BTASSERTN(data0 >= 1);
    data0--;
    mutex_unlock(&mutex0);

    thrd_sleep_ms(1);

    mutex_lock(&mutex1);
    std_printf(OSTR("Thread 0 signalling main thread.\r\n"));
    data1 = 1;
    cond_signal(&cond1);
    mutex_unlock(&mutex1);

    return (NULL);
}

static void *cond_main_1(void *arg_p)
{
    thrd_set_name("cond_1");

    /* test_signal */
    mutex_lock(&mutex1);

    if (data1 == 0) {
        cond_wait(&cond1, &mutex1, NULL);
    }

    BTASSERTN(data1 == 1);
    data1 = 0;
    mutex_unlock(&mutex1);

    thrd_sleep_ms(1);

    mutex_lock(&mutex2);
    std_printf(OSTR("Thread 1 signalling main thread.\r\n"));
    data2 = 1;
    cond_signal(&cond2);
    mutex_unlock(&mutex2);

    /* test_broadcast */
    mutex_lock(&mutex0);

    if (data0 == 0) {
        cond_wait(&cond0, &mutex0, NULL);
    }

    BTASSERTN(data0 >= 0);
    data0--;
    mutex_unlock(&mutex0);

    thrd_sleep_ms(1);

    mutex_lock(&mutex2);
    std_printf(OSTR("Thread 1 signalling main thread.\r\n"));
    data2 = 1;
    cond_signal(&cond2);
    mutex_unlock(&mutex2);

    return (NULL);
}

static int test_init(void)
{
    BTASSERT(cond_module_init() == 0);
    BTASSERT(cond_module_init() == 0);

    BTASSERT(mutex_init(&mutex0) == 0);
    BTASSERT(mutex_init(&mutex1) == 0);
    BTASSERT(mutex_init(&mutex2) == 0);

    BTASSERT(cond_init(&cond0) == 0);
    BTASSERT(cond_init(&cond1) == 0);
    BTASSERT(cond_init(&cond2) == 0);

    data0 = 0;
    data1 = 0;
    data2 = 0;

    return (0);
}

static int test_wait_timeout(void)
{
    int res;
    struct time_t timeout;

    /* No threads waiting. */
    BTASSERT(cond_signal(&cond0) == 0);
    BTASSERT(cond_signal(&cond1) == 0);
    BTASSERT(cond_signal(&cond2) == 0);

    timeout.seconds = 0;
    timeout.nanoseconds = 1;

    mutex_lock(&mutex0);
    res = cond_wait(&cond0, &mutex0, &timeout);
    mutex_unlock(&mutex0);

    BTASSERTI(res, ==, -ETIMEDOUT);

    mutex_lock(&mutex1);
    res = cond_wait(&cond1, &mutex1, &timeout);
    mutex_unlock(&mutex1);

    BTASSERTI(res, ==, -ETIMEDOUT);

    mutex_lock(&mutex2);
    cond_wait(&cond2, &mutex2, &timeout);
    mutex_unlock(&mutex2);

    BTASSERTI(res, ==, -ETIMEDOUT);

    return (0);
}

static int test_signal(void)
{
    int res;

    thrd_spawn(cond_main_0,
               NULL,
               -10,
               t0_stack,
               sizeof(t0_stack));
    thrd_spawn(cond_main_1,
               NULL,
               -10,
               t1_stack,
               sizeof(t1_stack));

    thrd_sleep_ms(1);

    /* Signal thread 0. */
    mutex_lock(&mutex0);
    std_printf(OSTR("Signalling thread 0.\r\n"));
    data0 = 1;
    res = cond_signal(&cond0);
    mutex_unlock(&mutex0);

    BTASSERTI(res, ==, 1);

    /* Wait for signal from thread 1. */
    mutex_lock(&mutex2);

    if (data2 == 0) {
        cond_wait(&cond2, &mutex2, NULL);
    }

    BTASSERT(data2 == 1);
    data2 = 0;
    mutex_unlock(&mutex2);

    return (0);
}

static int test_broadcast(void)
{
    int res;

    thrd_sleep_ms(1);

    /* Signal thread 0. */
    mutex_lock(&mutex0);
    std_printf(OSTR("Broadcasting from main thread.\r\n"));
    data0 = 2;
    res = cond_broadcast(&cond0);
    mutex_unlock(&mutex0);
    BTASSERTI(res, ==, 2);

    /* Wait for signal from thread 1. */
    mutex_lock(&mutex1);

    if (data1 == 0) {
        cond_wait(&cond1, &mutex1, NULL);
    }

    BTASSERT(data1 == 1);
    data1 = 0;
    mutex_unlock(&mutex1);

    /* Wait for signal from thread 2. */
    mutex_lock(&mutex2);

    if (data2 == 0) {
        cond_wait(&cond2, &mutex2, NULL);
    }

    BTASSERT(data2 == 1);
    data2 = 0;
    mutex_unlock(&mutex2);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_wait_timeout, "test_wait_timeout" },
        { test_signal, "test_signal" },
        { test_broadcast, "test_broadcast" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
