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

#include "simba.h"

static int to_nibble(int ch)
{
    ch = tolower(ch);

    if ((ch >= '0') && (ch <= '9')) {
        return (ch - '0');
    } else if ((ch >= 'a') && (ch <= 'f')) {
        return (ch - 'a' + 10);
    } else {
        return (-EINVAL);
    }
}

static int from_nibble(uint8_t nibble)
{
    if (nibble < 10) {
        return (nibble + '0');
    } else {
        return (nibble + 'a' - 10);
    }
}

int hex_to_bin(void *dst_p, const char *src_p, size_t size)
{
    size_t i;
    uint8_t *u8_dst_p;
    int ch;

    if ((size % 2) != 0) {
        return (-EINVAL);
    }

    u8_dst_p = (uint8_t *)dst_p;
    size /= 2;

    for (i = 0; i < size; i++) {
        /* First nibble. */
        ch = to_nibble(src_p[2 * i]);

        if (ch < 0) {
            return (ch);
        }

        u8_dst_p[i] = ((uint8_t)ch << 4);

        /* Second nibble. */
        ch = to_nibble(src_p[2 * i + 1]);

        if (ch < 0) {
            return (ch);
        }

        u8_dst_p[i] |= (uint8_t)ch;
    }

    return (size);
}

int hex_from_bin(char *dst_p, const void *src_p, size_t size)
{
    size_t i;
    const uint8_t *u8_src_p;

    u8_src_p = (const uint8_t *)src_p;

    for (i = 0; i < size; i++) {
        dst_p[2 * i] = from_nibble((u8_src_p[i] >> 4) & 0x0f);
        dst_p[2 * i + 1] = from_nibble(u8_src_p[i] & 0x0f);
    }

    dst_p[2 * size] = '\0';

    return (2 * size);
}
