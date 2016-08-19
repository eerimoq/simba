#
# @file src/boards/arduino_mega/board.mk
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
