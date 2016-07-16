/**
 * @file storage/setting.h
 * @version 1.0.0
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

#ifndef __STORAGE_SETTING_H__
#define __STORAGE_SETTING_H__

#include "simba.h"
#include "setting_port.h"

/**
 * Initialize the setting module.
 *
 * @return zero(0) or negative error code.
 */
int setting_module_init(void);

/**
 * Read setting at given address to given buffer.
 *
 * @param[in,out] dst_p Destination buffer.
 * @param[in] src Setting source address.
 * @param[in] size Number of words to read.
 *
 * @return Number of words read or negative error code.
 */
ssize_t setting_read(void *dst_p, size_t src, size_t size);

/**
 * Write setting from given buffer to given address.
 *
 * @param[in] dst Destination setting address.
 * @param[in] src_p Source buffer.
 * @param[in] size Number of words to write.
 *
 * @return Number of words written or negative error code.
 */
ssize_t setting_write(size_t dst, const void *src_p, size_t size);

#endif
