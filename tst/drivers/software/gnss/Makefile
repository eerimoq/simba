#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2017, Erik Moqvist
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

NAME = gnss_suite
TYPE = suite
BOARD ?= linux

CDEFS += \
	CONFIG_GNSS=1 \
	CONFIG_GNSS_DEBUG_LOG_MASK=LOG_ALL \
	CONFIG_HARNESS_HEAP_MAX=131072 \
	CONFIG_HARNESS_MOCK_VERBOSE=0

DRIVERS_SRC = sensors/gnss.c
ENCODE_SRC = nmea.c

STUB = $(SIMBA_ROOT)/src/drivers/sensors/gnss.c:chan_read,chan_write

include $(SIMBA_ROOT)/make/app.mk
