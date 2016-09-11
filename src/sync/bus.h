/**
 * @file sync/bus.h
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

#ifndef __SYNC_BUS_H__
#define __SYNC_BUS_H__

#include "simba.h"

struct bus_t {
    struct rwlock_t rwlock;
    struct binary_tree_t listeners;
};

struct bus_listener_t {
    struct binary_tree_node_t base;
    int id;
    void *chan_p;
    struct bus_listener_t *next_p;
};

/**
 * Initialize the bus module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code
 */
int bus_module_init(void);

/**
 * Initialize given bus.
 *
 * @param[in] self_p Bus to initialize.
 *
 * @return zero(0) or negative error code.
 */
int bus_init(struct bus_t *self_p);

/**
 * Initialize given listener to receive messages with given id, after
 * the listener is attached to the bus. A listener can only receive
 * messages of a single id, though, the same channel may be used in
 * multiple listeners with different ids (if the channel supports it).
 *
 * @param[in] self_p Listener to initialize.
 * @param[in] id Message id to receive.
 * @param[in] chan_p Channel to receive messages on.
 *
 * @return zero(0) or negative error code.
 */
int bus_listener_init(struct bus_listener_t *self_p,
                      int id,
                      void *chan_p);

/**
 * Attach given listener to given bus. Messages written to the bus
 * will be written to all listeners initialized with the written
 * message id.
 *
 * @param[in] self_p Bus to attach the listener to.
 * @param[in] listener_p Listener to attach to the bus.
 *
 * @return zero(0) or negative error code.
 */
int bus_attach(struct bus_t *self_p,
               struct bus_listener_t *listener_p);

/**
 * Detatch given listener from given bus. A detached listener will not
 * receive any messages from the bus.
 *
 * @param[in] self_p Bus to detach listener from.
 * @param[in] listener_p Listener to detach from the bus.
 *
 * @return zero(0) or negative error code.
 */
int bus_detatch(struct bus_t *self_p,
                struct bus_listener_t *listener_p);

/**
 * Write given message to given bus. All attached listeners to given
 * bus will receive the message.
 *
 * @param[in] self_p Bus to write the message to.
 * @param[in] id Message identity.
 * @param[in] buf_p Buffer to write to the bus. All listeners with
 *                  given message id will receive this data.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of listeners that received the message, or negative
 *         error code.
 */
int bus_write(struct bus_t *self_p,
              int id,
              const void *buf_p,
              size_t size);

#endif
