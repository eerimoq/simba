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

#include "simba.h"

#if CONFIG_CAN == 1

#include "can_port.i"

static ssize_t base_chan_read(void *base_p, void *buf_p, size_t size)
{
    struct can_driver_t *self_p;

    self_p = base_p;

    return (queue_read(&self_p->chin, buf_p, size));
}

static size_t base_chan_size(void *base_p)
{
    struct can_driver_t *self_p;

    self_p = base_p;

    return (queue_size(&self_p->chin));
}

int can_module_init(void)
{
    return (can_port_module_init());
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

    chan_init(&self_p->base,
              base_chan_read,
              (chan_write_fn_t)write_cb,
              base_chan_size);

    queue_init(&self_p->chin, rxbuf_p, size);

    sem_init(&self_p->sem, 0, 1);

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

    return (chan_write(&self_p->base, frame_p, size));
}

#endif
