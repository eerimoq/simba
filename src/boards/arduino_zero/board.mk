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

INC += $(SIMBA_ROOT)/src/boards/arduino_zero
SRC += $(SIMBA_ROOT)/src/boards/arduino_zero/board.c

BOARD_HOMEPAGE = "https://www.arduino.cc/en/Main/ArduinoBoardZero"
BOARD_PINOUT = "arduino-uno-pinout.png"
BOARD_DESC = "Arduino Zero"

MCU = samd21g18

SERIAL_PORT ?= /dev/arduino
TIMEOUT ?= 10
BAUDRATE ?= 115200

upload:
	@echo "Uploading '$(BIN)'."
	/home/erik/.arduino15/packages/arduino/tools/openocd/0.9.0-arduino6-static/bin/openocd -d2 -s /home/erik/.arduino15/packages/arduino/tools/openocd/0.9.0-arduino6-static/share/openocd/scripts/ -f /home/erik/.arduino15/packages/arduino/hardware/samd/1.6.15/variants/arduino_zero/openocd_scripts/arduino_zero.cfg -c telnet_port disabled; program {{$(BIN)}} verify reset 0x2000; shutdown 


rerun:
	@echo "Running '$(EXE)'."
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
