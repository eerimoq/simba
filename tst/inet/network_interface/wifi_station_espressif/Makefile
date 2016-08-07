#
# @file Makefile
# @version 5.0.0
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

NAME = wifi_station_espressif_suite

ifneq ($(SSID),)
CDEFS_EXTRA += SSID=$(SSID)
endif

ifneq ($(PASSWORD),)
CDEFS_EXTRA += PASSWORD=$(PASSWORD)
endif

ifneq ($(ESP8266_IP),)
CDEFS_EXTRA += ESP8266_IP=$(ESP8266_IP)
endif

RUN_PY = run.py

SIMBA_ROOT ?= ../../../..
include $(SIMBA_ROOT)/make/app.mk
