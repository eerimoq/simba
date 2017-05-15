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

CROSS_COMPILE = powerpc-eabivle-

LIBPATH += $(FREESCALE_POWERPC_EABIVLE_ROOT)/powerpc-eabivle/newlib/lib

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .relocate \
			--ram-section .bss \
			--ram-section .main_stack \
			--rom-section .text \
			${EXE}

CDEFS += F_CPU=$(F_CPU)UL

OPT ?= -O2

CFLAGS += \
	-Werror \
	-mcpu=$(MCPU) \
	$(OPT) \
        -ffunction-sections \
        -fdata-sections \
        -fpack-struct \
	-fstack-usage

CXXFLAGS += \
	-Werror \
	-mcpu=$(MCPU) \
	$(OPT) \
        -ffunction-sections \
        -fdata-sections \
        -fpack-struct \
	-fstack-usage

LDFLAGS += \
	-mcpu=$(MCPU) \
        -Wl,--cref \
        -T$(LINKER_SCRIPT)


ENDIANESS = big

build: $(BIN) $(HEX)

$(BIN): $(EXE)
	$(CROSS_COMPILE)objcopy -O binary $< $@

$(HEX): $(EXE)
	@echo "Creating $@"
	$(CROSS_COMPILE)objcopy -O ihex $< $@

include $(SIMBA_ROOT)/make/gnu.mk
