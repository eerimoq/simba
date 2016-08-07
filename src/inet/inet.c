/**
 * @file inet.c
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

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
