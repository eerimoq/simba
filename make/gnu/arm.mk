#
# @file make/gnu/arm.mk
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

CROSS_COMPILE = arm-none-eabi-
CFLAGS += -Werror
CXXFLAGS += -Werror

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
