/**
 * @file board.c
 * @version 5.0.0
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

    if (strncmp(&str_p[0], "gpio", 4) == 0) {
        if (std_strtol(&str_p[4], &pin) == NULL) {
            return (-1);
        }
        
        switch (pin) {

        case 0:
        case 2:
        case 12:
        case 13:
        case 14:
        case 15:
            break;

        case 4:
            pin = 5;
            break;
            
        case 5:
            pin = 4;
            break;
            
        default:
            return (-1);
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 2;
    } else {
        return (-1);
    }

    return (pin);
}
