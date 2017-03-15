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

int upgrade_module_init()
{
    return (-1);
}

int upgrade_bootloader_enter()
{
    static int count = 0;

    count++;

    switch (count) {
    case 1: return (0);
    case 2: return (-1);
    default: return (-1);
    }
}

int upgrade_bootloader_stay_set()
{
    return (-1);
}

int upgrade_bootloader_stay_clear()
{
    return (-1);
}

int upgrade_bootloader_stay_get()
{
    return (-1);
}

int upgrade_application_enter()
{
    static int count = 0;

    count++;

    switch (count) {
    case 1: return (0);
    case 2: return (-1);
    default: return (-1);
    }
}

int upgrade_application_erase()
{
    return (-1);
}

int upgrade_application_is_valid(int quick)
{
    return (-1);
}

int upgrade_binary_upload_begin()
{
    return (-1);
}

int upgrade_binary_upload(const void *buf_p,
                          size_t size)
{
    return (-1);
}

int upgrade_binary_upload_end()
{
    return (-1);
}
