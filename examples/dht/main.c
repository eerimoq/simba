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

int main()
{
    int res;
    struct dht_driver_t dht;
    float temperature;
    float humidty;

    sys_start();

    dht_module_init();

    /* Initialize the DHT driver. */
    dht_init(&dht, &pin_d2_dev);

    while (1) {
        thrd_sleep(2.5);

        /* Read temperature and humidty from the device. */
        res = dht_read(&dht, &temperature, &humidty);

        if (res != 0) {
            std_printf(OSTR("Read failed with %d: %S.\r\n"),
                       res,
                       errno_as_string(res));
            continue;
        }

        /* Print the read temperature and humidty. */
        std_printf(OSTR("Temperature: %f C\r\n"
                        "Humidty:     %f %%RH\r\n"
                        "\r\n"),
                   temperature,
                   humidty);
    }

    return (0);
}
