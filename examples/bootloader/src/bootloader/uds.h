/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __BOOTLOADER_UDS_H__
#define __BOOTLOADER_UDS_H__

#include "simba.h"

/* States. */
#define STATE_IDLE        1
#define STATE_SWDL        2

struct bootloader_uds_t {
    int state;
    void *chin_p;
    void *chout_p;
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
int bootloader_uds_init(struct bootloader_uds_t *self_p,
                        void *chin_p,
                        void *chout_p,
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
int bootloader_uds_handle_service(struct bootloader_uds_t *self_p);

/**
 * Run the main loop of the bootloader.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns Never returns.
 */
void bootloader_uds_main(struct bootloader_uds_t *self_p);

#endif
