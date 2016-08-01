#
# @file mcus/sam/3x8e/mcu.mk
# @version 4.1.0
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

MCU_NAME = "Atmel SAM3X8E Cortex-M3"
MCU_DESC = "Atmel SAM3X8E Cortex-M3 @ 84MHz, 96k sram, 512k flash"

LIBPATH += "$(SIMBA_ROOT)/src/mcus/$(MCU)"
LINKER_SCRIPT ?= script.ld

include $(SIMBA_ROOT)/src/mcus/sam/sam3.mk
