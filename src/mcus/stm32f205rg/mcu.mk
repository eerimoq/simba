#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2017, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Simba project.
#

INC += $(SIMBA_ROOT)/src/mcus/stm32f205rg
SRC += $(SIMBA_ROOT)/src/mcus/stm32f205rg/mcu.c \
       $(SIMBA_ROOT)/src/mcus/stm32f205rg/stm32.c

MCPU = cortex-m3
F_CPU = 120000000

ARCH = arm
FAMILY = stm32f2

MCU_HOMEPAGE = ""
MCU_NAME = "ST STM32F205RG ARM Cortex-M3"
MCU_DESC = "ST STM32F205RG ARM Cortex-M3 @ 120MHz, 128k sram, 1M flash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/arm.mk
