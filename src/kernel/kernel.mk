#
# @file kernel/kernel.mk
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


INC += $(SIMBA_ROOT)/src/kernel
INC += $(SIMBA_ROOT)/src/kernel/ports/$(ARCH)/$(TOOLCHAIN)

KERNEL_SRC ?= binary_tree.c \
              bus.c \
              chan.c \
              event.c \
              fs.c \
	      heap.c \
              log.c \
              queue.c \
              rwlock.c \
              sem.c \
              setting.c \
              shell.c \
              std.c \
              sys.c \
              thrd.c \
              time.c \
              timer.c

SRC += $(KERNEL_SRC:%=$(SIMBA_ROOT)/src/kernel/%)
