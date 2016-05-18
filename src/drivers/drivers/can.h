/**
 * @file drivers/can.h
 * @version 0.5.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

#ifndef __DRIVERS_CAN_H__
#define __DRIVERS_CAN_H__

#include "simba.h"
#include "can_port.h"

#define CAN_SPEED_1000KBPS CAN_PORT_SPEED_1000KBPS
#define CAN_SPEED_500KBPS  CAN_PORT_SPEED_500KBPS
#define CAN_SPEED_250KBPS  CAN_PORT_SPEED_250KBPS

struct can_frame_t {
    uint32_t id;          /* Frame ID. */
    int extended_id;      /* Extended frame id flag (29 bits). */
    int size;             /* Number of bytes in data array. */
    int rtr;              /* Remote transmission request. */
    uint32_t timestamp;   /* Receive timestamp. */
    union {
        uint8_t u8[8];
        uint32_t u32[2];
    } data;               /* Payload. */
};

extern struct can_device_t can_device[CAN_DEVICE_MAX];

/**
 * Initialize given driver object.
 *
 * @param[in] self_p Driver object to initialize.
 * @param[in] dev_p Device to use.
 * @param[in] filter_p RX filter configuration.
 * @param[in] frames_p RX frame buffer in interrupt.
 * @param[in] length Length of frames array.
 *
 * @return zero(0) or negative error code.
 */
int can_init(struct can_driver_t *self_p,
             struct can_device_t *dev_p,
             uint32_t speed,
             void *rxbuf_p,
             size_t size);

/**
 * Starts the CAN device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int can_start(struct can_driver_t *self_p);

/**
 * Stops the CAN device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int can_stop(struct can_driver_t *self_p);

/**
 * Read CAN frame(s).
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] frame_p Array of read frames.
 * @param[in] size Size of frames buffer in words.
 *
 * @return zero(0) or negative error code.
 */
int can_read(struct can_driver_t *self_p,
             struct can_frame_t *frame_p,
             size_t size);

/**
 * Write a CAN frame(s).
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] frame_p Array of frames to write.
 * @param[in] size Size of frames buffer in words.
 *
 * @return zero(0) or negative error code.
 */
int can_write(struct can_driver_t *self_p,
              const struct can_frame_t *frame_p,
              size_t size);

#endif
