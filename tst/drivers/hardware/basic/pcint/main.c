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

static volatile int count;
static struct pin_driver_t pin;

static void isr(void *arg_p)
{
    count++;
}

int test_init(void)
{
    BTASSERT(pcint_module_init() == 0);
    BTASSERT(pcint_module_init() == 0);

    pin_init(&pin, &pin_a8_dev, PIN_OUTPUT);

    return (0);
}

int test_rising(void)
{
    int i;
    struct pcint_driver_t pcint;

    pin_write(&pin, 0);
    count = 0;

    BTASSERT(pcint_init(&pcint,
                        &pcint_a9_dev,
                        PCINT_TRIGGER_RISING_EDGE,
                        isr,
                        NULL) == 0);
    BTASSERT(pcint_start(&pcint) == 0);

    /* 10 rising, 9 falling edges. */
    for (i = 0; i < 10; i++) {
        pin_write(&pin, 0);
        time_busy_wait_us(10000);
        pin_write(&pin, 1);
        time_busy_wait_us(10000);
    }

    BTASSERT(pcint_stop(&pcint) == 0);

    /* The interrupt is disabled, no increment. */
    pin_write(&pin, 0);
    time_busy_wait_us(10000);
    pin_write(&pin, 1);
    time_busy_wait_us(10000);

    std_printf(FSTR("count: %d\r\n"), count);
    BTASSERT(count == 10);

    return (0);
}

int test_falling(void)
{
    int i;
    struct pcint_driver_t pcint;

    pin_write(&pin, 1);
    count = 0;

    BTASSERT(pcint_init(&pcint,
                        &pcint_a9_dev,
                        PCINT_TRIGGER_FALLING_EDGE,
                        isr,
                        NULL) == 0);
    BTASSERT(pcint_start(&pcint) == 0);

    /* 10 falling, 9 rising edges. */
    for (i = 0; i < 10; i++) {
        pin_write(&pin, 1);
        time_busy_wait_us(10000);
        pin_write(&pin, 0);
        time_busy_wait_us(10000);
    }

    BTASSERT(pcint_stop(&pcint) == 0);

    /* The interrupt is disabled, no increment. */
    pin_write(&pin, 1);
    time_busy_wait_us(10000);
    pin_write(&pin, 0);
    time_busy_wait_us(10000);

    std_printf(FSTR("count: %d\r\n"), count);
    BTASSERT(count == 10);

    return (0);
}

int test_both(void)
{
    int i;
    struct pcint_driver_t pcint;

    pin_write(&pin, 1);
    count = 0;

    BTASSERT(pcint_init(&pcint,
                        &pcint_a9_dev,
                        PCINT_TRIGGER_BOTH_EDGES,
                        isr,
                        NULL) == 0);
    BTASSERT(pcint_start(&pcint) == 0);

    /* 10 falling, 9 rising edges, for a total of 19 interrupts. */
    for (i = 0; i < 10; i++) {
        pin_write(&pin, 1);
        time_busy_wait_us(10000);
        pin_write(&pin, 0);
        time_busy_wait_us(10000);
    }

    BTASSERT(pcint_stop(&pcint) == 0);

    /* The interrupt is disabled, no increment. */
    pin_write(&pin, 1);
    time_busy_wait_us(10000);
    pin_write(&pin, 0);
    time_busy_wait_us(10000);

    std_printf(FSTR("count: %d\r\n"), count);
    BTASSERT(count == 19);

    return (0);
}

int main()
{
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_rising, "test_rising" },
        { test_falling, "test_falling" },
        { test_both, "test_both" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(harness_testcases);

    return (0);
}
