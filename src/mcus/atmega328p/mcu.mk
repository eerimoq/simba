#
# @file mcus/atmega328p/mcu.mk
# @version 0.6.0
#
# @section License
# Copyright (C) 2014-2016, Erik Moqvist
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

INC += $(SIMBA_ROOT)/src/mcus/atmega328p
SRC += $(SIMBA_ROOT)/src/mcus/atmega328p/mcu.c

AVRDUDE_BAUDRATE = 57600
AVRDUDE_PROGRAMMER = arduino

F_CPU = 16000000
CPU = atmega328p
MAIN_STACK_END = 0x800900

ARCH = avr

MCU_HOMEPAGE = "http://www.atmel.com"
MCU_NAME = "Atmel ATMega328p"
MCU_DESC = "Atmel ATMega328p AVR @ 16MHz, 2k sram, 32k flash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/avr.mk
