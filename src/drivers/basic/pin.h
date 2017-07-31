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

#ifndef __DRIVERS_PIN_H__
#define __DRIVERS_PIN_H__

#include "simba.h"

/**
 * Configure the pin as an output pin.
 */
#define PIN_OUTPUT                                          0

/**
 * Configure the pin as an output open drain pin.
 */
#define PIN_OUTPUT_OPEN_DRAIN                               1

/**
 * Configure the pin as an output open drain pin with the internal
 * pull-up resistor enabled.
 */
#define PIN_OUTPUT_OPEN_DRAIN_PULL_UP                       2

/**
 * Configure the pin as an input pin.
 */
#define PIN_INPUT                                           3

/**
 * Configure the pin as an input pin with the internal pull-up
 * resistor enabled.
 */
#define PIN_INPUT_PULL_UP                                   4

/**
 * Configure the pin as an input pin with the internal pull-down
 * resistor enabled.
 */
#define PIN_INPUT_PULL_DOWN                                 5

#include "pin_port.h"

extern struct pin_device_t pin_device[PIN_DEVICE_MAX];

/**
 * Initialize the pin module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int pin_module_init(void);

/**
 * Initialize given driver object with given device and mode.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] mode Pin mode. One of the ``PIN_OUTPUT*`` and
 *                 ``PIN_INPUT*`` defines.
 *
 * @return zero(0) or negative error code.
 */
int pin_init(struct pin_driver_t *self_p,
             struct pin_device_t *dev_p,
             int mode);

/**
 * Write given value to given pin.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Driver object.
 * @param[in] value Non-zero for high and ``0`` for low output.
 *
 * @return zero(0) or negative error code.
 */
int pin_write(struct pin_driver_t *self_p, int value);

/**
 * Read the current value of given pin.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Driver object.
 *
 * @return ``1`` for high and ``0`` for low input, otherwise negative
 *         error code.
 */
int pin_read(struct pin_driver_t *self_p);

/**
 * Toggle the pin output value (high/low).
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int pin_toggle(struct pin_driver_t *self_p);

/**
 * Set the pin mode of given pin.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Driver object.
 * @param[in] mode Pin mode. One of the ``PIN_OUTPUT*`` and
 *                 ``PIN_INPUT*`` defines.
 *
 * @return zero(0) or negative error code.
 */
int pin_set_mode(struct pin_driver_t *self_p, int mode);

/**
 * Pin device mode to set.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Pin device.
 * @param[in] mode Pin mode. One of the ``PIN_OUTPUT*`` and
 *                 ``PIN_INPUT*`` defines.
 *
 * @return zero(0) or negative error code.
 */
static inline int pin_device_set_mode(const struct pin_device_t *dev_p,
                                      int mode)
{
    return (pin_port_device_set_mode(dev_p, mode));
}

/**
 * Read the value of given pin device.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Pin device.
 *
 * @return ``1`` for high and ``0`` for low input, otherwise negative
 *         error code.
 */
static inline int pin_device_read(const struct pin_device_t *dev_p)
{
    return (pin_port_device_read(dev_p));
}

/**
 * Write given value to given pin device.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Pin device.
 * @param[in] value Non-zero for high and ``0`` for low output.
 *
 * @return zero(0) or negative error code.
 */
int pin_device_write(const struct pin_device_t *dev_p,
                     int value);

/**
 * Write high to given pin device.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Pin device.
 *
 * @return zero(0) or negative error code.
 */
static inline int pin_device_write_high(const struct pin_device_t *dev_p)
{
    return (pin_port_device_write_high(dev_p));
}

/**
 * Write low to given pin device.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] self_p Pin device.
 *
 * @return zero(0) or negative error code.
 */
static inline int pin_device_write_low(const struct pin_device_t *dev_p)
{
    return (pin_port_device_write_low(dev_p));
}

/**
 * Check if given pin device is valid.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] dev_p Pin device to validate.
 *
 * @return true(1) if the pin device is valid, otherwise false(0).
 */
int pin_is_valid_device(struct pin_device_t *dev_p);

#endif
