/**
 * @file drivers/pwm.h
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

#ifndef __DRIVERS_PWM_H__
#define __DRIVERS_PWM_H__

#include "simba.h"

#include "pwm_port.h"

extern struct pwm_device_t pwm_device[PWM_DEVICE_MAX];

/**
 * Initialize driver object.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 *
 * @return zero(0) or negative error code.
 */
int pwm_init(struct pwm_driver_t *self_p,
             struct pwm_device_t *dev_p);

/**
 * Set duty cycle.
 *
 * @param[in] self_p Driver object.
 * @param[in] value Value to set [0..255]
 *
 * @return zero(0) or negative error code.
 */
int pwm_set_duty(struct pwm_driver_t *self_p,
                 uint8_t value);

#endif
