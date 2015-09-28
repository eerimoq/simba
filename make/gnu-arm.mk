#
# @file make/gnu-arm.mk
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

CROSS_COMPILE = arm-none-eabi-
CFLAGS += -Werror \
          -Wno-error=unused-variable \
          -fdata-sections \
          -ffunction-sections
LDFLAGS += -Wl,--gc-sections

ENDIANESS = little

CFLAGS += -mthumb -mcpu=$(MCPU) -DF_CPU=$(F_CPU)UL -funsigned-char \
          -funsigned-bitfields -fpack-struct -fshort-enums \
          -std=gnu99
LDFLAGS += -mcpu=$(MCPU) -DF_CPU=$(F_CPU)UL -Wl,--cref \
           -T$(SIMBA)/src/mcus/$(MCU)/script.ld

all: $(NAME).hex
$(NAME).hex: $(EXE)
	$(CROSS_COMPILE)objcopy -O ihex $< $@

include $(SIMBA)/make/gnu.mk
