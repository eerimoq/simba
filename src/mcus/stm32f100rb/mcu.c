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
    /* GPIO A. */
    { .regs_p = STM32_GPIOA, .bit =  0 },
    { .regs_p = STM32_GPIOA, .bit =  1 },
    { .regs_p = STM32_GPIOA, .bit =  2 },
    { .regs_p = STM32_GPIOA, .bit =  3 },
    { .regs_p = STM32_GPIOA, .bit =  4 },
    { .regs_p = STM32_GPIOA, .bit =  5 },
    { .regs_p = STM32_GPIOA, .bit =  6 },
    { .regs_p = STM32_GPIOA, .bit =  7 },
    { .regs_p = STM32_GPIOA, .bit =  8 },
    { .regs_p = STM32_GPIOA, .bit =  9 },
    { .regs_p = STM32_GPIOA, .bit = 10 },
    { .regs_p = STM32_GPIOA, .bit = 11 },
    { .regs_p = STM32_GPIOA, .bit = 12 },
    { .regs_p = STM32_GPIOA, .bit = 13 },
    { .regs_p = STM32_GPIOA, .bit = 14 },
    { .regs_p = STM32_GPIOA, .bit = 15 },

    /* GPIO B. */
    { .regs_p = STM32_GPIOB, .bit =  0 },
    { .regs_p = STM32_GPIOB, .bit =  1 },
    { .regs_p = STM32_GPIOB, .bit =  2 },
    { .regs_p = STM32_GPIOB, .bit =  3 },
    { .regs_p = STM32_GPIOB, .bit =  4 },
    { .regs_p = STM32_GPIOB, .bit =  5 },
    { .regs_p = STM32_GPIOB, .bit =  6 },
    { .regs_p = STM32_GPIOB, .bit =  7 },
    { .regs_p = STM32_GPIOB, .bit =  8 },
    { .regs_p = STM32_GPIOB, .bit =  9 },
    { .regs_p = STM32_GPIOB, .bit = 10 },
    { .regs_p = STM32_GPIOB, .bit = 11 },
    { .regs_p = STM32_GPIOB, .bit = 12 },
    { .regs_p = STM32_GPIOB, .bit = 13 },
    { .regs_p = STM32_GPIOB, .bit = 14 },
    { .regs_p = STM32_GPIOB, .bit = 15 },

    /* GPIO C. */
    { .regs_p = STM32_GPIOC, .bit =  0 },
    { .regs_p = STM32_GPIOC, .bit =  1 },
    { .regs_p = STM32_GPIOC, .bit =  2 },
    { .regs_p = STM32_GPIOC, .bit =  3 },
    { .regs_p = STM32_GPIOC, .bit =  4 },
    { .regs_p = STM32_GPIOC, .bit =  5 },
    { .regs_p = STM32_GPIOC, .bit =  6 },
    { .regs_p = STM32_GPIOC, .bit =  7 },
    { .regs_p = STM32_GPIOC, .bit =  8 },
    { .regs_p = STM32_GPIOC, .bit =  9 },
    { .regs_p = STM32_GPIOC, .bit = 10 },
    { .regs_p = STM32_GPIOC, .bit = 11 },
    { .regs_p = STM32_GPIOC, .bit = 12 },
    { .regs_p = STM32_GPIOC, .bit = 13 },
    { .regs_p = STM32_GPIOC, .bit = 14 },
    { .regs_p = STM32_GPIOC, .bit = 15 },

    /* GPIO D. */
    { .regs_p = STM32_GPIOD, .bit =  0 },
    { .regs_p = STM32_GPIOD, .bit =  1 },
    { .regs_p = STM32_GPIOD, .bit =  2 }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = STM32_USART1
    }
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX] = {
    {
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    }
};

/* Use software i2c. */
struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
    {
        .scl_p = &pin_device[0],
        .sda_p = &pin_device[1],
        .max_clock_stretching_us = 100000,
        .clock_stretching_sleep_us = 1000
    },
    {
        .scl_p = &pin_device[2],
        .sda_p = &pin_device[3],
        .max_clock_stretching_us = 100000,
        .clock_stretching_sleep_us = 1000
    }
};
