/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Google Inc.
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

#ifndef __LED_7SEG_HT16K33_MOCK_H__
#define __LED_7SEG_HT16K33_MOCK_H__

#include "simba.h"

int mock_write_led_7seg_ht16k33_module_init(int res);

int mock_write_led_7seg_ht16k33_init(struct i2c_soft_driver_t *i2c_p,
                                     int i2c_addr,
                                     int res);

int mock_write_led_7seg_ht16k33_start(int res);

int mock_write_led_7seg_ht16k33_display(int res);

int mock_write_led_7seg_ht16k33_clear(int res);

int mock_write_led_7seg_ht16k33_brightness(int brightness,
                                           int res);

int mock_write_led_7seg_ht16k33_set_num(int num,
                                        int base,
                                        int res);

int mock_write_led_7seg_ht16k33_show_colon(int show_colon,
                                           int res);

int mock_write_led_7seg_ht16k33_show_dot(int position,
                                         int show_colon,
                                         int res);

#endif
