/**
 * @file chan.c
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

#define CHAN_LIST_POLLING 0x1

int chan_module_init(void)
{
    return (0);
}

int chan_init(struct chan_t *chan_p,
              ssize_t (*read)(chan_t *chan_p, void *buf_p, size_t size),
              ssize_t (*write)(chan_t *chan_p, const void *buf_p, size_t size),
              size_t (*size)(chan_t *chan_p))
{
    chan_p->read = read;
    chan_p->write = write;
    chan_p->size = size;
    chan_p->writer_p = NULL;
    chan_p->reader_p = NULL;
    chan_p->list_p = NULL;

    return (0);
}

int chan_list_init(struct chan_list_t *list_p,
                   void *workspace_p,
                   size_t size)
{
    list_p->max = (size / sizeof(list_p->chans_pp[0]));

    if (list_p->max == 0) {
        return (-EINVAL);
    }

    list_p->chans_pp = workspace_p;
    list_p->len = 0;
    list_p->flags = 0;

    return (0);
}

int chan_list_destroy(struct chan_list_t *list_p)
{
    struct chan_t *chan_p = NULL;
    size_t i;

    sys_lock();

    for (i = 0; i < list_p->len; i++) {
        chan_p = list_p->chans_pp[i];
        chan_p->list_p = NULL;
    }

    sys_unlock();

    return (0);
}

ssize_t chan_read(chan_t *chan_p,
                  void *buf_p,
                  size_t size)
{
    return (((struct chan_t *)chan_p)->read(chan_p, buf_p, size));
}

ssize_t chan_write(chan_t *chan_p,
                   const void *buf_p,
                   size_t size)
{
    return (((struct chan_t *)chan_p)->write(chan_p, buf_p, size));
}

size_t chan_size(chan_t *chan_p)
{
    return (((struct chan_t *)chan_p)->size(chan_p));
}

int chan_list_add(struct chan_list_t *list_p, chan_t *chan_p)
{
    if (list_p->len == list_p->max) {
        return (-ENOMEM);
    }

    list_p->chans_pp[list_p->len] = chan_p;
    list_p->len++;

    return (0);
}

int chan_list_remove(struct chan_list_t *list, chan_t *chan_p)
{
    ASSERT(0, "chan_list_remove not implemented\n");

    return (0);
}

chan_t *chan_list_poll(struct chan_list_t *list_p)
{
    struct chan_t *chan_p = NULL;
    size_t i;

    sys_lock();

    while (1) {
        /* Check if data is available on any channel. */
        for (i = 0; i < list_p->len; i++) {
            chan_p = list_p->chans_pp[i];

            if (chan_p->size(chan_p) > 0) {
                goto out;
            }
        }

        /* Add the thread as a reader on all channels.*/
        list_p->flags = CHAN_LIST_POLLING;

        for (i = 0; i < list_p->len; i++) {
            chan_p = list_p->chans_pp[i];
            chan_p->reader_p = thrd_self();
            chan_p->list_p = list_p;
        }

        /* Not data was available, wait for data to be written to one
           of the channels. */
        thrd_suspend_irq(NULL);
    }

 out:
    sys_unlock();

    return (chan_p);
}

int chan_is_polled_irq(struct chan_t *chan_p)
{
    if (chan_p->list_p != NULL) {
        if (chan_p->list_p->flags & CHAN_LIST_POLLING) {
            chan_p->list_p->flags = 0;
            return (1);
        }
    }
    
    return (0);
}
