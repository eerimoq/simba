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

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* P0. */
    { .id = 0 },
    { .id = 1 },
    { .id = 2 },
    { .id = 3 },
    { .id = 4 },
    { .id = 5 },
    { .id = 6 },
    { .id = 7 },
    { .id = 8 },
    { .id = 9 },

    { .id = 10 },
    { .id = 11 },
    { .id = 12 },
    { .id = 13 },
    { .id = 14 },
    { .id = 15 },
    { .id = 16 },
    { .id = 17 },
    { .id = 18 },
    { .id = 19 },
    { .id = 20 },

    { .id = 21 },
    { .id = 22 },
    { .id = 23 },
    { .id = 24 },
    { .id = 25 },
    { .id = 26 },
    { .id = 27 },
    { .id = 28 },
    { .id = 29 },

    { .id = 30 },
    { .id = 31 },

    /* P1. */
    { .id = 32 },
    { .id = 33 },
    { .id = 34 },
    { .id = 35 },
    { .id = 36 },
    { .id = 37 },
    { .id = 38 },
    { .id = 39 },

    { .id = 40 },
    { .id = 41 },
    { .id = 42 },
    { .id = 43 },
    { .id = 44 },
    { .id = 45 },
    { .id = 46 },
    { .id = 47 },

};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = NRF5_UART0
    }
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX];

/* Use software i2c. */
struct i2c_device_t i2c_device[I2C_DEVICE_MAX];
