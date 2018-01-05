/**
 * @section License
 *
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

#ifndef __PWM_MOCK_H__
#define __PWM_MOCK_H__

#include "simba.h"

int mock_write_pwm_module_init(int res);

int mock_write_pwm_init(struct pwm_device_t *dev_p,
                        long frequency,
                        long duty_cycle,
                        int res);

int mock_write_pwm_start(int res);

int mock_write_pwm_stop(int res);

int mock_write_pwm_set_frequency(long value,
                                 int res);

int mock_write_pwm_get_frequency(long res);

int mock_write_pwm_set_duty_cycle(long value,
                                  int res);

int mock_write_pwm_get_duty_cycle(long res);

int mock_write_pwm_frequency(int hertz,
                             long res);

int mock_write_pwm_frequency_as_hertz(long value,
                                      int res);

int mock_write_pwm_duty_cycle(int percentage,
                              long res);

int mock_write_pwm_duty_cycle_as_percent(long value,
                                         int res);

int mock_write_pwm_pin_to_device(struct pin_device_t *pin_p,
                                 struct pwm_device_t *res);

#endif
