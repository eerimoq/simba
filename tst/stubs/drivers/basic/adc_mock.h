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

#ifndef __ADC_MOCK_H__
#define __ADC_MOCK_H__

#include "simba.h"

int mock_write_adc_module_init(int res);

int mock_write_adc_init(struct adc_device_t *dev_p,
                        struct pin_device_t *pin_dev_p,
                        int reference,
                        long sampling_rate,
                        int res);

int mock_write_adc_async_convert(uint16_t *samples_p,
                                 size_t length,
                                 int res);

int mock_write_adc_async_wait(int res);

int mock_write_adc_convert(uint16_t *samples_p,
                           size_t length,
                           int res);

int mock_write_adc_convert_isr(uint16_t *sample_p,
                               int res);

int mock_write_adc_is_valid_device(struct adc_device_t *dev_p,
                                   int res);

#endif
