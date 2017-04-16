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

#include "socket_device.h"

static ssize_t write_cb(void *arg_p,
                        const void *buf_p,
                        size_t size)
{
    struct can_driver_t *self_p;
    struct can_device_t *dev_p;
    const struct can_frame_t *frame_p;
    char line[64];

    self_p = arg_p;
    dev_p = self_p->dev_p;
    frame_p = (struct can_frame_t *)buf_p;

    /* Format the line to the to the client. */
    sprintf(&line[0],
            "%08x,%d,%d,%02x%02x%02x%02x%02x%02x%02x%02x\r\n",
            frame_p->id,
            (int)frame_p->extended_frame,
            (int)frame_p->size,
            frame_p->data.u8[0],
            frame_p->data.u8[1],
            frame_p->data.u8[2],
            frame_p->data.u8[3],
            frame_p->data.u8[4],
            frame_p->data.u8[5],
            frame_p->data.u8[6],
            frame_p->data.u8[7]);

    sys_lock();

    if (socket_device_is_can_device_connected_isr(dev_p) == 1) {
        if (socket_device_can_device_write_isr(dev_p, &line[0], 31) != 31) {
            size = -1;
        }
    }

    sys_unlock();

    return (size);
}

static int can_port_module_init()
{
    return (socket_device_module_init());
}

static int can_port_init(struct can_driver_t *self_p,
                         struct can_device_t *dev_p,
                         uint32_t speed)
{
    return (0);
}

static int can_port_start(struct can_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;

    return (0);
}

static int can_port_stop(struct can_driver_t *self_p)
{
    self_p->dev_p->drv_p = NULL;

    return (0);
}
