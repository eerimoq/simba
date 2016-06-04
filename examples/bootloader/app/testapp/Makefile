#
# @file Makefile
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

NAME = test_application
BOARD ?= linux

ifeq ($(BOARD),arduino_due)
    LINKER_SCRIPT_FILE = script.application.ld
endif

BOOTLOADER_SRC = common.c
BOOTLOADER_ROOT = ../../src
include $(BOOTLOADER_ROOT)/bootloader.mk
SIMBA_ROOT = ../../../..
include $(SIMBA_ROOT)/make/app.mk
