/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

int board_pin_string_to_device_index(const char *str_p)
{
    long pin;

    if (strncmp(&str_p[0], "d", 1) == 0) {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }

        if (pin == 0) {
            pin = 16;
        } else if (pin == 1) {
            pin = 5;
        } else if (pin == 2) {
            pin = 4;
        } else if (pin == 3) {
            pin = 0;
        } else if (pin == 4) {
            pin = 2;
        } else if (pin == 5) {
            pin = 14;
        } else if (pin == 6) {
            pin = 12;
        } else if (pin == 7) {
            pin = 13;
        } else if (pin == 8) {
            pin = 15;
        } else if (pin == 9) {
            pin = 3;
        } else if (pin == 10) {
            pin = 1;
        } else {
            pin = -1;
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 16;
    } else {
        return (-1);
    }

    return (pin);
}
