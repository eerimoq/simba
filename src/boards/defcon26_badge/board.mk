#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2018, Erik Moqvist
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

INC += $(SIMBA_ROOT)/src/boards/defcon26_badge
SRC += $(SIMBA_ROOT)/src/boards/defcon26_badge/board.c

BOARD_HOMEPAGE = "https://www.defcon.org/html/defcon-26/dc-26-index.html"
BOARD_PINOUT = "defcon26-badge.jpg"
BOARD_DESC = "DEF CON 26 Badge"

MCU = pic32mm0256gpm048

PROGRAMMER_PORT ?= /dev/arduino
SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/defcon26_badge/board.py
TIMEOUT ?= 10
BAUDRATE ?= 76800

upload:
	@echo "Uploading '$(EXE)'."
	pic32tool.py -p $(PROGRAMMER_PORT) flash_write -e -v $(S19)
	pic32tool.py -p $(PROGRAMMER_PORT) disconnect

rerun:
	@echo "Running '$(EXE)'."
	pic32tool.py -p $(PROGRAMMER_PORT) reset
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | python3 -u $(BACKTRACE_PY) $(EXE) $(CROSS_COMPILE) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
