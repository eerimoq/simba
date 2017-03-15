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
        case 0: pin = (SAM_PA + 8); break;
        case 1: pin = (SAM_PA + 9); break;
        case 2: pin = (SAM_PB + 25); break;
        case 3: pin = (SAM_PC + 28); break;
        case 4: pin = (SAM_PC + 26); break;
        case 5: pin = (SAM_PC + 25); break;
        case 6: pin = (SAM_PC + 24); break;
        case 7: pin = (SAM_PC + 23); break;
        case 8: pin = (SAM_PC + 22); break;
        case 9: pin = (SAM_PC + 21); break;
        case 10: pin = (SAM_PA + 28); break;
        case 11: pin = (SAM_PD + 7); break;
        case 12: pin = (SAM_PD + 8); break;
        case 13: pin = (SAM_PB + 27); break;
        case 14: pin = (SAM_PD + 4); break;
        case 15: pin = (SAM_PD + 5); break;
        case 16: pin = (SAM_PA + 13); break;
        case 17: pin = (SAM_PA + 12); break;
        case 18: pin = (SAM_PA + 11); break;
        case 19: pin = (SAM_PA + 10); break;
        case 20: pin = (SAM_PB + 12); break;
        case 21: pin = (SAM_PB + 13); break;
        case 22: pin = (SAM_PB + 26); break;
        case 23: pin = (SAM_PA + 14); break;
        case 24: pin = (SAM_PA + 15); break;
        case 25: pin = (SAM_PD + 0); break;
        case 26: pin = (SAM_PD + 1); break;
        case 27: pin = (SAM_PD + 2); break;
        case 28: pin = (SAM_PD + 3); break;
        case 29: pin = (SAM_PD + 6); break;
        case 30: pin = (SAM_PD + 9); break;
        case 31: pin = (SAM_PA + 7); break;
        case 32: pin = (SAM_PD + 10); break;
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
            pin = (SAM_PC + 1 - 33 + pin);
            break;
        case 42: pin = (SAM_PA + 19); break;
        case 43: pin = (SAM_PA + 20); break;
        case 44: pin = (SAM_PC + 19); break;
        case 45: pin = (SAM_PC + 18); break;
        case 46: pin = (SAM_PC + 17); break;
        case 47: pin = (SAM_PC + 16); break;
        case 48: pin = (SAM_PC + 15); break;
        case 49: pin = (SAM_PC + 14); break;
        case 50: pin = (SAM_PC + 13); break;
        case 51: pin = (SAM_PC + 12); break;
        case 52: pin = (SAM_PB + 21); break;
        case 53: pin = (SAM_PB + 14); break;
        default: return (-1);
        }
    } else if (tolower((int)str_p[0]) == 'a') {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }

        switch (pin) {
        case 0: pin = (SAM_PA + 16); break;
        case 1: pin = (SAM_PA + 24); break;
        case 2: pin = (SAM_PA + 23); break;
        case 3: pin = (SAM_PA + 22); break;
        case 4: pin = (SAM_PA + 6); break;
        case 5: pin = (SAM_PA + 4); break;
        case 6: pin = (SAM_PA + 3); break;
        case 7: pin = (SAM_PA + 2); break;
        case 8:
        case 9:
        case 10:
        case 11:
            pin = (SAM_PB + 17 - 8 + pin);
            break;
        default:
            return (-1);
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = (SAM_PB + 27);
    } else {
        return (-1);
    }

    return (pin);
}
