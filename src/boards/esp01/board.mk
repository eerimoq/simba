#
# @file board.mk
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

INC += $(SIMBA_ROOT)/src/boards/esp01

LINKER_SCRIPT = simba.flash.1m.ld
ESP_FLASH_SIZE_MAP = 2

BOARD_HOMEPAGE = "http://espressif.com"
BOARD_PINOUT = "esp01-pinout.png"
BOARD_DESC = "ESP-01"

MCU = esp8266
