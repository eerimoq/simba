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

static volatile int flag = 0;

static void isr(void *arg_p)
{
    flag++;
}

int test_exti(struct harness_t *harness_p)
{
    int i;
    struct exti_driver_t exti;
    struct pin_driver_t pin;

    pin_init(&pin, &pin_d4_dev, PIN_OUTPUT);
    pin_write(&pin, 1);
    
    exti_init(&exti,
              &exti_device[1],
              EXTI_TRIGGER_FALLING_EDGE,
              isr,
              NULL);
    exti_start(&exti);

    for (i = 0; i < 10; i++) {
        pin_write(&pin, 0);
        time_sleep(10000);
        pin_write(&pin, 1);
        time_sleep(10000);
    }

    std_printf(FSTR("flag = %d"), (int)flag);

    BTASSERT(flag == 10);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_exti, "test_exti" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
