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

CROSS_COMPILE = xtensa-lx106-elf-

CDEFS += F_CPU=$(F_CPU)UL \
	ICACHE_FLASH \
	__STRICT_ANSI__ \
	CONFIG_MONITOR_THREAD=0

CFLAGS += -Os \
          -Werror \
          -nostdlib \
          -mlongcalls \
          -falign-functions=4 \
          -mtext-section-literals \
          -ffunction-sections \
          -fdata-sections

CXXFLAGS += -Os \
          -Werror \
          -nostdlib \
          -mlongcalls \
          -mtext-section-literals \
          -ffunction-sections \
          -fdata-sections

LIBPATH += $(ESP8266_RTOS_SDK_ROOT)/lib

LDFLAGS += -u call_user_start \
           -Wl,--cref \
           -nostdlib \
           -Wl,-EL \
           -mtext-section-literals \
           -Wl,--gc-sections

RUNARGS = $(BIN)

ESPTOOL = $(SIMBA_ROOT)/3pp/esptool/esptool
EBOOT_ELF = $(SIMBA_ROOT)/3pp/esp8266Arduino/2.3.0/bootloaders/eboot/eboot.elf

build: $(BIN)
$(BIN): $(EXE)
	$(ESPTOOL) -eo $(EBOOT_ELF) -bo $@ -bm dio -bf 40 -bz \
		$(ESP_FLASH_SIZE) -bs .text -bp 4096 -ec \
		-eo $^ -bs .irom0.text -bs .text -bs .data -bs \
		.rodata -bc -ec

include $(SIMBA_ROOT)/make/gnu.mk
