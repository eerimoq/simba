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
