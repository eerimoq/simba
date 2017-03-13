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

/* Number of pixels in the chain. */
#define PIXEL_MAX                                          81

static uint8_t red[3 * PIXEL_MAX];
static uint8_t green[3 * PIXEL_MAX];
static uint8_t blue[3 * PIXEL_MAX];

int main()
{
    struct ws2812_driver_t ws2812;
    struct pin_device_t *pins[] = {
        &pin_gpio18_dev
    };
    int i;

    sys_start();

    ws2812_init(&ws2812, &pins[0], 1);

    /* Initialize the color buffers. */
    memset(&red[0], 0, sizeof(red));
    memset(&green[0], 0, sizeof(green));
    memset(&blue[0], 0, sizeof(blue));

    for (i = 0; i < PIXEL_MAX; i++) {
        red[3 * i + 1] = 255;
        green[3 * i] = 255;
        blue[3 * i + 2] = 255;
    }

    while (1) {
        std_printf(OSTR("Red.\r\n"));
        ws2812_write(&ws2812, &red[0], membersof(red) / 3);
        thrd_sleep(0.5);

        std_printf(OSTR("Green.\r\n"));
        ws2812_write(&ws2812, &green[0], membersof(green) / 3);
        thrd_sleep(0.5);

        std_printf(OSTR("Blue.\r\n"));
        ws2812_write(&ws2812, &blue[0], membersof(blue) / 3);
        thrd_sleep(0.5);
    }

    return (0);
}
