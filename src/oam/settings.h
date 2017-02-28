/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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

#include "settings_port.h"

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
