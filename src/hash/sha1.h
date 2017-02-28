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

#ifndef __HASH_SHA1_H__
#define __HASH_SHA1_H__

#include "simba.h"

struct sha1_t {
    struct {
        uint8_t buf[64]; 
        uint32_t size; 
    } block;
    uint32_t h[5]; 
    uint64_t size; 
}; 

/**
 * Initialize given SHA1 object.
 *
 * @param[in,out] self_p SHA1 object.
 *
 * @return zero(0) or negative error code.
 */
int sha1_init(struct sha1_t *self_p);

/**
 * Update the sha object with the given buffer. Repeated calls are
 * equivalent to a single call with the concatenation of all the
 * arguments.
 *
 * @param[in] self_p SHA1 object.
 * @param[in] buf_p Buffer to update the sha object with.
 * @param[in] size Size of the buffer.
 *
 * @return zero(0) or negative error code.
 */
int sha1_update(struct sha1_t *self_p,
                void *buf_p,
                size_t size);

/**
 * Return the digest of the strings passed to the sha1_update()
 * method so far. This is a 20-byte value which may contain non-ASCII
 * characters, including null bytes.
 *
 * @param[in] self_p SHA1 object.
 * @param[in] hash_p Hash sum.
 *
 * @return zero(0) or negative error code.
 */
int sha1_digest(struct sha1_t *self_p,
                uint8_t *hash_p);

#endif
