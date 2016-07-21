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
        
        switch (pin) {
        case 2:  pin = 11; break;
        case 3:  pin = 10; break;
        case 4:  pin = 14; break;
        case 5:  pin = 8;  break;
        case 6:  pin = 17; break;
        case 7:  pin = 18; break;
        case 8:  pin = 4;  break;
        case 9:  pin = 5;  break;
        case 10: pin = 6;  break;
        case 14: pin = 3;  break;
        case 15: pin = 1;  break;
        case 16: pin = 2;  break;
        default: return (-1);
        }
    } else if (tolower((int)str_p[0]) == 'a') {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }
        
        switch (pin) {
        case 0:  pin = 24; break;
        case 1:  pin = 23; break;
        case 2:  pin = 22; break;
        case 3:  pin = 21;  break;
        default: return (-1);
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 0;
    } else if (strcmp(str_p, "rx") == 0) {
        pin = 12;
    } else if (strcmp(str_p, "tx") == 0) {
        pin = 13;
    } else {
        return (-1);
    }

    return (pin);
}
