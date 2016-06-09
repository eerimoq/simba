/**
 * @file drivers.h
 * @version 0.6.0
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

#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#if defined(FAMILY_LINUX)
#    include "drivers/exti.h"
#    include "drivers/pin.h"
#    include "drivers/pwm.h"
#    include "drivers/adc.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/can.h"
#    include "drivers/sd.h"
#    include "drivers/flash.h"
#endif

#if defined(FAMILY_AVR)
#    include "drivers/exti.h"
#    include "drivers/pin.h"
#    include "drivers/pwm.h"
#    include "drivers/adc.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/i2c.h"
#    include "drivers/uart_soft.h"
#    include "drivers/owi.h"
#    include "drivers/ds18b20.h"
#    include "drivers/ds3231.h"
#    include "drivers/mcp2515.h"
#    include "drivers/nrf24l01.h"
#    include "drivers/sd.h"
#endif

#if defined(FAMILY_SAM)
#    include "drivers/chipid.h"
#    include "drivers/exti.h"
#    include "drivers/flash.h"
#    include "drivers/pin.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/sd.h"
#    include "drivers/can.h"
#    include "drivers/mcp2515.h"
#    include "drivers/adc.h"
#    include "drivers/dac.h"
#    include "drivers/usb.h"
#    include "drivers/usb_host.h"
#    include "drivers/usb/host/class/usb_host_class_hid.h"
#    include "drivers/usb/host/class/usb_host_class_mass_storage.h"
#endif

#if defined(FAMILY_ESP)
#    include "drivers/pin.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/adc.h"
#endif

#if defined(FAMILY_STM32F1)
#    include "drivers/pin.h"
#    include "drivers/uart.h"
#    include "drivers/flash.h"
#endif

#if defined(FAMILY_STM32F3)
#    include "drivers/pin.h"
#    include "drivers/uart.h"
#    include "drivers/flash.h"
#endif

#endif
