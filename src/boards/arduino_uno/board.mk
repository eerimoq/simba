#
# @file src/boards/arduino_uno/board.mk
# @version 6.0.0
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

INC += $(SIMBA_ROOT)/src/boards/arduino_uno
SRC += $(SIMBA_ROOT)/src/boards/arduino_uno/board.c

BOARD_HOMEPAGE = "https://www.arduino.cc/en/Main/ArduinoBoardUno"
BOARD_PINOUT = "arduino-uno-pinout.png"
BOARD_DESC = "Arduino Uno"

MCU = atmega328p

upload:
	@echo "Uploading $(EXE)"
	avrdude -p atmega328p -D -P $(SERIAL_PORT) -c arduino -V -b 115200 -U eeprom:w:$(SETTINGS_BIN):r
	avrdude -p atmega328p -D -P $(SERIAL_PORT) -c arduino -V -b 115200 -U flash:w:$(HEX)

rerun:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
