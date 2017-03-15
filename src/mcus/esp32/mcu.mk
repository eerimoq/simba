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

ESP32_ROOT ?= $(SIMBA_ROOT)/3pp/esp32
ESP_IDF_ROOT ?= $(ESP32_ROOT)/esp-idf

INC += $(SIMBA_ROOT)/src/mcus/esp32 \
       $(ESP32_ROOT)/inc/app_update/include \
       $(ESP32_ROOT)/inc/driver/include \
       $(ESP32_ROOT)/inc/esp32/include \
       $(ESP32_ROOT)/inc/newlib/include \
       $(ESP32_ROOT)/inc/freertos/include \
       $(ESP32_ROOT)/inc/nvs_flash/include \
       $(ESP32_ROOT)/inc/spi_flash/include \
       $(ESP32_ROOT)/inc/tcpip_adapter/include \
       $(ESP32_ROOT)/inc/lwip/include/lwip \
       $(ESP32_ROOT)/inc/lwip/include/lwip/port

SRC += $(SIMBA_ROOT)/src/mcus/esp32/mcu.c \
       $(SIMBA_ROOT)/src/mcus/esp32/esp32.c

LIBPATH += "$(SIMBA_ROOT)/src/mcus/esp32/ld" \
	$(ESP_IDF_ROOT)/components/esp32/ld \
	$(ESP32_ROOT)/lib

LDFLAGS += -Wl,-T$(LINKER_SCRIPT)

LIB += \
	app_update \
	hal \
	core \
	net80211 \
	phy \
	pp \
	coexist \
	rtc \
	smartconfig \
	wpa \
	bt \
	driver \
	esp32 \
	expat \
	freertos \
	log \
	lwip \
	newlib \
	nvs_flash \
	spi_flash \
	tcpip_adapter \
	vfs \
	wpa_supplicant \
	ethernet \
	ulp \
	gcc \
	c \
	m \
	xtensa-debug-module

F_CPU = 240000000

ARCH = esp32
FAMILY = esp32

MCU_HOMEPAGE = "http://www.espressif.com"
MCU_NAME = "Espressif ESP32"
MCU_DESC = "Espressif ESP32 @ 240 MHz, 512 kB sram, 4 MB flash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/esp32.mk
