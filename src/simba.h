/**
 * @file simba.h
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __SIMBA_H__
#define __SIMBA_H__

#if defined(ARCH_ESP)
typedef unsigned long off_t;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <ctype.h>

#include "compiler.h"
#include <config.h>
#include "config_default.h"
#include "board.h"
#include "mcu.h"

#include "kernel/errno.h"
#include "kernel/types.h"

#include "oam/setting.h"

#include "collections/binary_tree.h"
#include "collections/bits.h"
#include "collections/fifo.h"
#include "collections/list.h"
#include "collections/hash_map.h"

#include "kernel/time.h"
#include "sync/chan.h"
#include "kernel/sys.h"
#include "kernel/timer.h"
#include "kernel/thrd.h"

#include "sync/sem.h"
#include "sync/queue.h"
#include "sync/event.h"
#include "sync/rwlock.h"
#include "sync/bus.h"

#include "alloc/heap.h"
#include "alloc/circular_heap.h"

#include "filesystems/fat16.h"
#include "filesystems/spiffs.h"

#include "oam/console.h"
#include "filesystems/fs.h"
#include "oam/shell.h"

#include "debug/log.h"

#include "text/color.h"
#include "text/re.h"
#include "text/std.h"
#include "text/configfile.h"

#include "encode/base64.h"
#include "encode/json.h"

#include "hash/crc.h"
#include "hash/sha1.h"

#if defined(FAMILY_LINUX)
#    include "drivers/exti.h"
#    include "drivers/pin.h"
#    include "drivers/pwm.h"
#    include "drivers/adc.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/can.h"
#    include "drivers/sd.h"
#    include "drivers/flash.h"
#endif

#if defined(FAMILY_AVR)
#    include "drivers/exti.h"
#    include "drivers/pin.h"
#    include "drivers/pwm.h"
#    include "drivers/adc.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/i2c.h"
#    include "drivers/uart_soft.h"
#    include "drivers/owi.h"
#    include "drivers/ds18b20.h"
#    include "drivers/ds3231.h"
#    include "drivers/mcp2515.h"
#    include "drivers/nrf24l01.h"
#    include "drivers/sd.h"
#    include "drivers/watchdog.h"
#
#    if defined(MCU_ATMEGA32U4)
#        include "drivers/usb.h"
#        include "drivers/usb_device.h"
#        include "drivers/usb/device/descriptors.h"
#        include "drivers/usb/device/class/cdc.h"
#    endif
#endif

#if defined(FAMILY_SAM)
#    include "drivers/chipid.h"
#    include "drivers/exti.h"
#    include "drivers/flash.h"
#    include "drivers/pin.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/sd.h"
#    include "drivers/can.h"
#    include "drivers/mcp2515.h"
#    include "drivers/adc.h"
#    include "drivers/dac.h"
#    include "drivers/usb.h"
#    include "drivers/usb_host.h"
#    include "drivers/usb/host/class/hid.h"
#    include "drivers/usb/host/class/mass_storage.h"
#endif

#if defined(FAMILY_ESP)
#    include "drivers/pin.h"
#    include "drivers/spi.h"
#    include "drivers/uart.h"
#    include "drivers/adc.h"
#endif

#if defined(FAMILY_STM32F1)
#    include "drivers/pin.h"
#    include "drivers/uart.h"
#    include "drivers/flash.h"
#endif

#if defined(FAMILY_STM32F2)
#    include "drivers/pin.h"
#    include "drivers/uart.h"
#    include "drivers/flash.h"
#    include "drivers/sdio.h"
#    include "drivers/bcm43362.h"
#endif

#if defined(FAMILY_STM32F3)
#    include "drivers/pin.h"
#    include "drivers/uart.h"
#    include "drivers/flash.h"
#endif

#include "debug/harness.h"

#include "multimedia/midi.h"

#include "inet/types.h"
#include "inet/inet.h"
#include "inet/socket.h"
#include "inet/http_server.h"
#include "inet/http_websocket_server.h"
#include "inet/http_websocket_client.h"
#include "inet/mqtt_client.h"
#include "inet/network_interface.h"
#include "inet/network_interface/slip.h"

#if defined(FAMILY_ESP)
#    include "inet/network_interface/wifi_station_espressif.h"
#endif

#endif
