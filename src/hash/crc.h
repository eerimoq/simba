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
