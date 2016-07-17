#
# @file mcus/sam/sam3.mk
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

INC += $(SIMBA_ROOT)/src/mcus/sam
SRC += $(SIMBA_ROOT)/src/mcus/sam/sam3.c \
       $(SIMBA_ROOT)/src/mcus/sam/mcu.c

MCPU = cortex-m3
F_CPU = 84000000

MCU_HOMEPAGE = "http://www.atmel.com"

ARCH = arm
FAMILY = sam

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/arm.mk
