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

INC += $(SIMBA)/src/mcus/sam3x8e
SRC += $(SIMBA)/src/mcus/sam3x8e/mcu.c $(SIMBA)/src/mcus/sam3x8e/sam3x8e.c

MCU = cortex-m3

CFLAGS += -mthumb -mcpu=$(MCU) -funsigned-char \
          -funsigned-bitfields -fpack-struct -fshort-enums \
          -std=gnu99
LDFLAGS += -mcpu=$(MCU) -Wl,--cref -T$(SIMBA)/src/mcus/sam3x8e/script.ld

ARCH = arm
