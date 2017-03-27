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

int bus_module_init()
{
    return (0);
}

int bus_init(struct bus_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    binary_tree_init(&self_p->listeners);
    rwlock_init(&self_p->rwlock);

    return (0);
}

int bus_listener_init(struct bus_listener_t *self_p,
                      int id,
                      void *chan_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(chan_p != NULL, EINVAL);

    self_p->base.key = id;
    self_p->id = id;
    self_p->chan_p = chan_p;
    self_p->next_p = NULL;

    return (0);
}

int bus_attach(struct bus_t *self_p,
               struct bus_listener_t *listener_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(listener_p != NULL, EINVAL);

    struct bus_listener_t *head_p;

    rwlock_writer_take(&self_p->rwlock);

    /* Try to insert the node into the tree. It fails if there already
     * is a node with the same key (id).*/
    if (binary_tree_insert(&self_p->listeners, &listener_p->base) != 0) {
        head_p = (struct bus_listener_t *)binary_tree_search(
            &self_p->listeners, listener_p->id);

        listener_p->next_p = head_p->next_p;
        head_p->next_p = listener_p;
    }

    rwlock_writer_give(&self_p->rwlock);

    return (0);
}

int bus_detatch(struct bus_t *self_p,
                struct bus_listener_t *listener_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(listener_p != NULL, EINVAL);

    int res = 0;
    struct bus_listener_t *head_p, *curr_p, *prev_p;

    rwlock_writer_take(&self_p->rwlock);

    head_p = (struct bus_listener_t *)binary_tree_search(
        &self_p->listeners, listener_p->id);

    if (head_p == NULL) {
        res = -1;
    } else if (head_p == listener_p) {
        res = binary_tree_delete(&self_p->listeners, listener_p->id);

        if (listener_p->next_p != NULL) {
            (void)binary_tree_insert(&self_p->listeners,
                                     &listener_p->next_p->base);
        }
    } else {
        curr_p = head_p->next_p;
        prev_p = head_p;
        res = -1;

        while (curr_p != NULL) {
            if (curr_p == listener_p) {
                prev_p->next_p = listener_p->next_p;
                res = 0;
                break;
            }

            prev_p = curr_p;
            curr_p = curr_p->next_p;
        }
    }

    rwlock_writer_give(&self_p->rwlock);

    return (res);
}

int bus_write(struct bus_t *self_p,
              int id,
              const void *buf_p,
              size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    int number_of_receivers;
    struct bus_listener_t *curr_p;

    rwlock_reader_take(&self_p->rwlock);

    curr_p = (struct bus_listener_t *)binary_tree_search(
        &self_p->listeners, id);
    number_of_receivers = 0;

    while (curr_p != NULL) {
        ((struct chan_t *)curr_p->chan_p)->write(curr_p->chan_p,
                                                 buf_p,
                                                 size);
        number_of_receivers++;
        curr_p = curr_p->next_p;
    }

    rwlock_reader_give(&self_p->rwlock);

    return (number_of_receivers);
}
