/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
