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
    
    return (pwm_init(&self_p->pwm,
                     pwm_p,
                     pwm_frequency(10000),
                     pwm_duty_cycle(0)));
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
