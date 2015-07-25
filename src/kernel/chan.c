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

struct spin_lock_t chan_lock;

int chan_module_init()
{
    return (spin_init(&chan_lock));
}

int chan_init(struct chan_t *chan_p,
              ssize_t (*read)(chan_t *chan_p, void *buf_p, size_t size),
              ssize_t (*write)(chan_t *chan_p, const void *buf_p, size_t size),
              size_t (*size)(chan_t *chan_p))
{
    chan_p->read = read;
    chan_p->write = write;
    chan_p->size = size;
    LIST_SL_INIT(&chan_p->readers);

    return (0);
}

int chan_list_init(struct chan_list_t *list_p,
                   void *workspace_p,
                   size_t size)
{
    list_p->max = (size / (sizeof(list_p->chans_pp[0]) + sizeof(list_p->elements_p[0])));

    if (list_p->max == 0) {
        return (-EINVAL);
    }

    list_p->chans_pp = workspace_p;
    list_p->elements_p = (workspace_p + list_p->max * sizeof(list_p->chans_pp[0]));
    list_p->len = 0;
    list_p->flags = 0;

    return (0);
}

int chan_list_destroy(struct chan_list_t *list_p)
{
    struct chan_t *chan_p = NULL;
    struct chan_element_t *element_p, *previous_element_p, *iterator_element_p;
    size_t i;
    spin_irq_t irq;
    struct list_sl_iterator_t iterator;

    spin_lock(&chan_lock, &irq);

    for (i = 0; i < list_p->len; i++) {
        chan_p = list_p->chans_pp[i];
        element_p = &list_p->elements_p[i];

        /* Remove any queued elements. An emelemt is queued if the
           thrd_p pointer is set.*/
        if (element_p->thrd_p != NULL) {
            LIST_SL_REMOVE_ELEM(&chan_p->readers,
                                &iterator,
                                element_p,
                                iterator_element_p,
                                previous_element_p);
        }
    }

    spin_unlock(&chan_lock, &irq);

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
    list_p->elements_p[list_p->len].thrd_p = NULL;
    list_p->elements_p[list_p->len].list_p = list_p;
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
    struct chan_element_t *element_p;
    size_t i;
    spin_irq_t irq;

    while (1) {
        spin_lock(&chan_lock, &irq);

        /* Check for data on any chan.*/
        for (i = 0; i < list_p->len; i++) {
            chan_p = list_p->chans_pp[i];
            if (chan_p->size(chan_p) > 0) {
                goto out;
            }
        }

        /* Add thrd as reader on all chans.*/
        list_p->flags = CHAN_LIST_POLLING;

        for (i = 0; i < list_p->len; i++) {
            chan_p = list_p->chans_pp[i];
            element_p = &list_p->elements_p[i];

            /* Add if not already in the reader list from a previous
               poll.*/
            if (element_p->thrd_p == NULL) {
                element_p->thrd_p = thrd_self();
                LIST_SL_ADD_TAIL(&chan_p->readers, element_p);
            }
        }

        spin_unlock(&chan_lock, &irq);

        /* Not data was available, wait for data to be written to one
           of the channels. */
        thrd_suspend(NULL);
    }

 out:
    spin_unlock(&chan_lock, &irq);

    return (chan_p);
}
