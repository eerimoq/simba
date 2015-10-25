/**
 * @file mcus/sam/mcu.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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
    /* Conroller A, 0-29. */
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P9 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P10 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P11 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P12 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P13 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P14 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P15 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P16 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P17 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P18 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P19 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P20 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P21 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P22 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P23 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P24 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P25 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P26 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P27 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P28 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P29 },

    /* Conroller B, 30-61. */
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P9 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P10 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P11 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P12 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P13 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P14 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P15 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P16 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P17 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P18 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P19 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P20 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P21 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P22 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P23 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P24 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P25 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P26 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P27 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P28 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P29 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P30 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P31 },

    /* Conroller C, 62-93. */
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P9 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P10 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P11 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P12 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P13 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P14 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P15 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P16 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P17 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P18 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P19 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P20 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P21 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P22 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P23 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P24 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P25 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P26 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P27 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P28 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P29 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P30 },

    /* Conroller D, 93-102. */
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P9 }
};

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = SAM_SPI0,
        .mosi_p = &pin_device[26],
        .miso_p = &pin_device[25],
        .sck_p = &pin_device[27],
        .id = PERIPHERAL_ID_SPI0,
        .sem = { .count = 1, .head_p = NULL }
    }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = SAM_UART,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P8
            },
            .tx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P9
            }
        },
        .id = 8
    },

    {
        .drv_p = NULL,
        .regs_p = (struct sam_uart_t *)SAM_USART0,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P10
            },
            .tx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P11
            }
        },
        .id = 17
    },

    {
        .drv_p = NULL,
        .regs_p = (struct sam_uart_t *)SAM_USART1,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P12
            },
            .tx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P13
            }
        },
        .id = 18
    },

    {
        .drv_p = NULL,
        .regs_p = (struct sam_uart_t *)SAM_USART2,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOB,
                .mask = SAM_PIO_P21
            },
            .tx = {
                .regs_p = SAM_PIOB,
                .mask = SAM_PIO_P22
            }
        },
        .id = 19
    },

    {
        .drv_p = NULL,
        .regs_p = (struct sam_uart_t *)SAM_USART3,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOD,
                .mask = SAM_PIO_P5
            },
            .tx = {
                .regs_p = SAM_PIOD,
                .mask = SAM_PIO_P4
            }
        },
        .id = 20
    }
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX] = {
    {
        .banks = {
            {
                .index = 0,
                .regs_p = SAM_EEFC0,
                .begin = FLASH0_BEGIN,
                .end = FLASH0_END,
                .page_size = FLASH0_PAGE_SIZE
            },
            {
                .index = 1,
                .regs_p = SAM_EEFC1,
                .begin = FLASH1_BEGIN,
                .end = FLASH1_END,
                .page_size = FLASH1_PAGE_SIZE
            }
        },
        .sem = {
            .count = 1,
            .head_p = NULL
        }
    }
};
