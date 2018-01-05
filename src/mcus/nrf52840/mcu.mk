#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2018, Erik Moqvist
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

INC += $(SIMBA_ROOT)/src/mcus/nrf52840
SRC += $(SIMBA_ROOT)/src/mcus/nrf52840/mcu.c \
       $(SIMBA_ROOT)/src/mcus/nrf52840/nrf52840.c

MCPU = cortex-m4
F_CPU = 64000000

ARCH = arm
FAMILY = nrf5

MCU_HOMEPAGE = "https://www.nordicsemi.com/eng/Products/nRF52840"
MCU_NAME = "nRF52840 ARM Cortex-M4"
MCU_DESC = "nRF52840 ARM Cortex-M4 @ 64MHz, 256k sram, 1M flash"

LIBPATH += "$(SIMBA_ROOT)/src/mcus/$(MCU)"
LINKER_SCRIPT ?= script.ld

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/arm.mk
