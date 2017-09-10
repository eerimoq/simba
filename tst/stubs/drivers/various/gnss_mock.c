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

#include "simba.h"
#include "gnss_mock.h"

int mock_write_gnss_module_init(int res)
{
    harness_mock_write("gnss_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_module_init)()
{
    int res;

    harness_mock_read("gnss_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_init(void *chin_p,
                         void *chout_p,
                         int res)
{
    harness_mock_write("gnss_init(chin_p)",
                       chin_p,
                       sizeof(chin_p));

    harness_mock_write("gnss_init(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("gnss_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_init)(struct gnss_driver_t *self_p,
                                           void *chin_p,
                                           void *chout_p)
{
    int res;

    harness_mock_assert("gnss_init(chin_p)",
                        chin_p);

    harness_mock_assert("gnss_init(chout_p)",
                        chout_p);

    harness_mock_read("gnss_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_read(int res)
{
    harness_mock_write("gnss_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_read)(struct gnss_driver_t *self_p)
{
    int res;

    harness_mock_read("gnss_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_write(char *str_p,
                          int res)
{
    harness_mock_write("gnss_write(str_p)",
                       str_p,
                       strlen(str_p) + 1);

    harness_mock_write("gnss_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_write)(struct gnss_driver_t *self_p,
                                            char *str_p)
{
    int res;

    harness_mock_assert("gnss_write(str_p)",
                        str_p);

    harness_mock_read("gnss_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_get_date(struct date_t *date_p,
                             int res)
{
    harness_mock_write("gnss_get_date(): return (date_p)",
                       date_p,
                       sizeof(*date_p));

    harness_mock_write("gnss_get_date(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_get_date)(struct gnss_driver_t *self_p,
                                               struct date_t *date_p)
{
    int res;

    harness_mock_read("gnss_get_date(): return (date_p)",
                      date_p,
                      -1);

    harness_mock_read("gnss_get_date(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_get_position(float *latitude_p,
                                 float *longitude_p,
                                 int res)
{
    harness_mock_write("gnss_get_position(): return (latitude_p)",
                       latitude_p,
                       sizeof(*latitude_p));

    harness_mock_write("gnss_get_position(): return (longitude_p)",
                       longitude_p,
                       sizeof(*longitude_p));

    harness_mock_write("gnss_get_position(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_get_position)(struct gnss_driver_t *self_p,
                                                   float *latitude_p,
                                                   float *longitude_p)
{
    int res;

    harness_mock_read("gnss_get_position(): return (latitude_p)",
                      latitude_p,
                      -1);

    harness_mock_read("gnss_get_position(): return (longitude_p)",
                      longitude_p,
                      -1);

    harness_mock_read("gnss_get_position(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_get_speed(float *speed_p,
                              int res)
{
    harness_mock_write("gnss_get_speed(): return (speed_p)",
                       speed_p,
                       sizeof(*speed_p));

    harness_mock_write("gnss_get_speed(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_get_speed)(struct gnss_driver_t *self_p,
                                                float *speed_p)
{
    int res;

    harness_mock_read("gnss_get_speed(): return (speed_p)",
                      speed_p,
                      -1);

    harness_mock_read("gnss_get_speed(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_get_number_of_satellites(int *number_of_satellites_p,
                                             int res)
{
    harness_mock_write("gnss_get_number_of_satellites(): return (number_of_satellites_p)",
                       number_of_satellites_p,
                       sizeof(*number_of_satellites_p));

    harness_mock_write("gnss_get_number_of_satellites(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_get_number_of_satellites)(struct gnss_driver_t *self_p,
                                                               int *number_of_satellites_p)
{
    int res;

    harness_mock_read("gnss_get_number_of_satellites(): return (number_of_satellites_p)",
                      number_of_satellites_p,
                      -1);

    harness_mock_read("gnss_get_number_of_satellites(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_get_altitude(float *altitude_p,
                                 int res)
{
    harness_mock_write("gnss_get_altitude(): return (altitude_p)",
                       altitude_p,
                       sizeof(*altitude_p));

    harness_mock_write("gnss_get_altitude(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_get_altitude)(struct gnss_driver_t *self_p,
                                                   float *altitude_p)
{
    int res;

    harness_mock_read("gnss_get_altitude(): return (altitude_p)",
                      altitude_p,
                      -1);

    harness_mock_read("gnss_get_altitude(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_gnss_print(void *chan_p,
                          int res)
{
    harness_mock_write("gnss_print(): return (chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("gnss_print(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(gnss_print)(struct gnss_driver_t *self_p,
                                            void *chan_p)
{
    int res;

    harness_mock_read("gnss_print(): return (chan_p)",
                      chan_p,
                      -1);

    harness_mock_read("gnss_print(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
