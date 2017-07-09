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

#define CHAN_LIST_POLLING 0x1

static const struct chan_t null = {
    .read = chan_read_null,
    .write = chan_write_null,
    .write_isr = chan_write_null,
    .size = chan_size_null,
    .control = chan_control_null
};

int chan_module_init(void)
{
    return (0);
}

int chan_init(struct chan_t *self_p,
              chan_read_fn_t read,
              chan_write_fn_t write,
              chan_size_fn_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(read != NULL, EINVAL);
    ASSERTN(write != NULL, EINVAL);
    ASSERTN(size != NULL, EINVAL);

    self_p->read = read;
    self_p->write = write;
    self_p->write_isr = chan_write_null;
    self_p->size = size;
    self_p->control = chan_control_null;
    self_p->write_filter_cb = NULL;
    self_p->write_filter_isr_cb = NULL;
    self_p->reader_p = NULL;
    self_p->list_p = NULL;

    return (0);
}

int chan_set_write_filter_cb(struct chan_t *self_p,
                             chan_write_filter_fn_t write_filter_cb)
{
    self_p->write_filter_cb = write_filter_cb;

    return (0);
}

int chan_set_write_filter_isr_cb(struct chan_t *self_p,
                                 chan_write_filter_fn_t write_filter_isr_cb)
{
    self_p->write_filter_isr_cb = write_filter_isr_cb;

    return (0);
}

int chan_set_write_isr_cb(struct chan_t *self_p,
                          chan_write_fn_t write_isr_cb)
{
    self_p->write_isr = write_isr_cb;

    return (0);
}

int chan_list_init(struct chan_list_t *list_p,
                   void *workspace_p,
                   size_t size)
{
    ASSERTN(list_p != NULL, EINVAL);
    ASSERTN(workspace_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);
    ASSERTN((size / sizeof(list_p->chans_pp[0])) > 0, EINVAL);

    list_p->max = (size / sizeof(list_p->chans_pp[0]));
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

int chan_set_control_cb(struct chan_t *self_p,
                        chan_control_fn_t control_cb)
{
    ASSERTN(self_p != NULL, EINVAL);

    if (control_cb == NULL) {
        control_cb = chan_control_null;
    }

    self_p->control = control_cb;

    return (0);
}

ssize_t chan_read(void *self_p,
                  void *buf_p,
                  size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(((struct chan_t *)self_p)->read != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (((struct chan_t *)self_p)->read(self_p, buf_p, size));
}

ssize_t chan_write(void *v_self_p,
                   const void *buf_p,
                   size_t size)
{
    ASSERTN(v_self_p != NULL, EINVAL);
    ASSERTN(((struct chan_t *)v_self_p)->write != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    struct chan_t *self_p;

    self_p = v_self_p;

    if (self_p->write_filter_cb != NULL) {
        if (self_p->write_filter_cb(self_p, buf_p, size) == 1) {
            return (size);
        }
    }

    return (self_p->write(self_p, buf_p, size));
}

int chan_control(void *self_p,
                 int operation)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (((struct chan_t *)self_p)->control(self_p, operation));
}

RAM_CODE ssize_t chan_write_isr(void *self_in_p,
                                const void *buf_p,
                                size_t size)
{
    struct chan_t *self_p;

    self_p = self_in_p;

    if (self_p->write_filter_isr_cb != NULL) {
        if (self_p->write_filter_isr_cb(self_p, buf_p, size) == 1) {
            return (size);
        }
    }

    return (self_p->write_isr(self_p, buf_p, size));
}

RAM_CODE size_t chan_size(void *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (((struct chan_t *)self_p)->size(self_p));
}

int chan_list_add(struct chan_list_t *list_p, void *chan_p)
{
    ASSERTN(list_p != NULL, EINVAL);
    ASSERTN(chan_p != NULL, EINVAL);

    int res = 0;

    sys_lock();

    if (list_p->len == list_p->max) {
        res = -ENOMEM;
    } else {
        list_p->chans_pp[list_p->len] = chan_p;
        list_p->len++;
        ((struct chan_t *)chan_p)->list_p = list_p;
    }

    sys_unlock();

    return (res);
}

int chan_list_remove(struct chan_list_t *list_p, void *chan_p)
{
    ASSERTN(list_p != NULL, EINVAL);
    ASSERTN(chan_p != NULL, EINVAL);

    int i;
    int res = -1;

    sys_lock();

    for (i = 0; i < list_p->len; i++) {
        if (list_p->chans_pp[i] == chan_p) {
            list_p->len--;
            list_p->chans_pp[i] = list_p->chans_pp[list_p->len];
            ((struct chan_t *)chan_p)->list_p = NULL;
            res = 0;
            break;
        }
    }

    sys_unlock();

    return (res);
}

void *chan_list_poll(struct chan_list_t *list_p,
                     const struct time_t *timeout_p)
{
    ASSERTNRN(list_p != NULL, EINVAL);

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
        }

        /* No data was available, wait for data to be written to one
           of the channels. */
        if (thrd_suspend_isr(timeout_p) == -ETIMEDOUT) {
            for (i = 0; i < list_p->len; i++) {
                chan_p = list_p->chans_pp[i];
                chan_p->reader_p = NULL;
            }

            list_p->flags = 0;
            chan_p = NULL;

            goto out;
        }
    }

 out:
    sys_unlock();

    return (chan_p);
}

void *chan_poll(void *chan_p, const struct time_t *timeout_p)
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
    return ((void *)&null);
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
    /* Set to 1 because read will return immediately. */
    return (1);
}

int chan_control_null(void *self_p, int operation)
{
    return (0);
}

RAM_CODE int chan_is_polled_isr(struct chan_t *self_p)
{
    int i;
    int res;
    struct chan_t *chan_p;

    res = 0;

    if (self_p->list_p != NULL) {
        if (self_p->list_p->flags & CHAN_LIST_POLLING) {
            res = 1;
            self_p->list_p->flags = 0;

            for (i = 0; i < self_p->list_p->len; i++) {
                chan_p = self_p->list_p->chans_pp[i];

                if (self_p != chan_p) {
                    chan_p->reader_p = NULL;
                }
            }
        }
    }

    return (res);
}
