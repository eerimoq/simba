#
# @file mcus/esp8266/mcu.mk
# @version 4.0.0
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

ESP8266_RTOS_SDK_ROOT ?= $(SIMBA_ROOT)/3pp/ESP8266_RTOS_SDK

INC += $(SIMBA_ROOT)/src/mcus/esp8266 \
       $(ESP8266_RTOS_SDK_ROOT)/extra_include \
       $(ESP8266_RTOS_SDK_ROOT)/include \
       $(ESP8266_RTOS_SDK_ROOT)/include/espressif \
       $(ESP8266_RTOS_SDK_ROOT)/include/espressif/esp8266 \
       $(ESP8266_RTOS_SDK_ROOT)/include/lwip \
       $(ESP8266_RTOS_SDK_ROOT)/include/lwip/ipv4 \
       $(ESP8266_RTOS_SDK_ROOT)/include/lwip/ipv6

SRC += $(SIMBA_ROOT)/src/mcus/esp8266/mcu.c \
       $(SIMBA_ROOT)/src/mcus/esp8266/esp8266.c

LIBPATH += "$(SIMBA_ROOT)/src/mcus/esp8266/ld"

LDFLAGS += -Wl,--start-group \
            $(ESPLIBS:%=-l%) \
            -Wl,--end-group \
            -Wl,-T$(LINKER_SCRIPT)

LDFLAGS_AFTER += \
	$(ESPLIBS_AFTER:%=-l%)

ESPLIBS += \
 minic \
 hal \
 phy \
 pp \
 net80211 \
 wpa \
 crypto \
 main \
 freertos \
 lwip

ESPLIBS_AFTER += \
 gcc

F_CPU = 80000000

ARCH = esp
FAMILY = esp

MCU_HOMEPAGE = "http://www.espressif.com"
MCU_NAME = "Espressif ESP8266"
MCU_DESC = "Espressif ESP8266 @ 80 MHz, 82 kB dram, 4 MB flash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/esp.mk
