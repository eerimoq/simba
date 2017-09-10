/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

#ifndef __HX711_MOCK_H__
#define __HX711_MOCK_H__

#include "simba.h"

int mock_write_hx711_module_init(int res);

int mock_write_hx711_init(struct pin_device_t *pd_sck_p,
                          struct pin_device_t *dout_p,
                          float scale,
                          float offset,
                          int res);

int mock_write_hx711_start(int res);

int mock_write_hx711_stop(int res);

int mock_write_hx711_read(float *weight_p,
                          enum hx711_channel_gain_t channel_gain,
                          int res);

int mock_write_hx711_read_raw(int32_t *sample_p,
                              enum hx711_channel_gain_t channel_gain,
                              int res);

int mock_write_hx711_set_scale(float scale,
                               int res);

int mock_write_hx711_set_offset(float offset,
                                int res);

#endif
