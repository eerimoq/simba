#
# @file simba.mk
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

INC += $(SIMBA_ROOT)/src

# Alloc package.
ALLOC_SRC ?= circular_heap.c \
             heap.c

SRC += $(ALLOC_SRC:%=$(SIMBA_ROOT)/src/alloc/%)

# Collections package.
COLLECTIONS_SRC ?= binary_tree.c \
		   hash_map.c

SRC += $(COLLECTIONS_SRC:%=$(SIMBA_ROOT)/src/collections/%)

# Debug package.
DEBUG_SRC ?= log.c \
             harness.c

SRC += $(DEBUG_SRC:%=$(SIMBA_ROOT)/src/debug/%)

# Drivers package.
INC += $(SIMBA_ROOT)/src/drivers/ports/$(FAMILY)

ifeq ($(FAMILY),linux)
DRIVERS_SRC ?= adc.c \
               analog_input_pin.c \
               analog_output_pin.c \
               flash.c \
               pin.c \
               pwm.c \
               sd.c \
               spi.c \
               uart.c
endif

ifeq ($(FAMILY),avr)
DRIVERS_SRC_TMP = adc.c \
                  analog_input_pin.c \
                  analog_output_pin.c \
                  ds18b20.c \
                  ds3231.c \
                  exti.c \
                  i2c.c \
                  mcp2515.c \
                  nrf24l01.c \
                  spi.c \
                  owi.c \
                  pin.c \
                  pwm.c \
                  sd.c \
                  uart.c \
                  uart_soft.c \
		  watchdog.c

ifeq ($(MCU),atmega32u4)
DRIVERS_SRC_TMP += usb.c \
                   usb_device.c \
		   usb/device/descriptors.c \
		   usb/device/class/cdc.c
endif

DRIVERS_SRC ?= $(DRIVERS_SRC_TMP)

endif

ifeq ($(FAMILY),sam)
DRIVERS_SRC ?= adc.c \
               analog_input_pin.c \
               can.c \
               chipid.c \
               dac.c \
               exti.c \
               flash.c \
               mcp2515.c \
               pin.c \
               sd.c \
               spi.c \
               uart.c \
               usb.c \
               usb_host.c \
               usb/host/class/hid.c \
               usb/host/class/mass_storage.c
endif

ifeq ($(FAMILY),esp)
DRIVERS_SRC ?= adc.c \
               analog_input_pin.c \
               pin.c \
               spi.c \
               uart.c
endif

ifeq ($(FAMILY),stm32f1)
DRIVERS_SRC ?= flash.c \
               pin.c \
	       uart.c
endif

ifeq ($(FAMILY),stm32f2)
DRIVERS_SRC ?= bcm43362.c \
	       bcm43362/wwd_buffer.c \
	       bcm43362/wwd_bus.c \
	       bcm43362/wwd_network.c \
	       bcm43362/wwd_platform.c \
	       bcm43362/wwd_resource.c \
	       bcm43362/wwd_rtos.c \
	       bcm43362/wwd_sdio.c \
	       flash.c \
               pin.c \
	       sdio.c \
	       uart.c
endif

ifeq ($(FAMILY),stm32f3)
DRIVERS_SRC ?= flash.c \
               pin.c \
               uart.c
endif

SRC += $(DRIVERS_SRC:%=$(SIMBA_ROOT)/src/drivers/%)

# Encode package.
ENCODE_SRC ?= base64.c \
              json.c

SRC += $(ENCODE_SRC:%=$(SIMBA_ROOT)/src/encode/%)

# Hash package.
HASH_SRC ?= crc.c \
            sha1.c

SRC += $(HASH_SRC:%=$(SIMBA_ROOT)/src/hash/%)

ifneq ($(ARCH),$(filter $(ARCH), esp))
    INC += $(SIMBA_ROOT)/3pp/lwip-1.4.1/src/include \
           $(SIMBA_ROOT)/3pp/lwip-1.4.1/src/include/ipv4
endif

# Inet package.
INC += $(SIMBA_ROOT)/src/inet

INET_SRC_TMP ?= \
	http_server.c \
	http_websocket_server.c \
	http_websocket_client.c \
	inet.c \
	mqtt_client.c \
	network_interface.c \
	network_interface/slip.c \
	socket.c

