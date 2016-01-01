#
# @file make/gnu-avr.mk
# @version 0.1.0
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

CROSS_COMPILE = avr-

SIZEARGS = --mcu=$(MCU) --format=avr

CFLAGS += -mmcu=$(CPU) \
          -DF_CPU=$(F_CPU)UL \
          -D__DELAY_BACKWARD_COMPATIBLE__ \
          -O2 \
          -fdata-sections \
          -fpack-struct \
          -ffunction-sections

LDFLAGS += -mmcu=$(CPU) \
           -DF_CPU=$(F_CPU)UL \
           -Wl,--cref \
           -Wl,--defsym=__main_stack_end=$(MAIN_STACK_END) \
           -Wl,--gc-sections

ENDIANESS = little

SETTING_MEMORY = eeprom
SETTING_OFFSET = 128
SETTING_SIZE = 256
SRC += settings.c

# Use the environment variable AVRDUDE_PORT as default
AVRDUDE_PORT ?= $(shell python -c "import os, sys; sys.stdout.write(os.environ['AVRDUDE_PORT'] if 'AVRDUDE_PORT' in os.environ else '/dev/arduino')")
AVRDUDE_NO_VERIFY ?= -V
RUNARGS = ${MCU} ${NAME}.hex ${SETTINGS_BIN} -D -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
          $(AVRDUDE_NO_VERIFY) -b $(AVRDUDE_BAUDRATE)

CLEAN += $(NAME).hex

build: $(NAME).hex
$(NAME).hex: $(EXE)
	$(CROSS_COMPILE)objcopy -O ihex -R .eeprom $< $@

HELP_VARIABLES += "  AVRDUDE_PORT                avrdude serial port" $$(echo -e '\n') \
                  "  AVRDUDE_BAUDRATE            avrdude serial baudrate" $$(echo -e '\n')

include $(SIMBA_ROOT)/make/gnu.mk
