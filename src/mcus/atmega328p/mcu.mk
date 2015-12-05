#
# @file mcus/atmega328p/mcu.mk
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

INC += $(SIMBA)/src/mcus/atmega328p
SRC += $(SIMBA)/src/mcus/atmega328p/mcu.c

AVRDUDE_BAUDRATE = 57600
AVRDUDE_PROGRAMMER = arduino

F_CPU = 16000000
CPU = atmega328p
MAIN_STACK_END = 0x800900

ARCH = avr

MCU_DESC = "Atmel ATMega328p AVR @ 16MHz, 2k sram, 32k flash"

include $(SIMBA)/make/$(TOOLCHAIN)/avr.mk
