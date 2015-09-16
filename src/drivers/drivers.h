/**
 * @file drivers.h
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

#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "drivers/exti.h"
#include "drivers/pin.h"
#include "drivers/pwm.h"
#include "drivers/adc.h"
#include "drivers/spi.h"
#include "drivers/uart.h"
#include "drivers/uart_soft.h"
#include "drivers/owi.h"
#include "drivers/ds18b20.h"
#include "drivers/ds3231.h"
#include "drivers/canif.h"
#include "drivers/cantp.h"
#if !defined(ARCH_AVR)
#    include "drivers/can.h"
#endif
#include "drivers/mcp2515.h"
#include "drivers/nrf24l01.h"

#endif
