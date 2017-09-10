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

#ifndef __TIME_MOCK_H__
#define __TIME_MOCK_H__

#include "simba.h"

int mock_write_time_get(struct time_t *now_p,
                        int res);

int mock_write_time_set(struct time_t *new_p,
                        int res);

int mock_write_time_add(struct time_t *res_p,
                        struct time_t *left_p,
                        struct time_t *right_p,
                        int res);

int mock_write_time_subtract(struct time_t *res_p,
                             struct time_t *left_p,
                             struct time_t *right_p,
                             int res);

int mock_write_time_compare(struct time_t *left_p,
                            struct time_t *right_p,
                            enum time_compare_t res);

int mock_write_time_unix_time_to_date(struct date_t *date_p,
                                      struct time_t *time_p,
                                      int res);

int mock_write_time_busy_wait_us(int microseconds);

int mock_write_time_micros(int res);

int mock_write_time_micros_elapsed(int start,
                                   int stop,
                                   int res);

int mock_write_time_micros_resolution(int res);

int mock_write_time_micros_maximum(int res);

#endif
