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
    { .id =  0, .mask =  BIT(0), .iomux = 12 },
    { .id =  1, .mask =  BIT(1), .iomux =  5 },
    { .id =  2, .mask =  BIT(2), .iomux = 13 },
    { .id =  3, .mask =  BIT(3), .iomux =  4 },
    { .id =  4, .mask =  BIT(4), .iomux = 14 },
    { .id =  5, .mask =  BIT(5), .iomux = 15 },
    { .id =  6, .mask =  BIT(6), .iomux =  6 },
    { .id =  7, .mask =  BIT(7), .iomux =  7 },
    { .id =  8, .mask =  BIT(8), .iomux =  8 },
    { .id =  9, .mask =  BIT(9), .iomux =  9 },
    { .id = 10, .mask = BIT(10), .iomux = 10 },
    { .id = 11, .mask = BIT(11), .iomux = 11 },
    { .id = 12, .mask = BIT(12), .iomux =  0 },
    { .id = 13, .mask = BIT(13), .iomux =  1 },
    { .id = 14, .mask = BIT(14), .iomux =  2 },
    { .id = 15, .mask = BIT(15), .iomux =  3 },
    { .id = 16, .mask =  BIT(1), .iomux = -1 }
};

struct exti_device_t exti_device[EXTI_DEVICE_MAX] = {
    { .drv_p = NULL, .pin_p = &pin_device[0] },
    { .drv_p = NULL, .pin_p = &pin_device[1] },
    { .drv_p = NULL, .pin_p = &pin_device[2] },
    { .drv_p = NULL, .pin_p = &pin_device[3] },
    { .drv_p = NULL, .pin_p = &pin_device[4] },
    { .drv_p = NULL, .pin_p = &pin_device[5] },
    { .drv_p = NULL, .pin_p = &pin_device[6] },
    { .drv_p = NULL, .pin_p = &pin_device[7] },
    { .drv_p = NULL, .pin_p = &pin_device[8] },
    { .drv_p = NULL, .pin_p = &pin_device[9] },
    { .drv_p = NULL, .pin_p = &pin_device[10] },
    { .drv_p = NULL, .pin_p = &pin_device[11] },
    { .drv_p = NULL, .pin_p = &pin_device[12] },
    { .drv_p = NULL, .pin_p = &pin_device[13] },
    { .drv_p = NULL, .pin_p = &pin_device[14] },
    { .drv_p = NULL, .pin_p = &pin_device[15] }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    { .regs_p = ESP8266_UART0 },
    { .regs_p = ESP8266_UART1 }
};

struct adc_device_t adc_device[ADC_DEVICE_MAX];

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = ESP8266_SPI0,
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
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

/* Software i2c on ESP8266. */
struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
    {
        .scl_p = &pin_device[5],
        .sda_p = &pin_device[4],
        .max_clock_stretching_us = 100000,
        .clock_stretching_sleep_us = 1000
    },
    {
        .scl_p = &pin_device[12],
        .sda_p = &pin_device[13],
        .max_clock_stretching_us = 100000,
        .clock_stretching_sleep_us = 1000
    }
};
