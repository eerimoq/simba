/**
 * @file main.c
 * @version 0.5.0
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

struct pin_driver_t pin;
struct adc_driver_t adc;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(adc_init(&adc,
                      &adc_0_dev,
                      &pin_a0_dev,
                      ADC_REFERENCE_VCC,
                      10) == 0);

    BTASSERT(pin_init(&pin, &pin_d2_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_write(&pin, 1) == 0);

    return (0);
}

static int test_convert(struct harness_t *harness_p)
{
    int i;
    uint16_t samples[16];

    memset(samples, 0xffff, sizeof(samples));

    BTASSERT(adc_convert(&adc, samples, membersof(samples)) == 0);

    for (i = 0; i < membersof(samples); i++) {
        std_printf(FSTR("[%d]: %d\r\n"), i, samples[i]);
        BTASSERT(samples[i] != 0xffff);
    }

    return (0);
}

static int test_convert_isr(struct harness_t *harness_p)
{
    uint16_t sample;

    /* Low value. */
    sample = 0xffff;
    BTASSERT(pin_write(&pin, 0) == 0);
    time_sleep(10);
    sys_lock();
    
    if (adc_convert_isr(&adc, &sample) != 0) {
        sys_unlock();
        
        return (-1);
    }
    
    sys_unlock();
    std_printf(FSTR("low sample = %d\r\n"), sample);
    BTASSERT(sample < 300);

    /* High value. */
    sample = 0x0000;
    BTASSERT(pin_write(&pin, 1) == 0);
    time_sleep(10);
    sys_lock();
    
    if (adc_convert_isr(&adc, &sample) != 0) {
        sys_unlock();
        
        return (-1);
    }
    
    sys_unlock();
    std_printf(FSTR("high sample = %d\r\n"), sample);
    BTASSERT(sample > 500);
    
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_convert, "test_convert" },
        { test_convert_isr, "test_convert_isr" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();
    adc_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
