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

# Minimal set of files for a test suite.
ifeq ($(TYPE),suite)
  ALLOC_SRC += heap.c
  COLLECTIONS_SRC +=
  DEBUG_SRC += log.c harness.c
  DRIVERS_SRC += flash.c uart.c
  ENCODE_SRC +=
  HASH_SRC +=
  INET_SRC +=
  LWIP_SRC +=
  KERNEL_SRC += sys.c time.c timer.c thrd.c
  MULTIMEDIA_SRC +=
  OAM_SRC += console.c settings.c nvm.c
  FILESYSTEMS_SRC += fs.c
  SPIFFS_SRC +=
  SYNC_SRC += chan.c queue.c rwlock.c sem.c
  TEXT_SRC += std.c

  ifeq ($(FAMILY),$(filter $(FAMILY), sam stm32f1 stm32f2 stm32f3))
    ifneq ($(filter crc.c, $(HASH_SRC)), crc.c)
      HASH_SRC += crc.c
    endif
  endif

  ifeq ($(FAMILY),esp32)
      KERNEL_SRC += ports/esp32/gnu/thrd_port.S
  endif

  ifeq ($(FAMILY),spc5)
      KERNEL_SRC += ports/ppc/gnu/thrd_port.S
  endif

  ifneq ($(FAMILY),avr)
    DRIVERS_SRC += eeprom_soft.c
  endif

  ifeq ($(MCU),atmega32u4)
    DRIVERS_SRC += \
	usb.c \
	usb_device.c \
	usb/device/descriptors.c \
	usb/device/class/cdc.c \
	watchdog.c
  endif
endif

INC += $(SIMBA_ROOT)/src
INC += $(SIMBA_ROOT)/3pp/compat

# Alloc package.
ALLOC_SRC ?= circular_heap.c \
	     heap.c

SRC += $(ALLOC_SRC:%=$(SIMBA_ROOT)/src/alloc/%)

# Collections package.
COLLECTIONS_SRC ?= \
	binary_tree.c \
	circular_buffer.c \
	hash_map.c

SRC += $(COLLECTIONS_SRC:%=$(SIMBA_ROOT)/src/collections/%)

# Debug package.
DEBUG_SRC ?= log.c \
	     harness.c

SRC += $(DEBUG_SRC:%=$(SIMBA_ROOT)/src/debug/%)

# Drivers package.
INC += $(SIMBA_ROOT)/src/drivers/ports/$(FAMILY)

DRIVERS_SRC ?= \
	adc.c \
	analog_input_pin.c \
	analog_output_pin.c \
	can.c \
	chipid.c \
	dac.c \
	ds18b20.c \
	ds3231.c \
	eeprom_i2c.c \
	eeprom_soft.c \
	esp_wifi.c \
	esp_wifi/station.c \
	esp_wifi/softap.c \
	exti.c \
	flash.c \
	i2c.c \
	i2c_soft.c \
	led_7seg_ht16k33.c \
	mcp2515.c \
	nrf24l01.c \
	owi.c \
	pin.c \
	pwm.c \
	pwm_soft.c \
	random.c \
	sht3xd.c \
	sd.c \
	spi.c \
	uart.c \
	uart_soft.c \
	usb.c \
	usb_device.c \
	usb/device/descriptors.c \
	usb/device/class/cdc.c \
	watchdog.c \
	usb_host.c \
	usb/host/class/hid.c \
	usb/host/class/mass_storage.c \
	ws2812.c \
	xbee.c

SRC += $(DRIVERS_SRC:%=$(SIMBA_ROOT)/src/drivers/%)

ifeq ($(FAMILY),linux)
SRC += $(SIMBA_ROOT)/src/drivers/ports/linux/socket_device.c
endif

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
	isotp.c \
	mqtt_client.c \
	tftp_server.c \
	network_interface.c \
	network_interface/slip.c \
	network_interface/wifi.c \
	slip.c \
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
    INET_SRC_TMP += ssl.c
endif

INET_SRC ?= $(INET_SRC_TMP)

SRC += $(INET_SRC:%=$(SIMBA_ROOT)/src/inet/%)

