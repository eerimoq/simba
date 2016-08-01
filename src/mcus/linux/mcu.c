/**
 * @file mcu.c
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

struct pin_device_t pin_device[PIN_DEVICE_MAX];
struct exti_device_t exti_device[EXTI_DEVICE_MAX];
struct spi_device_t spi_device[SPI_DEVICE_MAX];
struct uart_device_t uart_device[UART_DEVICE_MAX];
struct can_device_t can_device[CAN_DEVICE_MAX];
struct pwm_device_t pwm_device[PWM_DEVICE_MAX];
struct adc_device_t adc_device[ADC_DEVICE_MAX];
struct flash_device_t flash_device[FLASH_DEVICE_MAX];
