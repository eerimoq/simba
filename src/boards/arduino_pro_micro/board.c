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

int board_pin_string_to_device_index(const char *str_p)
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
