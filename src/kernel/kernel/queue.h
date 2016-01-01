/**
 * @file kernel/queue.h
 * @version 0.1.0
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
            .writer_p = NULL,                                           \
            .reader_p = NULL,                                           \
            .list_p = NULL                                              \
        },                                                              \
        .buffer = {                                                     \
            .begin_p = _buf,                                            \
            .read_p = _buf,                                             \
            .write_p = _buf,                                            \
            .end_p = &_buf[_size],                                      \
        },                                                              \
        .buf_p = NULL,                                                  \
        .size = 0,                                                      \
        .left = 0                                                       \
    }

struct queue_buffer_t{
    char *begin_p;
    char *read_p;
    char *write_p;
    char *end_p;
};

/* Queue. */
struct queue_t {
    struct chan_t base;
    struct queue_buffer_t buffer;
    char *buf_p;
    size_t size;
    size_t left;
};

/**
 * Initialize given queue.
 *
 * @param[in] self_p Queue to initialize.
 * @param[in] buf_p Buffer.
 * @param[in] size Size of buffer.
 *
 * @return zero(0) or negative error code
 */
int queue_init(struct queue_t *self_p,
               void *buf_p,
               size_t size);

/**
 * Read from given queue. Blocks until size bytes has been read.
 *
 * @param[in] self_p Queue to read from.
 * @param[in] buf_p Buffer to read to.
 * @param[in] size Size to read.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t queue_read(struct queue_t *self_p,
                   void *buf_p,
                   size_t size);

/**
 * Write bytes to given queue. Blocks until size bytes has been
 * written.
 *
 * @param[in] self_p Queue to write to.
 * @param[in] buf_p Buffer to write from.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t queue_write(struct queue_t *self_p,
                    const void *buf_p,
                    size_t size);

/**
 * Write bytes to given queue from isr or with the system lock
 * taken (see `sys_lock()`). May write less than size bytes.
 *
 * @param[in] self_p Queue to write to.
 * @param[in] buf_p Buffer to write from.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t queue_write_isr(struct queue_t *self_p,
                        const void *buf_p,
                        size_t size);

/**
 * Get the number of bytes currently stored in the queue. May return
 * less bytes than number of bytes stored in the channel.
 *
 * @param[in] self_p Queue.
 *
 * @return Number of bytes in queue.
 */
ssize_t queue_size(struct queue_t *self_p);

#endif
