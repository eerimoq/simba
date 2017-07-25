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
    { .regs_p = NRF5_GPIO_P0, .pin = 0 },
    { .regs_p = NRF5_GPIO_P0, .pin = 1 },
    { .regs_p = NRF5_GPIO_P0, .pin = 2 },
    { .regs_p = NRF5_GPIO_P0, .pin = 3 },
    { .regs_p = NRF5_GPIO_P0, .pin = 4 },
    { .regs_p = NRF5_GPIO_P0, .pin = 5 },
    { .regs_p = NRF5_GPIO_P0, .pin = 6 },
    { .regs_p = NRF5_GPIO_P0, .pin = 7 },
    { .regs_p = NRF5_GPIO_P0, .pin = 8 },
    { .regs_p = NRF5_GPIO_P0, .pin = 9 },
    { .regs_p = NRF5_GPIO_P0, .pin = 10 },
    { .regs_p = NRF5_GPIO_P0, .pin = 11 },
    { .regs_p = NRF5_GPIO_P0, .pin = 12 },
    { .regs_p = NRF5_GPIO_P0, .pin = 13 },
    { .regs_p = NRF5_GPIO_P0, .pin = 14 },
    { .regs_p = NRF5_GPIO_P0, .pin = 15 },
    { .regs_p = NRF5_GPIO_P0, .pin = 16 },
    { .regs_p = NRF5_GPIO_P0, .pin = 17 },
    { .regs_p = NRF5_GPIO_P0, .pin = 18 },
    { .regs_p = NRF5_GPIO_P0, .pin = 19 },
    { .regs_p = NRF5_GPIO_P0, .pin = 20 },
    { .regs_p = NRF5_GPIO_P0, .pin = 21 },
    { .regs_p = NRF5_GPIO_P0, .pin = 22 },
    { .regs_p = NRF5_GPIO_P0, .pin = 23 },
    { .regs_p = NRF5_GPIO_P0, .pin = 24 },
    { .regs_p = NRF5_GPIO_P0, .pin = 25 },
    { .regs_p = NRF5_GPIO_P0, .pin = 26 },
    { .regs_p = NRF5_GPIO_P0, .pin = 27 },
    { .regs_p = NRF5_GPIO_P0, .pin = 28 },
    { .regs_p = NRF5_GPIO_P0, .pin = 29 },
    { .regs_p = NRF5_GPIO_P0, .pin = 30 },
    { .regs_p = NRF5_GPIO_P0, .pin = 31 },

    { .regs_p = NRF5_GPIO_P1, .pin = 0 },
    { .regs_p = NRF5_GPIO_P1, .pin = 1 },
    { .regs_p = NRF5_GPIO_P1, .pin = 2 },
    { .regs_p = NRF5_GPIO_P1, .pin = 3 },
    { .regs_p = NRF5_GPIO_P1, .pin = 4 },
    { .regs_p = NRF5_GPIO_P1, .pin = 5 },
    { .regs_p = NRF5_GPIO_P1, .pin = 6 },
    { .regs_p = NRF5_GPIO_P1, .pin = 7 },
    { .regs_p = NRF5_GPIO_P1, .pin = 8 },
    { .regs_p = NRF5_GPIO_P1, .pin = 9 },
    { .regs_p = NRF5_GPIO_P1, .pin = 10 },
    { .regs_p = NRF5_GPIO_P1, .pin = 11 },
    { .regs_p = NRF5_GPIO_P1, .pin = 12 },
    { .regs_p = NRF5_GPIO_P1, .pin = 13 },
    { .regs_p = NRF5_GPIO_P1, .pin = 14 },
    { .regs_p = NRF5_GPIO_P1, .pin = 15 }

};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = NRF5_UART0,
        .tx_pin_p = &pin_device[6],
        .rx_pin_p = &pin_device[8]
    }
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX];

/* Use software i2c. */
struct i2c_device_t i2c_device[I2C_DEVICE_MAX];
