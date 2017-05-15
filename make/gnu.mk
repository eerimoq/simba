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

SIZECMD = $(CROSS_COMPILE)size $(SIZEARGS) $(EXE) ; \
	  echo ; \
	  $(CROSS_COMPILE)size $(OBJ) $(SIMBA_GEN_O) -t | sort -n -k4 ; \
	  echo ; \
	  $(CROSS_COMPILE)readelf -W -S $(EXE) ; \
	  $(CROSS_COMPILE)readelf -W -s $(EXE) | sort -n -k3 | \
	      awk '{if ($$3 != 0) {print $$0}}' | grep -v "Symbol table"

SIZE_SUMMARY_CMD ?= $(CROSS_COMPILE)size $(EXE) | python -c "import sys; text, data, bss = sys.stdin.readlines()[1].split()[0:3]; print '{{\"program\": {}, \"data\": {}}}'.format(text, int(data) + int(bss))"

CC = $(CROSS_COMPILE)gcc$(CCVERSION:%=-%)
CXX = $(CROSS_COMPILE)g++$(CCVERSION:%=-%)

CFLAGS += \
	-c \
	-Wall \
	-funsigned-bitfields \
	-std=gnu99 \
	-Wno-error=unused-variable

CXXFLAGS += \
	-c \
	-Wall \
	-funsigned-bitfields \
	-std=c++11 \
	-Wno-error=unused-variable

LDFLAGS += \
	-Wl,-Map=$(MAP) \
	-Wl,--gc-sections

backtrace:
	$(CROSS_COMPILE)addr2line -f -p -C -a -e $(EXE) $(BACKTRACE)
