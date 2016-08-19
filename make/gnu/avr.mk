#
# @file make/gnu-avr.mk
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

CROSS_COMPILE = avr-

SIZEARGS = --mcu=$(MCU) --format=avr

CDEFS += F_CPU=$(F_CPU)UL \
	__DELAY_BACKWARD_COMPATIBLE__

CFLAGS += -mmcu=$(CPU) \
          -O2 \
          -ffunction-sections \
          -fdata-sections \
          -fpack-struct \
          -fshort-enums

LDFLAGS += -mmcu=$(CPU) \
           -Wl,--cref \
           -Wl,--defsym=__main_stack_end=$(MAIN_STACK_END)

# Use the environment variable AVRDUDE_PORT as default
AVRDUDE_PORT ?= $(shell python -c "import os, sys; sys.stdout.write(os.environ['AVRDUDE_PORT'] if 'AVRDUDE_PORT' in os.environ else '/dev/arduino')")
AVRDUDE_NO_VERIFY ?= -V
RUNARGS = ${MCU} ${HEX} ${SETTINGS_BIN} -D -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
          $(AVRDUDE_NO_VERIFY) -b $(AVRDUDE_BAUDRATE)

build: $(HEX)
$(HEX): $(EXE)
	@echo "Creating $@"
	$(CROSS_COMPILE)objcopy -O ihex -R .eeprom $< $@

HELP_VARIABLES += "  AVRDUDE_PORT                avrdude serial port" $$(echo -e '\n') \
                  "  AVRDUDE_BAUDRATE            avrdude serial baudrate" $$(echo -e '\n')

include $(SIMBA_ROOT)/make/gnu.mk
