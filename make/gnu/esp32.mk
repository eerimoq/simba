#
# @file make/esp.mk
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

CROSS_COMPILE = xtensa-esp32-elf-

CDEFS += F_CPU=$(F_CPU)UL \
	ICACHE_FLASH \
	__STRICT_ANSI__ \
	__ets__

CFLAGS += -Os \
          -Werror \
          -nostdlib \
          -mlongcalls \
          -mtext-section-literals \
          -ffunction-sections \
	  -fno-inline-functions \
	  -fsingle-precision-constant \
          -fdata-sections

CXXFLAGS += -Os \
          -Werror \
          -nostdlib \
          -mlongcalls \
          -mtext-section-literals \
          -ffunction-sections \
          -fdata-sections

LIBPATH += $(ESP_IDF_ROOT)/components/esp32/lib

LDFLAGS += -u call_user_start \
           -Wl,--cref \
           -nostdlib \
           -Wl,-EL \
           -mtext-section-literals \
           -Wl,--gc-sections

RUNARGS = $(BIN)

ESPTOOL = $(SIMBA_ROOT)/3pp/esptool/esptool

include $(SIMBA_ROOT)/make/gnu.mk
