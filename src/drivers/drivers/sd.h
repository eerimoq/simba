/**
 * @file drivers/sd.h
 * @version 1.0
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

#ifndef __DRIVERS_SD_H__
#define __DRIVERS_SD_H__

#include "simba.h"

#include "sd_port.h"

/**
 * Initialize driver object.
 *
 * @param[in,out] drv_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int sd_init(struct sd_driver_t *drv_p);

/**
 * Read given block from SD card.
 *
 * @param[in] drv_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] src_block Block to read from.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t sd_read(struct sd_driver_t *drv_p,
                void *dst_p,
                uint32_t src_block);

/**
 * Write data to the SD bus.
 *
 * @param[in] drv_p Initialized driver object.
 * @param[in] dst_block Block to write to.
 * @param[in] src_p Buffer to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t sd_write(struct sd_driver_t *drv_p,
                 uint32_t dst_block,
                 const void *src_p);

#endif
