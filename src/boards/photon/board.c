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

extern uint32_t __rom_begin;
extern uint32_t __text_end__;

/**
 * This datastructure is used ny the Photon bootloader to verify that
 * the application is valid. It must be reallocated to application
 * start address + 0x184 (see script.common.ld).
 */
struct module_info_t {
    const void* module_start_address; /* the first byte of this module
                                         in flash */
    const void* module_end_address;   /* the last byte (exclusive) of
                                         this smodule in flash. 4 byte
                                         crc starts here. */
    uint8_t reserved;
    uint8_t reserved2;
    uint16_t module_version;          /* 16 bit version */
    uint16_t platform_id;             /* The platform this module was
                                         compiled for. */
    uint8_t  module_function;         /* The module function */
    uint8_t  module_index;
};

__attribute__ ((section(".module_info"), used))
struct module_info_t module_info = {
    .module_start_address = (const void *)&__rom_begin,
    .module_end_address = (const void *)&__text_end__,
    .platform_id = 6,
};

int board_pin_string_to_device_index(const char *str_p)
{
    int gpio;
    long bit;

    /* Get gpio and bit. */
    if (str_p[0] != 'p') {
        return (-1);
    }

    if ((str_p[1] < 'a') || (str_p[1] > 'd')) {
        return (-1);
    }

    gpio = (str_p[1] - 'a');

    if (std_strtol(&str_p[2], &bit) == NULL) {
        return (-1);
    }

    if (bit < 0) {
        return (-1);
    }

    if ((gpio == 'd' && bit > 3) || (bit > 15)) {
        return (-1);
    }

    return (16 * gpio + bit);
}
