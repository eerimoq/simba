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

#ifndef __MCU_H__
#define __MCU_H__

#include "sam3.h"

/* Pin controller start indexes in devices array. */
#define SAM_PA 0
#define SAM_PB 30
#define SAM_PC 62
#define SAM_PD 93

#if defined(MCU_SAM3X8E)
#    define PIN_DEVICE_MAX             103
#    define EXTI_DEVICE_MAX PIN_DEVICE_MAX
#    define SPI_DEVICE_MAX               1
#    define UART_DEVICE_MAX              4
#    define PWM_DEVICE_MAX              12
#    define ADC_DEVICE_MAX               1
#    define DAC_DEVICE_MAX               1
#    define FLASH_DEVICE_MAX             1
#    define CAN_DEVICE_MAX               2
#    define USB_DEVICE_MAX               1
#    define I2C_DEVICE_MAX               2
#else
#     error "Unsupported MCU."
#endif

#endif
