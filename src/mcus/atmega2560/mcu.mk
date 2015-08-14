#!/usr/bin/env bash
#
# @file simba/boards/shtest.mk
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

INC += $(SIMBA)/src/mcus/atmega2560
SRC += $(SIMBA)/src/mcus/atmega2560/mcu.c

F_CPU = 16000000
MCU = atmega2560

CFLAGS += -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -funsigned-char \
          -funsigned-bitfields -fpack-struct -fshort-enums -std=gnu99
LDFLAGS += -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Wl,--cref

ARCH = avr
