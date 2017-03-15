#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2017, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Simba project.
#

INC += $(SIMBA_ROOT)/src/boards/photon
SRC += $(SIMBA_ROOT)/src/boards/photon/board.c

BOARD_HOMEPAGE = "https://docs.particle.io/datasheets/photon-datasheet/"
BOARD_PINOUT = "photon-pinout.png"
BOARD_DESC = "Particle IO Photon"

LIBPATH += "$(SIMBA_ROOT)/src/boards/$(BOARD)"
LINKER_SCRIPT ?= script.ld

MCU = stm32f205rg
SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/photon/board.py

upload:
	@echo "Uploading $(BIN)"
	python -u $(BOARD_PY) upload --port $(SERIAL_PORT) $(BIN)

rerun:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
