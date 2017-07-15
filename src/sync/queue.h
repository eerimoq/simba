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

#ifndef __SYNC_QUEUE_H__
#define __SYNC_QUEUE_H__

#include "simba.h"

#define QUEUE_FLAGS_NON_BLOCKING_READ                     0x1

/* Compile time declaration and initialization of a channel. */
#define QUEUE_INIT_DECL(_name, _buf, _size)             \
    struct queue_t _name = {                            \
        .base = {                                       \
            .read = (chan_read_fn_t)queue_read,         \
            .write = (chan_write_fn_t)queue_write,      \
            .size = (chan_size_fn_t)queue_size,         \
            .control = chan_control_null,               \
            .reader_p = NULL,                           \
            .list_p = NULL                              \
        },                                              \
        .writers = {                                    \
            .head_p = NULL,                             \
        },                                              \
        .writer_p = NULL,                               \
        .buf_p = _buf,                                  \
        .buffer = {                                     \
            .buf_p = _buf,                              \
            .size = _size,                              \
            .writepos = 0,                              \
            .readpos = 0                                \
        },                                              \
        .state = QUEUE_STATE_INITIALIZED,               \
        .reader = {                                     \
            .buf_p = NULL,                              \
            .size = 0,                                  \
            .left = 0                                   \
        }                                               \
    }

enum queue_state_t {
    /** Queue initialized state. */
    QUEUE_STATE_INITIALIZED = 0,

    /** Queue running state. */
    QUEUE_STATE_RUNNING,

    /** Queue stopped state. */
    QUEUE_STATE_STOPPED,
};

/* Queue. */
struct queue_t {
    struct chan_t base;
    struct thrd_prio_list_t writers;
    struct queue_writer_elem_t *writer_p;
    struct {
        char *buf_p;
        size_t size;
        size_t left;
    } reader;
    void *buf_p;
    struct circular_buffer_t buffer;
    enum queue_state_t state;
    int flags;
};

/**
 * Initialize given queue with given optional buffer.
 *
 * @param[in] self_p Queue to initialize.
 * @param[in] buf_p Buffer for data storage. Give as NULL to disable
 *                  buffering and only allow the writer to write
 *                  directly into the reader's buffer, blocking the
 *                  writer until all data has been written.
 * @param[in] size Size of given buffer.
 *
 * @return zero(0) or negative error code
 */
int queue_init(struct queue_t *self_p,
               void *buf_p,
               size_t size);

/**
 * Start given queue. It is not required to start a queue unless it
 * has been stopped.
 *
 * @param[in] self_p Queue to start.
 *
 * @return zero(0) or negative error code.
 */
int queue_start(struct queue_t *self_p);

/**
 * Stop given queue. Any ongoing read and write operations will return
 * with the currently read/written number of bytes. Any read and write
 * operations on a stopped queue will return zero(0).
 *
 * @param[in] self_p Queue to stop.
 *
 * @return true(1) if a thread was resumed, false(0) if no thread was
 *         resumed, or negative error code.
 */
int queue_stop(struct queue_t *self_p);

/**
 * Same as `queue_stop()` but from isr or with the system lock taken
 * (see `sys_lock()`).
 *
 */
int queue_stop_isr(struct queue_t *self_p);

/**
 * Read from given queue. Blocks until size bytes has been read.
 *
 * @param[in] self_p Queue to read from.
 * @param[in] buf_p Buffer to read to.
 * @param[in] size Size to read.
 *
 * @return Number of bytes read or negative error code.
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
 * @return Number of bytes written or negative error code.
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
 * @return Number of bytes written or negative error code.
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
 * @return Number of bytes in given queue.
 */
ssize_t queue_size(struct queue_t *self_p);

/**
 * Get the number of unused bytes in the queue.
 *
 * @param[in] self_p Queue.
 *
 * @return Number of unused bytes in given queue.
 */
ssize_t queue_unused_size(struct queue_t *self_p);

/**
 * Get the number of unused bytes in the queue from isr or with the
 * system lock taken (see `sys_lock()`).
 *
 * @param[in] self_p Queue.
 *
 * @return Number of unused bytes in given queue.
 */
ssize_t queue_unused_size_isr(struct queue_t *self_p);

/**
 * Ignore given number of bytes at the beginning of the queue by
 * discarding them.
 *
 * @param[in] self_p Queue.
 *
 * @return Number of bytes ignored or negative error code.
 */
ssize_t queue_ignore(struct queue_t *self_p,
                     size_t size);

#endif
