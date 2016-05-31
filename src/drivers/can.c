/**
 * @file can.c
 * @version 0.5.0
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

#include "simba.h"

#include "can_port.i"

int can_module_init(void)
{
    return (0);
}

int can_init(struct can_driver_t *self_p,
             struct can_device_t *dev_p,
             uint32_t speed,
             void *rxbuf_p,
             size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);
    ASSERTN(rxbuf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    self_p->dev_p = dev_p;

    chan_init(&self_p->chout,
              chan_read_null,
              (ssize_t (*)(chan_t *, const void *, size_t))write_cb,
              chan_size_null);

    queue_init(&self_p->chin, rxbuf_p, size);

    return (can_port_init(self_p, dev_p, speed));
}

int can_start(struct can_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (can_port_start(self_p));
}

int can_stop(struct can_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (can_port_stop(self_p));
}

ssize_t can_read(struct can_driver_t *self_p,
                 struct can_frame_t *frame_p,
                 size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (queue_read(&self_p->chin, frame_p, size));
}

ssize_t can_write(struct can_driver_t *self_p,
                  const struct can_frame_t *frame_p,
                  size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (chan_write(&self_p->chout, frame_p, size));
}
