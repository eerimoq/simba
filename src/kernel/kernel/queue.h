/**
 * @file kernel/queue.h
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
 * MERQUEUETABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __KERNEL_QUEUE_H__
#define __KERNEL_QUEUE_H__

#include "simba.h"

/* Compile time declaration and initialization of a channel. */
#define QUEUE_INIT_DECL(_name, _buf, _size)                             \
    struct queue_t _name = {                                            \
        .base = {                                                       \
            .read = (ssize_t (*)(void *, void *, size_t))queue_read,    \
            .write = (ssize_t (*)(void *, const void *, size_t))queue_write, \
            .size = (size_t (*)(void *))queue_size,                     \
            .readers = LIST_SL_INIT_STRUCT                              \
        },                                                              \
        .buf_p = _buf,                                                  \
        .size = _size,                                                  \
        .wrpos = 0,                                                     \
        .rdpos = 0,                                                     \
        .writers = LIST_SL_INIT_STRUCT,                                 \
    }

/* Queue. */
struct queue_t {
    struct chan_t base;
    void *buf_p;
    size_t size;
    volatile size_t wrpos;
    volatile size_t rdpos;
    struct list_singly_linked_t writers;
};

/**
 * Initialize given queue.
 * @param[in] queue Queue to initialize.
 * @param[in] buf Buffer.
 * @param[in] size Size of buffer.
 * @return zero(0) or negative error code
 */
int queue_init(struct queue_t *queue, void *buf_p, size_t size);

/**
 * Read from given queue. Blocks until size bytes
 * has been read.
 * @param[in] queue Queue to read from.
 * @param[in] buf Buffer to read to.
 * @param[in] size Size to read.
 * @return Number of read bytes or negative error code.
 */
ssize_t queue_read(struct queue_t *queue, void *buf_p, size_t size);

/**
 * Write bytes to given queue. Blocks until size bytes
 * has been written.
 * @param[in] queue Queue to write to.
 * @param[in] buf Buffer to write from.
 * @param[in] size Number of bytes to write.
 * @return Number of written bytes or negative error code.
 */
ssize_t queue_write(struct queue_t *queue, const void *buf_p, size_t size);

/**
 * Write bytes to given queue from interrupt context. May write
 * less than size bytes.
 * @param[in] queue Queue to write to.
 * @param[in] buf Buffer to write from.
 * @param[in] size Number of bytes to write.
 * @return Number of written bytes or negative error code.
 */
ssize_t queue_write_irq(struct queue_t *queue,
                        const void *buf_p,
                        size_t size);

/**
 * Get the number of bytes currently stored in the queue. May return
 * less bytes than number of bytes stored in the channel.
 * @param[in] queue Queue.
 * @return Number of bytes in queue.
 */
ssize_t queue_size(struct queue_t *queue);

#endif
