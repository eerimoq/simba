/**
 * @file alloc/circular_heap.h
 * @version 1.0.0
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

#ifndef __ALLOC_CIRCULAR_HEAP_H__
#define __ALLOC_CIRCULAR_HEAP_H__

#include "simba.h"

/* Circular_Heap. */
struct circular_heap_t {
    void *begin_p;
    void *end_p;
    void *alloc_p;
    void *free_p;
};

/**
 * Initialize given circular_heap.
 *
 * @param[in] self_p Circular heap to initialize.
 * @param[in] buf_p Memory buffer.
 * @param[in] size Size of the memory buffer.
 *
 * @return zero(0) or negative error code.
 */
int circular_heap_init(struct circular_heap_t *self_p,
                       void *buf_p,
                       size_t size);

/**
 * Allocate a buffer of given size from given circular heap.
 *
 * @param[in] self_p Circular heap to allocate from.
 * @param[in] size Number of bytes to allocate.
 *
 * @return Pointer to allocated buffer, or NULL on failure.
 */
void *circular_heap_alloc(struct circular_heap_t *self_p,
                          size_t size);

/**
 * Free the oldest allocated buffer.
 *
 * @param[in] self_p Circular heap to free to.
 * @param[in] buf_p Buffer to free. Must be the oldest allocated
 *                  buffer.
 *
 * @return zero(0) or negative error code.
 */
int circular_heap_free(struct circular_heap_t *self_p,
                       void *buf_p);

#endif
