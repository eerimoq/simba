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
