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

#ifndef __ALLOC_HEAP_H__
#define __ALLOC_HEAP_H__

#include "simba.h"

#define HEAP_FIXED_SIZES_MAX 8

struct heap_fixed_t {
    void *free_p;
    size_t size;
};

struct heap_dynamic_t {
    void *free_p;
};

/* Heap. */
struct heap_t {
    void *buf_p;
    size_t size;
    void *next_p;
    struct heap_fixed_t fixed[HEAP_FIXED_SIZES_MAX];
    struct heap_dynamic_t dynamic;
};

/**
 * Initialize given heap.
 *
 * @param[in] self_p Heap to initialize.
 * @param[in] buf_p Heap memory buffer.
 * @param[in] size Size of the heap memory buffer.
 *
 * @return zero(0) or negative error code.
 */
int heap_init(struct heap_t *self_p,
              void *buf_p,
              size_t size,
              size_t sizes[HEAP_FIXED_SIZES_MAX]);

/**
 * Allocate a buffer of given size from given heap.
 *
 * @param[in] self_p Heap to allocate from.
 * @param[in] size Number of bytes to allocate.
 *
 * @return Pointer to allocated buffer, or NULL on failure.
 */
void *heap_alloc(struct heap_t *self_p,
                 size_t size);

/**
 * Decrement the share count by once and free the buffer if the count
 * becomes zero(0).
 *
 * @param[in] self_p Heap of given buffer.
 * @param[in] buf_p Memory buffer to free.
 *
 * @return Share count after the free, or negative error code.
 */
int heap_free(struct heap_t *self_p,
              void *buf_p);

/**
 * Share given buffer ``count`` times.
 *
 * @param[in] self_p Heap of given buffer.
 * @param[in] buf_p Buffer to share.
 * @param[in] count Share count.
 *
 * @return zero(0) or negative error code.
 */
int heap_share(struct heap_t *self_p,
               const void *buf_p,
               int count);

#endif
