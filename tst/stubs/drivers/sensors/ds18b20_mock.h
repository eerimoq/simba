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

#ifndef __DS18B20_MOCK_H__
#define __DS18B20_MOCK_H__

#include "simba.h"

int mock_write_ds18b20_module_init(int res);

int mock_write_ds18b20_init(struct owi_driver_t *owi_p,
                            int res);

int mock_write_ds18b20_convert(int res);

int mock_write_ds18b20_read(const uint8_t *id_p,
                            float *temperature_p,
                            int res);

int mock_write_ds18b20_read_fixed_point(const uint8_t *id_p,
                                        int *temperature_p,
                                        int res);

int mock_write_ds18b20_read_string(const uint8_t *id_p,
                                   char *temperature_p,
                                   char *res);

int mock_write_ds18b20_get_temperature(const uint8_t *id_p,
                                       int *temperature_p,
                                       int res);

int mock_write_ds18b20_get_temperature_str(const uint8_t *id_p,
                                           char *temperature_p,
                                           char *res);

#endif
