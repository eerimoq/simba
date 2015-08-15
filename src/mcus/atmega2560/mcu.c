/**
 * @file mcus/atmega328p/mcu.c
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
    /* PIN D */
    { .sfr_p = &PIND, .mask = 0x04 },
    { .sfr_p = &PIND, .mask = 0x08 },
    { .sfr_p = &PIND, .mask = 0x10 },
    { .sfr_p = &PIND, .mask = 0x20 },
    { .sfr_p = &PIND, .mask = 0x40 },
    { .sfr_p = &PIND, .mask = 0x80 },

    /* PIN B */
    { .sfr_p = &PINB, .mask = 0x01 },
    { .sfr_p = &PINB, .mask = 0x02 },
    { .sfr_p = &PINB, .mask = 0x04 },
    { .sfr_p = &PINB, .mask = 0x08 },
    { .sfr_p = &PINB, .mask = 0x10 },
    { .sfr_p = &PINB, .mask = 0x20 },

    /* PIN C */
    { .sfr_p = &PINC, .mask = 0x01 },
    { .sfr_p = &PINC, .mask = 0x02 },
    { .sfr_p = &PINC, .mask = 0x04 },
    { .sfr_p = &PINC, .mask = 0x08 },
    { .sfr_p = &PINC, .mask = 0x10 },
    { .sfr_p = &PINC, .mask = 0x20 },
};

struct exti_device_t exti_device[EXTI_DEVICE_MAX] = {
    { .drv_p = NULL, .pin_p = &pin_d2_dev, .id = 0 },
    { .drv_p = NULL, .pin_p = &pin_d3_dev, .id = 1 }
};

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .mosi_p = &pin_d11_dev,
        .miso_p = &pin_d12_dev,
        .sck_p = &pin_d13_dev,
        .sem = { .count = 1, .head_p = NULL }
    }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    { .drv_p = NULL, .sfr_p = &UCSR0A },
    { .drv_p = NULL, .sfr_p = &UCSR1A },
    { .drv_p = NULL, .sfr_p = &UCSR2A },
    { .drv_p = NULL, .sfr_p = &UCSR3A }
};

struct pwm_device_t pwm_device[PWM_DEVICE_MAX] = {
    { .index = 0, .pin_dev_p = &pin_device[7] }, /* timer1, pin D9 */
    { .index = 1, .pin_dev_p = &pin_device[8] }, /* timer1, pin D10 */
    { .index = 2, .pin_dev_p = &pin_device[9] }, /* timer2, pin D11 */
    { .index = 3, .pin_dev_p = &pin_device[1] }  /* timer2, pin D3 */
};

struct adc_device_t adc_device[ADC_DEVICE_MAX] = {
    { .jobs = { .head_p = NULL, .tail_p = NULL } },
};
