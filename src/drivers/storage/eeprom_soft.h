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

#ifndef __DRIVERS_EEPROM_SOFT_H__
#define __DRIVERS_EEPROM_SOFT_H__

#include "simba.h"

struct eeprom_soft_block_t {
    uintptr_t address;
    size_t size;
};

struct eeprom_soft_driver_t {
    struct flash_driver_t *flash_p;
    const struct eeprom_soft_block_t *blocks_p;
    int number_of_blocks;
    size_t chunk_size;
    size_t eeprom_size;
    struct {
        const struct eeprom_soft_block_t *block_p;
        uintptr_t chunk_address;
        uint16_t revision;
    } current;
#if CONFIG_EEPROM_SOFT_SEMAPHORE == 1
    struct sem_t sem;
#endif
};

/**
 * Initialize software EEPROM module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int eeprom_soft_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] flash_p Flash driver.
 * @param[in] blocks_p Flash memory blocks to use.
 * @param[in] number_of_blocks Number of blocks.
 * @param[in] chunk_size Chunk size in bytes. This is the size of the
 *                       EEPROM. Eight bytes of the chunk will be used
 *                       to store metadata, so only `chunk_size - 8`
 *                       bytes are available to the user.
 *
 * @return zero(0) or negative error code.
 */
int eeprom_soft_init(struct eeprom_soft_driver_t *self_p,
                     struct flash_driver_t *flash_p,
                     const struct eeprom_soft_block_t *blocks_p,
                     int number_of_blocks,
                     size_t chunk_size);

/**
 * Mount given software EEPROM.
 *
 * @param[in] self_p Driver object to mount.
 *
 * @return zero(0) or negative error code.
 */
int eeprom_soft_mount(struct eeprom_soft_driver_t *self_p);

/**
 * Format given software EEPROM.
 *
 * @param[in] self_p Driver object to format.
 *
 * @return zero(0) or negative error code.
 */
int eeprom_soft_format(struct eeprom_soft_driver_t *self_p);

/**
 * Read into given buffer from given address.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] dst_p Buffer to read into.
 * @param[in] src Software EEPROM address to read from. Addressing
 *                starts at zero(0).
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t eeprom_soft_read(struct eeprom_soft_driver_t *self_p,
                         void *dst_p,
                         uintptr_t src,
                         size_t size);

/**
 * Write given buffer to given address.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] dst Software EEPROM address to write to. Addressing
 *                starts at zero(0).
 * @param[in] src_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t eeprom_soft_write(struct eeprom_soft_driver_t *self_p,
                          uintptr_t dst,
                          const void *src_p,
                          size_t size);

#endif
