/**
 * @file main.c
 * @version 6.0.0
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
