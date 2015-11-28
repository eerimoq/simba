/**
 * @file kernel/bits.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#ifndef __KERNEL_BITS_H__
#define __KERNEL_BITS_H__

#include "simba.h"

/**
 * Insert value of given size and position into value.
 *
 * @param[in] value_p Value to insert into.
 * @param[in] position Position in value where to insert, counted from LSB.
 * @param[in] size Number of bits to insert.
 * @param[in] value Value to intert.
 */
static inline uint32_t bits_insert_32(uint32_t dst,
                                      int position,
                                      int size,
                                      uint32_t src)
{
    uint32_t mask;

    mask = (((1 << size) - 1) << position);
    dst &= ~mask;
    dst |= ((src << position) & mask);

    return (dst);
}

#endif
