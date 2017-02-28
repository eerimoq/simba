/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
