#
# @file mcus/atmega2560/mcu.mk
# @version 5.0.0
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

INC += $(SIMBA_ROOT)/src/mcus/atmega2560
SRC += $(SIMBA_ROOT)/src/mcus/atmega2560/mcu.c

F_CPU = 16000000
CPU = atmega2560
MAIN_STACK_END = 0x802200

AVRDUDE_BAUDRATE = 115200
AVRDUDE_PROGRAMMER = wiring

ARCH = avr
FAMILY = avr

MCU_HOMEPAGE = "http://www.atmel.com"
MCU_NAME = "Atmel ATMega2560"
MCU_DESC = "Atmel ATMega2560 AVR @ 16MHz, 8k sram, 256k flash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/avr.mk
