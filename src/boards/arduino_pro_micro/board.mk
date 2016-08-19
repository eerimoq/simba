#
# @file src/boards/arduino_pro_micro/board.mk
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

INC += $(SIMBA_ROOT)/src/boards/arduino_pro_micro
SRC += $(SIMBA_ROOT)/src/boards/arduino_pro_micro/board.c

BOARD_HOMEPAGE = "https://www.sparkfun.com/products/12640"
BOARD_PINOUT = "arduino-pro-micro-pinout.png"
BOARD_DESC = "Arduino Pro Micro"

MCU = atmega32u4

SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/arduino_pro_micro/board.py
RUN_PY = $(SIMBA_ROOT)/src/boards/arduino_pro_micro/run.py
TIMEOUT ?= 10

upload:
	@echo "Uploading $(EXE)"
	python -u $(BOARD_PY) upload --port $(SERIAL_PORT) $(HEX)

rerun:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
