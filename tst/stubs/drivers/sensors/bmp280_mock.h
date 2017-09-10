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

#ifndef __BMP280_MOCK_H__
#define __BMP280_MOCK_H__

#include "simba.h"

int mock_write_bmp280_module_init(int res);

int mock_write_bmp280_init(struct bmp280_transport_t *transport_p,
                           enum bmp280_mode_t mode,
                           enum bmp280_standby_time_t standby_time,
                           enum bmp280_filter_t filter,
                           enum bmp280_temperature_oversampling_t temperature_oversampling,
                           enum bmp280_pressure_oversampling_t pressure_oversampling,
                           int res);

int mock_write_bmp280_start(int res);

int mock_write_bmp280_stop(int res);

int mock_write_bmp280_read(float *temperature_p,
                           float *pressure_p,
                           int res);

int mock_write_bmp280_read_fixed_point(long *temperature_p,
                                       long *pressure_p,
                                       int res);

int mock_write_bmp280_transport_i2c_init(struct i2c_driver_t *i2c_p,
                                         int i2c_address,
                                         int res);

int mock_write_bmp280_transport_spi_init(struct spi_driver_t *spi_p,
                                         int res);

#endif
