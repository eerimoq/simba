/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

static int test_date(struct harness_t *harness_p)
{
    int i;
    struct i2c_driver_t i2c;
    struct ds3231_driver_t ds3231;
    struct date_t date;

    i2c_init(&i2c,
             &i2c_device[0],
             I2C_BAUDRATE_100KBPS,
             -1);
    BTASSERT(ds3231_init(&ds3231, &i2c) == 0);

    i2c_start(&i2c);

    /* Set the date in the DS3231 device. */    
    date.second = 20;
    date.minute = 15;
    date.hour = 22;
    date.day = 7;
    date.date = 5;
    date.month = 6;
    date.year = 16;

    BTASSERT(ds3231_set_date(&ds3231, &date) == 0);

    /* Get the date a few times and print it. */
    for (i = 0; i < 5; i++) {
        memset(&date, 0, sizeof(date));
        BTASSERT(ds3231_get_date(&ds3231, &date) == 0);

        std_printf(FSTR("second = %d\r\n"), date.second);
        std_printf(FSTR("minute = %d\r\n"), date.minute);
        std_printf(FSTR("hour = %d\r\n"), date.hour);
        std_printf(FSTR("day = %d\r\n"), date.day);
        std_printf(FSTR("date = %d\r\n"), date.date);
        std_printf(FSTR("month = %d\r\n"), date.month);
        std_printf(FSTR("year = %d\r\n\r\n"), date.year);

        thrd_sleep_us(1000000);
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

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_date, "test_date" },
        { NULL, NULL }
    };

    sys_start();
    i2c_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
