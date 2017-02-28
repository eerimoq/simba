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

#ifndef __ENCODE_BASE64_H__
#define __ENCODE_BASE64_H__

#include "simba.h"

/**
 * Encode given buffer. The encoded data will be ~33.3% larger than
 * the source data. Choose the destination buffer size accordingly.
 *
 * @param[out] dst_p Encoded output data.
 * @param[in] src_p Input data.
 * @param[in] size Number of bytes in the input data.
 *
 * @return zero(0) or negative error code.
 */
int base64_encode(char *dst_p, const void *src_p, size_t size);

/**
 * Decode given base64 encoded buffer. The decoded data will be ~25%
 * smaller than the destination data. Choose the destination buffer
 * size accordingly.
 *
 * @param[out] dst_p Output data.
 * @param[in] src_p Encoded input data.
 * @param[in] size Number of bytes in the encoded input data.
 *
 * @return zero(0) or negative error code.
 */
int base64_decode(void *dst_p, const char *src_p, size_t size);

#endif
