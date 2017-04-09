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

int circular_buffer_init(struct circular_buffer_t *self_p,
                         void *buf_p,
                         size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    self_p->buf_p = buf_p;
    self_p->size = size;
    self_p->writepos = 0;
    self_p->readpos = 0;

    return (0);
}

ssize_t circular_buffer_write(struct circular_buffer_t *self_p,
                              const void *buf_p,
                              size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    size_t first_chunk_size;
    size_t unused_size;
    const char *b_p;

    unused_size = circular_buffer_unused_size(self_p);

    if (unused_size < size) {
        size = unused_size;
    }

    b_p = buf_p;
    first_chunk_size = (self_p->size - self_p->writepos);

    if (first_chunk_size <= size) {
        memcpy(&self_p->buf_p[self_p->writepos], &b_p[0], first_chunk_size);
        memcpy(&self_p->buf_p[0], &b_p[first_chunk_size], size - first_chunk_size);
        self_p->writepos = (size - first_chunk_size);
    } else {
        memcpy(&self_p->buf_p[self_p->writepos], &b_p[0], size);
        self_p->writepos += size;
    }

    return (size);
}

ssize_t circular_buffer_read(struct circular_buffer_t *self_p,
                             void *buf_p,
                             size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    size_t first_chunk_size;
    size_t used_size;
    char *b_p;

    used_size = circular_buffer_used_size(self_p);

    if (size > used_size) {
        size = used_size;
    }

    b_p = buf_p;
    first_chunk_size = (self_p->size - self_p->readpos);

    if (first_chunk_size <= size) {
        memcpy(&b_p[0], &self_p->buf_p[self_p->readpos], first_chunk_size);
        memcpy(&b_p[first_chunk_size], &self_p->buf_p[0], size - first_chunk_size);
        self_p->readpos = (size - first_chunk_size);
    } else {
        memcpy(&b_p[0], &self_p->buf_p[self_p->readpos], size);
        self_p->readpos += size;
    }

    return (size);
}

ssize_t circular_buffer_used_size(struct circular_buffer_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->size - circular_buffer_unused_size(self_p) - 1);
}

ssize_t circular_buffer_unused_size(struct circular_buffer_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    ssize_t available;

    available = (self_p->readpos - self_p->writepos - 1);

    if (available < 0) {
        available += self_p->size;
    }

    return (available);
}

ssize_t circular_buffer_skip_front(struct circular_buffer_t *self_p,
                                   size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);

    size_t first_chunk_size;
    size_t used_size;

    used_size = circular_buffer_used_size(self_p);

    if (size > used_size) {
        size = used_size;
    }

    first_chunk_size = (self_p->size - self_p->readpos);

    if (first_chunk_size <= size) {
        self_p->readpos = (size - first_chunk_size);
    } else {
        self_p->readpos += size;
    }

    return (size);
}

ssize_t circular_buffer_reverse_skip_back(struct circular_buffer_t *self_p,
                                          size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);

    size_t used_size;

    used_size = circular_buffer_used_size(self_p);

    if (size > used_size) {
        size = used_size;
    }

    if (size > self_p->writepos) {
        self_p->writepos = (self_p->size - (size - self_p->writepos));
    } else {
        self_p->writepos -= size;
    }

    return (size);
}

ssize_t circular_buffer_array_one(struct circular_buffer_t *self_p,
                                  void **buf_pp,
                                  size_t size)
{
    size_t first_chunk_size;

    if (self_p->writepos >= self_p->readpos) {
        first_chunk_size = (self_p->writepos - self_p->readpos);
    } else {
        first_chunk_size = (self_p->size - self_p->readpos);
    }

    if (size > first_chunk_size) {
        size = first_chunk_size;
    }

    if (size > 0) {
        *buf_pp = &self_p->buf_p[self_p->readpos];
    }

    return (size);
}

ssize_t circular_buffer_array_two(struct circular_buffer_t *self_p,
                                  void **buf_pp,
                                  size_t size)
{
    /* Return immediately if there is no second chunk. */
    if (self_p->writepos >= self_p->readpos) {
        return (0);
    }

    if (size > self_p->writepos) {
        size = self_p->writepos;
    }

    if (size > 0) {
        *buf_pp = &self_p->buf_p[0];
    }

    return (size);
}
