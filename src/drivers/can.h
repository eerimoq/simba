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

#ifndef __DRIVERS_CAN_H__
#define __DRIVERS_CAN_H__

#include "simba.h"
#include "can_port.h"

#define CAN_SPEED_1000KBPS CAN_PORT_SPEED_1000KBPS
#define CAN_SPEED_500KBPS  CAN_PORT_SPEED_500KBPS
#define CAN_SPEED_250KBPS  CAN_PORT_SPEED_250KBPS

struct can_frame_t {
    uint32_t id;                    /* Frame ID. */
    struct {
        uint8_t extended_frame : 1; /* Extended frame. Id is (29 bits). */
        uint8_t rtr : 1;            /* Remote transmission request. */
        uint8_t size : 4;           /* Number of bytes in data array. */
    };
#if CONFIG_CAN_FRAME_TIMESTAMP == 1
    uint32_t timestamp;             /* Frame reception/transmission
                                       timestamp. */
#endif
    union {
        uint8_t u8[8];
        uint32_t u32[2];
    } data;                         /* Payload. */
};

extern struct can_device_t can_device[CAN_DEVICE_MAX];

/**
 * Initialize CAN module. This function must be called before calling
 * any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int can_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[in] self_p Driver object to initialize.
 * @param[in] dev_p CAN device to use.
 * @param[in] speed Can bus speed. One of the defines with the
 *                  prefix``CAN_SPEED_``.
 * @param[in] rxbuf_p CAN frame reception buffer.
 * @param[in] size Size of the reception buffer in bytes.
 *
 * @return zero(0) or negative error code.
 */
int can_init(struct can_driver_t *self_p,
             struct can_device_t *dev_p,
             uint32_t speed,
             void *rxbuf_p,
             size_t size);

/**
 * Starts the CAN device using configuration in given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int can_start(struct can_driver_t *self_p);

/**
 * Stops the CAN device referenced by given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int can_stop(struct can_driver_t *self_p);

/**
 * Read one or more CAN frames from the CAN bus. Blocks until the
 * frame(s) are received.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] frame_p Array of read frames.
 * @param[in] size Size of frames buffer in bytes. Must be a multiple
 *                 of ``sizeof(struct can_frame_t)``.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t can_read(struct can_driver_t *self_p,
                 struct can_frame_t *frame_p,
                 size_t size);

/**
 * Write one or more CAN frames to the CAN bus. Blocks until the
 * frame(s) have been transmitted.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] frame_p Array of frames to write.
 * @param[in] size Size of frames buffer in bytes. Must be a multiple
 *                 of ``sizeof(struct can_frame_t)``.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t can_write(struct can_driver_t *self_p,
                  const struct can_frame_t *frame_p,
                  size_t size);

#endif
