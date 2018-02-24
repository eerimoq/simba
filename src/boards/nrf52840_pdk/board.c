/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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
    ASSERTN(str_p != NULL, EINVAL);

    int pin;
    long value;

    pin = -ENODEV;

    if (strcmp(str_p, "led") == 0) {
        pin = 17;

        if ((str_p[3] >= '1') && (str_p[3] <= '4')) {
            pin += (str_p[3] - '1');
        }
    } else if (strcmp(str_p, "ain") == 0) {
        if ((str_p[3] >= '0') && (str_p[3] <= '3')) {
            pin = (2 + str_p[3] - '0');
        } else if ((str_p[3] >= '4') && (str_p[3] <= '7')) {
            pin = (28 + str_p[3] - '4');
        }
    } else if (strcmp(str_p, "btn") == 0) {
        if ((str_p[3] >= '1') && (str_p[3] <= '4')) {
            pin = (13 + str_p[3] - '1');
        }
    } else if (strcmp(str_p, "p") == 0) {
        if (std_strtol(&str_p[1], &value) != NULL) {
            if ((value >= 0) && (value <= 31)) {
                pin = value;
            }
        }
    }

    return (pin);
}
