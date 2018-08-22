#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2018, Erik Moqvist
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
	$(SIMBA_ROOT)/src/mcus/pic32mm0256gpm048 \
	$(SIMBA_ROOT)/src/mcus/pic32mm
SRC += \
	$(SIMBA_ROOT)/src/mcus/pic32mm/pic32mm.c \
	$(SIMBA_ROOT)/src/mcus/pic32mm/pic32mm.S \
	$(SIMBA_ROOT)/src/mcus/pic32mm0256gpm048/mcu.c \
	$(SIMBA_ROOT)/3pp/libc/string0.c

F_CPU = 25000000
MCPU = e200z0

LIBPATH += "$(SIMBA_ROOT)/src/mcus/pic32mm0256gpm048"
LINKER_SCRIPT ?= script.ld

ARCH = mips
FAMILY = pic32mm

MCU_HOMEPAGE = ""
MCU_NAME = ""
MCU_DESC = "PIC32MM0256GPM048 @ 25MHz, 32k RAM, 256k Flash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/mips.mk
