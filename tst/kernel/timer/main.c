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

#define EVENT_MASK 0x1

static struct thrd_t *thrd_p;
struct event_t event;

static void callback(void *arg_p)
{
    uint32_t mask = EVENT_MASK;

    event_write_isr(&event, &mask, sizeof(mask));
}

int test_timer(struct harness_t *harness_p)
{
    int i;
    uint32_t mask;
    struct timer_t timer;
    struct time_t now;
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 100000000
    };

    event_init(&event);
    timer_init(&timer, &timeout, callback, NULL, 0);

    thrd_p = thrd_self();

    /* Single shot timer. */
    timer_start(&timer);

    mask = EVENT_MASK;
    event_read(&event, &mask, sizeof(mask));

    thrd_sleep_us(100000);

    /* Periodic timer. */
    std_printf(FSTR("Starting a periodic timer with 100 ms period.\r\n"));
    timer_init(&timer, &timeout, callback, NULL, TIMER_PERIODIC);
    timer_start(&timer);

    std_printf(FSTR(" MS  MESSAGE\r\n"));

    for (i = 0; i < 5; i++) {
        mask = EVENT_MASK;
        event_read(&event, &mask, sizeof(mask));
        time_get(&now);
        std_printf(FSTR("%03u: timeout %d.\r\n"),
                   (now.nanoseconds / 1000000),
                   i);
    }

    BTASSERT(timer_stop(&timer) == 0);

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

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
