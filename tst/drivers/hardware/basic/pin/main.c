/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

static int test_init(void)
{
    BTASSERT(pin_module_init() == 0);
    BTASSERT(pin_module_init() == 0);

    return (0);
}

static int test_output(void)
{
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);
    BTASSERT(pin_init(&pin, &pin_d9_dev, PIN_OUTPUT) == 0);

    /* Default low. */
    BTASSERT(pin_read(&testpin) == 0);

    /* Write high. */
    BTASSERT(pin_write(&pin, 1) == 0);
    BTASSERT(pin_read(&testpin) == 1);

    /* Write low. */
    BTASSERT(pin_write(&pin, 0) == 0);
    BTASSERT(pin_read(&testpin) == 0);

    /* Toggle. */
    BTASSERT(pin_toggle(&pin) == 0);
    BTASSERT(pin_read(&testpin) == 1);

    BTASSERT(pin_toggle(&pin) == 0);
    BTASSERT(pin_read(&testpin) == 0);

    /* Re-initialize the pin from high state. */
    BTASSERT(pin_write(&pin, 1) == 0);
    BTASSERT(pin_init(&pin, &pin_d9_dev, PIN_OUTPUT) == 0);

    /* Default low. */
    BTASSERT(pin_read(&testpin) == 0);

    return (0);
}

static int test_output_open_drain(void)
{
    int res;
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);

    res = pin_init(&pin, &pin_d9_dev, PIN_OUTPUT_OPEN_DRAIN);

    if (res == 0) {
        /* Default floating. */
        std_printf(OSTR("value: %d\r\n"), pin_read(&testpin));

        /* Write low. */
        BTASSERT(pin_write(&pin, 0) == 0);
        BTASSERT(pin_read(&testpin) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_output_open_drain_pull_up(void)
{
    int res;
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);

    res = pin_init(&pin, &pin_d9_dev, PIN_OUTPUT_OPEN_DRAIN_PULL_UP);

    if (res == 0) {
        /* Default high. */
        BTASSERT(pin_read(&testpin) == 1);

        /* Write low. */
        BTASSERT(pin_write(&pin, 0) == 0);
        BTASSERT(pin_read(&testpin) == 0);

        /* Toggle. */
        BTASSERT(pin_toggle(&pin) == 0);
        BTASSERT(pin_read(&testpin) == 1);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_input(void)
{
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_init(&pin, &pin_d9_dev, PIN_INPUT) == 0);

    /* Read high. */
    BTASSERT(pin_write(&testpin, 1) == 0);
    BTASSERT(pin_read(&pin) == 1);

    /* Read low. */
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_read(&pin) == 0);

    return (0);
}

static int test_input_pull_up(void)
{
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);
    BTASSERT(pin_init(&pin, &pin_d9_dev, PIN_INPUT_PULL_UP) == 0);

    /* Read high. */
    BTASSERT(pin_read(&pin) == 1);

    /* Read low. */
    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_read(&pin) == 0);

    return (0);
}

static int test_input_pull_down(void)
{
    int res;
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);

    res = pin_init(&pin, &pin_d9_dev, PIN_INPUT_PULL_DOWN);

    if (res == 0) {
        /* Read high. */
        BTASSERT(pin_read(&pin) == 1);

        /* Read low. */
        BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_OUTPUT) == 0);
        BTASSERT(pin_write(&testpin, 0) == 0);
        BTASSERT(pin_read(&pin) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_device_output(void)
{
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);
    BTASSERT(pin_device_set_mode(&pin_d9_dev, PIN_OUTPUT) == 0);

    /* Default low. */
    BTASSERT(pin_read(&testpin) == 0);

    /* Write high. */
    BTASSERT(pin_device_write_high(&pin_d9_dev) == 0);
    BTASSERT(pin_read(&testpin) == 1);

    /* Write low. */
    BTASSERT(pin_device_write_low(&pin_d9_dev) == 0);
    BTASSERT(pin_read(&testpin) == 0);

    /* Re-initialize the pin from high state. */
    BTASSERT(pin_device_write_high(&pin_d9_dev) == 0);
    BTASSERT(pin_device_set_mode(&pin_d9_dev, PIN_OUTPUT) == 0);

    /* Default low. */
    BTASSERT(pin_read(&testpin) == 0);

    return (0);
}

static int test_device_output_open_drain(void)
{
    int res;
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);

    res = pin_device_set_mode(&pin_d9_dev, PIN_OUTPUT_OPEN_DRAIN);

    if (res == 0) {
        /* Default floating. */
        std_printf(OSTR("value: %d\r\n"), pin_read(&testpin));

        /* Write low. */
        BTASSERT(pin_device_write_low(&pin_d9_dev) == 0);
        BTASSERT(pin_read(&testpin) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_device_output_open_drain_pull_up(void)
{
    int res;
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);

    res = pin_device_set_mode(&pin_d9_dev, PIN_OUTPUT_OPEN_DRAIN_PULL_UP);

    if (res == 0) {
        /* Default high. */
        BTASSERT(pin_read(&testpin) == 1);

        /* Write low. */
        BTASSERT(pin_device_write_low(&pin_d8_dev) == 0);
        BTASSERT(pin_read(&testpin) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_device_input(void)
{
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_device_set_mode(&pin_d9_dev, PIN_INPUT) == 0);

    /* Read high. */
    BTASSERT(pin_write(&testpin, 1) == 0);
    BTASSERT(pin_device_read(&pin_d8_dev) == 1);

    /* Read low. */
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_device_read(&pin_d8_dev) == 0);

    return (0);
}

static int test_device_input_pull_up(void)
{
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);
    BTASSERT(pin_device_set_mode(&pin_d9_dev, PIN_INPUT_PULL_UP) == 0);
    time_busy_wait_us(1);

    /* Read high since the internal pull-up resistor is enabled. */
    BTASSERT(pin_device_read(&pin_d9_dev) == 1);

    /* Set the test pin low to pull low harder than the pull-up pulls
       high. */
    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_device_read(&pin_d9_dev) == 0);

    return (0);
}

static int test_device_input_pull_down(void)
{
    int res;
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_INPUT) == 0);

    res = pin_device_set_mode(&pin_d9_dev, PIN_INPUT_PULL_DOWN);

    if (res == 0) {
        /* Read high. */
        BTASSERT(pin_device_read(&pin_d8_dev) == 1);

        /* Read low. */
        BTASSERT(pin_init(&testpin, &pin_d8_dev, PIN_OUTPUT) == 0);
        BTASSERT(pin_write(&testpin, 0) == 0);
        BTASSERT(pin_device_read(&pin_d8_dev) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_output, "test_output" },
        { test_output_open_drain, "test_output_open_drain" },
        { test_output_open_drain_pull_up, "test_output_open_drain_pull_up" },
        { test_input, "test_input" },
        { test_input_pull_up, "test_input_pull_up" },
        { test_input_pull_down, "test_input_pull_down" },
        { test_device_output, "test_device_output" },
        { test_device_output_open_drain, "test_device_output_open_drain" },
        {
            test_device_output_open_drain_pull_up,
            "test_device_output_open_drain_pull_up"
        },
        { test_device_input, "test_device_input" },
        { test_device_input_pull_up, "test_device_input_pull_up" },
        { test_device_input_pull_down, "test_device_input_pull_down" },
        { NULL, NULL }
    };

    sys_start();
    pin_module_init();

    harness_run(testcases);

    return (0);
}
