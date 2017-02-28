#
# @section License
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>
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

CDEFS += F_CPU=$(F_CPU)UL \
	__DELAY_BACKWARD_COMPATIBLE__

CFLAGS += -mmcu=$(CPU) \
          -O2 \
          -ffunction-sections \
          -fdata-sections \
          -fpack-struct \
          -fshort-enums

CXXFLAGS += -mmcu=$(CPU) \
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
