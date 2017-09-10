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

#ifndef __NMEA_MOCK_H__
#define __NMEA_MOCK_H__

#include "simba.h"

int mock_write_nmea_encode(char *dst_p,
                           struct nmea_sentence_t *src_p,
                           ssize_t res);

int mock_write_nmea_decode(struct nmea_sentence_t *dst_p,
                           char *src_p,
                           size_t size,
                           ssize_t res);

int mock_write_nmea_decode_fix_time(char *src_p,
                                    int *hour_p,
                                    int *minute_p,
                                    int *second_p,
                                    int res);

int mock_write_nmea_decode_date(char *src_p,
                                int *year_p,
                                int *month_p,
                                int *date_p,
                                int res);

int mock_write_nmea_decode_position(struct nmea_position_t *src_p,
                                    long *degrees_p,
                                    int res);

#endif
