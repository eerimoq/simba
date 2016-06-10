#
# @file boards/linux/board.mk
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

INC += $(SIMBA_ROOT)/src/boards/linux
SRC += $(SIMBA_ROOT)/src/boards/linux/board.c

BOARD_HOMEPAGE = "http://www.kernel.org"
BOARD_PINOUT = "linux-pinout.png"
BOARD_DESC = "Linux"

MCU = linux

upload:

run:
	@echo "Running $(EXE)"
	python -u $(RUNSCRIPT) run ./$(EXE) $(BAUDRATE) $(SIMBA_ROOT) \
                  $(RUNLOG) $(RUN_END_PATTERN) $(RUN_END_PATTERN_SUCCESS) \
                  $(RUNARGS)
