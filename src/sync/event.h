/**
 * @file sync/event.h
 * @version 2.0.0
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

#ifndef __SYNC_EVENT_H__
#define __SYNC_EVENT_H__

#include "simba.h"

/**
 * Event channel.
 */
struct event_t {
    struct chan_t base;
    uint32_t mask;          /* Events that occured. */
};

/**
 * Initialize given event channel.
 *
 * @param[in] self_p Event channel to initialize.
 *
 * @return zero(0) or negative error code
 */
int event_init(struct event_t *self_p);

/**
 * Wait for an event to occur in given event mask. This function
 * blocks until at least one of the events in the event mask has been
 * set. When the function returns, given event mask has been
 * overwritten with the events that actually occured.
 *
 * @param[in] self_p Event channel object.
 * @param[in, out] buf_p The mask of events to wait for. When the
 *                       function returns the mask contains the events
 *                       that have occured.
 * @param[in] size Size to read (always sizeof(mask)).
 *
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_read(struct event_t *self_p,
                   void *buf_p,
                   size_t size);

/**
 * Write given event(s) to given event channel.
 *
 * @param[in] self_p Event channel object.
 * @param[in] buf_p The mask of events to write.
 * @param[in] size Must always be sizeof(mask).
 *
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_write(struct event_t *self_p,
                    const void *buf_p,
                    size_t size);

/**
 * Write given events to the event channel from isr or with the system
 * lock taken (see `sys_lock()`).
 *
 * @param[in] self_p Event channel object.
 * @param[in] buf_p The mask of events to write.
 * @param[in] size Must always be sizeof(mask).
 *
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_write_isr(struct event_t *self_p,
                        const void *buf_p,
                        size_t size);

/**
 * Checks if there are events active on the event channel.
 *
 * @param[in] self_p Event channel object.
 *
 * @return one(1) is at least one event is active, otherwise zero(0).
 */
ssize_t event_size(struct event_t *self_p);

#endif
