#
# @file make/gnu.mk
# @version 0.3.0
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

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc

CFLAGS += -c \
          -Wall \
          -funsigned-char \
          -funsigned-bitfields \
          -fshort-enums \
          -std=gnu99 \
          -Wno-error=unused-variable

LDFLAGS += -Wl,-Map=$(NAME).map \
           -Wl,--gc-sections

CLEAN += $(NAME).map
