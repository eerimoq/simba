/**
 * @file drivers/flash.h
 * @version 0.1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#ifndef __DRIVERS_FLASH_H__
#define __DRIVERS_FLASH_H__

#include "simba.h"

#include "flash_port.h"

extern struct flash_device_t flash_device[FLASH_DEVICE_MAX];

/**
 * Initialize flash module.
 */
int flash_module_init(void);

/**
 * Initialize driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] dev_p Device to use.
 *
 * @return zero(0) or negative error code.
 */
int flash_init(struct flash_driver_t *self_p,
               struct flash_device_t *dev_p);

/**
 * Read data from the flash memory.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] dst_p Buffer to read into.
 * @param[in] src Address in flash memory to read from.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t flash_read(struct flash_driver_t *self_p,
                   void *dst_p,
                   size_t src,
                   size_t size);

/**
 * Write data to the flash memory.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] dst Address in flash memory to write to.
 * @param[in] src_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t flash_write(struct flash_driver_t *self_p,
                    size_t dst,
                    const void *src_p,
                    size_t size);

#endif
