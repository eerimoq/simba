/**
 * @file slib/crc.h
 * @version 0.7.0
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

#ifndef __SLIB_CRC_H__
#define __SLIB_CRC_H__

#include "simba.h"

/**
 * Calculate crc using the polynom
 * ``x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x^1+x^0``.
 *
 * @param[in] crc Initial crc. Often zero(0).
 * @param[in] buf_p Buffer to calculate crc of.
 * @param[in] size Size of the buffer.
 *
 * @return Calculated crc.
 */
uint32_t crc_32(uint32_t crc, const void *buf_p, size_t size);

#endif
