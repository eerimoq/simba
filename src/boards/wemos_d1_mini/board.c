/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

/**
 * Mapping table Dx -> pin number.
 */
static const int digital_pin_map[] = {
    16,
     5,
     4,
     0,
     2,
    14,
    12,
    13,
    15,
};

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
        case 4:
        case 5:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            break;

        default:
            return (-1);
        }
    } else if ((str_p[0] == 'd') || (str_p[0] == 'D')) {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }

        if ((pin < 0) || (pin >= membersof(digital_pin_map))) {
            return (-1);
        }

        pin = digital_pin_map[pin];
    } else if (strcmp(str_p, "led") == 0) {
        pin = 2;
    } else {
        return (-1);
    }

    return (pin);
}
