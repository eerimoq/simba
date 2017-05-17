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

MCU ?= spc56d40l1
BAUDRATE ?= 115200
BAM_BAUDRATE ?= 19200
BOARD ?= spc56ddiscovery

NAME ?= spc5tool-$(MCU)-$(BAUDRATE)-$(BAM_BAUDRATE)

CDEFS += \
	CONFIG_MINIMAL_SYSTEM=1 \
	CONFIG_FLASH=1 \
	CONFIG_FLASH_DEVICE_SEMAPHORE=0 \
	CONFIG_UART=1 \
	CONFIG_START_CONSOLE=CONFIG_START_CONSOLE_NONE \
	CONFIG_SYSTEM_INTERRUPTS=0 \
	CONFIG_SPC5_BOOT_ENTRY_RCHW=0 \
	CONFIG_SPC5_RAM_CLEAR_ALL=0 \
	CONFIG_SYSTEM_TICK=0 \
	CONFIG_CRC_TABLE_LOOKUP=0 \
	BAUDRATE=$(BAUDRATE)

ifeq ($(BAM_BAUDRATE), 9600)
CDEFS += CONFIG_EXTERNAL_OSCILLATOR_FREQUENCY_HZ=8000000
endif

# Optimize for size.
OPT = -Os
CFLAGS += -flto
CXXFLAGS += -flto
LDFLAGS += -flto

LINKER_SCRIPT ?= script.spc5tool.ld

default:
	for mcu in spc56d30l1 spc56d40l1 ; do \
	    for baudrate in 115200 921600 ; do \
	        for bam_baudrate in 9600 19200 ; do \
	            $(MAKE) MCU=$${mcu} BAUDRATE=$${baudrate} BAM_BAUDRATE=$${bam_baudrate} clean ; \
	            $(MAKE) MCU=$${mcu} BAUDRATE=$${baudrate} BAM_BAUDRATE=$${bam_baudrate} all ; \
	            $(MAKE) MCU=$${mcu} BAUDRATE=$${baudrate} BAM_BAUDRATE=$${bam_baudrate} install ; \
	        done ; \
	    done ; \
	done

SIMBA_ROOT ?= ../..
include $(SIMBA_ROOT)/make/app.mk

install:
	cp $(BUILDDIR)/$(NAME).bin .
