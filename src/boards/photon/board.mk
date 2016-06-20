#
# @file src/boards/photon/board.mk
# @version 1.0.0
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

INC += $(SIMBA_ROOT)/src/boards/photon
SRC += $(SIMBA_ROOT)/src/boards/photon/board.c

BOARD_HOMEPAGE = "https://docs.particle.io/datasheets/photon-datasheet/"
BOARD_PINOUT = "particle-io-photon.png"
BOARD_DESC = "Particle IO Photon"

MCU = stm32f205rg
SERIAL_PORT = arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/photon/board.py

upload:
	@echo "Uploading $(NAME).bin"
	python -u $(BOARD_PY) upload --port $(SERIAL_PORT) $(NAME).bin

run:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
