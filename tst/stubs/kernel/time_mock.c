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

#include "simba.h"
#include "time_mock.h"

int mock_write_time_get(struct time_t *now_p,
                        int res)
{
    harness_mock_write("time_get(): return (now_p)",
                       now_p,
                       sizeof(*now_p));

    harness_mock_write("time_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_get)(struct time_t *now_p)
{
    int res;

    harness_mock_read("time_get(): return (now_p)",
                      now_p,
                      sizeof(*now_p));

    harness_mock_read("time_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_set(struct time_t *new_p,
                        int res)
{
    harness_mock_write("time_set(new_p)",
                       new_p,
                       sizeof(*new_p));

    harness_mock_write("time_set(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_set)(struct time_t *new_p)
{
    int res;

    harness_mock_assert("time_set(new_p)",
                        new_p,
                        sizeof(*new_p));

    harness_mock_read("time_set(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_add(struct time_t *res_p,
                        struct time_t *left_p,
                        struct time_t *right_p,
                        int res)
{
    harness_mock_write("time_add(): return (res_p)",
                       res_p,
                       sizeof(*res_p));

    harness_mock_write("time_add(left_p)",
                       left_p,
                       sizeof(*left_p));

    harness_mock_write("time_add(right_p)",
                       right_p,
                       sizeof(*right_p));

    harness_mock_write("time_add(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_add)(struct time_t *res_p,
                                          struct time_t *left_p,
                                          struct time_t *right_p)
{
    int res;

    harness_mock_read("time_add(): return (res_p)",
                      res_p,
                      sizeof(*res_p));

    harness_mock_assert("time_add(left_p)",
                        left_p,
                        sizeof(*left_p));

    harness_mock_assert("time_add(right_p)",
                        right_p,
                        sizeof(*right_p));

    harness_mock_read("time_add(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_subtract(struct time_t *res_p,
                             struct time_t *left_p,
                             struct time_t *right_p,
                             int res)
{
    harness_mock_write("time_subtract(): return (res_p)",
                       res_p,
                       sizeof(*res_p));

    harness_mock_write("time_subtract(left_p)",
                       left_p,
                       sizeof(*left_p));

    harness_mock_write("time_subtract(right_p)",
                       right_p,
                       sizeof(*right_p));

    harness_mock_write("time_subtract(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_subtract)(struct time_t *res_p,
                                               struct time_t *left_p,
                                               struct time_t *right_p)
{
    int res;

    harness_mock_read("time_subtract(): return (res_p)",
                      res_p,
                      sizeof(*res_p));

    harness_mock_assert("time_subtract(left_p)",
                        left_p,
                        sizeof(*left_p));

    harness_mock_assert("time_subtract(right_p)",
                        right_p,
                        sizeof(*right_p));

    harness_mock_read("time_subtract(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_compare(struct time_t *left_p,
                            struct time_t *right_p,
                            enum time_compare_t res)
{
    harness_mock_write("time_compare(left_p)",
                       left_p,
                       sizeof(*left_p));

    harness_mock_write("time_compare(right_p)",
                       right_p,
                       sizeof(*right_p));

    harness_mock_write("time_compare(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

enum time_compare_t __attribute__ ((weak)) STUB(time_compare)(struct time_t *left_p,
                                                              struct time_t *right_p)
{
    enum time_compare_t res;

    harness_mock_assert("time_compare(left_p)",
                        left_p,
                        sizeof(*left_p));

    harness_mock_assert("time_compare(right_p)",
                        right_p,
                        sizeof(*right_p));

    harness_mock_read("time_compare(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_unix_time_to_date(struct date_t *date_p,
                                      struct time_t *time_p,
                                      int res)
{
    harness_mock_write("time_unix_time_to_date(): return (date_p)",
                       date_p,
                       sizeof(*date_p));

    harness_mock_write("time_unix_time_to_date(time_p)",
                       time_p,
                       sizeof(*time_p));

    harness_mock_write("time_unix_time_to_date(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_unix_time_to_date)(struct date_t *date_p,
                                                        struct time_t *time_p)
{
    int res;

    harness_mock_read("time_unix_time_to_date(): return (date_p)",
                      date_p,
                      sizeof(*date_p));

    harness_mock_assert("time_unix_time_to_date(time_p)",
                        time_p,
                        sizeof(*time_p));

    harness_mock_read("time_unix_time_to_date(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_busy_wait_us(int microseconds)
{
    harness_mock_write("time_busy_wait_us(microseconds)",
                       &microseconds,
                       sizeof(microseconds));

    return (0);
}

void __attribute__ ((weak)) STUB(time_busy_wait_us)(int microseconds)
{
    harness_mock_assert("time_busy_wait_us(microseconds)",
                        &microseconds,
                        sizeof(microseconds));
}

int mock_write_time_micros(int res)
{
    harness_mock_write("time_micros()",
                       NULL,
                       0);

    harness_mock_write("time_micros(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_micros)()
{
    int res;

    harness_mock_assert("time_micros()",
                        NULL,
                        0);

    harness_mock_read("time_micros(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_micros_elapsed(int start,
                                   int stop,
                                   int res)
{
    harness_mock_write("time_micros_elapsed(start)",
                       &start,
                       sizeof(start));

    harness_mock_write("time_micros_elapsed(stop)",
                       &stop,
                       sizeof(stop));

    harness_mock_write("time_micros_elapsed(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_micros_elapsed)(int start,
                                                     int stop)
{
    int res;

    harness_mock_assert("time_micros_elapsed(start)",
                        &start,
                        sizeof(start));

    harness_mock_assert("time_micros_elapsed(stop)",
                        &stop,
                        sizeof(stop));

    harness_mock_read("time_micros_elapsed(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_micros_resolution(int res)
{
    harness_mock_write("time_micros_resolution()",
                       NULL,
                       0);

    harness_mock_write("time_micros_resolution(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_micros_resolution)()
{
    int res;

    harness_mock_assert("time_micros_resolution()",
                        NULL,
                        0);

    harness_mock_read("time_micros_resolution(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_time_micros_maximum(int res)
{
    harness_mock_write("time_micros_maximum()",
                       NULL,
                       0);

    harness_mock_write("time_micros_maximum(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(time_micros_maximum)()
{
    int res;

    harness_mock_assert("time_micros_maximum()",
                        NULL,
                        0);

    harness_mock_read("time_micros_maximum(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
