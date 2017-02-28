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

CROSS_COMPILE = xtensa-esp32-elf-

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .dram0.bss \
			--ram-section .dram0.data \
			--rom-section .flash.rodata \
			--rom-section .flash.text \
			${EXE}

CDEFS += F_CPU=$(F_CPU)UL

CFLAGS += -Os \
          -Werror \
          -nostdlib \
          -mlongcalls \
          -ffunction-sections \
          -fdata-sections

CXXFLAGS += -Os \
          -Werror \
          -nostdlib \
          -mlongcalls \
          -ffunction-sections \
          -fdata-sections

LDFLAGS += -Wl,-static \
           -nostdlib \
           -Wl,-EL

RUNARGS = $(BIN)

ESPTOOL_PY = $(SIMBA_ROOT)/3pp/esp32/esp-idf/components/esptool_py/esptool/esptool.py
UPGRADE_PY = $(SIMBA_ROOT)/bin/upgrade.py

build: $(BIN) $(UBIN)

$(BIN): $(EXE)
	@echo "Creating $@"
	$(ESPTOOL_PY) --chip esp32 elf2image --flash_mode dio --flash_freq 40m --flash_size 2MB -o $@ $<

$(UBIN): $(BIN)
	@echo "Creating $@"
	$(UPGRADE_PY) -d $(UPGRADE_BINARY_DESCRIPTION) -o $@ $<

include $(SIMBA_ROOT)/make/gnu.mk
