/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
