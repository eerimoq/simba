#
# @file src/boards/arduino_pro_micro/board.mk
# @version 2.0.0
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

upload:
	@echo "Uploading $(EXE)"
	python -u $(BOARD_PY) upload --port $(SERIAL_PORT) $(NAME).hex

run:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    --dtr 1 \
			    --rts 1 \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
