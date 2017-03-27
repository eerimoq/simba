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
