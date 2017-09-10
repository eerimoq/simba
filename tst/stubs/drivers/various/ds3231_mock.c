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

#include "simba.h"
#include "ds3231_mock.h"

int mock_write_ds3231_init(struct i2c_driver_t *i2c_p,
                           int res)
{
    harness_mock_write("ds3231_init(i2c_p)",
                       i2c_p,
                       sizeof(*i2c_p));

    harness_mock_write("ds3231_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds3231_init)(struct ds3231_driver_t *self_p,
                                             struct i2c_driver_t *i2c_p)
{
    int res;

    harness_mock_assert("ds3231_init(i2c_p)",
                        i2c_p);

    harness_mock_read("ds3231_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds3231_set_date(struct date_t *date_p,
                               int res)
{
    harness_mock_write("ds3231_set_date(date_p)",
                       date_p,
                       sizeof(*date_p));

    harness_mock_write("ds3231_set_date(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds3231_set_date)(struct ds3231_driver_t *self_p,
                                                 struct date_t *date_p)
{
    int res;

    harness_mock_assert("ds3231_set_date(date_p)",
                        date_p);

    harness_mock_read("ds3231_set_date(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds3231_get_date(struct date_t *date_p,
                               int res)
{
    harness_mock_write("ds3231_get_date(): return (date_p)",
                       date_p,
                       sizeof(*date_p));

    harness_mock_write("ds3231_get_date(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds3231_get_date)(struct ds3231_driver_t *self_p,
                                                 struct date_t *date_p)
{
    int res;

    harness_mock_read("ds3231_get_date(): return (date_p)",
                      date_p,
                      -1);

    harness_mock_read("ds3231_get_date(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
