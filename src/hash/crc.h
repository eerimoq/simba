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

#ifndef __HASH_CRC_H__
#define __HASH_CRC_H__

#include "simba.h"

/**
 * The polynomial ``x^8 + x^5 + x^4 + x^0``.
 */
#define CRC_8_POLYNOMIAL_8_5_4_0                         0x8c

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

/**
 * Calculate a 8 bits crc using given polynomial.
 *
 * @param[in] crc Initial crc. Must be 0x00 on first call.
 * @param[in] polynimial CRC polynomial.
 * @param[in] buf_p Buffer to calculate crc of.
 * @param[in] size Size of the buffer.
 *
 * @return Calculated crc.
 */
uint8_t crc_8(uint8_t crc, uint8_t polynomial, const void *buf_p, size_t size);

#endif
