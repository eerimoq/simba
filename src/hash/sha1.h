/**
 * @file hash/sha1.h
 * @version 1.1.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
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
