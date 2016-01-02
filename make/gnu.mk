#
# @file make/gnu.mk
# @version 0.1.0
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

KERNEL_OBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(KERNEL_SRC:%.c=%.o)))
DRIVERS_OBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(DRIVERS_SRC:%.c=%.o)))
SLIB_OBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(SLIB_SRC:%.c=%.o)))

SIZECMD = $(CROSS_COMPILE)size $(SIZEARGS) ${EXE} ; \
          echo "Kernel package:" ; $(CROSS_COMPILE)size $(KERNEL_OBJ) -t ; echo ; \
          echo "Drivers package:" ; $(CROSS_COMPILE)size $(DRIVERS_OBJ) obj/mcu.o \
               obj/board.o -t ; echo ; \
          echo "Slib package:" ; $(CROSS_COMPILE)size $(SLIB_OBJ) -t ; echo ; \
          echo "Other:" ; $(CROSS_COMPILE)size $(filter-out obj/mcu.o obj/board.o \
               $(KERNEL_OBJ) $(DRIVERS_OBJ) $(SLIB_OBJ),$(OBJ) obj/simba_gen.o) -t

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc

CFLAGS += -c \
          -Wall \
          -funsigned-char \
          -funsigned-bitfields \
          -ffunction-sections \
          -fdata-sections \
          -fshort-enums \
          -std=gnu99 \
          -Wno-error=unused-variable

LDFLAGS += -Wl,-Map=$(NAME).map \
           -Wl,--gc-sections

CLEAN += $(NAME).map
