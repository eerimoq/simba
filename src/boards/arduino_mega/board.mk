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

INC += $(SIMBA_ROOT)/src/boards/arduino_mega
SRC += $(SIMBA_ROOT)/src/boards/arduino_mega/board.c

BOARD_HOMEPAGE = "https://www.arduino.cc/en/Main/ArduinoBoardMega"
BOARD_PINOUT = "arduino-mega-pinout.png"
BOARD_DESC = "Arduino Mega"

MCU = atmega2560
SERIAL_PORT ?= /dev/arduino
TIMEOUT ?= 10

upload:
	@echo "Uploading $(EXE)"
	avrdude -p atmega2560 -D -P $(SERIAL_PORT) -c wiring -V -b 115200 -U eeprom:w:$(SETTINGS_BIN):r
	avrdude -p atmega2560 -D -P $(SERIAL_PORT) -c wiring -V -b 115200 -U flash:w:$(HEX)

rerun:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
