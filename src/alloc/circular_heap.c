/**
 * @file circular_heap.c
 * @version 3.1.0
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
 * MERHEAPTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
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
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

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
