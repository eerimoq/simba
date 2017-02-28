/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
    int duty_cycle;

    for (i = 0; i < membersof(pwm); i++) {
        BTASSERT(pwm_init(&pwm[i], &pwm_device[i]) == 0);
    }

    /* Test various duty cycles. */
    for (i = 0; i <= 100; i += 10) {
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

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_duty_cycle_convert, "test_duty_cycle_convert" },
        { test_duty_cycle, "test_duty_cycle" },
        { NULL, NULL }
    };

    sys_start();
    pwm_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
