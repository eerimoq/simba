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

struct header_t {
    size_t size;
};

int circular_heap_init(struct circular_heap_t *self_p,
                       void *buf_p,
                       size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    self_p->begin_p = buf_p;
    self_p->end_p = (buf_p + size);
    self_p->alloc_p = buf_p;
    self_p->free_p = buf_p;

    return (0);
}

void *circular_heap_alloc(struct circular_heap_t *self_p,
                          size_t size)
{
    ASSERTNRN(self_p != NULL, EINVAL);
    ASSERTNRN(size > 0, EINVAL);

    struct header_t *header_p = NULL;

    size += sizeof(*header_p);

    /* Align the buffer to a 4 byte boundary. */
    size += 3;
    size &= 0xffffffc;

    /* Does it fit before end_p or free_p? */
    if (self_p->alloc_p >= self_p->free_p)  {
        /* begin_p <-> free_p <-> alloc_p <-> end_p */
        if ((self_p->end_p - self_p->alloc_p) > size)  {
            header_p = self_p->alloc_p;
            self_p->alloc_p += size;
        } else if ((self_p->free_p - self_p->begin_p) > size) {
            header_p = self_p->begin_p;
            self_p->alloc_p = (self_p->begin_p + size);
        }
    } else {
        /* begin_p <-> alloc_p <-> free_p <-> end_p */
        if ((self_p->free_p - self_p->alloc_p) > size)  {
            header_p = self_p->alloc_p;
            self_p->alloc_p += size;
        }
    }

    if (header_p != NULL) {
        header_p->size = size;

        return (&header_p[1]);
    } else {
        return (NULL);
    }
}

int circular_heap_free(struct circular_heap_t *self_p,
                       void *buf_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(buf_p > self_p->begin_p, EINVAL);
    ASSERTN(buf_p < self_p->end_p, EINVAL);

    struct header_t *header_p;

    header_p = buf_p;
    header_p--;

    ASSERTN((header_p == self_p->free_p)
            || (header_p == self_p->begin_p), EINVAL);

    if (header_p == self_p->begin_p) {
        self_p->free_p = self_p->begin_p;
    }

    self_p->free_p += header_p->size;

    return (0);
}
