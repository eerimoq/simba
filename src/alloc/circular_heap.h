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
