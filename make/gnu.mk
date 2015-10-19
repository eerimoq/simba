#
# @file make/gnu.mk
# @version 1.0
#
# @section License
# Copyright (C) 2014-2015, Erik Moqvist
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
          echo "Kernel package:" ; $(CROSS_COMPILE)size $(KERNEL_SRC:%.c=obj/%.o) -t ; echo ; \
          echo "Drivers package:" ; $(CROSS_COMPILE)size $(DRIVERS_SRC:%.c=obj/%.o) obj/mcu.o \
               obj/board.o -t ; echo ; \
          echo "Slib package:" ; $(CROSS_COMPILE)size $(SLIB_SRC:%.c=obj/%.o) -t ; echo ; \
          echo "Other:" ; $(CROSS_COMPILE)size $(filter-out obj/mcu.o obj/board.o \
                                                            $(KERNEL_SRC:%.c=obj/%.o) \
               $(DRIVERS_SRC:%.c=obj/%.o) $(SLIB_SRC:%.c=obj/%.o),$(OBJ) obj/simba_gen.o) -t

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc

CFLAGS += -c \
          -Wall \
          -funsigned-char \
          -funsigned-bitfields \
          -fpack-struct \
          -fshort-enums \
          -std=gnu99 \
          -Wno-error=unused-variable
LDFLAGS += -Wl,-Map=$(NAME).map

CLEAN += $(NAME).map
