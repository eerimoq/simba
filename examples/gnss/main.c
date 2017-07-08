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

static struct gnss_driver_t gnss;

int main()
{
    int res;
    struct time_t timeout;
    struct date_t date;
    long latitude;
    long longitude;
    long speed;
    int age;

    sys_start();

    /* Initialization. */
    gnss_module_init();

    if (gnss_init(&gnss, sys_get_stdin()) != 0) {
        std_printf(OSTR("GNSS initialization failed.\r\n"));

        return (-1);
    }

    /* Read and print GNSS data. */
    timeout.seconds = 1;
    timeout.nanoseconds = 0;

    std_printf(OSTR("Waiting for GNSS data...\r\n"));

    while (1) {
        if (chan_poll(sys_get_stdin(), &timeout) != NULL) {
            res = gnss_read(&gnss);

            if (res != 0) {
                std_printf(OSTR("gnss_read() failed with %d: %s.\r\n"),
                           res,
                           errno_as_string(res));
            }
        }

        /* Date. */
        age = gnss_get_date(&gnss, &date);

        if (age >= 0) {
            std_printf(OSTR("Date: %02u:%02u:%02u %u-%02u-%02u "
                            "(age: %d seconds)\r\n"),
                       date.hour,
                       date.minute,
                       date.second,
                       date.year,
                       date.month,
                       date.date,
                       age);
        }

        /* Position. */
        age = gnss_get_position(&gnss, &latitude, &longitude);

        if (age >= 0) {
            std_printf(OSTR("Latitude: %ld.%06lu degrees, Longitude: %ld.%06lu degrees "
                            "(age: %d seconds)\r\n"),
                       latitude / 1000000,
                       abs(latitude) % 1000000,
                       longitude / 1000000,
                       abs(longitude) % 1000000,
                       age);
        }

        /* Speed. */
        age = gnss_get_speed(&gnss, &speed);

        if (age >= 0) {
            std_printf(OSTR("Speed: %ld.%03lu m/s (age: %d seconds)\r\n"),
                       speed / 1000,
                       speed % 1000,
                       age);
        }
    }

    return (0);
}
