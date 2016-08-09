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
    
    BTASSERT(exti_init(&exti,
                       &exti_d3_dev,
                       EXTI_TRIGGER_FALLING_EDGE,
                       isr,
                       NULL) == 0);
    BTASSERT(exti_start(&exti) == 0);

    for (i = 0; i < 10; i++) {
        pin_write(&pin, 0);
        time_busy_wait_us(10000);
        pin_write(&pin, 1);
        time_busy_wait_us(10000);
    }

    std_printf(FSTR("flag = %d\r\n"), (int)flag);
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

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
