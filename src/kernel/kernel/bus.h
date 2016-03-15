/**
 * @file kernel/bus.h
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

#ifndef __KERNEL_BUS_H__
#define __KERNEL_BUS_H__

#include "simba.h"

struct bus_t {
    struct sem_t sem;
    struct bus_chan_t *head_p;
};

struct bus_chan_t {
    int id;
    chan_t *chan_p;
    struct bus_chan_t *next_p;
};

struct bus_message_header_t {
    int id;      /** Unique message identifier. */
    size_t size; /** Size of the message, not including the header size. */
};

/**
 * Initialize the bus module.
 *
 * @return zero(0) or negative error code
 */
int bus_module_init(void);

/**
 * Initialize a bus.
 *
 * @param[in] self_p Bus to initialize.
 *
 * @return zero(0) or negative error code.
 */
int bus_init(struct bus_t *self_p);

/**
 * Attach given channel to given bus.
 *
 * @param[in] self_p Bus to attach to.
 * @param[in] chan_p Channel to attach to given bus.
 *
 * @return zero(0) or negative error code.
 */
int bus_attach(struct bus_t *self_p,
               struct bus_chan_t *chan_p);

/**
 * Detatch given channel from given bus.
 *
 * @param[in] self_p Bus to detach from.
 * @param[in] chan_p Channel to detach from given bus.
 *
 * @return zero(0) or negative error code.
 */
int bus_detatch(struct bus_t *self_p,
                struct bus_chan_t *chan_p);

/**
 * Write given message to given bus.
 *
 * @param[in] self_p Bus to write to.
 * @param[in] message_p Message to write to given bus.
 *
 * @return Number of channels that received the message or negative
 *         error code.
 */
int bus_write(struct bus_t *self_p,
              struct bus_message_header_t *message_p);

/**
 * Initialize given channel.
 *
 * @param[in] self_p Channel to initialize.
 * @param[in] chan_p Channel to write messages to.
 * @param[in] id Message id to receive.
 *
 * @return zero(0) or negative error code.
 */
int bus_chan_init(struct bus_chan_t *self_p,
                  chan_t *chan_p,
                  int id);

#endif
