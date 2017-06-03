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

static struct thrd_t *thrd_p;
struct event_t event;

static void callback(void *arg_p)
{
    uint32_t mask;

    mask = *(uint32_t *)arg_p;

    event_write_isr(&event, &mask, sizeof(mask));
}

static int test_single_shot(struct harness_t *harness_p)
{
    uint32_t mask;
    uint32_t callback_mask;
    struct timer_t timer;
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 100000000
    };

    event_init(&event);
    callback_mask = 0x1;
    BTASSERT(timer_init(&timer, &timeout, callback, &callback_mask, 0) == 0);

    thrd_p = thrd_self();

    /* Single shot timer. */
    BTASSERT(timer_start(&timer) == 0);

    mask = 0x1;
    event_read(&event, &mask, sizeof(mask));

    /* Not necessary to stop an expired timer, but should still
       work. */
    BTASSERT(timer_stop(&timer) == 0);

    return (0);
}

static int test_periodic(struct harness_t *harness_p)
{
    int i;
    uint32_t mask;
    uint32_t callback_mask;
    struct timer_t timer;
    struct time_t now;
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 100000000
    };

    event_init(&event);
    callback_mask = 0x1;
    thrd_p = thrd_self();

    /* Periodic timer. */
    std_printf(FSTR("Starting a periodic timer with 100 ms period.\r\n"));
    BTASSERT(timer_init(&timer,
                        &timeout,
                        callback,
                        &callback_mask,
                        TIMER_PERIODIC) == 0);
    BTASSERT(timer_start(&timer) == 0);

    std_printf(FSTR(" MS  MESSAGE\r\n"));

    for (i = 0; i < 5; i++) {
        mask = 0x1;
        event_read(&event, &mask, sizeof(mask));
        time_get(&now);
        std_printf(FSTR("%03u: timeout %d.\r\n"),
                   (now.nanoseconds / 1000000),
                   i);
    }

    BTASSERT(timer_stop(&timer) == 1);

    return (0);
}

static int test_multiple_timers(struct harness_t *harness_p)
{
    int i;
    int j;
    int period_ms;
    uint32_t mask;
    uint32_t callback_masks[32];
    int timeout_count[32];
    struct timer_t timers[32];
    struct time_t now;
    struct time_t timeout = {
        .seconds = 0,
        .nanoseconds = 0
    };

    event_init(&event);
    thrd_p = thrd_self();

    /* Periodic timers. */
    for (i = 0; i < membersof(timers); i++) {
        period_ms = (1 + 2 * i);
        std_printf(FSTR("Starting periodic timer %d with %d ms period.\r\n"),
                   i,
                   period_ms);
        callback_masks[i] = (1 << i);
        timeout_count[i] = 0;
        timeout.nanoseconds = (1000000 * period_ms);
        BTASSERT(timer_init(&timers[i],
                            &timeout,
                            callback,
                            &callback_masks[i],
                            TIMER_PERIODIC) == 0);
        BTASSERT(timer_start(&timers[i]) == 0);
    }

    std_printf(FSTR(" MS  MESSAGE\r\n"));

    for (i = 0; i < 30; i++) {
        mask = 0xffffffff;
        event_read(&event, &mask, sizeof(mask));
        time_get(&now);

        for (j = 0; j < membersof(timers); j++) {
            if (mask & (1 << j)) {
                timeout_count[j]++;
                std_printf(FSTR("%03u: timeout %d, timer %d.\r\n"),
                           (now.nanoseconds / 1000000),
                           i,
                           j);
            }
        }
    }

    for (i = 0; i < membersof(timers); i++) {
        BTASSERT(timer_stop(&timers[i]) == 1);
        if (i == 0) {
            BTASSERT(timeout_count[i] > 0);
        } else {
            BTASSERTI(timeout_count[i], <=, timeout_count[i - 1]);
        }
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_single_shot, "test_single_shot" },
        { test_periodic, "test_periodic" },
        { test_multiple_timers, "test_multiple_timers" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
