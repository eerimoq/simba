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

INC += $(SIMBA_ROOT)/src/boards/maple_esp32
SRC += $(SIMBA_ROOT)/src/boards/maple_esp32/board.c

# ToDo: Should be 16M flash.
LINKER_SCRIPT ?= simba.flash.ld
ESP_FLASH_SIZE = 4M

ifeq ($(PARTITION), bootloader)
PARTITIONS_BIN ?= $(SIMBA_ROOT)/3pp/esp32/bin/partitions_bootloader_application.bin
erase: all
upload: erase
else
PARTITIONS_BIN ?= $(SIMBA_ROOT)/3pp/esp32/bin/partitions_singleapp.bin
endif

BOARD_HOMEPAGE = "http://www.analoglamb.com/product/maple-esp32/"
BOARD_PINOUT = "maple-esp32-pinout.jpg"
BOARD_DESC = "Maple-ESP32"

MCU = esp32
SERIAL_PORT ?= /dev/arduino
CONSOLE_RESET_TYPE ?= 0
TIMEOUT ?= 10
BAUDRATE ?= 115200

upload:
	@echo "Uploading $(EXE)."
	python -u $(SIMBA_ROOT)/3pp/esp32/esp-idf/components/esptool_py/esptool/esptool.py \
	--chip esp32 --port $(SERIAL_PORT) --baud 921600 write_flash -z \
	--flash_mode dio --flash_freq 40m --flash_size 4MB \
	0x1000 $(SIMBA_ROOT)/3pp/esp32/bin/bootloader.bin \
	0x10000 $(BIN) \
	0x8000 $(PARTITIONS_BIN)

rerun:
	@echo "Running '$(EXE)'."
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
			    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | python3 -u $(BACKTRACE_PY) $(EXE) $(CROSS_COMPILE) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0

erase:
	@echo "Erasing."
	python -u $(ESPTOOL_PY) \
	--chip esp32 --port $(SERIAL_PORT) --baud 921600 erase_flash
