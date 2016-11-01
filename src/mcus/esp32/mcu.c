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
    { .id =  0, .iomux = 16 },
    { .id =  1, .iomux = 33 },
    { .id =  2, .iomux = 15 },
    { .id =  3, .iomux = 32 },
    { .id =  4, .iomux = 17 },
    { .id =  5, .iomux = 26 },
    { .id =  6, .iomux = 23 },
    { .id =  7, .iomux = 24 },
    { .id =  8, .iomux = 25 },
    { .id =  9, .iomux = 20 },

    /* 10-19. */
    { .id = 10, .iomux = 21 },
    { .id = 11, .iomux = 22 },
    { .id = 12, .iomux = 12 },
    { .id = 13, .iomux = 13 },
    { .id = 14, .iomux = 11 },
    { .id = 15, .iomux = 14 },
    { .id = 16, .iomux = 18 },
    { .id = 17, .iomux = 19 },
    { .id = 18, .iomux = 27 },
    { .id = 19, .iomux = 28 },

    /* 20-29. */
    { .id = 20, .iomux = 29 },
    { .id = 21, .iomux = 30 },
    { .id = 22, .iomux = 31 },
    { .id = 23, .iomux = 34 },
    { .id = 24, .iomux = 35 },
    { .id = 25, .iomux =  8 },
    { .id = 26, .iomux =  9 },
    { .id = 27, .iomux = 10 },
    /* { .id = 28, .iomux = -1 }, */
    /* { .id = 29, .iomux = -1 }, */

    /* 30-39. */
    /* { .id = 30, .iomux = -1 }, */
    /* { .id = 31, .iomux = -1 }, */
    { .id = 32, .iomux =  6 },
    { .id = 33, .iomux =  7 },
    { .id = 34, .iomux =  4 },
    { .id = 35, .iomux =  5 },
    { .id = 36, .iomux =  0 },
    { .id = 37, .iomux =  1 },
    { .id = 38, .iomux =  2 },
    { .id = 39, .iomux =  3 }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART0,
        .interrupt = {
            .source = ESP32_INTR_SOURCE_UART0,
            .cpu = ESP32_CPU_INTR_UART_NUM
        }
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART1,
        .interrupt = {
            .source = ESP32_INTR_SOURCE_UART1,
            .cpu = ESP32_CPU_INTR_UART_NUM
        }
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART2,
        .interrupt = {
            .source = ESP32_INTR_SOURCE_UART2,
            .cpu = ESP32_CPU_INTR_UART_NUM
        }
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

struct flash_device_t flash_device[FLASH_DEVICE_MAX];
