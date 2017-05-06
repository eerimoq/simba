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
    int pin;
    long pad;

    if (str_p[0] == 'p') {
        if (std_strtol(&str_p[2], &pad) == NULL) {
            return (-1);
        }

        switch (str_p[1]) {

        case 'a':
            if ((pad < 0) || (pad > 15)) {
                return (-1);
            }

            pin = pad;
            break;

        case 'b':
            if ((pad < 0) || (pad > 15)) {
                return (-1);
            }

            pin = (16 + pad);
            break;

        case 'c':
            if ((pad < 0) || (pad > 10)) {
                return (-1);
            }

            pin = (32 + pad);
            break;

        case 'h':
            if ((pad < 9) || (pad > 10)) {
                return (-1);
            }

            pin = (43 + pad - 9);
            break;

        default:
            return (-1);
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 34;
    } else {
        return (-1);
    }

    return (pin);
}
