#
# @file mcus/esp32/mcu.mk
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

ESP_IDF_ROOT ?= $(SIMBA_ROOT)/3pp/esp-idf

INC += $(SIMBA_ROOT)/src/mcus/esp32 \
       $(ESP_IDF_ROOT)/components/esp32/include \
       $(ESP_IDF_ROOT)/components/freertos/include

SRC += $(SIMBA_ROOT)/src/mcus/esp32/mcu.c \
       $(SIMBA_ROOT)/src/mcus/esp32/esp32.c

LIBPATH += "$(SIMBA_ROOT)/src/mcus/esp32/ld" \
	$(ESP_IDF_ROOT)/components/esp32/ld \
	$(ESP_IDF_ROOT)/components/esp32

LDFLAGS += -Wl,--start-group \
            $(ESPLIBS:%=-l%) \
            -Wl,--end-group \
            -Wl,-T$(LINKER_SCRIPT)

LDFLAGS_AFTER += \
	$(ESPLIBS_AFTER:%=-l%)

ESPLIBS += \
 hal \
 phy \
 pp \
 net80211 \
 wpa \
 crypto

ESPLIBS_AFTER += \
 gcc

# Used by arduino script to find the libraries.
LIB ?= $(ESPLIBS) $(ESPLIBS_AFTER)

F_CPU = 240000000

ARCH = esp32
FAMILY = esp32

MCU_HOMEPAGE = "http://www.espressif.com"
MCU_NAME = "Espressif ESP32"
MCU_DESC = "Espressif ESP32 @ 80 MHz, 82 kB dram, 4 MB flash"

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .data \
			--ram-section .rodata \
			--ram-section .bss \
			--rom-section .text \
			--rom-section .irom0.text \
			${EXE}

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/esp32.mk
