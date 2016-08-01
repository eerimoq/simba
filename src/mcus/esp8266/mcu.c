/**
 * @file mcus/esp8266/mcu.c
 * @version 4.1.0
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
    { .id = 15, .mask = BIT(15), .iomux =  3 }
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
        .sem = { .count = 0, .count_max = 1, .head_p = NULL }
    }
};
