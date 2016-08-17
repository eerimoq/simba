/**
 * @file pin.c
 * @version 7.0.0
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

int analog_input_pin_module_init(void)
{
    return (adc_module_init());
}

int analog_input_pin_init(struct analog_input_pin_t *self_p,
                          struct pin_device_t *pin_p)
{
    return (adc_init(&self_p->adc,
                     &adc_device[0],
                     pin_p,
                     ADC_REFERENCE_VCC,
                     1));
}

int analog_input_pin_read(struct analog_input_pin_t *self_p)
{
    uint16_t sample;
    
    if (adc_convert(&self_p->adc, &sample, 1) == 0) {
        return (sample);
    }

    return (-1);
}

int analog_input_pin_read_isr(struct analog_input_pin_t *self_p)
{
    uint16_t sample;
    
    if (adc_convert_isr(&self_p->adc, &sample) == 0) {
        return (sample);
    }

    return (-1);
}
