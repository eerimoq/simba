/**
 * @file drivers/can.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#include "simba.h"

#define CAN_MAILBOX_MAX 32

enum can_tp_type_t {
    can_tp_type_single_t = 0,
    can_tp_type_first_t,
    can_tp_type_consecutive_t,
    can_tp_type_flow_t
};

void ISR(can_rx)
{
    struct canif_frame_t *frame_p;
    struct can_device_t *dev_p = &can_device[0];

    if (dev_p->read_pos == dev_p->write_pos) {
        return;
    }

    /* Read frame. */
    frame = &dev_p->frames[dev_p->write_pos];
    frame_p->id = 0;
    frame_p->size = 0;
    frame_p->rtr = 0;
    frame_p->timestamp = 0;
    frame_p->data[0] = 0;
    dev_p->write_pos++;
    dev_p->write_pos %= dev_p->length;

    /* Notify rx thread. */
    if (dev_p->drv_p->thrd != NULL) {
        thrd_resume_irq(dev_p->drv_p->thrd, 0);
    }
}

int can_module_init(void)
{
    return (0);
}

int can_init(struct can_driver_t *drv_p,
             struct can_device_t *dev_p,
             struct canif_filter_t *filter_p,
             struct canif_frame_t *frames_p,
             size_t length)
{
    int i;
    uint32_t mask;

    drv_p->base.read = (int (*)(void *, int, struct canif_frame_t *))can_read;
    drv_p->base.write = (int (*)(void *, int, const struct canif_frame_t *))can_write;

    /* Initialize message filtering. */
    for (i = 0; i < CAN_MAILBOX_MAX; i++) {
        mask = 0xffffffff;

        while (filter_p->mailbox != CANIF_MAILBOX_INVALID) {
            mask &= filter_p->id;
        }

        //REG = filter;
    }
    
    return (0);
}

int can_start(struct can_driver_t *drv_p)
{
    return (0);
}

int can_stop(struct can_driver_t *drv_p)
{
    return (0);
}

int can_read(struct can_driver_t *drv_p,
             int mailbox,
             struct canif_frame_t *frame_p)
{
    struct can_device_t *dev_p = drv_p->dev_p;
    
    /* if (is_empty(drv_p, mailbox)) { */
    /*     thrd_suspend(); */
    /* } */

    *frame = dev_p->frames[dev_p->read_pos];
    dev_p->read_pos++;
    dev_p->read_pos %= dev_p->length;

    return (0);
}

int can_write(struct can_driver_t *drv_p,
              int mailbox,
              const struct canif_frame_t *frame_p)
{
    //    drv_p->dev_p->mailbox[mailbox] = *frame;
    return (0);
}
