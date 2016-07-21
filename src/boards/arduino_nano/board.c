/**
 * @file board.c
 * @version 2.0.0
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

int board_pin_str_to_pin(const char *str_p)
{
    long pin;

    if (tolower((int)str_p[0]) == 'd') {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }
        
        if ((pin < 2) || (pin > 13)) {
            return (-1);
        }

        if (pin == 11) {
            pin = 12;
        } else if (pin == 12) {
            pin = 11;
        }
    } else if (tolower((int)str_p[0]) == 'a') {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }
        
        if ((pin < 0) || (pin > 5)) {
            return (-1);
        }
        
        pin += 14;
    } else if (strcmp(str_p, "led") == 0) {
        pin = 13;
    } else if (strcmp(str_p, "rx") == 0) {
        pin = 0;
    } else if (strcmp(str_p, "tx") == 0) {
        pin = 1;
    } else {
        return (-1);
    }

    return (pin);
}
