#
# @file Makefile
# @version 6.0.0
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

NAME = fs_suite
BOARD ?= linux

CDEFS += CONFIG_MINIMAL_SYSTEM=1 \
	CONFIG_FS_CMD_FS_COUNTERS_LIST=1 \
	CONFIG_FS_CMD_FS_COUNTERS_RESET=1 \
	CONFIG_FS_CMD_FS_PARAMETERS_LIST=1 \
	CONFIG_FS_CMD_FS_FILESYSTEMS_LIST=1 \
	CONFIG_FS_CMD_FS_APPEND=1 \
	CONFIG_FS_CMD_FS_READ=1 \
	CONFIG_FS_CMD_FS_LIST=1 \
	CONFIG_FS_CMD_FS_WRITE=1 \
	CONFIG_SPIFFS=1 \
	CONFIG_THRD_ENV=1

SIMBA_ROOT ?= ../../..
include $(SIMBA_ROOT)/make/app.mk
