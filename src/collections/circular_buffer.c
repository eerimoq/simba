/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

int circular_buffer_init(struct circular_buffer_t *self_p,
                         void *buf_p,
                         size_t size)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(buf_p != NULL, -EINVAL);
    ASSERTN(size > 0, -EINVAL);

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
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(buf_p != NULL, -EINVAL);

    size_t first_chunk_size;
    size_t unused_size;
    const char *b_p;

    unused_size = circular_buffer_unused_size(self_p);

    if (unused_size < size) {
        size = unused_size;
    }

    b_p = buf_p;
    first_chunk_size = (self_p->size - self_p->writepos);

    if (first_chunk_size < size) {
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
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(buf_p != NULL, -EINVAL);

    size_t first_chunk_size;
    size_t used_size;
    char *b_p;

    used_size = circular_buffer_used_size(self_p);

    if (size > used_size) {
        size = used_size;
    }

    b_p = buf_p;
    first_chunk_size = (self_p->size - self_p->readpos);

    if (first_chunk_size < size) {
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
    return (self_p->size - circular_buffer_unused_size(self_p) - 1);
}

ssize_t circular_buffer_unused_size(struct circular_buffer_t *self_p)
{
    ssize_t available;

    available = (self_p->readpos - self_p->writepos - 1);

    if (available < 0) {
        available += self_p->size;
    }

    return (available);
}
