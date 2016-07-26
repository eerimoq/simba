/**
 * @file collections/bits.h
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
 * MERBITSTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __COLLECTIONS_BITS_H__
#define __COLLECTIONS_BITS_H__

#include "simba.h"

/**
 * Insert given number of bits into another value at given position.
 *
 * For example, `bits_insert_32(0xffffffff, 4, 8, 0x12)` would return
 * `0xfffff12f`.
 *
 * @param[in] dst Value to insert into.
 * @param[in] position Bit position, counted from LSB, in `dst` where
 *                     to insert `src`, 0-31.
 * @param[in] size Number of bits to insert. 0-31.
 * @param[in] src Value to insert into dst.
 *
 * @return The resulting value of the insertion.
 */
static inline uint32_t bits_insert_32(uint32_t dst,
                                      int position,
                                      int size,
                                      uint32_t src)
{
    uint32_t mask;

    mask = (((1LU << size) - 1) << position);
    dst &= ~mask;
    dst |= ((src << position) & mask);

    return (dst);
}

#endif
