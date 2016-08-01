/**
 * @file hash/crc.h
 * @version 4.1.0
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

#ifndef __HASH_CRC_H__
#define __HASH_CRC_H__

#include "simba.h"

/**
 * Calculate a 32 bits crc using the polynomial
 * ``x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x^1+x^0``.
 *
 * @param[in] crc Initial crc. Often 0x00000000.
 * @param[in] buf_p Buffer to calculate crc of.
 * @param[in] size Size of the buffer.
 *
 * @return Calculated crc.
 */
uint32_t crc_32(uint32_t crc, const void *buf_p, size_t size);

/**
 * Calculate a 16 bits crc using the CCITT algorithm (polynomial
 * ``x^16+x^12+x^5+x^1``).
 *
 * @param[in] crc Initial crc. Should be 0xffff for CCITT.
 * @param[in] buf_p Buffer to calculate crc of.
 * @param[in] size Size of the buffer.
 *
 * @return Calculated crc.
 */
uint16_t crc_ccitt(uint16_t crc, const void *buf_p, size_t size);

/**
 * Calculate a 16 bits crc using the XModem algorithm (polynomial
 * ``x^16+x^12+x^5+x^1``).
 *
 * @param[in] crc Initial crc. Should be 0x0000 for XModem.
 * @param[in] buf_p Buffer to calculate crc of.
 * @param[in] size Size of the buffer.
 *
 * @return Calculated crc.
 */
uint16_t crc_xmodem(uint16_t crc, const void *buf_p, size_t size);

/**
 * Calculate a 8 bits crc using the CRC-7 algorithm (polynomial
 * ``x^7+x^3+1``).
 *
 * @param[in] buf_p Buffer to calculate crc of.
 * @param[in] size Size of the buffer.
 *
 * @return Calculated crc.
 */
uint8_t crc_7(const void* buf_p, size_t size);

#endif
