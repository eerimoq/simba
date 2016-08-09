/**
 * @file board.c
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

int board_pin_string_to_device_index(const char *str_p)
{
    int port;
    int gpio;
    long bit;

    /* Get gpio and bit. */
    if (str_p[0] != 'p') {
        return (-1);
    }

    port = tolower((int)str_p[1]);

    if ((port < 'a') || (port > 'd')) {
        return (-1);
    }

    gpio = (port - 'a');

    if (std_strtol(&str_p[2], &bit) == NULL) {
        return (-1);
    }

    if (bit < 0) {
        return (-1);
    }

    if ((gpio == 'd' && bit > 3) || (bit > 15)) {
        return (-1);
    }

    return (16 * gpio + bit);
}
