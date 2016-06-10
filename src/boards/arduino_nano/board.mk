#
# @file simba/boards/shtest.mk
# @version 0.6.0
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

upload:
	@echo "Uploading $(EXE)"
	python -u $(RUNSCRIPT) upload ./$(EXE) $(BAUDRATE) $(SIMBA_ROOT) \
                  $(RUNLOG) $(RUN_END_PATTERN) $(RUN_END_PATTERN_SUCCESS) \
                  $(RUNARGS)

run:
	@echo "Running $(EXE)"
	python -u $(RUNSCRIPT) run ./$(EXE) $(BAUDRATE) $(SIMBA_ROOT) \
                  $(RUNLOG) $(RUN_END_PATTERN) $(RUN_END_PATTERN_SUCCESS) \
                  $(RUNARGS)
