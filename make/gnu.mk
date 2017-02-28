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

SIZECMD = $(CROSS_COMPILE)size $(SIZEARGS) ${EXE} ; \
          echo ; \
          $(CROSS_COMPILE)size $(OBJ) -t | sort -n -k4 ; \
          $(CROSS_COMPILE)readelf -W -s $(EXE) | sort -n -k3 | \
	      awk '{if ($$3 != 0) {print $$0}}' | grep -v "Symbol table"

SIZE_SUMMARY_CMD ?= $(CROSS_COMPILE)size ${EXE} | python -c "import sys; text, data, bss = sys.stdin.readlines()[1].split()[0:3]; print '{{\"program\": {}, \"data\": {}}}'.format(text, int(data) + int(bss))"

CC = $(CROSS_COMPILE)gcc$(CCVERSION:%=-%)
CXX = $(CROSS_COMPILE)g++$(CCVERSION:%=-%)

CFLAGS += -c \
          -Wall \
          -funsigned-bitfields \
          -std=gnu99 \
          -Wno-error=unused-variable

CXXFLAGS += -c \
          -Wall \
          -funsigned-bitfields \
          -std=c++11 \
          -Wno-error=unused-variable

LDFLAGS += -Wl,-Map=$(MAP) \
           -Wl,--gc-sections
