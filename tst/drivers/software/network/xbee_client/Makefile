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

NAME = xbee_client_suite
TYPE = suite
BOARD ?= linux

CDEFS += \
	CONFIG_XBEE=1 \
	CONFIG_XBEE_CLIENT=1 \
	CONFIG_XBEE_CLIENT_DEBUG_LOG_MASK=LOG_ALL \
	CONFIG_XBEE_CLIENT_RESPONSE_TIMEOUT_MS=10

SYNC_SRC += cond.c
DRIVERS_SRC = network/xbee_client.c

STUB = $(SIMBA_ROOT)/src/drivers/network/xbee_client.c:xbee_[!c]*

include $(SIMBA_ROOT)/make/app.mk
