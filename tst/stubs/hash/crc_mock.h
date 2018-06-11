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

#ifndef __CRC_MOCK_H__
#define __CRC_MOCK_H__

#include "simba.h"

int mock_write_crc_32(uint32_t crc,
                      const void *buf_p,
                      size_t size,
                      uint32_t res);

int mock_write_crc_ccitt(uint16_t crc,
                         const void *buf_p,
                         size_t size,
                         uint16_t res);

int mock_write_crc_xmodem(uint16_t crc,
                          const void *buf_p,
                          size_t size,
                          uint16_t res);

int mock_write_crc_7(const void* buf_p,
                     size_t size,
                     uint8_t res);

int mock_write_crc_8(uint8_t crc,
                     uint8_t polynomial,
                     const void *buf_p,
                     size_t size,
                     uint8_t res);

#endif
