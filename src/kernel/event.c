/**
 * @file event.c
 * @version 0.7.0
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
 * MEREVENTTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
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

    /* Resume waiting thread. */
    if (self_p->base.reader_p != NULL)  {
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
