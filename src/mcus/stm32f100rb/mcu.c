/**
 * @file mcus/atmega328p/mcu.c
 * @version 1.1.0
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

struct flash_device_t flash_device[FLASH_DEVICE_MAX];
