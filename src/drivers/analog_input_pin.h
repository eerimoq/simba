/**
 * @file drivers/analog_input_pin.h
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

#ifndef __DRIVERS_ANALOG_INPUT_PIN_H__
#define __DRIVERS_ANALOG_INPUT_PIN_H__

#include "simba.h"

struct analog_input_pin_t {
    struct adc_driver_t adc;    
};

/**
 * Initialize the analog input pin module. This function must be
 * called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int analog_input_pin_module_init(void);

/**
 * Initialize given driver object with given device and mode.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 *
 * @return zero(0) or negative error code.
 */
int analog_input_pin_init(struct analog_input_pin_t *self_p,
                          struct pin_device_t *dev_p);

/**
 * Read the current value of given pin.
 *
 * @param[in] self_p Driver object.
 *
 * @return Analog pin value, otherwise negative error code.
 */
int analog_input_pin_read(struct analog_input_pin_t *self_p);

/**
 * Read the current value of given pin from an isr or with the system
 * lock taken.
 *
 * @param[in] self_p Driver object.
 *
 * @return Analog pin value, otherwise negative error code.
 */
int analog_input_pin_read_isr(struct analog_input_pin_t *self_p);

#endif
