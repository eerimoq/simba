/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __DRIVERS_PWM_H__
#define __DRIVERS_PWM_H__

#include "simba.h"

#include "pwm_port.h"

extern struct pwm_device_t pwm_device[PWM_DEVICE_MAX];

/**
 * Initialize the pwm module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int pwm_module_init(void);

/**
 * Initialize given PWM driver object.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p PWM device to use.
 *
 * @return zero(0) or negative error code.
 */
int pwm_init(struct pwm_driver_t *self_p,
             struct pwm_device_t *dev_p);

/**
 * Set the duty cycle.
 *
 * @param[in] self_p Driver object.
 * @param[in] value Duty cycle. Use `pwm_duty_cycle()` to convert a
 *                  duty cycle percentage to a value expected by this
 *                  function.
 *
 * @return zero(0) or negative error code.
 */
int pwm_set_duty_cycle(struct pwm_driver_t *self_p,
                       int value);

/**
 * Get current duty cycle.
 *
 * @param[in] self_p Driver object.
 *
 * @return Current duty cycle.
 */
int pwm_get_duty_cycle(struct pwm_driver_t *self_p);

/**
 * Convert a duty cycle percentage to a value for `pwm_init()` and
 * `pwm_set_duty_cycle()`.
 *
 * @param[in] percentage Duty cycle percentage.
 *
 * @return Duty cycle.
 */
int pwm_duty_cycle(int percentage);

/**
 * Convert a duty cycle value for `pwm_init()` and
 * `pwm_set_duty_cycle()` to a percentage.
 *
 * @param[in] value Duty cycle.
 *
 * @return Duty cycle percentage.
 */
int pwm_duty_cycle_as_percent(int value);

/**
 * Get the PWM device for given pin.
 *
 * @param[in] pin_p The pin device to get the pwm device for.
 *
 * @return PWM device, or NULL on error.
 */
struct pwm_device_t *pwm_pin_to_device(struct pin_device_t *pin_p);

#endif
