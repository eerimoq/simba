/**
 * @file mcu.h
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

#ifndef __MCU_H__
#define __MCU_H__

#include "sam3.h"

#if defined(MCU_SAM_3X8E)
#    define PIN_DEVICE_MAX  103
#    define EXTI_DEVICE_MAX 2
#    define SPI_DEVICE_MAX  1
#    define UART_DEVICE_MAX 4
#    define PWM_DEVICE_MAX  12
#    define ADC_DEVICE_MAX  1
#else
#     error "Unsupported MCU."
#endif

#endif
