/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "pwm_mock.h"

int mock_write_pwm_module_init(int res)
{
    harness_mock_write("pwm_module_init()",
                       NULL,
                       0);

    harness_mock_write("pwm_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_module_init)()
{
    int res;

    harness_mock_assert("pwm_module_init()",
                        NULL,
                        0);

    harness_mock_read("pwm_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_init(struct pwm_device_t *dev_p,
                        long frequency,
                        long duty_cycle,
                        int res)
{
    harness_mock_write("pwm_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("pwm_init(frequency)",
                       &frequency,
                       sizeof(frequency));

    harness_mock_write("pwm_init(duty_cycle)",
                       &duty_cycle,
                       sizeof(duty_cycle));

    harness_mock_write("pwm_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_init)(struct pwm_driver_t *self_p,
                                          struct pwm_device_t *dev_p,
                                          long frequency,
                                          long duty_cycle)
{
    int res;

    harness_mock_assert("pwm_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("pwm_init(frequency)",
                        &frequency,
                        sizeof(frequency));

    harness_mock_assert("pwm_init(duty_cycle)",
                        &duty_cycle,
                        sizeof(duty_cycle));

    harness_mock_read("pwm_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_start(int res)
{
    harness_mock_write("pwm_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_start)(struct pwm_driver_t *self_p)
{
    int res;

    harness_mock_read("pwm_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_stop(int res)
{
    harness_mock_write("pwm_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_stop)(struct pwm_driver_t *self_p)
{
    int res;

    harness_mock_read("pwm_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_set_frequency(long value,
                                 int res)
{
    harness_mock_write("pwm_set_frequency(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pwm_set_frequency(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_set_frequency)(struct pwm_driver_t *self_p,
                                                   long value)
{
    int res;

    harness_mock_assert("pwm_set_frequency(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("pwm_set_frequency(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_get_frequency(long res)
{
    harness_mock_write("pwm_get_frequency(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

long __attribute__ ((weak)) STUB(pwm_get_frequency)(struct pwm_driver_t *self_p)
{
    long res;

    harness_mock_read("pwm_get_frequency(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_set_duty_cycle(long value,
                                  int res)
{
    harness_mock_write("pwm_set_duty_cycle(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pwm_set_duty_cycle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_set_duty_cycle)(struct pwm_driver_t *self_p,
                                                    long value)
{
    int res;

    harness_mock_assert("pwm_set_duty_cycle(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("pwm_set_duty_cycle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_get_duty_cycle(long res)
{
    harness_mock_write("pwm_get_duty_cycle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

long __attribute__ ((weak)) STUB(pwm_get_duty_cycle)(struct pwm_driver_t *self_p)
{
    long res;

    harness_mock_read("pwm_get_duty_cycle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_frequency(int hertz,
                             long res)
{
    harness_mock_write("pwm_frequency(hertz)",
                       &hertz,
                       sizeof(hertz));

    harness_mock_write("pwm_frequency(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

long __attribute__ ((weak)) STUB(pwm_frequency)(int hertz)
{
    long res;

    harness_mock_assert("pwm_frequency(hertz)",
                        &hertz,
                        sizeof(hertz));

    harness_mock_read("pwm_frequency(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_frequency_as_hertz(long value,
                                      int res)
{
    harness_mock_write("pwm_frequency_as_hertz(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pwm_frequency_as_hertz(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_frequency_as_hertz)(long value)
{
    int res;

    harness_mock_assert("pwm_frequency_as_hertz(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("pwm_frequency_as_hertz(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_duty_cycle(int percentage,
                              long res)
{
    harness_mock_write("pwm_duty_cycle(percentage)",
                       &percentage,
                       sizeof(percentage));

    harness_mock_write("pwm_duty_cycle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

long __attribute__ ((weak)) STUB(pwm_duty_cycle)(int percentage)
{
    long res;

    harness_mock_assert("pwm_duty_cycle(percentage)",
                        &percentage,
                        sizeof(percentage));

    harness_mock_read("pwm_duty_cycle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_duty_cycle_as_percent(long value,
                                         int res)
{
    harness_mock_write("pwm_duty_cycle_as_percent(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pwm_duty_cycle_as_percent(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_duty_cycle_as_percent)(long value)
{
    int res;

    harness_mock_assert("pwm_duty_cycle_as_percent(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("pwm_duty_cycle_as_percent(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_pin_to_device(struct pin_device_t *pin_p,
                                 struct pwm_device_t *res)
{
    harness_mock_write("pwm_pin_to_device(pin_p)",
                       pin_p,
                       sizeof(*pin_p));

    harness_mock_write("pwm_pin_to_device(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct pwm_device_t *__attribute__ ((weak)) STUB(pwm_pin_to_device)(struct pin_device_t *pin_p)
{
    struct pwm_device_t *res;

    harness_mock_assert("pwm_pin_to_device(pin_p)",
                        pin_p,
                        sizeof(*pin_p));

    harness_mock_read("pwm_pin_to_device(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
