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

static struct analog_output_pin_t pin;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(analog_output_pin_init(&pin, &pin_d10_dev) == 0);

    /* Bad pwm pin. */
    BTASSERT(analog_output_pin_init(&pin, &pin_d5_dev) == -1);

    return (0);
}

static int test_write(struct harness_t *harness_p)
{
    BTASSERT(analog_output_pin_write(&pin, 256) == 0);
    BTASSERT(analog_output_pin_read(&pin) == 256);
    
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_write, "test_write" },
        { NULL, NULL }
    };

    sys_start();
    analog_output_pin_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
