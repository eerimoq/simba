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

#include "simba.h"

#if CONFIG_ANALOG_OUTPUT_PIN == 1

int analog_output_pin_module_init(void)
{
    return (0);
}

int analog_output_pin_init(struct analog_output_pin_t *self_p,
                           struct pin_device_t *pin_p)
{
    struct pwm_device_t *pwm_p;
    
    pwm_p = pwm_pin_to_device(pin_p);

    if (pwm_p == NULL) {
        return (-1);
    }
    
    return (pwm_init(&self_p->pwm, pwm_p));
}

int analog_output_pin_write(struct analog_output_pin_t *self_p,
                            int value)
{
    return (pwm_set_duty_cycle(&self_p->pwm, value / 4));
}

int analog_output_pin_read(struct analog_output_pin_t *self_p)
{
    return (pwm_get_duty_cycle(&self_p->pwm) * 4);
}

#endif
