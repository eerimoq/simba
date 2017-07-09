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
    /* PA0-15. */
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

    /* PB0-15. */
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

    /* PC0-10. */
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

    /* PH9-10. */
    { .id = 121 },
    { .id = 122 }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    { .drv_p = NULL, .regs_p = SPC5_LINFLEX_0 },
    { .drv_p = NULL, .regs_p = SPC5_LINFLEX_1 },
    { .drv_p = NULL, .regs_p = SPC5_LINFLEX_2 }
};

static int32_t cflash_sector_sizes[] = {
    0x8000,
    0x4000,
    0x4000,
    0x8000,
    0x8000,
    -1
};

static int32_t dflash_sector_sizes[] = {
    0x4000,
    0x4000,
    0x4000,
    0x4000,
    -1
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX] = {
    {
        .regs_p = SPC5_CFLASH,
        .address = SPC5_CFLASH_ADDRESS,
        .size = SPC5_CFLASH_SIZE,
        .sector_sizes_p = cflash_sector_sizes,
        .program_size = 2,
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    },
    {
        .regs_p = SPC5_DFLASH,
        .address = SPC5_DFLASH_ADDRESS,
        .size = SPC5_DFLASH_SIZE,
        .sector_sizes_p = dflash_sector_sizes,
        .program_size = 1,
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    }
};

struct can_device_t can_device[CAN_DEVICE_MAX] = {
    { .regs_p = SPC5_FLEXCAN_0 }
};

/* Use software i2c. */
struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
    {
        .scl_p = &pin_device[0],
        .sda_p = &pin_device[1],
        .max_clock_stretching_us = 100000,
        .clock_stretching_sleep_us = 1000
    }
};
