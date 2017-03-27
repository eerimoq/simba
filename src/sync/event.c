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

int event_init(struct event_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    chan_init(&self_p->base,
              (ssize_t (*)(void *, void *, size_t))event_read,
              (ssize_t (*)(void *, const void *, size_t))event_write,
              (size_t (*)(void *))event_size);

    self_p->mask = 0;
    self_p->reader_mask = 0;

    return (0);
}

ssize_t event_read(struct event_t *self_p,
                   void *buf_p,
                   size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size == sizeof(uint32_t), EINVAL);

    uint32_t *mask_p, mask;

    mask_p = (uint32_t *)buf_p;

    sys_lock();

    mask = (self_p->mask & *mask_p);

    /* Event already set? Otherwise wait for it. */
    if (mask != 0) {
        *mask_p = mask;
    } else {
        self_p->reader_mask = *mask_p;
        self_p->base.reader_p = thrd_self();
        thrd_suspend_isr(NULL);
        *mask_p = (self_p->mask & *mask_p);
    }

    /* Remove read events from the event channel. */
    self_p->mask &= (~(*mask_p));

    sys_unlock();

    return (size);
}

ssize_t event_write(struct event_t *self_p,
                    const void *buf_p,
                    size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size == sizeof(uint32_t), EINVAL);

    sys_lock();
    size = event_write_isr(self_p, buf_p, size);
    sys_unlock();

    return (size);
}

ssize_t event_write_isr(struct event_t *self_p,
                        const void *buf_p,
                        size_t size)
{
    if (chan_is_polled_isr(&self_p->base)) {
        thrd_resume_isr(self_p->base.reader_p, 0);
        self_p->base.reader_p = NULL;
    }

    self_p->mask |= *(uint32_t *)buf_p;

    /* Resume reader thread waiting for given event(s). */
    if ((self_p->base.reader_p != NULL)
        && ((self_p->reader_mask & self_p->mask) != 0))  {
        thrd_resume_isr(self_p->base.reader_p, 0);
        self_p->base.reader_p = NULL;
    }

    return (size);
}

ssize_t event_size(struct event_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->mask != 0);
}
