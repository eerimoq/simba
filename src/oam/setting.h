/**
 * @file oam/setting.h
 * @version 5.0.0
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

#ifndef __OAM_SETTING_H__
#define __OAM_SETTING_H__

#include "simba.h"

#define SETTING_AREA_CRC_OFFSET (CONFIG_SETTING_AREA_SIZE - 4)

/**
 * Setting types. Each setting must have be one of these types.
 */
enum setting_type_t {
    setting_type_int8_t = 0,
    setting_type_int16_t,
    setting_type_int32_t,
    setting_type_string_t
};

struct setting_t {
    FAR const char *name_p;
    enum setting_type_t type;
    uint32_t address;
    size_t size;
};

#include "setting_port.h"

/**
 * Initialize the setting module.
 *
 * @return zero(0) or negative error code.
 */
int setting_module_init(void);

/**
 * Read the value of given setting by address.
 *
 * @param[out] dst_p The read value.
 * @param[in] src Setting source address.
 * @param[in] size Number of words to read.
 *
 * @return Number of words read or negative error code.
 */
ssize_t setting_read(void *dst_p, size_t src, size_t size);

/**
 * Write given value to given setting by address.
 *
 * @param[in] dst Destination setting address.
 * @param[in] src_p Value to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of words written or negative error code.
 */
ssize_t setting_write(size_t dst, const void *src_p, size_t size);

/**
 * Read the value of given setting by name.
 *
 * @param[in] name_p Setting name.
 * @param[out] dst_p The read value.
 * @param[in] size Size of the destination buffer.
 *
 * @return Number of words read or negative error code.
 */
ssize_t setting_read_by_name(const char *name_p,
                             void *dst_p,
                             size_t size);

/**
 * Write given value to given setting by name.
 *
 * @param[in] name_p Setting name.
 * @param[in] src_p Value to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of words read or negative error code.
 */
ssize_t setting_write_by_name(const char *name_p,
                              const void *src_p,
                              size_t size);

/**
 * Overwrite all settings with their default values.
 *
 * @return zero(0) or negative error code.
 */
int setting_reset(void);

#endif
