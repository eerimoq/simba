#
# @file simba/drivers/drivers.mk
# @version 1.0
#
# @section License
# Copyright (C) 2014-2015, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# This file is part of the Simba project.
#

INC += $(SIMBA)/src/drivers
INC += $(SIMBA)/src/drivers/ports/$(ARCH)

ifeq ($(ARCH),linux)
DRIVERS_SRC ?= pin.c \
               sd.c \
               spi.c \
               uart.c
endif

ifeq ($(ARCH),avr)
DRIVERS_SRC ?= adc.c \
               ds18b20.c \
               ds3231.c \
               exti.c \
               mcp2515.c \
               nrf24l01.c \
               owi.c \
               pin.c \
               pwm.c \
               sd.c \
               uart.c \
               uart_soft.c
endif

ifeq ($(ARCH),arm)
DRIVERS_SRC ?= flash.c \
               pin.c \
               sd.c \
               spi.c \
               uart.c
endif

SRC += $(DRIVERS_SRC:%=$(SIMBA)/src/drivers/%)
