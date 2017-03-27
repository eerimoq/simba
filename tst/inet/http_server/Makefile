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

NAME = http_server_suite
TYPE = suite
BOARD ?= linux

SRC += socket_stub.c ssl_stub.c
CDEFS += \
	CONFIG_MINIMAL_SYSTEM=1 \
	CONFIG_FS_CMD_THRD_LIST=1 \
	CONFIG_MODULE_INIT_LOG=1

ifeq ($(BOARD), linux)
CDEFS += \
	CONFIG_HTTP_SERVER_SSL=1
endif

SRC_IGNORE = $(SIMBA_ROOT)/src/inet/socket.c

ENCODE_SRC = base64.c
SYNC_SRC = event.c
HASH_SRC = sha1.c
INET_SRC = \
	http_server.c \
	http_websocket_server.c \
	inet.c

SIMBA_ROOT ?= ../../..
include $(SIMBA_ROOT)/make/app.mk
