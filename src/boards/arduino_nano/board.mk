#
# @file src/boards/arduino_nano/board.mk
# @version 4.0.0
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

INC += $(SIMBA_ROOT)/src/boards/arduino_nano
SRC += $(SIMBA_ROOT)/src/boards/arduino_nano/board.c

BOARD_HOMEPAGE = "https://www.arduino.cc/en/Main/ArduinoBoardNano"
BOARD_PINOUT = "arduino-nano-pinout.png"
BOARD_DESC = "Arduino Nano"

MCU = atmega328p
SERIAL_PORT ?= /dev/arduino

upload:
	@echo "Uploading $(EXE)"
	avrdude -p atmega328p -D -P $(SERIAL_PORT) -c arduino -V -b 57600 -U eeprom:w:$(SETTINGS_BIN):r
	avrdude -p atmega328p -D -P $(SERIAL_PORT) -c arduino -V -b 57600 -U flash:w:$(HEX)

run:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
