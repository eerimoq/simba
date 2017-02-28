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

CROSS_COMPILE = arm-none-eabi-
CFLAGS += -Werror
CXXFLAGS += -Werror

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .relocate \
			--ram-section .bss \
			--ram-section .main_stack \
			--rom-section .text \
			--rom-section .settings \
			${EXE}

ifeq ($(NDEBUG),yes)
CFLAGS += -O2
CXXFLAGS += -O2
else
CFLAGS += -g
CXXFLAGS += -g
endif

CDEFS += F_CPU=$(F_CPU)UL

CFLAGS += -mthumb \
          -mcpu=$(MCPU) \
          -ffunction-sections \
          -fdata-sections \
          -fpack-struct

CXXFLAGS += -mthumb \
          -mcpu=$(MCPU) \
          -ffunction-sections \
          -fdata-sections \
          -fpack-struct

LDFLAGS += -mcpu=$(MCPU) \
           -Wl,--cref \
           -T$(LINKER_SCRIPT) \
           -mthumb

build: $(BIN)
$(BIN): $(EXE)
	$(CROSS_COMPILE)objcopy -O binary $< $@

include $(SIMBA_ROOT)/make/gnu.mk
