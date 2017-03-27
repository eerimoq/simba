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

static struct analog_input_pin_t pins[2];

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(analog_input_pin_init(&pins[0], &pin_a0_dev) == 0);
    BTASSERT(analog_input_pin_init(&pins[1], &pin_a1_dev) == 0);

    return (0);
}

static int test_read(struct harness_t *harness_p)
{
    int value;

    value = analog_input_pin_read(&pins[0]);
    std_printf(FSTR("value a0 = %d\r\n"), value);
    BTASSERT(value >= 0);

    value = analog_input_pin_read(&pins[1]);
    std_printf(FSTR("value a1 = %d\r\n"), value);
    BTASSERT(value >= 0);
    
    return (0);
}

static int test_read_isr(struct harness_t *harness_p)
{
    int value;

    sys_lock();
    value = analog_input_pin_read_isr(&pins[0]);
    sys_unlock();
    std_printf(FSTR("value a0 = %d\r\n"), value);
    BTASSERT(value >= 0);

    sys_lock();
    value = analog_input_pin_read_isr(&pins[1]);
    sys_unlock();
    std_printf(FSTR("value a1 = %d\r\n"), value);
    BTASSERT(value >= 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_read, "test_read" },
        { test_read_isr, "test_read_isr" },
        { NULL, NULL }
    };

    sys_start();
    analog_input_pin_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
