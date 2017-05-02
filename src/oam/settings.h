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

#ifndef __OAM_SETTINGS_H__
#define __OAM_SETTINGS_H__

#include "simba.h"

#define SETTINGS_AREA_CRC_OFFSET (CONFIG_SETTINGS_AREA_SIZE - 4)

/**
 * Settings types. Each setting must have be one of these types.
 */
enum setting_type_t {
    setting_type_int32_t = 0,
    setting_type_string_t,
    setting_type_blob_t
};

struct setting_t {
    FAR const char *name_p;
    enum setting_type_t type;
    uint32_t address;
    size_t size;
};

/**
 * Initialize the settings module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int settings_module_init(void);

/**
 * Read the value of given setting by address.
 *
 * @param[out] dst_p The read value.
 * @param[in] src Setting source address.
 * @param[in] size Number of words to read.
 *
 * @return Number of words read or negative error code.
 */
ssize_t settings_read(void *dst_p, size_t src, size_t size);

/**
 * Write given value to given setting by address.
 *
 * @param[in] dst Destination setting address.
 * @param[in] src_p Value to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of words written or negative error code.
 */
ssize_t settings_write(size_t dst, const void *src_p, size_t size);

/**
 * Read the value of given setting by name.
 *
 * @param[in] name_p Setting name.
 * @param[out] dst_p The read value.
 * @param[in] size Size of the destination buffer.
 *
 * @return Number of words read or negative error code.
 */
ssize_t settings_read_by_name(const char *name_p,
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
ssize_t settings_write_by_name(const char *name_p,
                               const void *src_p,
                               size_t size);

/**
 * Overwrite all settings with their default values.
 *
 * @return zero(0) or negative error code.
 */
int settings_reset(void);

#endif
