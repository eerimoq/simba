#
# @section License
#
# The MIT License (MIT)
# 
# Copyright (c) 2014-2016, Erik Moqvist
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

INC += $(SIMBA_ROOT)/src
INC += $(SIMBA_ROOT)/3pp/compat

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
	       can.c \
               dac.c \
               ds18b20.c \
	       exti.c \
               flash.c \
               pin.c \
               i2c_soft.c \
               owi.c \
               pwm.c \
               pwm_soft.c \
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
                  i2c_soft.c \
                  mcp2515.c \
                  nrf24l01.c \
                  spi.c \
                  owi.c \
                  pin.c \
                  pwm.c \
                  pwm_soft.c \
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
               ds18b20.c \
               exti.c \
               flash.c \
               mcp2515.c \
               owi.c \
               pin.c \
               i2c_soft.c \
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
               esp_wifi.c \
               esp_wifi/station.c \
               esp_wifi/softap.c \
               exti.c \
               flash.c \
               pin.c \
               pwm_soft.c \
               i2c_soft.c \
               spi.c \
               uart.c \
               uart_soft.c
endif

ifeq ($(FAMILY),esp32)
DRIVERS_SRC ?= adc.c \
               analog_input_pin.c \
               can.c \
               dac.c \
	       ds18b20.c \
               flash.c \
               esp_wifi.c \
               esp_wifi/station.c \
               esp_wifi/softap.c \
	       owi.c \
	       pin.c \
               spi.c \
	       uart.c
endif

ifeq ($(FAMILY),stm32f1)
DRIVERS_SRC ?= flash.c \
               pin.c \
               i2c_soft.c \
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
               i2c_soft.c \
	       sdio.c \
	       uart.c
endif

ifeq ($(FAMILY),stm32f3)
DRIVERS_SRC ?= flash.c \
               pin.c \
               i2c_soft.c \
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

# Inet package.
ifneq ($(ARCH),$(filter $(ARCH), esp esp32))
    INC += $(SIMBA_ROOT)/3pp/lwip-1.4.1/src/include \
           $(SIMBA_ROOT)/3pp/lwip-1.4.1/src/include/ipv4
endif

INC += $(SIMBA_ROOT)/3pp/mbedtls/include

INET_SRC_TMP = \
	http_server.c \
	http_websocket_server.c \
	http_websocket_client.c \
	inet.c \
	mqtt_client.c \
	network_interface.c \
	network_interface/slip.c \
	network_interface/wifi.c \
	socket.c \
	ping.c

ifeq ($(FAMILY),$(filter $(FAMILY), esp esp32))
    INET_SRC_TMP += network_interface/driver/esp.c
endif

ifneq ($(ARCH),$(filter $(ARCH), esp esp32 linux))
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
	3pp/compat/arch/sys_arch.c

    SRC += $(LWIP_SRC:%=$(SIMBA_ROOT)/%)
endif

