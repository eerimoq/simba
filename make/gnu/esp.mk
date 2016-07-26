#
# @file make/esp.mk
# @version 3.0.0
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

ENDIANESS = little

SETTING_MEMORY = flash
SETTING_OFFSET = 128
SETTING_SIZE = 256
SRC += settings.c

RUNARGS = ${NAME}.bin

build: $(NAME).bin
$(NAME).bin: $(EXE)
	rm -f eagle.app.flash.bin eagle.app.v6.text.bin eagle.app.v6.data.bin eagle.app.v6.rodata.bin eagle.app.v6.irom0text.bin
	$(CROSS_COMPILE)objcopy --only-section .text -O binary $< eagle.app.v6.text.bin
	$(CROSS_COMPILE)objcopy --only-section .data -O binary $< eagle.app.v6.data.bin
	$(CROSS_COMPILE)objcopy --only-section .rodata -O binary $< eagle.app.v6.rodata.bin
	$(CROSS_COMPILE)objcopy --only-section .irom0.text -O binary $< eagle.app.v6.irom0text.bin
	python $(ESP8266_RTOS_SDK_ROOT)/tools/gen_appbin.py $< 2 0 0 $(ESP_FLASH_SIZE_MAP)
	mv eagle.app.flash.bin $(NAME).bin
	rm eagle.app.v6.text.bin eagle.app.v6.data.bin eagle.app.v6.rodata.bin eagle.app.v6.irom0text.bin


include $(SIMBA_ROOT)/make/gnu.mk
