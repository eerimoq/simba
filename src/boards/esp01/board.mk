#
# @file board.mk
# @version 1.1.0
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

INC += $(SIMBA_ROOT)/src/boards/esp01

LINKER_SCRIPT = simba.flash.1m.ld
ESP_FLASH_SIZE_MAP = 2

BOARD_HOMEPAGE = "http://espressif.com"
BOARD_PINOUT = "esp01-pinout.png"
BOARD_DESC = "ESP-01"

MCU = esp8266
SERIAL_PORT = arduino

upload:
	@echo "Uploading $(EXE)"
	$(SIMBA_ROOT)/3pp/esptool/esptool.py \
	    --port /dev/$(SERIAL_PORT) \
	    --baud 460800 \
	    write_flash \
            0x00000 $(SIMBA_ROOT)/3pp/ESP8266_RTOS_SDK/bin/boot_v1.4.bin \
            0x01000 $(NAME).bin

run:
	@echo "Running '$(EXE)'."
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
