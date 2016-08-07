/**
 * @file main.c
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

static struct pin_driver_t pin;

static int test_device_input_output(struct harness_t *harness_p)
{
    BTASSERT(pin_device_set_mode(&pin_led_dev, PIN_OUTPUT) == 0);

    /* Write high. */
    BTASSERT(pin_device_write_high(&pin_led_dev) == 0);
    BTASSERT(pin_device_read(&pin_led_dev) == 1);

    /* Write low. */
    BTASSERT(pin_device_write_low(&pin_led_dev) == 0);
    BTASSERT(pin_device_read(&pin_led_dev) == 0);

    return (0);
}

static int test_input_output(struct harness_t *harness_p)
{
    BTASSERT(pin_init(&pin, &pin_led_dev, PIN_OUTPUT) == 0);

    /* Write high. */
    BTASSERT(pin_write(&pin, 1) == 0);
    BTASSERT(pin_read(&pin) == 1);

    /* Write low. */
    BTASSERT(pin_write(&pin, 0) == 0);
    BTASSERT(pin_read(&pin) == 0);

    /* Toggle. */
    BTASSERT(pin_toggle(&pin) == 0);
    BTASSERT(pin_read(&pin) == 1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_device_input_output, "test_device_input_output" },
        { test_input_output, "test_input_output" },
        { NULL, NULL }
    };

    sys_start();
    pin_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
