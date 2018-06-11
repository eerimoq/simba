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

#ifndef __I2C_SOFT_MOCK_H__
#define __I2C_SOFT_MOCK_H__

#include "simba.h"

int mock_write_i2c_soft_module_init(int res);

int mock_write_i2c_soft_init(struct pin_device_t *scl_dev_p,
                             struct pin_device_t *sda_dev_p,
                             long baudrate,
                             long max_clock_stretching_us,
                             long clock_stretching_sleep_us,
                             int res);

int mock_write_i2c_soft_start(int res);

int mock_write_i2c_soft_stop(int res);

int mock_write_i2c_soft_read(int address,
                             void *buf_p,
                             size_t size,
                             ssize_t res);

int mock_write_i2c_soft_write(int address,
                              const void *buf_p,
                              size_t size,
                              ssize_t res);

int mock_write_i2c_soft_scan(int address,
                             int res);

#endif
