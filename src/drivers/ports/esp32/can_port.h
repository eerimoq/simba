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

#ifndef __DRIVERS_CAN_PORT_H__
#define __DRIVERS_CAN_PORT_H__

#define CAN_PORT_SPEED_1000KBPS (ESP32_CAN_BTIM0_BRP(0x4)               \
                                 | ESP32_CAN_BTIM0_SJW(0x1)             \
                                 | ((ESP32_CAN_BTIM1_TSEG1(0x4)         \
                                     | ESP32_CAN_BTIM1_TSEG2(0x1)       \
                                     | ESP32_CAN_BTIM1_SAM) << 8))
                                   
#define CAN_PORT_SPEED_500KBPS (ESP32_CAN_BTIM0_BRP(0x4)                \
                                | ESP32_CAN_BTIM0_SJW(0x1)              \
                                | ((ESP32_CAN_BTIM1_TSEG1(0xc)          \
                                    | ESP32_CAN_BTIM1_TSEG2(0x1)        \
                                    | ESP32_CAN_BTIM1_SAM) << 8))

#define CAN_PORT_SPEED_250KBPS (ESP32_CAN_BTIM0_BRP(0x9)                \
                                | ESP32_CAN_BTIM0_SJW(0x1)              \
                                | ((ESP32_CAN_BTIM1_TSEG1(0xc)          \
                                    | ESP32_CAN_BTIM1_TSEG2(0x1)        \
                                    | ESP32_CAN_BTIM1_SAM) << 8))

struct can_device_t {
    struct can_driver_t *drv_p;
    volatile struct esp32_can_t *regs_p;
    struct pin_device_t *tx_pin_p;
    struct pin_device_t *rx_pin_p;
};

struct can_driver_t {
    struct chan_t base; /* Used as output channel. */
    struct can_device_t *dev_p;
    uint32_t speed;
    struct thrd_t *thrd_p;
    const struct can_frame_t *txframe_p;
    size_t txsize;
    struct queue_t chin;
    struct sem_t sem;
};

#endif