ifeq ($(ARCH), esp)
INET_SRC_TMP += network_interface/wifi_station_espressif.c \
	ping.c
endif

INET_SRC ?= $(INET_SRC_TMP)

ifneq ($(ARCH),$(filter $(ARCH), esp linux))
    LWIP_SRC ?= \
	3pp/lwip-1.4.1/src/core/stats.c \
	3pp/lwip-1.4.1/src/core/tcp_out.c \
	3pp/lwip-1.4.1/src/core/udp.c \
	3pp/lwip-1.4.1/src/core/timers.c \
	3pp/lwip-1.4.1/src/core/netif.c \
	3pp/lwip-1.4.1/src/core/def.c \
	3pp/lwip-1.4.1/src/core/raw.c \
	3pp/lwip-1.4.1/src/core/dns.c \
	3pp/lwip-1.4.1/src/core/tcp_in.c \
	3pp/lwip-1.4.1/src/core/memp.c \
	3pp/lwip-1.4.1/src/core/pbuf.c \
	3pp/lwip-1.4.1/src/core/tcp.c \
	3pp/lwip-1.4.1/src/core/init.c \
	3pp/lwip-1.4.1/src/core/dhcp.c \
	3pp/lwip-1.4.1/src/core/ipv4/ip_frag.c \
	3pp/lwip-1.4.1/src/core/ipv4/ip.c \
	3pp/lwip-1.4.1/src/core/ipv4/ip_addr.c \
	3pp/lwip-1.4.1/src/core/ipv4/icmp.c \
	3pp/lwip-1.4.1/src/core/ipv4/igmp.c \
	3pp/lwip-1.4.1/src/core/ipv4/inet.c \
	3pp/lwip-1.4.1/src/core/ipv4/inet_chksum.c \
	3pp/lwip-1.4.1/src/core/mem.c \
	3pp/lwip-1.4.1/src/netif/etharp.c \
	3pp/lwip-1.4.1/src/netif/ethernetif.c \
	3pp/lwip-1.4.1/src/api/tcpip.c \
	src/inet/arch/sys_arch.c

    SRC += $(LWIP_SRC:%=$(SIMBA_ROOT)/%)
endif

SRC += $(INET_SRC:%=$(SIMBA_ROOT)/src/inet/%)

#Kernel package.
INC += $(SIMBA_ROOT)/src/kernel/ports/$(ARCH)/$(TOOLCHAIN)

KERNEL_SRC ?= sys.c \
              thrd.c \
              time.c \
              timer.c

SRC += $(KERNEL_SRC:%=$(SIMBA_ROOT)/src/kernel/%)

# Multimedia package.
MULTIMEDIA_SRC ?= midi.c

SRC += $(MULTIMEDIA_SRC:%=$(SIMBA_ROOT)/src/multimedia/%)

# Oam package.
OAM_SRC ?= console.c \
	   settings.c \
	   shell.c

SRC += $(OAM_SRC:%=$(SIMBA_ROOT)/src/oam/%)

# Filesystems package.
INC += $(SIMBA_ROOT)/3pp/spiffs-0.3.5/src

FILESYSTEMS_SRC ?= fat16.c \
	       fs.c \
	       spiffs.c

SRC += $(FILESYSTEMS_SRC:%=$(SIMBA_ROOT)/src/filesystems/%)

SPIFFS_SRC ?= \
	3pp/spiffs-0.3.5/src/spiffs_nucleus.c \
	3pp/spiffs-0.3.5/src/spiffs_gc.c \
	3pp/spiffs-0.3.5/src/spiffs_hydrogen.c \
	3pp/spiffs-0.3.5/src/spiffs_cache.c \
	3pp/spiffs-0.3.5/src/spiffs_check.c

SRC += $(SPIFFS_SRC:%=$(SIMBA_ROOT)/%)

# Sync package.
SYNC_SRC ?= bus.c \
            chan.c \
            event.c \
            queue.c \
            rwlock.c \
            sem.c

SRC += $(SYNC_SRC:%=$(SIMBA_ROOT)/src/sync/%)

# Text package.
TEXT_SRC ?= configfile.c \
	    std.c \
            re.c

SRC += $(TEXT_SRC:%=$(SIMBA_ROOT)/src/text/%)
