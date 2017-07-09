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

ESP8266_RTOS_SDK_ROOT ?= $(SIMBA_ROOT)/3pp/ESP8266_RTOS_SDK

INC += $(SIMBA_ROOT)/src/mcus/esp8266 \
       $(ESP8266_RTOS_SDK_ROOT)/extra_include \
       $(ESP8266_RTOS_SDK_ROOT)/include \
       $(ESP8266_RTOS_SDK_ROOT)/include/espressif \
       $(ESP8266_RTOS_SDK_ROOT)/include/espressif/esp8266 \
       $(ESP8266_RTOS_SDK_ROOT)/include/lwip \
       $(ESP8266_RTOS_SDK_ROOT)/include/lwip/ipv4 \
       $(ESP8266_RTOS_SDK_ROOT)/include/lwip/ipv6

SRC += $(SIMBA_ROOT)/src/mcus/esp8266/mcu.c \
       $(SIMBA_ROOT)/src/mcus/esp8266/esp8266.c

LIBPATH += "$(SIMBA_ROOT)/src/mcus/esp8266/ld"

LDFLAGS += -Wl,-T$(LINKER_SCRIPT)

LIB_MINIC ?= minic
LIB_CIROM ?= cirom

LIB += \
	hal \
	gcc \
	phy \
	pp \
	net80211 \
	wpa \
	crypto \
	main \
	freertos \
	lwip \
	m \
	$(LIB_CIROM) \
	stdc++ \
	$(LIB_MINIC)

F_CPU = 80000000

ARCH = esp
FAMILY = esp

MCU_HOMEPAGE = "http://www.espressif.com"
MCU_NAME = "Espressif ESP8266"
MCU_DESC = "Espressif ESP8266 @ 80 MHz, 82 kB dram, 4 MB flash"

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .data \
			--ram-section .rodata \
			--ram-section .bss \
			--rom-section .text \
			--rom-section .irom0.text \
			${EXE}

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/esp.mk
