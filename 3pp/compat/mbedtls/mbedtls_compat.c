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

int mbedtls_hardware_poll(void *data_p,
                          unsigned char *output_p,
                          size_t len,
                          size_t *olen)
{
    uint32_t random;
    uint32_t *u32_output_p;
    int full;
    int rest;
    int i;
    
    full = (len / 4);
    rest = (len % 4);
    u32_output_p = (uint32_t *)output_p;
    
    for (i = 0; i < full; i++) {
        *u32_output_p++ = random_read();
    }

    if (rest > 0) {
        output_p = (unsigned char *)u32_output_p;
        random = random_read();

        for (i = 0; i < rest; i++) {
            *output_p++ = (unsigned char)random;
            random >>= 8;
        }
    }

    *olen = len;
    
    return (0);    
}
