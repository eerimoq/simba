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

#ifndef __COLLECTIONS_CIRCULAR_BUFFER_H__
#define __COLLECTIONS_CIRCULAR_BUFFER_H__

#include "simba.h"

/* Circular buffer. */
struct circular_buffer_t {
    char *buf_p;
    size_t size;
    size_t writepos;
    size_t readpos;
};

/**
 * Initialize given circular buffer.
 *
 * @param[in] self_p Circular buffer to initialize.
 * @param[in] buf_p Memory buffer.
 * @param[in] size Size of the memory buffer.
 *
 * @return zero(0) or negative error code.
 */
int circular_buffer_init(struct circular_buffer_t *self_p,
                         void *buf_p,
                         size_t size);

/**
 * Write data to given circular buffer.
 *
 * @param[in] self_p Circular buffer.
 * @param[in] buf_p Memory buffer to write.
 * @param[in] size Size of the memory buffer.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t circular_buffer_write(struct circular_buffer_t *self_p,
                              const void *buf_p,
                              size_t size);

/**
 * Read data from given circular buffer.
 *
 * @param[in] self_p Circular buffer to free to.
 * @param[in] buf_p Memory buffer to read into.
 * @param[in] size Size of the memory buffer.
 *
 * @return Number of bytes read or negative error code. The buffer is
 *         empty if zero(0) is returned.
 */
ssize_t circular_buffer_read(struct circular_buffer_t *self_p,
                             void *buf_p,
                             size_t size);

/**
 * Returns the number of used bytes in given circular buffer.
 *
 * @param[in] self_p Circular buffer.
 *
 * @return Number of used bytes.
 */
ssize_t circular_buffer_used_size(struct circular_buffer_t *self_p);

/**
 * Returns the number of unused bytes in given circular buffer.
 *
 * @param[in] self_p Circular buffer.
 *
 * @return Number of unused bytes.
 */
ssize_t circular_buffer_unused_size(struct circular_buffer_t *self_p);

/**
 * Skip given number of written bytes at the front of the buffer.
 *
 * @param[in] self_p Circular buffer.
 * @param[in] size Number of bytes to skip.
 *
 * @return Number of skipped bytes or negative error code.
 */
ssize_t circular_buffer_skip_front(struct circular_buffer_t *self_p,
                                   size_t size);

/**
 * Skip given number of written bytes at the back of the buffer.
 *
 * @param[in] self_p Circular buffer.
 * @param[in] size Number of bytes to skip.
 *
 * @return Number of skipped bytes or negative error code.
 */
ssize_t circular_buffer_reverse_skip_back(struct circular_buffer_t *self_p,
                                          size_t size);

/**
 * Get a pointer to the next byte to read from the buffer. Use
 * `circular_buffer_array_two()` to get the second array, if there is
 * a wrap around.
 *
 * @param[in] self_p Circular buffer.
 * @param[out] buf_pp A poitner to the start of the array. Only valid
 *                    if the return value is greater than zero(0).
 * @param[in] size Number of bytes asked for.
 *
 * @return Number of bytes in array or negative error code.
 */
ssize_t circular_buffer_array_one(struct circular_buffer_t *self_p,
                                  void **buf_pp,
                                  size_t size);

/**
 * Get a pointer to the next byte to read from the buffer, following a
 * wrap around.
 *
 * @param[in] self_p Circular buffer.
 * @param[out] buf_pp A poitner to the start of the array. Only valid
 *                    if the return value is greater than zero(0).
 * @param[in] size Number of bytes asked for.
 *
 * @return Number of bytes in array or negative error code.
 */
ssize_t circular_buffer_array_two(struct circular_buffer_t *self_p,
                                  void **buf_pp,
                                  size_t size);

#endif
