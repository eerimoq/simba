#
# @file make/gnu/linux.mk
# @version 4.1.0
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

CROSS_COMPILE =
CFLAGS += -Werror -Wno-error=unused-variable -DCONFIG_PROFILE_STACK=0
LDFLAGS_AFTER += -lpthread -lrt

SETTING_MEMORY = file
SETTING_OFFSET = 0
SETTING_SIZE = 4096
ENDIANESS = little

CFLAGS += -pg -fprofile-arcs -ftest-coverage
LDFLAGS += -pg -fprofile-arcs -ftest-coverage -lgcov

CFLAGS += -g
LDFLAGS += -g

include $(SIMBA_ROOT)/make/gnu.mk
