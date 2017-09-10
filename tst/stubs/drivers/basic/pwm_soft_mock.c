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
#include "pwm_soft_mock.h"

int mock_write_pwm_soft_module_init(long frequency,
                                    int res)
{
    harness_mock_write("pwm_soft_module_init(frequency)",
                       &frequency,
                       sizeof(frequency));

    harness_mock_write("pwm_soft_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_soft_module_init)(long frequency)
{
    int res;

    harness_mock_assert("pwm_soft_module_init(frequency)",
                        &frequency);

    harness_mock_read("pwm_soft_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_set_frequency(long value,
                                      int res)
{
    harness_mock_write("pwm_soft_set_frequency(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pwm_soft_set_frequency(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_soft_set_frequency)(long value)
{
    int res;

    harness_mock_assert("pwm_soft_set_frequency(value)",
                        &value);

    harness_mock_read("pwm_soft_set_frequency(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_get_frequency(long res)
{
    harness_mock_write("pwm_soft_get_frequency(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

long __attribute__ ((weak)) STUB(pwm_soft_get_frequency)()
{
    long res;

    harness_mock_read("pwm_soft_get_frequency(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_init(struct pin_device_t *pin_dev_p,
                             long duty_cycle,
                             int res)
{
    harness_mock_write("pwm_soft_init(pin_dev_p)",
                       &pin_dev_p,
                       sizeof(pin_dev_p));

    harness_mock_write("pwm_soft_init(duty_cycle)",
                       &duty_cycle,
                       sizeof(duty_cycle));

    harness_mock_write("pwm_soft_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_soft_init)(struct pwm_soft_driver_t *self_p,
                                               struct pin_device_t *pin_dev_p,
                                               long duty_cycle)
{
    int res;

    harness_mock_assert("pwm_soft_init(pin_dev_p)",
                        &pin_dev_p);

    harness_mock_assert("pwm_soft_init(duty_cycle)",
                        &duty_cycle);

    harness_mock_read("pwm_soft_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_start(int res)
{
    harness_mock_write("pwm_soft_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_soft_start)(struct pwm_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("pwm_soft_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_stop(int res)
{
    harness_mock_write("pwm_soft_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_soft_stop)(struct pwm_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("pwm_soft_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_set_duty_cycle(long value,
                                       int res)
{
    harness_mock_write("pwm_soft_set_duty_cycle(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pwm_soft_set_duty_cycle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_soft_set_duty_cycle)(struct pwm_soft_driver_t *self_p,
                                                         long value)
{
    int res;

    harness_mock_assert("pwm_soft_set_duty_cycle(value)",
                        &value);

    harness_mock_read("pwm_soft_set_duty_cycle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_get_duty_cycle(unsigned int res)
{
    harness_mock_write("pwm_soft_get_duty_cycle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

unsigned int __attribute__ ((weak)) STUB(pwm_soft_get_duty_cycle)(struct pwm_soft_driver_t *self_p)
{
    unsigned int res;

    harness_mock_read("pwm_soft_get_duty_cycle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_duty_cycle(int percentage,
                                   long res)
{
    harness_mock_write("pwm_soft_duty_cycle(percentage)",
                       &percentage,
                       sizeof(percentage));

    harness_mock_write("pwm_soft_duty_cycle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

long __attribute__ ((weak)) STUB(pwm_soft_duty_cycle)(int percentage)
{
    long res;

    harness_mock_assert("pwm_soft_duty_cycle(percentage)",
                        &percentage);

    harness_mock_read("pwm_soft_duty_cycle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pwm_soft_duty_cycle_as_percent(long value,
                                              int res)
{
    harness_mock_write("pwm_soft_duty_cycle_as_percent(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pwm_soft_duty_cycle_as_percent(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pwm_soft_duty_cycle_as_percent)(long value)
{
    int res;

    harness_mock_assert("pwm_soft_duty_cycle_as_percent(value)",
                        &value);

    harness_mock_read("pwm_soft_duty_cycle_as_percent(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
