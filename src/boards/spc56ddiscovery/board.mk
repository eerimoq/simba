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

INC += $(SIMBA_ROOT)/src/boards/spc56ddiscovery
SRC += $(SIMBA_ROOT)/src/boards/spc56ddiscovery/board.c

BOARD_HOMEPAGE = "http://www.st.com/en/evaluation-tools/spc56d-discovery.html"
BOARD_PINOUT = "spc56d-discovery-pinout.png"
BOARD_DESC = "SPC56D Discovery"

MCU ?= spc56d40l1

SERIAL_PORT ?= /dev/arduino
CONTROL_PORT ?=
CONTROL_PORT_ARG = $(CONTROL_PORT:%=--control-port %)
TIMEOUT ?= 10
SPC5TOOL_PY ?= $(SIMBA_ROOT)/bin/spc5tool.py
RUN_PY ?= $(SIMBA_ROOT)/src/boards/spc56ddiscovery/run.py
BAUDRATE ?= 115200

upload:
	python -u $(SPC5TOOL_PY) --port $(SERIAL_PORT) $(CONTROL_PORT_ARG) \
	flash_write --erase --verify $(HEX)

rerun:
	@echo "Running '$(EXE)'."
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    $(CONTROL_PORT_ARG) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
			    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0

erase:
	@echo "Erasing."
	python -u $(SPC5TOOL_PY) --port $(SERIAL_PORT) $(CONTROL_PORT_ARG) \
	upload
	python -u $(SPC5TOOL_PY) --port $(SERIAL_PORT) $(CONTROL_PORT_ARG) \
	flash_erase 0x0 0x40000
	python -u $(SPC5TOOL_PY) --port $(SERIAL_PORT) $(CONTROL_PORT_ARG) \
	flash_erase 0x800000 0x10000
