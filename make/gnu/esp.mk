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

CROSS_COMPILE = xtensa-lx106-elf-

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .data \
			--ram-section .rodata \
			--ram-section .bss \
			--rom-section .text \
			--rom-section .irom0.text \
			${EXE}

OPT ?= -Os

CDEFS += F_CPU=$(F_CPU)UL \
	ICACHE_FLASH \
	__STRICT_ANSI__ \
	__ets__

CFLAGS += \
	$(OPT) \
        -Werror \
        -nostdlib \
        -mlongcalls \
        -mtext-section-literals \
        -ffunction-sections \
	-fno-inline-functions \
	-fsingle-precision-constant \
        -fdata-sections

CXXFLAGS += \
	$(OPT) \
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
