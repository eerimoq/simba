/**
 * @file mcus/esp32/mcu.c
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* 0-9. */
    { .index = 23 },
    { .index = 41 },
    { .index = 22 },
    { .index = 40 },
    { .index = 24 },
    { .index = 34 },
    { .index = 31 },
    { .index = 32 },
    { .index = 33 },
    { .index = 28 },

    /* 10-19. */
    { .index = 29 },
    { .index = 30 },
    { .index = 18 },
    { .index = 20 },
    { .index = 17 },
    { .index = 21 },
    { .index = 25 },
    { .index = 27 },
    { .index = 35 },
    { .index = 38 },

    /* 20-29. */
    { .index = 42 },
    { .index = 39 },
    { .index = 36 },
    { .index = 14 },
    { .index = 15 },
    { .index = 16 },
    { .index = 12 },
    { .index = 13 },
    { .index = 10 },
    { .index = 11 },

    /* 30-33. */
    { .index = 5 },
    { .index = 6 },
    { .index = 7 },
    { .index = 8 },
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART0
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART1
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART2
    }
};

/* struct spi_device_t spi_device[SPI_DEVICE_MAX] = { */
/*     { */
/*         .drv_p = NULL, */
/*         .regs_p = ESP32_SPI1, */
/*         .ss_p = NULL, */
/*         .mosi_p = &pin_device[8], */
/*         .miso_p = &pin_device[7], */
/*         .sck_p = &pin_device[6], */
/*         .sem = { .count = 0, .count_max = 1, .head_p = NULL } */
/*     }, */
/*     { */
/*         .drv_p = NULL, */
/*         .regs_p = ESP32_SPI2, */
/*         .ss_p = NULL, */
/*         .mosi_p = &pin_device[13], */
/*         .miso_p = &pin_device[12], */
/*         .sck_p = &pin_device[14], */
/*         .sem = { .count = 0, .count_max = 1, .head_p = NULL } */
/*     }, */
/*     { */
/*         .drv_p = NULL, */
/*         .regs_p = ESP32_SPI3, */
/*         .ss_p = NULL, */
/*         .mosi_p = &pin_device[23], */
/*         .miso_p = &pin_device[19], */
/*         .sck_p = &pin_device[18], */
/*         .sem = { .count = 0, .count_max = 1, .head_p = NULL } */
/*     } */
/* }; */

/* struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = { */
/*     { */
/*         .drv_p = NULL, */
/*         .regs_p = ESP32_I2C0 */
/*     }, */
/*     { */
/*         .drv_p = NULL, */
/*         .regs_p = ESP32_I2C1 */
/*     } */
/* }; */
