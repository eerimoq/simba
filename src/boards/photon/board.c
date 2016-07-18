/**
 * @file board.c
 * @version 2.0.0
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
