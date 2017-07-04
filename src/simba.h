/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#ifndef __SIMBA_H__
#define __SIMBA_H__

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(ARCH_AVR) && defined(__cplusplus)

#define __CORRECT_ISO_CPP_STDLIB_H_PROTO
#define __time_t_defined  1
#define time_t int
#define __timer_t_defined 1

#if defined(ARCH_ARM)
    typedef long ssize_t;
#else
#    include <cstdio>
#endif

#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdint>

#undef time_t

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <ctype.h>

#endif

#include "compiler.h"
#include <config.h>
#include "config_default.h"
#include "board.h"
#include "mcu.h"

#include "kernel/errno.h"
#include "kernel/types.h"
#include "kernel/assert.h"

#include "oam/settings.h"

#include "collections/binary_tree.h"
#include "collections/bits.h"
#include "collections/fifo.h"
#include "collections/list.h"
#include "collections/hash_map.h"
#include "collections/circular_buffer.h"

#include "kernel/time.h"

#include "sync/sem.h"

#include "sync/chan.h"
#include "kernel/sys.h"
#include "kernel/timer.h"
#include "kernel/thrd.h"

#include "sync/queue.h"
#include "sync/event.h"
#include "sync/rwlock.h"
#include "sync/bus.h"

#include "alloc/heap.h"
#include "alloc/circular_heap.h"

#if CONFIG_FAT16 == 1
#    include "filesystems/fat16.h"
#endif
#include "filesystems/spiffs.h"

#include "oam/console.h"
#include "filesystems/fs.h"
#include "oam/shell.h"
#include "oam/service.h"
#include "oam/nvm.h"

#include "debug/log.h"

#include "text/color.h"
#include "text/re.h"
#include "text/std.h"
#include "text/configfile.h"
#include "text/emacs.h"

#include "encode/base64.h"
#include "encode/json.h"

#include "hash/crc.h"
#include "hash/sha1.h"

#include "inet/types.h"
#include "inet/inet.h"

#ifdef PORT_HAS_EXTI
#    include "drivers/exti.h"
#endif
#ifdef PORT_HAS_PIN
#    include "drivers/pin.h"
#endif
#ifdef PORT_HAS_I2C_SOFT
#    include "drivers/i2c_soft.h"
#endif
#ifdef PORT_HAS_PWM
#    include "drivers/pwm.h"
#endif
#ifdef PORT_HAS_PWM_SOFT
#    include "drivers/pwm_soft.h"
#endif
#ifdef PORT_HAS_ADC
#    include "drivers/adc.h"
#endif
#ifdef PORT_HAS_DAC
#    include "drivers/dac.h"
#endif
#ifdef PORT_HAS_SPI
#    include "drivers/spi.h"
#endif
#ifdef PORT_HAS_UART
#    include "drivers/uart.h"
#endif
#ifdef PORT_HAS_SD
#    include "drivers/sd.h"
#endif
#ifdef PORT_HAS_DS18B20
#    include "drivers/ds18b20.h"
#endif
#ifdef PORT_HAS_OWI
#    include "drivers/owi.h"
#endif
#ifdef PORT_HAS_FLASH
#    include "drivers/flash.h"
#endif
#ifdef PORT_HAS_ANALOG_INPUT_PIN
#    include "drivers/analog_input_pin.h"
#endif
#ifdef PORT_HAS_ANALOG_OUTPUT_PIN
#    include "drivers/analog_output_pin.h"
#endif
#ifdef PORT_HAS_CAN
#    include "drivers/can.h"
#endif
#ifdef PORT_HAS_I2C
#    include "drivers/i2c.h"
#endif
#ifdef PORT_HAS_UART_SOFT
#    include "drivers/uart_soft.h"
#endif
#ifdef PORT_HAS_DS3231
#    include "drivers/ds3231.h"
#endif
#ifdef PORT_HAS_MCP2515
#    include "drivers/mcp2515.h"
#endif
#ifdef PORT_HAS_NRF24L01
#    include "drivers/nrf24l01.h"
#endif
#ifdef PORT_HAS_WATCHDOG
#    include "drivers/watchdog.h"
#endif
#ifdef PORT_HAS_USB
#    include "drivers/usb.h"
#endif
#ifdef PORT_HAS_USB_DEVICE
#    include "drivers/usb_device.h"
#    include "drivers/usb/device/descriptors.h"
#    include "drivers/usb/device/class/cdc.h"
#endif
#ifdef PORT_HAS_CHIPID
#    include "drivers/chipid.h"
#endif
#ifdef PORT_HAS_USB_HOST
#    include "drivers/usb_host.h"
#    include "drivers/usb/host/class/hid.h"
#    include "drivers/usb/host/class/mass_storage.h"
#endif
#ifdef PORT_HAS_ESP_WIFI
#    include "drivers/esp_wifi.h"
#    include "drivers/esp_wifi/station.h"
#    include "drivers/esp_wifi/softap.h"
#endif
#ifdef PORT_HAS_RANDOM
#    include "drivers/random.h"
#endif
#ifdef CONFIG_LED_7SEG_HT16K33
#    include "drivers/led_7seg_ht16k33.h"
#endif
#ifdef PORT_HAS_WS2812
#    include "drivers/ws2812.h"
#endif
#ifdef PORT_HAS_SHT3XD
#    include "drivers/sht3xd.h"
#endif
#ifdef PORT_HAS_EEPROM_SOFT
#    include "drivers/eeprom_soft.h"
#endif
#ifdef PORT_HAS_EEPROM_I2C
#    include "drivers/eeprom_i2c.h"
#endif
#ifdef PORT_HAS_XBEE
#    include "drivers/xbee.h"
#endif

#include "inet/isotp.h"

#include "debug/harness.h"

#include "multimedia/midi.h"

#include "inet/socket.h"

#if defined(FAMILY_ESP) || defined(FAMILY_ESP32) || defined(FAMILY_LINUX)
#    include "inet/ssl.h"
#endif

#include "inet/slip.h"
#include "inet/http_server.h"
#include "inet/http_websocket_server.h"
#include "inet/http_websocket_client.h"
#include "inet/tftp_server.h"
#include "inet/mqtt_client.h"
#include "inet/network_interface.h"
#include "inet/network_interface/slip.h"
#include "inet/network_interface/wifi.h"
#include "inet/ping.h"

#include "oam/soam.h"

#if defined(FAMILY_ESP) || defined(FAMILY_ESP32)
#    include "inet/network_interface/driver/esp.h"
#endif

#if defined(FAMILY_LINUX) || defined(FAMILY_ESP32)
#    include "oam/upgrade.h"
#    include "oam/upgrade/kermit.h"
#    include "oam/upgrade/uds.h"
#    include "oam/upgrade/http.h"
#    include "oam/upgrade/tftp.h"
#endif

#include <simba_gen.h>

#ifdef __cplusplus
}
#endif

#endif
