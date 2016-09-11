/**
 * @file chan.c
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

#define CHAN_LIST_POLLING 0x1

static struct chan_t null;

int chan_module_init(void)
{
    chan_init(&null, chan_read_null, chan_write_null, chan_size_null);

    return (0);
}

int chan_init(struct chan_t *self_p,
              thrd_read_fn_t read,
              thrd_write_fn_t write,
              thrd_size_fn_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(read != NULL, EINVAL);
    ASSERTN(write != NULL, EINVAL);
    ASSERTN(size != NULL, EINVAL);

    self_p->read = read;
    self_p->write = write;
    self_p->size = size;
    self_p->writer_p = NULL;
    self_p->reader_p = NULL;
    self_p->list_p = NULL;

    return (0);
}

int chan_list_init(struct chan_list_t *list_p,
                   void *workspace_p,
                   size_t size)
{
    ASSERTN(list_p != NULL, EINVAL);
    ASSERTN(workspace_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

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
    ASSERTN(list_p != NULL, EINVAL);

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

ssize_t chan_read(void *self_p,
                  void *buf_p,
                  size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (((struct chan_t *)self_p)->read(self_p, buf_p, size));
}

ssize_t chan_write(void *self_p,
                   const void *buf_p,
                   size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (((struct chan_t *)self_p)->write(self_p, buf_p, size));
}

size_t chan_size(void *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (((struct chan_t *)self_p)->size(self_p));
}

int chan_list_add(struct chan_list_t *list_p, void *chan_p)
{
    ASSERTN(list_p != NULL, EINVAL);
    ASSERTN(chan_p != NULL, EINVAL);

    if (list_p->len == list_p->max) {
        return (-ENOMEM);
    }

    list_p->chans_pp[list_p->len] = chan_p;
    list_p->len++;

    return (0);
}

int chan_list_remove(struct chan_list_t *list_p, void *chan_p)
{
    ASSERTN(list_p != NULL, EINVAL);
    ASSERTN(chan_p != NULL, EINVAL);

    int i;
    
    for (i = 0; i < list_p->max; i++) {
        if (list_p->chans_pp[i] == chan_p) {
            list_p->max--;
            list_p->chans_pp[i] = list_p->chans_pp[list_p->max];
            return (0);
        }
    }
    
    return (-1);
}

void *chan_list_poll(struct chan_list_t *list_p,
                       struct time_t *timeout_p)
{
    ASSERTN(list_p != NULL, EINVAL);

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
        if (thrd_suspend_isr(timeout_p) == -ETIMEDOUT) {
            chan_p = NULL;
            goto out;
        }
    }

 out:
    sys_unlock();
    
    return (chan_p);
}

void *chan_poll(void *chan_p, struct time_t *timeout_p)
{
    void *res_p;
    struct chan_list_t list;
    struct chan_t *workspace_p;
    
    chan_list_init(&list, &workspace_p, sizeof(workspace_p));
    chan_list_add(&list, chan_p);
    res_p = chan_list_poll(&list, timeout_p);
    chan_list_remove(&list, chan_p);
    
    return (res_p);
}

void *chan_null(void)
{
    return (&null);
}

ssize_t chan_read_null(void *self_p,
                       void *buf_p,
                       size_t size)
{
    return (-1);
}

ssize_t chan_write_null(void *self_p,
                        const void *buf_p,
                        size_t size)
{
    return (size);
}

size_t chan_size_null(void *self_p)
{
    return (0);
}

int chan_is_polled_isr(struct chan_t *self_p)
{
    if (self_p->list_p != NULL) {
        if (self_p->list_p->flags & CHAN_LIST_POLLING) {
            self_p->list_p->flags = 0;
            return (1);
        }
    }
    
    return (0);
}
