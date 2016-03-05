/**
 * @file bootloader.h
 * @version 0.2.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

#ifndef __BOOTLOADER_BOOTLOADER_H__
#define __BOOTLOADER_BOOTLOADER_H__

/* States. */
#define STATE_IDLE        1
#define STATE_SWDL        2

struct bootloader_t {
    int state;
    chan_t *chin_p;
    chan_t *chout_p;
    struct {
        uint32_t address;
        uint32_t size;
        uint32_t offset;
        int (*write)(struct bootloader_t *self_p,
                     size_t size);
    } swdl;
};

/**
 * Initialize the bootloader object.
 *
 * @param[in,out] self_p Bootloader object.
 * @param[in] chin_p Input channel.
 * @param[in] chout_p Output channel.
 *
 * @returns zero(0) or negative error code
 */
int bootloader_init(struct bootloader_t *self_p,
                    chan_t *chin_p,
                    chan_t *chout_p);

/**
 * Handle a service.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns zero(0) or negative error code.
 */
int bootloader_handle_service(struct bootloader_t *self_p);

/**
 * Run the main loop of the bootloader.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns Never returns.
 */
void bootloader_main(struct bootloader_t *self_p);

#endif
