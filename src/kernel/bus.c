/**
 * @file bus.c
 * @version 0.2.0
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

int bus_module_init()
{
    return (0);
}

int bus_init(struct bus_t *self_p)
{
    self_p->head_p = NULL;
    rwlock_init(&self_p->rwlock);

    return (0);
}

int bus_attach(struct bus_t *self_p,
               struct bus_listener_t *listener_p)
{
    rwlock_writer_get(&self_p->rwlock, NULL);

    listener_p->next_p = self_p->head_p;
    self_p->head_p = listener_p;

    rwlock_writer_put(&self_p->rwlock);

    return (0);
}

int bus_detatch(struct bus_t *self_p,
                struct bus_listener_t *listener_p)
{
    int res = -1;
    struct bus_listener_t *curr_p, *prev_p;

    rwlock_writer_get(&self_p->rwlock, NULL);

    curr_p = self_p->head_p;
    prev_p = NULL;

    while (curr_p != NULL) {
        if (curr_p == listener_p) {
            if (prev_p != NULL) {
                prev_p->next_p = curr_p->next_p;     
            } else {
                self_p->head_p = NULL;                
            }

            res = 0;
            break;
        }

        prev_p = curr_p;
        curr_p = curr_p->next_p;
    }

    rwlock_writer_put(&self_p->rwlock);

    return (res);
}

int bus_write(struct bus_t *self_p,
              struct bus_message_header_t *message_p)
{
    int number_of_receivers;
    struct bus_listener_t *curr_p;

    rwlock_reader_get(&self_p->rwlock, NULL);

    curr_p = self_p->head_p;
    number_of_receivers = 0;

    while (curr_p != NULL) {
        if (curr_p->id == message_p->id) {
            ((struct chan_t *)curr_p->chan_p)->write(curr_p->chan_p,
                                                     message_p,
                                                     message_p->size);
            number_of_receivers++;
        }

        curr_p = curr_p->next_p;
    }

    rwlock_reader_put(&self_p->rwlock);

    return (number_of_receivers);
}

int bus_listener_init(struct bus_listener_t *self_p,
                      int id,
                      chan_t *chan_p)
{
    self_p->id = id;
    self_p->chan_p = chan_p;
    self_p->next_p = NULL;

    return (0);
}
