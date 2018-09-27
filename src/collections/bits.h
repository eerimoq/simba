/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#ifndef __COLLECTIONS_BITS_H__
#define __COLLECTIONS_BITS_H__

#include "simba.h"

/**
 * Create a bit mask of given width.
 *
 * @param[in] width Width in bits.
 *
 * @return The bit mask.
 */
static inline uint32_t bits_mask_32(int width)
{
    return ((1 << width) - 1);
}

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

/**
 * Reverse the order of given 8 bits.
 *
 * @return Reversed bits.
 */
uint8_t bits_reverse_8(uint8_t value);

/**
 * Reverse the order of given 16 bits.
 *
 * @return Reversed bits.
 */
uint16_t bits_reverse_16(uint16_t value);

/**
 * Reverse the order of given 32 bits.
 *
 * @return Reversed bits.
 */
uint32_t bits_reverse_32(uint32_t value);

#endif
