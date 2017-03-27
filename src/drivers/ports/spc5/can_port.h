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

/* #define CAN_PORT_SPEED_1000KBPS (CAN_BR_PHASE2(3)         \ */
/*                                  | CAN_BR_PHASE1(3)       \ */
/*                                  | CAN_BR_PROPAG(2)       \ */
/*                                  | CAN_BR_SJW(3)          \ */
/*                                  | CAN_BR_BRP(0x6)) */

#define CAN_PORT_SPEED_500KBPS  (SPC5_FLEXCAN_CTRL_PSEG2(1)             \
                                 | SPC5_FLEXCAN_CTRL_PSEG1(1)           \
                                 | SPC5_FLEXCAN_CTRL_PROPSEG(2)         \
                                 | SPC5_FLEXCAN_CTRL_RJW(2)             \
                                 | SPC5_FLEXCAN_CTRL_PRESDIV(3))

/* #define CAN_PORT_SPEED_250KBPS  (CAN_BR_PHASE2(1)         \ */
/*                                  | CAN_BR_PHASE1(1)       \ */
/*                                  | CAN_BR_PROPAG(2)       \ */
/*                                  | CAN_BR_SJW(2)          \ */
/*                                  | CAN_BR_BRP(0x29)) */

struct can_device_t {
    struct can_driver_t *drv_p;
    volatile struct spc5_flexcan_t *regs_p;
    int id;
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
