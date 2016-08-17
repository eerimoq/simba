/**
 * @file heap.c
 * @version 7.0.0
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

struct heap_buffer_header_t {
    union {
        struct heap_fixed_t *fixed_p;
        struct heap_buffer_header_t *next_p;
    } u;
    size_t size;
    int count;
};

static void *alloc_fixed_size(struct heap_t *self_p,
                              size_t size)
{
    struct heap_buffer_header_t *header_p;
    struct heap_fixed_t *fixed_p = self_p->fixed;
    size_t left;
    char *next_p;

    while (fixed_p != &self_p->fixed[HEAP_FIXED_SIZES_MAX]) {
        if (size <= fixed_p->size) {
            if (fixed_p->free_p != NULL) {
                header_p = fixed_p->free_p;
                fixed_p->free_p = *((void **)fixed_p->free_p);
            } else {
                next_p = self_p->next_p;

                /* Out of memory?. */
                left = (self_p->size - (next_p - (char *)self_p->buf_p));

                if (left < (sizeof(*header_p) + size)) {
                    break;
                }

                header_p = self_p->next_p;
                next_p += (sizeof(*header_p) + size);
                self_p->next_p = next_p;
            }

            /* Initialize the allocated buffer. */
            header_p->u.fixed_p = fixed_p;
            header_p->size = size;
            header_p->count = 1;

            return (&header_p[1]);
            
        }

        fixed_p++;
    }

    return (NULL);
}

static void *alloc_dynamic_size(struct heap_t *self_p,
                                size_t size)
{
    struct heap_buffer_header_t *header_p, *prev_p;
    size_t left;
    char *next_p;

    /* Allocate from the free list. */
    header_p = self_p->dynamic.free_p;
    prev_p = NULL;

    while (header_p != NULL) {
        if (size <= header_p->size) {
            header_p->count = 1;
            
            if (prev_p != NULL) {
                prev_p->u.next_p = header_p->u.next_p;
            } else {
                self_p->dynamic.free_p = header_p->u.next_p;
            }

            return (&header_p[1]);
        }

        prev_p = header_p;
        header_p = header_p->u.next_p;
    }

    next_p = self_p->next_p;

    /* Allocate new memory. */
    left = (self_p->size - (next_p - (char *)self_p->buf_p));
    
    if (left < (sizeof(*header_p) + size)) {
        return (NULL);
    }
    
    header_p = self_p->next_p;
    next_p += (sizeof(*header_p) + size);
    self_p->next_p = next_p;

    /* Initialize the allocated buffer. */
    header_p->size = size;
    header_p->count = 1;

    return (&header_p[1]);
}

static int free_fixed_size(struct heap_t *self_p,
                           struct heap_buffer_header_t *header_p)
{
    struct heap_fixed_t *fixed_p;

    /* Add the buffer to the free list of its fixed size. */
    fixed_p = header_p->u.fixed_p;
    header_p->u.next_p = fixed_p->free_p;
    fixed_p->free_p = header_p;

    return (0);    
}

static int free_dynamic_buffer(struct heap_t *self_p,
                               struct heap_buffer_header_t *header_p)
{
    header_p->u.next_p = self_p->dynamic.free_p;
    self_p->dynamic.free_p = header_p;

    return (0);
}

int heap_init(struct heap_t *self_p,
              void *buf_p,
              size_t size,
              size_t sizes[HEAP_FIXED_SIZES_MAX])
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    int i;

    self_p->buf_p = buf_p;
    self_p->size = size;
    self_p->next_p = buf_p;

    for (i = 0; i < HEAP_FIXED_SIZES_MAX; i++) {
        self_p->fixed[i].free_p = NULL;
        self_p->fixed[i].size = sizes[i];
    }

    self_p->dynamic.free_p = NULL;

    return (0);
}

void *heap_alloc(struct heap_t *self_p,
                 size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    void *buf_p = NULL;

    sys_lock();

    if (size <= self_p->fixed[HEAP_FIXED_SIZES_MAX - 1].size) {
        buf_p = alloc_fixed_size(self_p, size);
    } else {
        buf_p = alloc_dynamic_size(self_p, size);
    }

    sys_unlock();

    return (buf_p);
}

int heap_free(struct heap_t *self_p,
              void *buf_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    int count;
    struct heap_buffer_header_t *header_p;

    header_p = &((struct heap_buffer_header_t *)buf_p)[-1];

    sys_lock();

    if (header_p->count > 0) {
        header_p->count--;
        count = header_p->count;
        
        /* Free when count is zero. */
        if (count == 0) {
            if (header_p->u.fixed_p != NULL) {
                count = free_fixed_size(self_p, header_p);
            } else {
                count = free_dynamic_buffer(self_p, header_p);
            }
        }
    } else {
        count = -1;
    }

    sys_unlock();

    return (count);
}

int heap_share(struct heap_t *self_p,
               const void *buf_p,
               int count)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(count >= 0, EINVAL);

    struct heap_buffer_header_t *header_p;

    header_p = &((struct heap_buffer_header_t *)buf_p)[-1];

    sys_lock();
    header_p->count += count;
    sys_unlock();

    return (0);
}
