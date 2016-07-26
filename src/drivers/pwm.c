/**
 * @file pwm.c
 * @version 3.1.0
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

#include "pwm_port.i"

int pwm_init(struct pwm_driver_t *self_p,
             struct pwm_device_t *dev_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    return (pwm_port_init(self_p, dev_p));
}

int pwm_set_duty(struct pwm_driver_t *self_p,
                 uint8_t value)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (pwm_port_set_duty(self_p, value));
}
