#
# @section License
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>
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
