#
# @file make/gnu.mk
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

SIZECMD = $(CROSS_COMPILE)size $(SIZEARGS) ${EXE} ; \
          echo ; \
          $(CROSS_COMPILE)size $(OBJ) -t

SIZE_SUMMARY_CMD = $(CROSS_COMPILE)size ${EXE} | python -c "import sys; text, data, bss = sys.stdin.readlines()[1].split()[0:3]; print '{{\"program\": {}, \"data\": {}}}'.format(text, int(data) + int(bss))"


CC = $(CROSS_COMPILE)gcc$(CCVERSION:%=-%)

CFLAGS += -c \
          -Wall \
          -funsigned-bitfields \
          -std=gnu99 \
          -Wno-error=unused-variable

LDFLAGS += -Wl,-Map=$(MAP) \
           -Wl,--gc-sections
