#
# @file make/gnu/arm.mk
# @version 0.2.0
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
CFLAGS += -Werror \
          -O2

ENDIANESS = little

CFLAGS += -mthumb \
          -mcpu=$(MCPU) \
          -DF_CPU=$(F_CPU)UL \
          -ffunction-sections \
          -fdata-sections \
          -fpack-struct

LDFLAGS += -mcpu=$(MCPU) \
           -DF_CPU=$(F_CPU)UL \
           -Wl,--cref \
           -T$(SIMBA_ROOT)/src/mcus/$(MCU)/script.ld \
           -mthumb

SETTING_MEMORY = flash
SETTING_OFFSET = 0
SETTING_SIZE = 4096
SRC += settings.c

RUNARGS = ${NAME}.bin

build: $(NAME).bin
$(NAME).bin: $(EXE)
	$(CROSS_COMPILE)objcopy -O binary $< $@

include $(SIMBA_ROOT)/make/gnu.mk
