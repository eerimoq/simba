#
# @file lwip/lwip.mk
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

INC += \
	$(SIMBA_ROOT)/src/inet

ifneq ($(ARCH),$(filter $(ARCH), esp))
    INC += $(SIMBA_ROOT)/3pp/lwip-1.4.1/src/include \
           $(SIMBA_ROOT)/3pp/lwip-1.4.1/src/include/ipv4
endif

INET_SRC ?= \
	http_server.c \
	http_websocket_server.c \
	http_websocket_client.c \
	mqtt_client.c \
	network_interface.c \
	network_interface_slip.c \
	socket.c

ifneq ($(ARCH),$(filter $(ARCH), esp linux))
    LWIP_SRC ?= \
	../../3pp/lwip-1.4.1/src/core/stats.c \
	../../3pp/lwip-1.4.1/src/core/tcp_out.c \
	../../3pp/lwip-1.4.1/src/core/udp.c \
	../../3pp/lwip-1.4.1/src/core/timers.c \
	../../3pp/lwip-1.4.1/src/core/netif.c \
	../../3pp/lwip-1.4.1/src/core/def.c \
	../../3pp/lwip-1.4.1/src/core/raw.c \
	../../3pp/lwip-1.4.1/src/core/dns.c \
	../../3pp/lwip-1.4.1/src/core/tcp_in.c \
	../../3pp/lwip-1.4.1/src/core/memp.c \
	../../3pp/lwip-1.4.1/src/core/pbuf.c \
	../../3pp/lwip-1.4.1/src/core/tcp.c \
	../../3pp/lwip-1.4.1/src/core/init.c \
	../../3pp/lwip-1.4.1/src/core/dhcp.c \
	../../3pp/lwip-1.4.1/src/core/ipv4/ip_frag.c \
	../../3pp/lwip-1.4.1/src/core/ipv4/ip.c \
	../../3pp/lwip-1.4.1/src/core/ipv4/ip_addr.c \
	../../3pp/lwip-1.4.1/src/core/ipv4/icmp.c \
	../../3pp/lwip-1.4.1/src/core/ipv4/igmp.c \
	../../3pp/lwip-1.4.1/src/core/ipv4/inet.c \
	../../3pp/lwip-1.4.1/src/core/ipv4/inet_chksum.c \
	../../3pp/lwip-1.4.1/src/core/mem.c \
	../../3pp/lwip-1.4.1/src/netif/etharp.c \
	../../3pp/lwip-1.4.1/src/netif/ethernetif.c \
	../../3pp/lwip-1.4.1/src/api/tcpip.c \
	arch/sys_arch.c

    SRC += $(LWIP_SRC:%=$(SIMBA_ROOT)/src/inet/%)
endif

SRC += $(INET_SRC:%=$(SIMBA_ROOT)/src/inet/%)
