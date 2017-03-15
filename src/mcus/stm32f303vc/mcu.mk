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
