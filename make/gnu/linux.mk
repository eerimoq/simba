#
# @file make/gnu/linux.mk
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
CXXFLAGS += -Werror -Wno-error=unused-variable -DCONFIG_PROFILE_STACK=0

CFLAGS += -pg -fprofile-arcs -ftest-coverage
CXXFLAGS += -pg -fprofile-arcs -ftest-coverage
LDFLAGS += -pg -fprofile-arcs -ftest-coverage

CFLAGS += -g
CXXFLAGS += -g
LDFLAGS += -g

LIB += \
	gcov \
	pthread \
	rt

include $(SIMBA_ROOT)/make/gnu.mk
