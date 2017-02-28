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

static uint32_t inet_checksum_begin(void)
{
  return (0);
}

static uint32_t inet_checksum_acc(uint32_t acc, uint16_t *buf_p, size_t size)
{
    while (size > 1) {
        acc += htons(*buf_p);
        buf_p++;
        size -= 2;
    }

    if (size > 0) {
        acc += (htons(*buf_p) & 0xff00);
    }

    return (acc);
}

static uint16_t inet_checksum_end(uint32_t acc)
{
    acc = (acc >> 16) + (acc & 0xffffUL);

    if ((acc & 0xffff0000UL) != 0) {
        acc = (acc >> 16) + (acc & 0xffffUL);
    }

    return (~acc);
}

int inet_module_init()
{
    return (0);
}

int inet_aton(const char *src_p,
              struct inet_ip_addr_t *dst_p)
{
    int i;
    long number;
    const char *number_p;

    dst_p->number = 0;
    number_p = src_p;

    for (i = 0; i < 3; i++) {
        number_p = std_strtol(number_p, &number);

        if ((number_p == NULL)
            || (*number_p != '.')
            || (number < 0)
            || (number > 255)) {
            return (-EINVAL);
        }

        dst_p->number <<= 8;
        dst_p->number += number;

        /* Skip the dot. */
        number_p++;
    }

    number_p = std_strtol(number_p, &number);

    if ((number_p == NULL)
        || (*number_p != '\0')
        || (number < 0)
        || (number > 255)) {
        return (-EINVAL);
    }

    dst_p->number <<= 8;
    dst_p->number += number;

    dst_p->number = htonl(dst_p->number);

    return (0);
}

char *inet_ntoa(const struct inet_ip_addr_t *src_p,
                char *dst_p)
{
    uint32_t number = ntohl(src_p->number);

    std_sprintf(dst_p,
                FSTR("%u.%u.%u.%u"),
                (int)(number >> 24) & 0xff,
                (int)(number >> 16) & 0xff,
                (int)(number >>  8) & 0xff,
                (int)(number >>  0) & 0xff);

    return (dst_p);
}

uint16_t inet_checksum(void *buf_p, size_t size)
{
    uint32_t acc;

    acc = inet_checksum_begin();
    acc = inet_checksum_acc(acc, buf_p, size);

    return (inet_checksum_end(acc));
}
