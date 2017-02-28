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

CROSS_COMPILE = xtensa-lx106-elf-

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .data \
			--ram-section .rodata \
			--ram-section .bss \
			--rom-section .text \
			--rom-section .irom0.text \
			${EXE}

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
