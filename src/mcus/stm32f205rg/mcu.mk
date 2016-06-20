#
# @file mcus/stm32f100rb/mcu.mk
# @version 1.0.0
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

INC += $(SIMBA_ROOT)/src/mcus/stm32f100rb
SRC += $(SIMBA_ROOT)/src/mcus/stm32f100rb/mcu.c \
       $(SIMBA_ROOT)/src/mcus/stm32f100rb/stm32.c

MCPU = cortex-m3
F_CPU = 12000000

ARCH = arm
FAMILY = stm32f2

MCU_HOMEPAGE = ""
MCU_NAME = "ST STM32F205FG ARM Cortex-M3"
MCU_DESC = "ST STM32F205FG ARM Cortex-M3 @ 120MHz, 128k sram, 1M flash"

LIBPATH += "$(SIMBA_ROOT)/src/mcus/$(MCU)"
LINKER_SCRIPT_FILE ?= script.ld
LINKER_SCRIPT ?= $(SIMBA_ROOT)/src/mcus/$(MCU)/$(LINKER_SCRIPT_FILE)

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/arm.mk
