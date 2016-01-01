/**
 * @file drivers/pin.h
 * @version 0.1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#ifndef __DRIVERS_PIN_H__
#define __DRIVERS_PIN_H__

#include "simba.h"

#define PIN_OUTPUT 0
#define PIN_INPUT  1

#include "pin_port.h"

extern struct pin_device_t pin_device[PIN_DEVICE_MAX];

/**
 * Initialize PIN driver.
 */
int pin_module_init(void);

/**
 * Initialize driver object from given config.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] mode Pin mode.
 *
 * @return zero(0) or negative error code.
 */
int pin_init(struct pin_driver_t *self_p,
             struct pin_device_t *dev_p,
             int mode);

/**
 * Write value to given pin.
 *
 * @param[in] self_p Driver object.
 * @param[in] value Value to vrite
 *
 * @return zero(0) or negative error code.
 */
int pin_write(struct pin_driver_t *self_p, int value);

/**
 * Read value on pin.
 *
 * @param[in] self_p Driver object.
 *
 * @return Value or negative error code.
 */
int pin_read(struct pin_driver_t *self_p);

/**
 * Toggle pin state.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int pin_toggle(struct pin_driver_t *self_p);

/**
 * Set pin mode.
 *
 * @param[in] self_p Driver object.
 * @param[in] mode New pin mode.
 *
 * @return zero(0) or negative error code.
 */
int pin_set_mode(struct pin_driver_t *self_p, int mode);

#endif
