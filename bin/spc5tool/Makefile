#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2016, Erik Moqvist
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

BAUDRATE ?= 115200

NAME ?= spc5tool.$(BAUDRATE)
BOARD ?= linux

CDEFS += \
	CONFIG_MINIMAL_SYSTEM=1 \
	CONFIG_FLASH=1 \
	CONFIG_UART=1 \
	CONFIG_START_CONSOLE=CONFIG_START_CONSOLE_NONE \
	CONFIG_SYSTEM_INTERRUPTS=0 \
	CONFIG_SPC5_BOOT_ENTRY_RCHW=0 \
	CONFIG_SPC5_RAM_CLEAR_ALL=0 \
	CONFIG_SYSTEM_TICK=0 \
	CONFIG_CRC_TABLE_LOOKUP=0 \
	BAUDRATE=$(BAUDRATE)

# Optimize for size.
OPT = -Os
CFLAGS += -flto
CXXFLAGS += -flto
LDFLAGS += -flto

LINKER_SCRIPT ?= script.spc5tool.ld

default:
	$(MAKE) BAUDRATE=115200 clean
	$(MAKE) BAUDRATE=115200 all
	$(MAKE) BAUDRATE=115200 install
	$(MAKE) BAUDRATE=821600 clean
	$(MAKE) BAUDRATE=921600 all
	$(MAKE) BAUDRATE=921600 install

SIMBA_ROOT ?= ../..
include $(SIMBA_ROOT)/make/app.mk

install:
	cp $(BUILDDIR)/$(NAME).bin .
