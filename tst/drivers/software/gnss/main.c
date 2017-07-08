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

struct gnss_driver_t gnss;
struct chan_t transport;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(gnss_module_init() == 0);
    BTASSERT(gnss_module_init() == 0);

    BTASSERT(chan_init(&transport,
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);
    BTASSERT(gnss_init(&gnss, &transport) == 0);

    return (0);
}

static int test_get_no_data(struct harness_t *harness_p)
{
    struct date_t date;
    long latitude;
    long longitude;
    long speed;

    /* Date should be unmodified. */
    date.year = 1200;
    BTASSERTI(gnss_get_date(&gnss, &date), ==, -ENODATA);
    BTASSERTI(date.year, ==, 1200);

    /* Position should be unmodified. */
    latitude = 23;
    longitude = 24;
    BTASSERTI(gnss_get_position(&gnss, &latitude, &longitude), ==, -ENODATA);
    BTASSERTI(latitude, ==, 23);
    BTASSERTI(longitude, ==, 24);

    /* Speed should be unmodified. */
    speed = 654;
    BTASSERTI(gnss_get_speed(&gnss, &speed), ==, -ENODATA);
    BTASSERTI(speed, ==, 654);

    return (0);
}

static int test_read(struct harness_t *harness_p)
{
    int i;
    ssize_t res;
    char sentence[] =
        "$GPRMC,123519,A,4807.038,N,01131.000,W,022.4,084.4,230394,003.1,W*78\r\n";
    struct date_t date;
    long latitude;
    long longitude;
    long speed;

    res = 1;

    for (i = 0; i < strlen(sentence); i++) {
        harness_mock_write("chan_read(): return (res)", &res, sizeof(res));
        harness_mock_write("chan_read(): return (buf_p)", &sentence[i], 1);
    }

    BTASSERTI(gnss_read(&gnss), ==, 0);

    /* Date. */
    date.year = 1200;
    BTASSERTI(gnss_get_date(&gnss, &date), >=, 0);
    BTASSERTI(date.year, ==, 94);
    BTASSERTI(date.month, ==, 3);
    BTASSERTI(date.date, ==, 23);
    BTASSERTI(date.hour, ==, 12);
    BTASSERTI(date.minute, ==, 35);
    BTASSERTI(date.second, ==, 19);

    /* Position. */
    latitude = 23;
    longitude = 24;
    BTASSERTI(gnss_get_position(&gnss, &latitude, &longitude), >=, 0);
    BTASSERTI(latitude, ==, 48000000 + (7038000 / 60));
    BTASSERTI(longitude, ==, -(11000000 + (31000000 / 60)));

    /* Speed. */
    speed = 654;
    BTASSERTI(gnss_get_speed(&gnss, &speed), >=, 0);
    BTASSERTI(speed, ==, 11523);

    return (0);
}

static int test_read_read_failed(struct harness_t *harness_p)
{
    ssize_t res;

    res = -EIO;
    harness_mock_write("chan_read(): return (res)",
                       &res,
                       sizeof(res));

    BTASSERTI(gnss_read(&gnss), ==, -EIO);

    res = 0;
    harness_mock_write("chan_read(): return (res)",
                       &res,
                       sizeof(res));

    BTASSERTI(gnss_read(&gnss), ==, -EIO);

    return (0);
}

ssize_t STUB(chan_read)(void *self_p,
                        void *buf_p,
                        size_t size)
{
    ssize_t res;

    if (harness_mock_read("chan_read(): return (res)",
                          &res,
                          sizeof(res)) == -1) {
        res = size;
    }

    harness_mock_read("chan_read(): return (buf_p)",
                      buf_p,
                      res);

    return (res);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_get_no_data, "test_get_no_data" },
        { test_read, "test_read" },
        { test_read_read_failed, "test_read_read_failed" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
