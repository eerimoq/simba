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

static int pwm_port_init(struct pwm_driver_t *self_p,
                         const struct pwm_device_t *dev_p)
{
    return (0);
}

static int pwm_port_set_duty_cycle(struct pwm_driver_t *self_p,
                                   int value)
{
    return (0);
}

static int pwm_port_get_duty_cycle(struct pwm_driver_t *self_p)
{
    return (-1);
}

static int pwm_port_duty_cycle(int percentage)
{
    return (-1);
}

static int pwm_port_duty_cycle_as_percent(int value)
{
    return (-1);
}

static struct pwm_device_t *pwm_port_pin_to_device(struct pin_device_t *pin_p)
{
    return (NULL);
}
