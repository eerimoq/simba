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

static int test_duty_cycle_convert(struct harness_t *harness_p)
{
    BTASSERT(pwm_duty_cycle_as_percent(pwm_duty_cycle(0)) == 0);
    BTASSERT(pwm_duty_cycle_as_percent(pwm_duty_cycle(100)) == 100);

    return (0);
}

static int test_duty_cycle(struct harness_t *harness_p)
{
    struct pwm_driver_t pwm[PWM_DEVICE_MAX];
    int i, j;
    long duty_cycle;

    for (i = 0; i < membersof(pwm); i++) {
        BTASSERT(pwm_init(&pwm[i],
                          &pwm_device[i],
                          pwm_frequency(1000),
                          pwm_duty_cycle(0)) == 0);
        BTASSERT(pwm_start(&pwm[i]) == 0)
    }

    /* Test various duty cycles. */
    for (i = 0; i <= 100; i += 10) {
        std_printf(OSTR("Duty cycle: %d\r\n"), i);

        for (j = 0; j < membersof(pwm); j++) {
            duty_cycle = pwm_duty_cycle((i + 10 * j) % 100);
            BTASSERT(pwm_set_duty_cycle(&pwm[j], duty_cycle) == 0);
        }

        thrd_sleep_ms(1000);
    }

    /* Test maximum duty cycle. */
    for (j = 0; j < membersof(pwm); j++) {
        duty_cycle = pwm_duty_cycle(100);
        BTASSERT(pwm_set_duty_cycle(&pwm[j], duty_cycle) == 0);
    }

    thrd_sleep_ms(100);

    /* Test minimum duty cycle. */
    for (j = 0; j < membersof(pwm); j++) {
        duty_cycle = pwm_duty_cycle(0);
        BTASSERT(pwm_set_duty_cycle(&pwm[j], duty_cycle) == 0);
    }

    thrd_sleep_ms(100);

    for (i = 0; i < membersof(pwm); i++) {
        BTASSERT(pwm_stop(&pwm[i]) == 0)
    }

    thrd_sleep_ms(100);

    return (0);
}

static int test_frequency(struct harness_t *harness_p)
{
    struct pwm_driver_t pwm[PWM_DEVICE_MAX];
    int i, j;
    long frequency;

    for (i = 0; i < membersof(pwm); i++) {
        BTASSERT(pwm_init(&pwm[i],
                          &pwm_device[i],
                          pwm_frequency(1000),
                          pwm_duty_cycle(70)) == 0);
        BTASSERT(pwm_start(&pwm[i]) == 0)
    }

    /* Test various frequencies. */
    for (i = 0; i <= 1000; i += 100) {
        std_printf(OSTR("Frequency: %d\r\n"), i);

        for (j = 0; j < membersof(pwm); j++) {
            frequency = pwm_frequency(i);
            BTASSERT(pwm_set_frequency(&pwm[j], frequency) == 0);
        }

        thrd_sleep_ms(1000);
    }

    for (i = 0; i < membersof(pwm); i++) {
        BTASSERT(pwm_stop(&pwm[i]) == 0)
    }

    thrd_sleep_ms(100);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_duty_cycle_convert, "test_duty_cycle_convert" },
        { test_duty_cycle, "test_duty_cycle" },
        { test_frequency, "test_frequency" },
        { NULL, NULL }
    };

    sys_start();
    pwm_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
