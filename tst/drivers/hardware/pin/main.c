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

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(pin_module_init() == 0);
    BTASSERT(pin_module_init() == 0);

    return (0);
}

static int test_output(struct harness_t *harness_p)
{
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);
    BTASSERT(pin_init(&pin, &pin_d3_dev, PIN_OUTPUT) == 0);

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
    BTASSERT(pin_init(&pin, &pin_d3_dev, PIN_OUTPUT) == 0);

    /* Default low. */
    BTASSERT(pin_read(&testpin) == 0);

    return (0);
}

static int test_output_open_drain(struct harness_t *harness_p)
{
    int res;
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);

    res = pin_init(&pin, &pin_d3_dev, PIN_OUTPUT_OPEN_DRAIN);

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

static int test_output_open_drain_pull_up(struct harness_t *harness_p)
{
    int res;
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);

    res = pin_init(&pin, &pin_d3_dev, PIN_OUTPUT_OPEN_DRAIN_PULL_UP);

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

static int test_input(struct harness_t *harness_p)
{
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_init(&pin, &pin_d3_dev, PIN_INPUT) == 0);

    /* Read high. */
    BTASSERT(pin_write(&testpin, 1) == 0);
    BTASSERT(pin_read(&pin) == 1);

    /* Read low. */
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_read(&pin) == 0);

    return (0);
}

static int test_input_pull_up(struct harness_t *harness_p)
{
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);
    BTASSERT(pin_init(&pin, &pin_d3_dev, PIN_INPUT_PULL_UP) == 0);

    /* Read high. */
    BTASSERT(pin_read(&pin) == 1);

    /* Read low. */
    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_read(&pin) == 0);

    return (0);
}

static int test_input_pull_down(struct harness_t *harness_p)
{
    int res;
    struct pin_driver_t testpin;
    struct pin_driver_t pin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);

    res = pin_init(&pin, &pin_d3_dev, PIN_INPUT_PULL_DOWN);

    if (res == 0) {
        /* Read high. */
        BTASSERT(pin_read(&pin) == 1);

        /* Read low. */
        BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_OUTPUT) == 0);
        BTASSERT(pin_write(&testpin, 0) == 0);
        BTASSERT(pin_read(&pin) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_device_output(struct harness_t *harness_p)
{
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);
    BTASSERT(pin_device_set_mode(&pin_d3_dev, PIN_OUTPUT) == 0);

    /* Default low. */
    BTASSERT(pin_read(&testpin) == 0);

    /* Write high. */
    BTASSERT(pin_device_write_high(&pin_d2_dev) == 0);
    BTASSERT(pin_read(&testpin) == 1);

    /* Write low. */
    BTASSERT(pin_device_write_low(&pin_d2_dev) == 0);
    BTASSERT(pin_read(&testpin) == 0);

    /* Re-initialize the pin from high state. */
    BTASSERT(pin_device_write_high(&pin_d2_dev) == 0);
    BTASSERT(pin_device_set_mode(&pin_d3_dev, PIN_OUTPUT) == 0);

    /* Default low. */
    BTASSERT(pin_read(&testpin) == 0);

    return (0);
}

static int test_device_output_open_drain(struct harness_t *harness_p)
{
    int res;
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);

    res = pin_device_set_mode(&pin_d3_dev, PIN_OUTPUT_OPEN_DRAIN);

    if (res == 0) {
        /* Default floating. */
        std_printf(OSTR("value: %d\r\n"), pin_read(&testpin));

        /* Write low. */
        BTASSERT(pin_device_write_low(&pin_d2_dev) == 0);
        BTASSERT(pin_read(&testpin) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_device_output_open_drain_pull_up(struct harness_t *harness_p)
{
    int res;
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);

    res = pin_device_set_mode(&pin_d3_dev, PIN_OUTPUT_OPEN_DRAIN_PULL_UP);

    if (res == 0) {
        /* Default high. */
        BTASSERT(pin_read(&testpin) == 1);

        /* Write low. */
        BTASSERT(pin_device_write_low(&pin_d2_dev) == 0);
        BTASSERT(pin_read(&testpin) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

static int test_device_input(struct harness_t *harness_p)
{
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_device_set_mode(&pin_d3_dev, PIN_INPUT) == 0);

    /* Read high. */
    BTASSERT(pin_write(&testpin, 1) == 0);
    BTASSERT(pin_device_read(&pin_d2_dev) == 1);

    /* Read low. */
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_device_read(&pin_d2_dev) == 0);

    return (0);
}

static int test_device_input_pull_up(struct harness_t *harness_p)
{
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);
    BTASSERT(pin_device_set_mode(&pin_d3_dev, PIN_INPUT_PULL_UP) == 0);

    /* Read high. */
    BTASSERT(pin_device_read(&pin_d2_dev) == 1);

    /* Read low. */
    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_write(&testpin, 0) == 0);
    BTASSERT(pin_device_read(&pin_d2_dev) == 0);

    return (0);
}

static int test_device_input_pull_down(struct harness_t *harness_p)
{
    int res;
    struct pin_driver_t testpin;

    BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_INPUT) == 0);

    res = pin_device_set_mode(&pin_d3_dev, PIN_INPUT_PULL_DOWN);

    if (res == 0) {
        /* Read high. */
        BTASSERT(pin_device_read(&pin_d2_dev) == 1);

        /* Read low. */
        BTASSERT(pin_init(&testpin, &pin_d2_dev, PIN_OUTPUT) == 0);
        BTASSERT(pin_write(&testpin, 0) == 0);
        BTASSERT(pin_device_read(&pin_d2_dev) == 0);
    } else {
        BTASSERT(res == -ENOSYS);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_output, "test_output" },
        { test_output_open_drain, "test_output_open_drain" },
        { test_output_open_drain_pull_up, "test_output_open_drain_pull_up" },
        { test_input, "test_input" },
        { test_input_pull_up, "test_input_pull_up" },
        { test_input_pull_down, "test_input_pull_down" },
        { test_device_output, "test_device_output" },
        { test_device_output_open_drain, "test_device_output_open_drain" },
        { test_device_output_open_drain_pull_up, "test_device_output_open_drain_pull_up" },
        { test_device_input, "test_device_input" },
        { test_device_input_pull_up, "test_device_input_pull_up" },
        { test_device_input_pull_down, "test_device_input_pull_down" },
        { NULL, NULL }
    };

    sys_start();
    pin_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
