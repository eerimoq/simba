/**
 * @file uds.h
 * @version 0.6.0
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

#ifndef __BOOTLOADER_UDS_H__
#define __BOOTLOADER_UDS_H__

#include "simba.h"

/* States. */
#define STATE_IDLE        1
#define STATE_SWDL        2

struct uds_t {
    int state;
    chan_t *chin_p;
    chan_t *chout_p;
    uint32_t application_address;
    uint32_t application_size;
    struct flash_driver_t *flash_p;
    struct {
        uint32_t address;
        uint32_t size;
        uint32_t offset;
        uint8_t next_block_sequence_counter;
    } swdl;
};

/**
 * Initialize the bootloader object.
 *
 * @param[in,out] self_p Bootloader object.
 * @param[in] chin_p Input channel.
 * @param[in] chout_p Output channel.
 * @param[in] application_address Start address of the application
 *                                flash area and the application entry
 *                                address.
 * @param[in] application_size Size of the application flash area. The
 *                             maximum appliction size is this size
 *                             minus one, because the last byte is
 *                             used as application valid flag.
 * @param[in] flash_p Flash memory to store the application in.
 *
 * @returns zero(0) or negative error code.
 */
int uds_init(struct uds_t *self_p,
             chan_t *chin_p,
             chan_t *chout_p,
             uint32_t application_address,
             uint32_t application_size,
             struct flash_driver_t *flash_p);

/**
 * Handle a service.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns zero(0) or negative error code.
 */
int uds_handle_service(struct uds_t *self_p);

/**
 * Run the main loop of the bootloader.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns Never returns.
 */
void uds_main(struct uds_t *self_p);

#endif
