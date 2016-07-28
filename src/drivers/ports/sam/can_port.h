/**
 * @file drivers/ports/sam/can_port.h
 * @version 3.1.1
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

#ifndef __DRIVERS_CAN_PORT_H__
#define __DRIVERS_CAN_PORT_H__

#include <io.h>

#define CAN_PORT_SPEED_1000KBPS (CAN_BR_PHASE2(3)         \
                                 | CAN_BR_PHASE1(3)       \
                                 | CAN_BR_PROPAG(2)       \
                                 | CAN_BR_SJW(3)          \
                                 | CAN_BR_BRP(0x6))

#define CAN_PORT_SPEED_500KBPS  (CAN_BR_PHASE2(1)         \
                                 | CAN_BR_PHASE1(1)       \
                                 | CAN_BR_PROPAG(2)       \
                                 | CAN_BR_SJW(2)          \
                                 | CAN_BR_BRP(0x14))

#define CAN_PORT_SPEED_250KBPS  (CAN_BR_PHASE2(1)         \
                                 | CAN_BR_PHASE1(1)       \
                                 | CAN_BR_PROPAG(2)       \
                                 | CAN_BR_SJW(2)          \
                                 | CAN_BR_BRP(0x29))

struct can_device_t {
    struct can_driver_t *drv_p;
    volatile struct sam_can_t *regs_p;
    int id;
    struct {
        volatile struct sam_pio_t *pio_p;
        uint32_t mask;
    } rx;
    struct {
        volatile struct sam_pio_t *pio_p;
        uint32_t mask;
    } tx;
};

struct can_driver_t {
    struct can_device_t *dev_p;
    uint32_t speed;
    struct thrd_t *thrd_p;
    const struct can_frame_t *txframe_p;
    size_t txsize;
    struct chan_t chout;
    struct queue_t chin;
    struct sem_t sem;
};

#endif