ifeq ($(ARCH),$(filter $(ARCH), esp esp32 linux))
MBED_TLS_SRC ?= \
	3pp/mbedtls/library/aes.c \
	3pp/mbedtls/library/aesni.c \
	3pp/mbedtls/library/arc4.c \
	3pp/mbedtls/library/asn1parse.c \
	3pp/mbedtls/library/asn1write.c \
	3pp/mbedtls/library/base64.c \
	3pp/mbedtls/library/bignum.c \
	3pp/mbedtls/library/blowfish.c \
	3pp/mbedtls/library/camellia.c \
	3pp/mbedtls/library/ccm.c \
	3pp/mbedtls/library/certs.c \
	3pp/mbedtls/library/cipher.c \
	3pp/mbedtls/library/cipher_wrap.c \
	3pp/mbedtls/library/cmac.c \
	3pp/mbedtls/library/ctr_drbg.c \
	3pp/mbedtls/library/debug.c \
	3pp/mbedtls/library/des.c \
	3pp/mbedtls/library/dhm.c \
	3pp/mbedtls/library/ecdh.c \
	3pp/mbedtls/library/ecdsa.c \
	3pp/mbedtls/library/ecjpake.c \
	3pp/mbedtls/library/ecp.c \
	3pp/mbedtls/library/ecp_curves.c \
	3pp/mbedtls/library/entropy.c \
	3pp/mbedtls/library/entropy_poll.c \
	3pp/mbedtls/library/error.c \
	3pp/mbedtls/library/gcm.c \
	3pp/mbedtls/library/havege.c \
	3pp/mbedtls/library/hmac_drbg.c \
	3pp/mbedtls/library/Makefile \
	3pp/mbedtls/library/md2.c \
	3pp/mbedtls/library/md4.c \
	3pp/mbedtls/library/md5.c \
	3pp/mbedtls/library/md.c \
	3pp/mbedtls/library/md_wrap.c \
	3pp/mbedtls/library/memory_buffer_alloc.c \
	3pp/mbedtls/library/net_sockets.c \
	3pp/mbedtls/library/oid.c \
	3pp/mbedtls/library/padlock.c \
	3pp/mbedtls/library/pem.c \
	3pp/mbedtls/library/pk.c \
	3pp/mbedtls/library/pkcs11.c \
	3pp/mbedtls/library/pkcs12.c \
	3pp/mbedtls/library/pkcs5.c \
	3pp/mbedtls/library/pkparse.c \
	3pp/mbedtls/library/pk_wrap.c \
	3pp/mbedtls/library/pkwrite.c \
	3pp/mbedtls/library/platform.c \
	3pp/mbedtls/library/ripemd160.c \
	3pp/mbedtls/library/rsa.c \
	3pp/mbedtls/library/sha1.c \
	3pp/mbedtls/library/sha256.c \
	3pp/mbedtls/library/sha512.c \
	3pp/mbedtls/library/ssl_cache.c \
	3pp/mbedtls/library/ssl_ciphersuites.c \
	3pp/mbedtls/library/ssl_cli.c \
	3pp/mbedtls/library/ssl_cookie.c \
	3pp/mbedtls/library/ssl_srv.c \
	3pp/mbedtls/library/ssl_ticket.c \
	3pp/mbedtls/library/ssl_tls.c \
	3pp/mbedtls/library/threading.c \
	3pp/mbedtls/library/timing.c \
	3pp/mbedtls/library/version.c \
	3pp/mbedtls/library/version_features.c \
	3pp/mbedtls/library/x509.c \
	3pp/mbedtls/library/x509_create.c \
	3pp/mbedtls/library/x509_crl.c \
	3pp/mbedtls/library/x509_crt.c \
	3pp/mbedtls/library/x509_csr.c \
	3pp/mbedtls/library/x509write_crt.c \
	3pp/mbedtls/library/x509write_csr.c \
	3pp/mbedtls/library/xtea.c

    SRC += $(MBED_TLS_SRC:%=$(SIMBA_ROOT)/%)

    INET_SRC_TMP += ssl.c
endif

INET_SRC ?= $(INET_SRC_TMP)

SRC += $(INET_SRC:%=$(SIMBA_ROOT)/src/inet/%)

#Kernel package.
INC += $(SIMBA_ROOT)/src/kernel/ports/$(ARCH)/$(TOOLCHAIN)

KERNEL_SRC_TMP = sys.c \
              thrd.c \
              time.c \
              timer.c

ifeq ($(FAMILY),esp32)
    KERNEL_SRC_TMP += ports/esp32/gnu/thrd_port.S
endif

KERNEL_SRC ?= $(KERNEL_SRC_TMP)

SRC += $(KERNEL_SRC:%=$(SIMBA_ROOT)/src/kernel/%)

# Multimedia package.
MULTIMEDIA_SRC ?= midi.c

SRC += $(MULTIMEDIA_SRC:%=$(SIMBA_ROOT)/src/multimedia/%)

# Oam package.
OAM_SRC ?= console.c \
	   service.c \
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
            emacs.c \
            std.c \
            re.c

ifneq ($(ARCH),$(filter $(ARCH), avr))
INC += \
	$(SIMBA_ROOT)/3pp/atto \
	$(SIMBA_ROOT)/3pp/atto/curses

ATTO_SRC +=  \
	3pp/atto/buffer.c \
	3pp/atto/command.c \
	3pp/atto/complete.c \
	3pp/atto/display.c \
	3pp/atto/gap.c \
	3pp/atto/key.c \
	3pp/atto/atto_main.c \
	3pp/atto/replace.c \
	3pp/atto/search.c \
	3pp/atto/window.c \
	3pp/atto/curses/curses.c

SRC += $(ATTO_SRC:%=$(SIMBA_ROOT)/%)
endif

SRC += $(TEXT_SRC:%=$(SIMBA_ROOT)/src/text/%)
