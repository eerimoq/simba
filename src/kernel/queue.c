/**
 * @file queue.c
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

#include "simba.h"

#define BUFFER_SIZE(buffer_p) \
    ((buffer_p)->end_p - (buffer_p)->begin_p - 1)
#define BUFFER_USED_UNTIL_END(buffer_p) \
    ((buffer_p)->end_p - (buffer_p)->read_p)
#define WRITER_SIZE(queue_p) \
    ((queue_p->base.writer_p != NULL) * queue_p->left)
#define BUFFER_UNUSED_UNTIL_END(buffer_p) \
    ((buffer_p)->end_p - (buffer_p)->write_p)
#define BUFFER_UNUSED(buffer_p) \
    (BUFFER_SIZE(buffer_p) - get_buffer_used(buffer_p))

static inline size_t get_buffer_used(struct queue_buffer_t *buffer_p)
{
    if (buffer_p->begin_p == NULL) {
        return (0);
    }

    if (buffer_p->write_p == buffer_p->read_p) {
        return (0);
    } else if (buffer_p->write_p > buffer_p->read_p) {
        return (buffer_p->write_p - buffer_p->read_p);
    } else {
        return ((buffer_p->end_p - buffer_p->begin_p)
                - (buffer_p->read_p - buffer_p->write_p));
    }
}

int queue_init(struct queue_t *queue_p,
               void *buf_p,
               size_t size)
{
    chan_init(&queue_p->base,
              (ssize_t (*)(void *, void *, size_t))queue_read,
              (ssize_t (*)(void *, const void *, size_t))queue_write,
              (size_t (*)(void *))queue_size);

    queue_p->buffer.begin_p = buf_p;
    queue_p->buffer.read_p = buf_p;
    queue_p->buffer.write_p = buf_p;
    queue_p->buffer.end_p = &((char*)buf_p)[size];

    queue_p->buf_p = NULL;
    queue_p->size = 0;
    queue_p->left = 0;

    return (0);
}

ssize_t queue_read(struct queue_t *queue_p, void *buf_p, size_t size)
{
    size_t left, n, buffer_used_until_end, buffer_used;

    left = size;

    sys_lock();

    /* Copy data from queue buffer. */
    if (queue_p->buffer.begin_p != NULL) {
        buffer_used = get_buffer_used(&queue_p->buffer);

        if (left < buffer_used) {
            n = left;
        } else {
            n = buffer_used;
        }

        buffer_used_until_end = BUFFER_USED_UNTIL_END(&queue_p->buffer);

        if (n <= buffer_used_until_end) {
            memcpy(buf_p, queue_p->buffer.read_p, n);
            queue_p->buffer.read_p += n;
        } else {
            memcpy(buf_p, queue_p->buffer.read_p, buffer_used_until_end);
            memcpy(buf_p + buffer_used_until_end,
                   queue_p->buffer.begin_p,
                   (n - buffer_used_until_end));
            queue_p->buffer.read_p = queue_p->buffer.begin_p;
            queue_p->buffer.read_p += (n - buffer_used_until_end);
        }

        buf_p += n;
        left -= n;
    }

    /* Copy data from the writer, if one is present. */
    if (queue_p->base.writer_p != NULL) {
        if (left < queue_p->left) {
            n = left;
        } else {
            n = queue_p->left;
        }

        memcpy(buf_p, queue_p->buf_p, n);
        queue_p->buf_p += n;
        queue_p->left -= n;
        buf_p += n;
        left -= n;

        /* Writer buffer empty. */
        if (queue_p->left == 0) {
            /* Wake the writer. */
            thrd_resume_irq(queue_p->base.writer_p, 0);
            queue_p->base.writer_p = NULL;
        }
    }

    /* Suspend this thread if more data should be read. */
    if (left > 0) {
        /* The writer writes the remaining data to the reader buffer. */
        queue_p->base.reader_p = thrd_self();
        queue_p->buf_p = buf_p;
        queue_p->left = left;

        thrd_suspend_irq(NULL);
    }

    sys_unlock();

    return (size);
}

ssize_t queue_write(struct queue_t *queue_p,
                    const void *buf_p,
                    size_t size)
{
    size_t left;

    left = size;

    sys_lock();

    left -= queue_write_irq(queue_p, buf_p, size);

    /* The writer writes the remaining data. */
    if (left > 0) {
        buf_p += (size - left);
        queue_p->base.writer_p = thrd_self();
        queue_p->buf_p = (void *)buf_p;
        queue_p->left = left;

        thrd_suspend_irq(NULL);
    }

    sys_unlock();

    return (size);
}

ssize_t queue_write_irq(struct queue_t *queue_p,
                        const void *buf_p,
                        size_t size)
{
    size_t n, left;
    size_t buffer_unused_until_end, buffer_unused;

    left = size;

    if (chan_is_polled_irq(&queue_p->base)) {
        thrd_resume_irq(queue_p->base.reader_p, 0);
        queue_p->base.reader_p = NULL;
    }

    /* Copy data to the reader, if one is present. */
    if (queue_p->base.reader_p != NULL) {
        if (left < queue_p->left) {
            n = left;
        } else {
            n = queue_p->left;
        }
        
        memcpy(queue_p->buf_p, buf_p, n);

        queue_p->buf_p += n;
        queue_p->left -= n;
        buf_p += n;
        left -= n;

        /* Read buffer full. */
        if (queue_p->left == 0) {
            /* Wake the reader. */
            thrd_resume_irq(queue_p->base.reader_p, 0);
            queue_p->base.reader_p = NULL;
        }
    }

    if ((left > 0) && (queue_p->buffer.begin_p != NULL)) {
        buffer_unused = BUFFER_UNUSED(&queue_p->buffer);

        if (left < buffer_unused) {
            n = left;
        } else {
            n = buffer_unused;
        }

        buffer_unused_until_end = BUFFER_UNUSED_UNTIL_END(&queue_p->buffer);

        if (n <= buffer_unused_until_end) {
            memcpy(queue_p->buffer.write_p, buf_p, n);
            queue_p->buffer.write_p += n;
        } else {
            memcpy(queue_p->buffer.write_p, buf_p, buffer_unused_until_end);
            memcpy(queue_p->buffer.begin_p,
                   buf_p + buffer_unused_until_end,
                   (n - buffer_unused_until_end));
            queue_p->buffer.write_p = queue_p->buffer.begin_p;
            queue_p->buffer.write_p += (n - buffer_unused_until_end);
        }

        buf_p += n;
        left -= n;
    }

    return (size - left);
}

ssize_t queue_size(struct queue_t *queue_p)
{
    return (get_buffer_used(&queue_p->buffer) + WRITER_SIZE(queue_p));
}