ifeq ($(filter $(INET_SRC), ssl.c), ssl.c)
  MBED_TLS_SRC ?= \
	3pp/compat/mbedtls/library/mbedtls_aes.c \
	3pp/compat/mbedtls/library/mbedtls_aesni.c \
	3pp/compat/mbedtls/library/mbedtls_arc4.c \
	3pp/compat/mbedtls/library/mbedtls_asn1parse.c \
	3pp/compat/mbedtls/library/mbedtls_asn1write.c \
	3pp/compat/mbedtls/library/mbedtls_base64.c \
	3pp/compat/mbedtls/library/mbedtls_bignum.c \
	3pp/compat/mbedtls/library/mbedtls_camellia.c \
	3pp/compat/mbedtls/library/mbedtls_ccm.c \
	3pp/compat/mbedtls/library/mbedtls_cipher.c \
	3pp/compat/mbedtls/library/mbedtls_cipher_wrap.c \
	3pp/compat/mbedtls/library/mbedtls_cmac.c \
	3pp/compat/mbedtls/library/mbedtls_ctr_drbg.c \
	3pp/compat/mbedtls/library/mbedtls_des.c \
	3pp/compat/mbedtls/library/mbedtls_dhm.c \
	3pp/compat/mbedtls/library/mbedtls_ecdh.c \
	3pp/compat/mbedtls/library/mbedtls_ecdsa.c \
	3pp/compat/mbedtls/library/mbedtls_ecjpake.c \
	3pp/compat/mbedtls/library/mbedtls_ecp.c \
	3pp/compat/mbedtls/library/mbedtls_ecp_curves.c \
	3pp/compat/mbedtls/library/mbedtls_entropy.c \
	3pp/compat/mbedtls/library/mbedtls_entropy_poll.c \
	3pp/compat/mbedtls/library/mbedtls_error.c \
	3pp/compat/mbedtls/library/mbedtls_gcm.c \
	3pp/compat/mbedtls/library/mbedtls_havege.c \
	3pp/compat/mbedtls/library/mbedtls_hmac_drbg.c \
	3pp/compat/mbedtls/library/mbedtls_md2.c \
	3pp/compat/mbedtls/library/mbedtls_md4.c \
	3pp/compat/mbedtls/library/mbedtls_md5.c \
	3pp/compat/mbedtls/library/mbedtls_md.c \
	3pp/compat/mbedtls/library/mbedtls_md_wrap.c \
	3pp/compat/mbedtls/library/mbedtls_memory_buffer_alloc.c \
	3pp/compat/mbedtls/library/mbedtls_oid.c \
	3pp/compat/mbedtls/library/mbedtls_padlock.c \
	3pp/compat/mbedtls/library/mbedtls_pem.c \
	3pp/compat/mbedtls/library/mbedtls_pk.c \
	3pp/compat/mbedtls/library/mbedtls_pkcs11.c \
	3pp/compat/mbedtls/library/mbedtls_pkcs12.c \
	3pp/compat/mbedtls/library/mbedtls_pkcs5.c \
	3pp/compat/mbedtls/library/mbedtls_pkparse.c \
	3pp/compat/mbedtls/library/mbedtls_pk_wrap.c \
	3pp/compat/mbedtls/library/mbedtls_pkwrite.c \
	3pp/compat/mbedtls/library/mbedtls_platform.c \
	3pp/compat/mbedtls/library/mbedtls_ripemd160.c \
	3pp/compat/mbedtls/library/mbedtls_rsa.c \
	3pp/compat/mbedtls/library/mbedtls_sha1.c \
	3pp/compat/mbedtls/library/mbedtls_sha256.c \
	3pp/compat/mbedtls/library/mbedtls_sha512.c \
	3pp/compat/mbedtls/library/mbedtls_ssl_cache.c \
	3pp/compat/mbedtls/library/mbedtls_ssl_ciphersuites.c \
	3pp/compat/mbedtls/library/mbedtls_ssl_cli.c \
	3pp/compat/mbedtls/library/mbedtls_ssl_cookie.c \
	3pp/compat/mbedtls/library/mbedtls_ssl_srv.c \
	3pp/compat/mbedtls/library/mbedtls_ssl_ticket.c \
	3pp/compat/mbedtls/library/mbedtls_ssl_tls.c \
	3pp/compat/mbedtls/library/mbedtls_threading.c \
	3pp/compat/mbedtls/library/mbedtls_version.c \
	3pp/compat/mbedtls/library/mbedtls_version_features.c \
	3pp/compat/mbedtls/library/mbedtls_x509.c \
	3pp/compat/mbedtls/library/mbedtls_x509_create.c \
	3pp/compat/mbedtls/library/mbedtls_x509_crl.c \
	3pp/compat/mbedtls/library/mbedtls_x509_crt.c \
	3pp/compat/mbedtls/library/mbedtls_x509_csr.c \
	3pp/compat/mbedtls/library/mbedtls_x509write_crt.c \
	3pp/compat/mbedtls/library/mbedtls_x509write_csr.c \
	3pp/compat/mbedtls/library/mbedtls_xtea.c \
	3pp/compat/mbedtls/mbedtls_compat.c

    SRC += $(MBED_TLS_SRC:%=$(SIMBA_ROOT)/%)
endif

#Kernel package.
INC += $(SIMBA_ROOT)/src/kernel/ports/$(ARCH)/$(TOOLCHAIN)

KERNEL_SRC_TMP = sys.c \
	      thrd.c \
	      time.c \
	      timer.c

ifeq ($(FAMILY),esp32)
    KERNEL_SRC_TMP += ports/esp32/gnu/thrd_port.S
endif

ifeq ($(FAMILY),spc5)
    KERNEL_SRC_TMP += ports/ppc/gnu/thrd_port.S
endif

KERNEL_SRC ?= $(KERNEL_SRC_TMP)

SRC += $(KERNEL_SRC:%=$(SIMBA_ROOT)/src/kernel/%)

# Multimedia package.
MULTIMEDIA_SRC ?= midi.c

SRC += $(MULTIMEDIA_SRC:%=$(SIMBA_ROOT)/src/multimedia/%)

# OAM package.
INC += $(SIMBA_ROOT)/src/oam/ports/$(FAMILY)

OAM_SRC_TMP ?= \
	console.c \
	nvm.c \
	service.c \
	settings.c \
	shell.c \
	soam.c

ifeq ($(FAMILY), $(filter $(FAMILY), esp32 linux))
OAM_SRC_TMP += \
	upgrade.c \
	upgrade/http.c \
	upgrade/kermit.c \
	upgrade/tftp.c \
	upgrade/uds.c
endif

OAM_SRC ?= $(OAM_SRC_TMP)

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

  ifeq ($(filter $(TEXT_SRC), emacs.c),emacs.c)
    ATTO_SRC ?=  \
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
  endif

  SRC += $(ATTO_SRC:%=$(SIMBA_ROOT)/%)
endif

SRC += $(TEXT_SRC:%=$(SIMBA_ROOT)/src/text/%)
