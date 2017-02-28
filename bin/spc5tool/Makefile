#
# @section License
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>
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
