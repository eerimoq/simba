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

int analog_output_pin_module_init(void)
{
    return (0);
}

int analog_output_pin_init(struct analog_output_pin_t *self_p,
                          struct pin_device_t *pin_p)
{
    struct pwm_device_t *pwm_p;
    
    pwm_p = pwm_pin_to_device(pin_p);

    if (pwm_p == NULL) {
        return (-1);
    }
    
    return (pwm_init(&self_p->pwm, pwm_p));
}

int analog_output_pin_write(struct analog_output_pin_t *self_p,
                            int value)
{
    return (pwm_set_duty(&self_p->pwm, value / 4));
}

int analog_output_pin_read(struct analog_output_pin_t *self_p)
{
    return (pwm_get_duty(&self_p->pwm) * 4);
}
