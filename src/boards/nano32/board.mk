#
# @file src/boards/nano32/board.mk
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

INC += $(SIMBA_ROOT)/src/boards/nano32
SRC += $(SIMBA_ROOT)/src/boards/nano32/board.c

LINKER_SCRIPT ?= simba.flash.ld
ESP_FLASH_SIZE = 4M

BOARD_HOMEPAGE = "http://esp32.de"
BOARD_PINOUT = "nano32-pinout.jpg"
BOARD_DESC = "NANO32"

MCU = esp32
SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/nano32/board.py
RUN_PY ?= $(SIMBA_ROOT)/src/boards/nano32/run.py
CONSOLE_RESET_TYPE ?= 0
TIMEOUT ?= 10

upload:
	@echo "Uploading $(EXE)"
	python -u $(SIMBA_ROOT)/3pp/esp32/esp-idf/components/esptool_py/esptool/esptool.py \
	--chip esp32 --port $(SERIAL_PORT) --baud 115200 write_flash --flash_mode dio \
	--flash_freq 40m 0x1000 $(SIMBA_ROOT)/3pp/esp32/bin/bootloader.bin \
	0x10000 $(BIN) 0x4000 $(SIMBA_ROOT)/3pp/esp32/bin/partitions_singleapp.bin
