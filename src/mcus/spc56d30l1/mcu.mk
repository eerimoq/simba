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

INC += \
	$(SIMBA_ROOT)/src/mcus/spc56d30l1 \
	$(SIMBA_ROOT)/src/mcus/spc5
SRC += \
	$(SIMBA_ROOT)/src/mcus/spc5/spc5.c \
	$(SIMBA_ROOT)/src/mcus/spc5/spc5.S \
	$(SIMBA_ROOT)/src/mcus/spc56d30l1/mcu.c

F_CPU = 48000000
MCPU = e200z0

LIBPATH += "$(SIMBA_ROOT)/src/mcus/spc56d30l1"
LINKER_SCRIPT ?= script.ld

ARCH = ppc
FAMILY = spc5

MCU_HOMEPAGE = ""
MCU_NAME = ""
MCU_DESC = "SPC56D30L1 PPC @ 48MHz, 12k SRAM, 128k CFlash, 64k DFlash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/ppc.mk
