/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

#include "simba.h"
#include "nmea_mock.h"

int mock_write_nmea_encode(char *dst_p,
                           struct nmea_sentence_t *src_p,
                           ssize_t res)
{
    harness_mock_write("nmea_encode(): return (dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("nmea_encode(src_p)",
                       src_p,
                       sizeof(*src_p));

    harness_mock_write("nmea_encode(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(nmea_encode)(char *dst_p,
                                                 struct nmea_sentence_t *src_p)
{
    ssize_t res;

    harness_mock_read("nmea_encode(): return (dst_p)",
                      dst_p,
                      sizeof(*dst_p));

    harness_mock_assert("nmea_encode(src_p)",
                        src_p,
                        sizeof(*src_p));

    harness_mock_read("nmea_encode(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nmea_decode(struct nmea_sentence_t *dst_p,
                           char *src_p,
                           size_t size,
                           ssize_t res)
{
    harness_mock_write("nmea_decode(): return (dst_p)",
                       dst_p,
                       sizeof(*dst_p));

    harness_mock_write("nmea_decode(): return (src_p)",
                       src_p,
                       strlen(src_p) + 1);

    harness_mock_write("nmea_decode(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("nmea_decode(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(nmea_decode)(struct nmea_sentence_t *dst_p,
                                                 char *src_p,
                                                 size_t size)
{
    ssize_t res;

    harness_mock_read("nmea_decode(): return (dst_p)",
                      dst_p,
                      sizeof(*dst_p));

    harness_mock_read("nmea_decode(): return (src_p)",
                      src_p,
                      sizeof(*src_p));

    harness_mock_assert("nmea_decode(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("nmea_decode(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nmea_decode_fix_time(char *src_p,
                                    int *hour_p,
                                    int *minute_p,
                                    int *second_p,
                                    int res)
{
    harness_mock_write("nmea_decode_fix_time(src_p)",
                       src_p,
                       strlen(src_p) + 1);

    harness_mock_write("nmea_decode_fix_time(): return (hour_p)",
                       hour_p,
                       sizeof(*hour_p));

    harness_mock_write("nmea_decode_fix_time(): return (minute_p)",
                       minute_p,
                       sizeof(*minute_p));

    harness_mock_write("nmea_decode_fix_time(): return (second_p)",
                       second_p,
                       sizeof(*second_p));

    harness_mock_write("nmea_decode_fix_time(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nmea_decode_fix_time)(char *src_p,
                                                      int *hour_p,
                                                      int *minute_p,
                                                      int *second_p)
{
    int res;

    harness_mock_assert("nmea_decode_fix_time(src_p)",
                        src_p,
                        sizeof(*src_p));

    harness_mock_read("nmea_decode_fix_time(): return (hour_p)",
                      hour_p,
                      sizeof(*hour_p));

    harness_mock_read("nmea_decode_fix_time(): return (minute_p)",
                      minute_p,
                      sizeof(*minute_p));

    harness_mock_read("nmea_decode_fix_time(): return (second_p)",
                      second_p,
                      sizeof(*second_p));

    harness_mock_read("nmea_decode_fix_time(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nmea_decode_date(char *src_p,
                                int *year_p,
                                int *month_p,
                                int *date_p,
                                int res)
{
    harness_mock_write("nmea_decode_date(src_p)",
                       src_p,
                       strlen(src_p) + 1);

    harness_mock_write("nmea_decode_date(): return (year_p)",
                       year_p,
                       sizeof(*year_p));

    harness_mock_write("nmea_decode_date(): return (month_p)",
                       month_p,
                       sizeof(*month_p));

    harness_mock_write("nmea_decode_date(): return (date_p)",
                       date_p,
                       sizeof(*date_p));

    harness_mock_write("nmea_decode_date(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nmea_decode_date)(char *src_p,
                                                  int *year_p,
                                                  int *month_p,
                                                  int *date_p)
{
    int res;

    harness_mock_assert("nmea_decode_date(src_p)",
                        src_p,
                        sizeof(*src_p));

    harness_mock_read("nmea_decode_date(): return (year_p)",
                      year_p,
                      sizeof(*year_p));

    harness_mock_read("nmea_decode_date(): return (month_p)",
                      month_p,
                      sizeof(*month_p));

    harness_mock_read("nmea_decode_date(): return (date_p)",
                      date_p,
                      sizeof(*date_p));

    harness_mock_read("nmea_decode_date(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nmea_decode_position(struct nmea_position_t *src_p,
                                    long *degrees_p,
                                    int res)
{
    harness_mock_write("nmea_decode_position(src_p)",
                       src_p,
                       sizeof(*src_p));

    harness_mock_write("nmea_decode_position(): return (degrees_p)",
                       degrees_p,
                       sizeof(*degrees_p));

    harness_mock_write("nmea_decode_position(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nmea_decode_position)(struct nmea_position_t *src_p,
                                                      long *degrees_p)
{
    int res;

    harness_mock_assert("nmea_decode_position(src_p)",
                        src_p,
                        sizeof(*src_p));

    harness_mock_read("nmea_decode_position(): return (degrees_p)",
                      degrees_p,
                      sizeof(*degrees_p));

    harness_mock_read("nmea_decode_position(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
