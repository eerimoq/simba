/**
 * @file inet.c
 * @version 1.0.0
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

int inet_aton(const char *addr_str_p,
              struct inet_ip_addr_t *addr_p)
{
    int i;
    long number;
    const char *number_p;

    addr_p->number = 0;
    number_p = addr_str_p;

    for (i = 0; i < 3; i++) {
        number_p = std_strtol(number_p, &number);

        if ((number_p == NULL)
            || (*number_p != '.')
            || (number < 0)
            || (number > 255)) {
            return (-EINVAL);
        }

        addr_p->number <<= 8;
        addr_p->number += number;

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

    addr_p->number <<= 8;
    addr_p->number += number;
    
    addr_p->number = htonl(addr_p->number);

    return (0);
}
