#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2017, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Simba project.
#

CROSS_COMPILE = avr-

SIZE_SUMMARY_CMD ?= $(SIMBA_ROOT)/bin/memory_usage.py \
			--ram-section .data \
			--ram-section .bss \
			--ram-section .noinit \
			--rom-section .text \
			${EXE}

SIZEARGS = --mcu=$(MCU) --format=avr

OPT ?= -O2

CDEFS += F_CPU=$(F_CPU)UL \
	__DELAY_BACKWARD_COMPATIBLE__

CFLAGS += \
	-mmcu=$(CPU) \
        $(OPT) \
        -ffunction-sections \
        -fdata-sections \
        -fpack-struct \
        -fshort-enums

CXXFLAGS += \
	-mmcu=$(CPU) \
        $(OPT) \
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
RUNARGS = $(MCU) $(HEX) $(SETTINGS_BIN) -D -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
          $(AVRDUDE_NO_VERIFY) -b $(AVRDUDE_BAUDRATE)

build: $(HEX)
$(HEX): $(EXE)
	@echo "Creating $@"
	$(CROSS_COMPILE)objcopy -O ihex -R .eeprom $< $@

HELP_VARIABLES += "  AVRDUDE_PORT                avrdude serial port" $$(echo -e '\n') \
                  "  AVRDUDE_BAUDRATE            avrdude serial baudrate" $$(echo -e '\n')

include $(SIMBA_ROOT)/make/gnu.mk
