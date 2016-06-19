/**
 * @file main.c
 * @version 1.0.0
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

static struct pin_driver_t red;
static struct pin_driver_t green;
static struct pin_driver_t blue;
static struct pin_driver_t white;

static int test_binary_counter(struct harness_t *harness_p)
{
    int i;

    /* Initialize the four pins. */
    pin_init(&red, &pin_led_dev, PIN_OUTPUT);
    pin_init(&green, &pin_d9_dev, PIN_OUTPUT);
    pin_init(&blue, &pin_d6_dev, PIN_OUTPUT);
    pin_init(&white, &pin_d3_dev, PIN_OUTPUT);

    i = 0;

    for (i = 0; i < 16; i++) {
        thrd_usleep(500000);
        pin_write(&red, (i & 0x1) != 0);
        pin_write(&green, (i & 0x2) != 0);
        pin_write(&blue, (i & 0x4) != 0);
        pin_write(&white, (i & 0x8) != 0);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_binary_counter, "test_binary_counter" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();
    pin_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
