/**
 * @file main.c
 * @version 0.3.0
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

static int test_convert(struct harness_t *harness_p)
{
    int i;
    int samples[16];
    struct adc_driver_t adc;

    adc_init(&adc,
             &adc_0_dev,
             &pin_a0_dev,
             ADC_REFERENCE_VCC,
             10);

    adc_convert(&adc, samples, membersof(samples));

    for (i = 0; i < membersof(samples); i++) {
        std_printf(FSTR("[%d]: %d"), i, samples[i]);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_convert, "test_convert" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();
    adc_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
