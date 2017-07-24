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

        if ((pin >= 0) && (pin <= 1)) {
            pin += 32;
        } else if ((pin >= 2) && (pin <= 3)) {
            pin += 36;
        } else if (pin == 4) {
            pin = 53;
        } else if (pin == 5) {
            pin = 35;
        } else if ((pin >= 6) && (pin <= 9)) {
            pin += 51;
        } else if ((pin >= 10) && (pin <= 13)) {
            pin += 2;
        } else if ((pin >= 14) && (pin <= 15)) {
            pin = (77 - pin);
        } else if ((pin >= 16) && (pin <= 17)) {
            pin = (71 - pin);
        } else if ((pin >= 18) && (pin <= 21)) {
            pin = (45 - pin);
        } else if ((pin >= 22) && (pin <= 29)) {
            pin -= 22;
        } else if ((pin >= 30) && (pin <= 37)) {
            pin = (53 - pin);
        } else if (pin == 38) {
            pin = 31;
        } else if ((pin >= 39) && (pin <= 41)) {
            pin = (89 - pin);
        } else if ((pin >= 42) && (pin <= 49)) {
            pin = (127 - pin);
        } else if ((pin >= 50) && (pin <= 53)) {
            pin = (61 - pin);
        } else {
            pin = -ENODEV;
        }
    } else if (tolower((int)str_p[0]) == 'a') {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }

        if ((pin >= 0) && (pin <= 7)) {
            pin += 40;
        } else if ((pin >= 8) && (pin <= 15)) {
            pin += 62;
        } else {
            pin = -ENODEV;
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 15;
    } else {
        pin = -ENODEV;
    }

    return (pin);
}
