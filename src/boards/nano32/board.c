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

    if (strncmp(&str_p[0], "gpio", 4) == 0) {
        if (std_strtol(&str_p[4], &pin) == NULL) {
            return (-1);
        }

        if ((pin >= 0) && (pin <= 19)) {
            /* Correct pin number. */
        } else if ((pin >= 21) && (pin <= 23)) {
            pin -= 1;
        } else if ((pin >= 25) && (pin <= 27)) {
            pin -= 2;
        } else if ((pin >= 32) && (pin <= 36)) {
            pin -= 6;
        } else if (pin == 39) {
            pin -= 6;
        } else {
            return (-1);
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 16;
    } else {
        return (-1);
    }

    return (pin);
}
