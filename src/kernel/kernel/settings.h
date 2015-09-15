/**
 * @file kernel/settings.h
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

#ifndef __KERNEL_SETTINGS_H__
#define __KERNEL_SETTINGS_H__

#include "simba.h"
#include "settings_port.h"

/* Base address for non-kernel serttings. */
#define SETTINGS_BASE_ADDR 0x100

/**
 * Initialize settings module.
 * @return zero(0) or negative error code.
 */
int settings_module_init(void);

/**
 * Read setting at address to buffer.
 * @param[in,out] dst_p Destination buffer.
 * @param[in] src Setting source address.
 * @param[in] size Number of words to read.
 * @return zero(0) or negative error code.
 */
int settings_read(void *dst_p, size_t src, size_t size);

/**
 * Write setting from buffer to address.
 * @param[in] dst Destination setting address.
 * @param[in] src_p Source buffer.
 * @param[in] size Number of words to write.
 * @return zero(0) or negative error code.
 */
int settings_write(size_t dst, void *src_p, size_t size);

#endif
