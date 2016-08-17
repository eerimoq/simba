/**
 * @file drivers/analog_output_pin.h
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

#ifndef __DRIVERS_ANALOG_OUTPUT_PIN_H__
#define __DRIVERS_ANALOG_OUTPUT_PIN_H__

#include "simba.h"

struct analog_output_pin_t {
    struct pwm_driver_t pwm;
};

/**
 * Initialize the analog output pin module. This function must be
 * called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int analog_output_pin_module_init(void);

/**
 * Initialize given driver object with given device and mode.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 *
 * @return zero(0) or negative error code.
 */
int analog_output_pin_init(struct analog_output_pin_t *self_p,
                           struct pin_device_t *dev_p);

/**
 * Write given value to the analog pin.
 *
 * @param[in] self_p Driver object.
 * @param[in] value The value to write to the pin. A number in the
 *                  range 0 to 1023, where 0 is lowest output and 1023
 *                  is highest output.
 *
 * @return zero(0) or negative error code.
 */
int analog_output_pin_write(struct analog_output_pin_t *self_p,
                            int value);

/**
 * Read the value that is currently written to given analog output
 * pin.
 *
 * @param[in] self_p Driver object.
 *
 * @return Value in the range 0 to 1023, or negative error code.
 */
int analog_output_pin_read(struct analog_output_pin_t *self_p);

#endif
