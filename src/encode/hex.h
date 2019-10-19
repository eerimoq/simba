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

#ifndef __ENCODE_HEX_H__
#define __ENCODE_HEX_H__

#include "simba.h"

/**
 * Convert given hex string to binary data.
 *
 * @param[out] dst_p Binary output.
 * @param[in] src_p Input hex string.
 * @param[in] size Length of the input hex string, not including
 *                 null-termination.
 *
 * @return Number of bytes written to the output buffer, or negative
 *         error code.
 */
int hex_to_bin(void *dst_p, const char *src_p, size_t size);

/**
 * Convert given binary data to a hex string.
 *
 * @param[out] dst_p Output hex string.
 * @param[in] src_p Input data.
 * @param[in] size Input size in bytes.
 *
 * @return Output hex string length, not including
 *         null-termination. Always twice the input size.
 */
int hex_from_bin(char *dst_p, const void *src_p, size_t size);

#endif
