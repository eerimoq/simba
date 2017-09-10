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

#ifndef __SCIENCE_MOCK_H__
#define __SCIENCE_MOCK_H__

#include "simba.h"

int mock_write_science_module_init(int res);

int mock_write_science_pressure_to_altitude(float pressure,
                                            float pressure_at_sea_level,
                                            float res);

int mock_write_science_pressure_from_altitude(float altitude,
                                              float pressure_at_sea_level,
                                              float res);

int mock_write_science_mps_to_kmph(float speed,
                                   float res);

int mock_write_science_mps_from_kmph(float speed,
                                     float res);

int mock_write_science_mps_to_knots(float speed,
                                    float res);

int mock_write_science_mps_from_knots(float speed,
                                      float res);

int mock_write_science_mps_to_mph(float speed,
                                  float res);

int mock_write_science_mps_from_mph(float speed,
                                    float res);

#endif
