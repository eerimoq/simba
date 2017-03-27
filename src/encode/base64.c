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

#include "simba.h"

/**
 * Index must be a number 0-64.
 */
static char index_to_encoded(int index)
{
    if ((index >= 0) && (index <= 25)) {
        return ('A' + index - 0);
    } else if ((index >= 26) && (index <= 51)) {
        return ('a' + index - 26);
    } else if ((index >= 52) && (index <= 61)) {
        return ('0' + index - 52);
    } else if (index == 62) {
        return ('+');
    } else if (index == 63) {
        return ('/');
    } else {
        return ('=');
    }
}

static int encoded_to_index(char encoded)
{
    if ((encoded >= 'A') && (encoded <= 'Z')) {
        return (0 + encoded - 'A');
    } else if ((encoded >= 'a') && (encoded <= 'z')) {
        return (26 + encoded - 'a');
    } else if ((encoded >= '0') && (encoded <= '9')) {
        return (52 + encoded - '0');
    } else if (encoded == '+') {
        return (62);
    } else if (encoded == '/') {
        return (63);
    } else if (encoded == '=') {
        return (0);
    }

    return (-1);
}

int base64_encode(char *dst_p, const void *src_p, size_t size)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    int i;
    int j;
    int index;
    const uint8_t *s_p = src_p;

    for (i = 0, j = 0; i < size; i += 3, j += 4) {
        index = ((s_p[i + 0] & 0xfc) >> 2);
        dst_p[j + 0] = index_to_encoded(index);
        index = ((s_p[i + 0] & 0x03) << 4);

        if (i + 1 < size) {
            index |= ((s_p[i + 1] & 0xf0) >> 4);
            dst_p[j + 1] = index_to_encoded(index);
            index = (s_p[i + 1] & 0x0f) << 2;
            
            if (i + 2 < size) {
                index |= ((s_p[i + 2] & 0xc0) >> 6);
                dst_p[j + 2] = index_to_encoded(index);
                index = (s_p[i + 2] & 0x3f);
                dst_p[j + 3] = index_to_encoded(index);
            } else {
                dst_p[j + 2] = index_to_encoded(index);
                dst_p[j + 3] = index_to_encoded(64);
            }
        } else {
            dst_p[j + 1] = index_to_encoded(index);
            dst_p[j + 2] = index_to_encoded(64);
            dst_p[j + 3] = index_to_encoded(64);
        }
    }

    return (0);
}

int base64_decode(void *dst_p, const char *src_p, size_t size)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    int i;
    int j;
    int index[4];
    uint8_t *d_p = dst_p;
    
    if ((size % 4) != 0) {
        return (-EINVAL);
    }

    for (i = 0, j = 0; i < size; i += 4, j += 3) {
        index[0] = encoded_to_index(src_p[i + 0]);
        index[1] = encoded_to_index(src_p[i + 1]);
        index[2] = encoded_to_index(src_p[i + 2]);
        index[3] = encoded_to_index(src_p[i + 3]);

        if ((index[0] == -1)
            || (index[1] == -1)
            || (index[2] == -1)
            || (index[3] == -1)) {
            return (-1);
        }

        d_p[j] = ((index[0] << 2) | (index[1] >> 4));
        d_p[j + 1] = ((index[1] << 4) | (index[2] >> 2));
        d_p[j + 2] = ((index[2] << 6) | index[3]);
    }
    
    return (0);
}
