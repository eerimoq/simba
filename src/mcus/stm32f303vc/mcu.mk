#
# @file mcus/stm32f303vc/mcu.mk
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

INC += $(SIMBA_ROOT)/src/mcus/stm32f303vc
SRC += $(SIMBA_ROOT)/src/mcus/stm32f303vc/mcu.c \
       $(SIMBA_ROOT)/src/mcus/stm32f303vc/stm32.c

MCPU = cortex-m4
F_CPU = 72000000

ARCH = arm
FAMILY = stm32f3

MCU_HOMEPAGE = "http://www.st.com/content/st_com/en/products/microcontrollers/stm32-32-bit-arm-cortex-mcus/stm32f3-series/stm32f303/stm32f303vc.html"
MCU_NAME = "ST STM32F303VC ARM Cortex-M4"
MCU_DESC = "ST STM32F303VC ARM Cortex-M4 @ 72MHz, 40k sram, 256k flash"

LIBPATH += "$(SIMBA_ROOT)/src/mcus/$(MCU)"
LINKER_SCRIPT ?= script.ld

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/arm.mk
