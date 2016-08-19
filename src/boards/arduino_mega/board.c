/**
 * @file board.c
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
    long pin;

    if (tolower((int)str_p[0]) == 'd') {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }
        
        if ((pin < 0) || (pin > 53)) {
            return (-1);
        }
    } else if (tolower((int)str_p[0]) == 'a') {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }
        
        if ((pin < 0) || (pin > 15)) {
            return (-1);
        }
        
        pin += 54;
    } else if (strcmp(str_p, "led") == 0) {
        pin = 13;
    } else {
        return (-1);
    }

    return (pin);
}
