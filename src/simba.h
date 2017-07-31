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
#include <math.h>

#undef time_t

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <ctype.h>
#include <math.h>

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

#include "sync/mutex.h"
#include "sync/cond.h"
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
#include "encode/nmea.h"

#include "hash/crc.h"
#include "hash/sha1.h"

#include "inet/types.h"
#include "inet/inet.h"

#ifdef PORT_HAS_EXTI
#    include "drivers/basic/exti.h"
#endif
#ifdef PORT_HAS_PIN
#    include "drivers/basic/pin.h"
#endif
#ifdef PORT_HAS_I2C_SOFT
#    include "drivers/network/i2c_soft.h"
#endif
#ifdef PORT_HAS_PWM
#    include "drivers/basic/pwm.h"
#endif
#ifdef PORT_HAS_PWM_SOFT
#    include "drivers/basic/pwm_soft.h"
#endif
#ifdef PORT_HAS_ADC
#    include "drivers/basic/adc.h"
#endif
#ifdef PORT_HAS_DAC
#    include "drivers/basic/dac.h"
#endif
#ifdef PORT_HAS_SPI
#    include "drivers/network/spi.h"
#endif
#ifdef PORT_HAS_UART
#    include "drivers/network/uart.h"
#endif
#ifdef PORT_HAS_SD
#    include "drivers/storage/sd.h"
#endif
#ifdef PORT_HAS_DS18B20
#    include "drivers/sensors/ds18b20.h"
#endif
#ifdef PORT_HAS_OWI
#    include "drivers/network/owi.h"
#endif
#ifdef PORT_HAS_FLASH
#    include "drivers/storage/flash.h"
#endif
#ifdef PORT_HAS_ANALOG_INPUT_PIN
#    include "drivers/basic/analog_input_pin.h"
#endif
#ifdef PORT_HAS_ANALOG_OUTPUT_PIN
#    include "drivers/basic/analog_output_pin.h"
#endif
#ifdef PORT_HAS_CAN
#    include "drivers/network/can.h"
#endif
#ifdef PORT_HAS_I2C
#    include "drivers/network/i2c.h"
#endif
#ifdef PORT_HAS_UART_SOFT
#    include "drivers/network/uart_soft.h"
#endif
#ifdef PORT_HAS_DS3231
#    include "drivers/various/ds3231.h"
#endif
#ifdef PORT_HAS_MCP2515
#    include "drivers/network/mcp2515.h"
#endif
#ifdef PORT_HAS_NRF24L01
#    include "drivers/network/nrf24l01.h"
#endif
#ifdef PORT_HAS_WATCHDOG
#    include "drivers/basic/watchdog.h"
#endif
#ifdef PORT_HAS_USB
#    include "drivers/network/usb.h"
#endif
#ifdef PORT_HAS_USB_DEVICE
#    include "drivers/network/usb_device.h"
#    include "drivers/network/usb/device/descriptors.h"
#    include "drivers/network/usb/device/class/cdc.h"
#endif
#ifdef PORT_HAS_CHIPID
#    include "drivers/basic/chipid.h"
#endif
#ifdef PORT_HAS_USB_HOST
#    include "drivers/network/usb_host.h"
#    include "drivers/network/usb/host/class/hid.h"
#    include "drivers/network/usb/host/class/mass_storage.h"
#endif
#ifdef PORT_HAS_ESP_WIFI
#    include "drivers/network/esp_wifi.h"
#    include "drivers/network/esp_wifi/station.h"
#    include "drivers/network/esp_wifi/softap.h"
#endif
#ifdef PORT_HAS_RANDOM
#    include "drivers/basic/random.h"
#endif
#ifdef CONFIG_LED_7SEG_HT16K33
#    include "drivers/displays/led_7seg_ht16k33.h"
#endif
#ifdef PORT_HAS_WS2812
#    include "drivers/displays/ws2812.h"
#endif
#ifdef PORT_HAS_SHT3XD
#    include "drivers/sensors/sht3xd.h"
#endif
#ifdef PORT_HAS_EEPROM_SOFT
#    include "drivers/storage/eeprom_soft.h"
#endif
#ifdef PORT_HAS_EEPROM_I2C
#    include "drivers/storage/eeprom_i2c.h"
#endif
#ifdef PORT_HAS_XBEE
#    include "drivers/network/xbee.h"
#endif
#ifdef PORT_HAS_XBEE_CLIENT
#    include "drivers/network/xbee_client.h"
#endif
#ifdef PORT_HAS_HX711
#    include "drivers/sensors/hx711.h"
#endif
#ifdef PORT_HAS_GNSS
#    include "drivers/various/gnss.h"
#endif
#ifdef PORT_HAS_BMP280
#    include "drivers/sensors/bmp280.h"
#endif
#ifdef PORT_HAS_DHT
#    include "drivers/sensors/dht.h"
#endif
#ifdef PORT_HAS_PCINT
#    include "drivers/basic/pcint.h"
#endif
#ifdef PORT_HAS_HD44780
#    include "drivers/displays/hd44780.h"
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

#include "science/science.h"
#include "science/math.h"

#include <simba_gen.h>

#ifdef __cplusplus
}
#endif

#endif
