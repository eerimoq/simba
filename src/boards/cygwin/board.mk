#
# @file src/boards/cygwin/board.mk
# @version 3.1.1
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

BOARD_HOMEPAGE = "http://www.cygwin.com"
BOARD_PINOUT = "cygwin-pinout.png"
BOARD_DESC = "Cygwin"

MCU = linux

upload:

run:
	@echo "Running $(EXE)"
	./$(EXE) | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0

run-debugger: all
	gdb $(EXE) --eval-command "break main" --eval-command run

profile:
	gprof $(EXE)

coverage:
	geninfo . -o coverage.info
	genhtml coverage.info

jenkins-coverage:
	gcovr -r $(readlink -f ../../..) -x -e ".*main.c"
