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

    chan_init(&self_p->base,
              base_chan_read,
              (ssize_t (*)(void *, const void *, size_t))write_cb,
              base_chan_size);

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

    return (chan_write(&self_p->base, frame_p, size));
}

#endif
