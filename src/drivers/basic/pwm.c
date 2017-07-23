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

#if CONFIG_PWM == 1

#include "pwm_port.i"

int pwm_module_init()
{
    return (0);
}

int pwm_init(struct pwm_driver_t *self_p,
             struct pwm_device_t *dev_p,
             long frequency,
             long duty_cycle)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    self_p->dev_p = dev_p;
    self_p->frequency = frequency;
    self_p->duty_cycle = duty_cycle;

    return (pwm_port_init(self_p, dev_p));
}

int pwm_start(struct pwm_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    if (pwm_set_frequency(self_p, self_p->frequency) != 0) {
        return (-1);
    }

    return (pwm_set_duty_cycle(self_p, self_p->duty_cycle));
}

int pwm_stop(struct pwm_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (pwm_set_duty_cycle(self_p, 0));
}

int pwm_set_frequency(struct pwm_driver_t *self_p,
                      long value)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->frequency = value;

    return (pwm_port_set_frequency(self_p, value));
}

long pwm_get_frequency(struct pwm_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->frequency);
}

int pwm_set_duty_cycle(struct pwm_driver_t *self_p,
                       long value)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->duty_cycle = value;

    return (pwm_port_set_duty_cycle(self_p, value));
}

long pwm_get_duty_cycle(struct pwm_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->duty_cycle);
}

long pwm_duty_cycle(int percentage)
{
    return (pwm_port_duty_cycle(percentage));
}

int pwm_duty_cycle_as_percent(long value)
{
    return (pwm_port_duty_cycle_as_percent(value));
}

long pwm_frequency(int hertz)
{
    return (pwm_port_frequency(hertz));
}

int pwm_frequency_as_hertz(long value)
{
    return (pwm_port_frequency_as_hertz(value));
}

struct pwm_device_t *pwm_pin_to_device(struct pin_device_t *pin_p)
{
    ASSERTNRN(pin_p != NULL, EINVAL);

    int i;

    for (i = 0; i < PWM_DEVICE_MAX; i++) {
        if (pwm_device[i].pin_dev_p == pin_p) {
            return (&pwm_device[i]);
        }
    }

    return (NULL);
}

#endif
