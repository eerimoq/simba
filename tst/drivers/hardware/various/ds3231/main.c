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

static struct i2c_driver_t i2c;
static struct ds3231_driver_t ds3231;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(i2c_init(&i2c,
                      &i2c_device[0],
                      I2C_BAUDRATE_100KBPS,
                      -1) == 0);
    BTASSERT(i2c_start(&i2c) == 0);
    BTASSERT(ds3231_init(&ds3231, &i2c) == 0);

    return (0);
}

static int test_scan(struct harness_t *harness_p)
{
    int number_of_slaves_found;
    int address;

    number_of_slaves_found = 0;

    for (address = 0; address < 128; address++) {
        if (i2c_scan(&i2c, address) == 1) {
            std_printf(OSTR("Found slave with address 0x%02x.\r\n"),
                       address);
            number_of_slaves_found++;
        }
    }

    std_printf(OSTR("Number of slaves found: %d\r\n"),
               number_of_slaves_found);

    return (0);
}

static int test_date(struct harness_t *harness_p)
{
    int i;
    struct date_t date;

    /* Set the date in the DS3231 device. */
    date.year = 16;
    date.month = 6;
    date.date = 5;
    date.day = 7;
    date.hour = 22;
    date.minute = 15;
    date.second = 20;

    BTASSERT(ds3231_set_date(&ds3231, &date) == 0);

    /* Get the date a few times and print it. */
    for (i = 0; i < 5; i++) {
        memset(&date, 0, sizeof(date));
        BTASSERT(ds3231_get_date(&ds3231, &date) == 0);

        std_printf(FSTR("year = %d\r\n"
                        "month = %d\r\n"
                        "date = %d\r\n"
                        "day = %d\r\n"
                        "hour = %d\r\n"
                        "minute = %d\r\n"
                        "second = %d\r\n"
                        "\r\n"),
                   date.year,
                   date.month,
                   date.date,
                   date.day,
                   date.hour,
                   date.minute,
                   date.second);

        thrd_sleep_ms(1000);
    }

    /* Get the date from the DS3231 device. */
    memset(&date, 0, sizeof(date));
    BTASSERT(ds3231_get_date(&ds3231, &date) == 0);

    BTASSERT(date.year == 16);
    BTASSERT(date.month == 6);
    BTASSERT(date.date == 5);
    BTASSERT(date.day == 7);
    BTASSERT(date.hour == 22);
    BTASSERT(date.minute == 15);
    BTASSERT(date.second > 20);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_scan, "test_scan" },
        { test_date, "test_date" },
        { NULL, NULL }
    };

    sys_start();
    i2c_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
