#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2018, Erik Moqvist
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

CROSS_COMPILE = mips-unknown-elf-

# LIBPATH += $(FREESCALE_POWERPC_EABIVLE_ROOT)/powerpc-eabivle/newlib/lib

# SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
# 			--ram-section .relocate \
# 			--ram-section .bss \
# 			--ram-section .main_stack \
# 			--rom-section .text \
# 			${EXE}

CDEFS += F_CPU=$(F_CPU)UL

OPT ?= -O2

CFLAGS += \
	-Werror \
	-mips3 \
	-mmicromips \
	-minterlink-compressed \
	$(OPT) \
        -ffunction-sections \
        -fdata-sections \
        -fpack-struct \
	-fstack-usage \
	-Wno-maybe-uninitialized \
	-Wno-stringop-truncation \
	-nostdlib \
	-G0

CXXFLAGS += \
	-Werror \
	-mips3 \
	-mmicromips \
	-minterlink-compressed \
	$(OPT) \
        -ffunction-sections \
        -fdata-sections \
        -fpack-struct \
	-fstack-usage \
	-nostdlib \
	-G0

LDFLAGS += \
        -Wl,--cref \
	-mips3 \
	-mmicromips \
	-minterlink-compressed \
        -T$(LINKER_SCRIPT) \
	-nostdlib \
	-G0

ENDIANESS = big

build: $(BIN) $(S19)

$(BIN): $(EXE)
	$(CROSS_COMPILE)objcopy -O binary $< $@

$(S19): $(EXE)
	@echo "Creating $@"
	$(CROSS_COMPILE)objcopy -O srec $< $@

include $(SIMBA_ROOT)/make/gnu.mk
