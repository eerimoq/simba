#
# @file board.mk
# @version 0.5.0
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

INC += $(SIMBA_ROOT)/src/boards/esp12e

LINKER_SCRIPT = simba.flash.4m.ld
ESP_FLASH_SIZE_MAP = 6

BOARD_HOMEPAGE = "http://espressif.com"
BOARD_PINOUT = "esp12e-pinout.jpg"
BOARD_DESC = "ESP-12E Development Board"

MCU = esp8266
