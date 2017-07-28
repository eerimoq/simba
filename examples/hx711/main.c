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

/* Change theses settings to match your weigh scale. */
#define SCALE  0.0000103f
#define OFFSET -89280.0f

int main()
{
    struct hx711_driver_t hx711;
    float weight_a_128;
    float weight_a_64;
    float weight_b_32;
    int res;

    sys_start();

    /* Initialize the driver. */
    std_printf(OSTR("Initializing the HX711 driver... "));

    res = hx711_init(&hx711,
                     &pin_d2_dev,
                     &pin_d3_dev,
                     SCALE,
                     OFFSET);

    if (res == 0) {
        std_printf(OSTR("done.\r\n"));
    } else {
        std_printf(OSTR("failed with %d.\r\n"), res);
        return (res);
    }

    /* Start the driver. */
    std_printf(OSTR("Starting the HX711 driver... "));

    res = hx711_start(&hx711);

    if (res == 0) {
        std_printf(OSTR("done.\r\n"));
    } else {
        std_printf(OSTR("failed with %d.\r\n"), res);
        return (res);
    }

    /* Continiously read samples from the device and print them. */
    while (1) {
        res = hx711_read(&hx711, &weight_a_128, hx711_channel_gain_a_128_t);

        if (res != 0) {
            std_printf(OSTR("Read 128 failed with %d.\r\n"), res);
        }

        res = hx711_read(&hx711, &weight_a_64, hx711_channel_gain_a_64_t);

        if (res != 0) {
            std_printf(OSTR("Read 64 failed with %d.\r\n"), res);
        }

        res = hx711_read(&hx711, &weight_b_32, hx711_channel_gain_b_32_t);

        if (res != 0) {
            std_printf(OSTR("Read 32 failed with %d.\r\n"), res);
        }

        /* Print the samples. */
        std_printf(OSTR("Weight channel A gain 128: %f\r\n"
                        "Weight channel A gain 64:  %f\r\n"
                        "Weight channel B gain 32:  %f\r\n"
                        "\r\n"),
                   weight_a_128,
                   weight_a_64,
                   weight_b_32);
    }

    return (0);
}
