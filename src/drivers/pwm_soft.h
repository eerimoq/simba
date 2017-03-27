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

#ifndef __DRIVERS_PWM_SOFT_H__
#define __DRIVERS_PWM_SOFT_H__

#include "simba.h"

#include "pwm_soft_port.h"

struct pwm_soft_driver_t {
    struct pin_device_t *pin_dev_p;
    long frequency;
    long duty_cycle;
    unsigned int delta;
    struct thrd_t *thrd_p;
    struct pwm_soft_driver_t *next_p;
};

/**
 * Initialize the software PWM module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @param[in] frequency PWM module frequency in Hertz. All software
 *                      PWM:s will run at this frequency. The
 *                      frequency can later be changed by calling
 *                      `pwm_soft_set_frequency()`.
 *
 * @return zero(0) or negative error code.
 */
int pwm_soft_module_init(long frequency);

/**
 * Set the frequency. The frequency is the same for all software
 * PWM:s. All software PWM:s must be stopped before calling this
 * function, otherwize a negative error code will be returned.
 *
 * @param[in] value Frequency to set in Hertz. All software PWM:s will
 *                  run at this frequency.
 *
 * @return zero(0) or negative error code.
 */
int pwm_soft_set_frequency(long value);

/**
 * Get current frequency.
 *
 * @return Current frequency in Hertz.
 */
long pwm_soft_get_frequency(void);

/**
 * Initialize given software PWM driver object.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] pin_dev_p Pin device to use.
 * @param[in] duty_cycle Initial duty cycle.
 *
 * @return zero(0) or negative error code.
 */
int pwm_soft_init(struct pwm_soft_driver_t *self_p,
                  struct pin_device_t *pin_dev_p,
                  long duty_cycle);

/**
 * Start outputting the PWM signal on the pin given to
 * `pwm_soft_init()`.
 *
 * @param[in] self_p Driver object to start.
 *
 * @return zero(0) or negative error code.
 */
int pwm_soft_start(struct pwm_soft_driver_t *self_p);

/**
 * Stop outputting the PWM signal on the pin given to
 * `pwm_soft_init()`.
 *
 * @param[in] self_p Driver object to stop.
 *
 * @return zero(0) or negative error code.
 */
int pwm_soft_stop(struct pwm_soft_driver_t *self_p);

/**
 * Set the duty cycle. Calls `pwm_soft_stop()` and `pwm_soft_start()`
 * to restart outputting the PWM signal with the new duty cycle.
 *
 * @param[in] self_p Driver object.
 * @param[in] value Duty cycle. Use `pwm_soft_duty_cycle()` to convert
 *                  a duty cycle percentage to a value expected by
 *                  this function.
 *
 * @return zero(0) or negative error code.
 */
int pwm_soft_set_duty_cycle(struct pwm_soft_driver_t *self_p,
                            long value);

/**
 * Get current duty cycle. Use `pwm_soft_duty_cycle_as_percent()` to
 * convert a duty cycle to a percentage.
 *
 * @param[in] self_p Driver object.
 *
 * @return Current duty cycle.
 */
unsigned int pwm_soft_get_duty_cycle(struct pwm_soft_driver_t *self_p);

/**
 * Convert a duty cycle percentage to a value for `pwm_soft_init()`
 * and `pwm_soft_set_duty_cycle()`.
 *
 * @param[in] percentage Duty cycle percentage.
 *
 * @return Duty cycle.
 */
long pwm_soft_duty_cycle(int percentage);

/**
 * Convert a duty cycle value for `pwm_soft_init()` and
 * `pwm_soft_set_duty_cycle()` to a percentage.
 *
 * @param[in] value Duty cycle.
 *
 * @return Duty cycle percentage.
 */
int pwm_soft_duty_cycle_as_percent(long value);

#endif
