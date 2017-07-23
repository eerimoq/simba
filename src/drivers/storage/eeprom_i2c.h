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

#ifndef __DRIVERS_EEPROM_I2C_H__
#define __DRIVERS_EEPROM_I2C_H__

#include "simba.h"

struct eeprom_i2c_driver_t {
    struct i2c_driver_t *i2c_p;
    int i2c_address;
    uint32_t size;
};

/**
 * Initialize EEPROM I2C module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int eeprom_i2c_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] i2c_p Initialized and started I2C driver object.
 * @param[in] i2c_address The I2C address of the EEPROM.
 * @param[in] size Size of the EEPROM in bytes.
 *
 * @return zero(0) or negative error code.
 */
int eeprom_i2c_init(struct eeprom_i2c_driver_t *self_p,
                    struct i2c_driver_t *i2c_p,
                    int i2c_address,
                    uint32_t size);

/**
 * Read into given buffer from given EEPROM address.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] dst_p Buffer to read into.
 * @param[in] src EEPROM address to read from.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t eeprom_i2c_read(struct eeprom_i2c_driver_t *self_p,
                        void *dst_p,
                        uint32_t src,
                        size_t size);

/**
 * Write given buffer to given EEPROM address.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] dst EEPROM address to write to.
 * @param[in] src_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t eeprom_i2c_write(struct eeprom_i2c_driver_t *self_p,
                         uint32_t dst,
                         const void *src_p,
                         size_t size);

#endif
