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

#if defined(FAMILY_SPC5)

static volatile int interrupt = 0;

static void isr_watchdog_timeout(void)
{
    interrupt = 1;
}

#endif

static int test_interrupt(struct harness_t *harness_p)
{
#if defined(FAMILY_SPC5)

    watchdog_start_ms(50, isr_watchdog_timeout);

    thrd_sleep_ms(40);
    watchdog_kick();

    /* Wait for the interrupt. */
    while (interrupt == 0);

    watchdog_kick();

    watchdog_stop();
    thrd_sleep_ms(60);

    return (0);

#else

    return (1);

#endif
}

static int test_no_interrupt(struct harness_t *harness_p)
{
    watchdog_start_ms(20, 0);

    thrd_sleep_ms(10);
    watchdog_kick();

    thrd_sleep_ms(10);
    watchdog_kick();

    thrd_sleep_ms(10);
    watchdog_kick();

    watchdog_stop();
    thrd_sleep_ms(30);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_interrupt, "test_interrupt" },
        { test_no_interrupt, "test_no_interrupt" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
