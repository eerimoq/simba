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

#include "simba.h"

#if CONFIG_EEPROM_I2C == 1

/**
 * Try to write given buffer to the EEPROM.
 */
static ssize_t try_write(struct eeprom_i2c_driver_t *self_p,
                         uint8_t *buf_p,
                         size_t size)
{
    int attempt;
    ssize_t res;

    attempt = 0;

    while (attempt < CONFIG_EEPROM_I2C_NUMBER_OF_ATTEMPTS) {
        res = i2c_write(self_p->i2c_p,
                        self_p->i2c_address,
                        buf_p,
                        size);

        if (res == size) {
            break;
        }

        attempt++;
    }

    return (res);
}

int eeprom_i2c_module_init()
{
    return (0);
}

int eeprom_i2c_init(struct eeprom_i2c_driver_t *self_p,
                    struct i2c_driver_t *i2c_p,
                    int i2c_address,
                    uint32_t size)
{
    self_p->i2c_p = i2c_p;
    self_p->i2c_address = i2c_address;
    self_p->size = size;

    return (0);
}

ssize_t eeprom_i2c_read(struct eeprom_i2c_driver_t *self_p,
                        void *dst_p,
                        uint32_t src,
                        size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dst_p != NULL, EINVAL);

    ssize_t res;
    uint8_t buf[2];

    if (src >= self_p->size) {
        return (-EINVAL);
    }

    if ((src + size) > self_p->size) {
        return (-EINVAL);
    }

    /* Write the address to read from. */
    buf[0] = (uint8_t)(src >> 8);
    buf[1] = (uint8_t)src;

    res = try_write(self_p, &buf[0], sizeof(buf));

    if (res != sizeof(buf)) {
        return (res);
    }

    /* Read data from the EEPROM. */
    return (i2c_read(self_p->i2c_p,
                     self_p->i2c_address,
                     dst_p,
                     size));
}

ssize_t eeprom_i2c_write(struct eeprom_i2c_driver_t *self_p,
                         uint32_t dst,
                         const void *src_p,
                         size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    size_t i;
    ssize_t res;
    const uint8_t *u8_src_p;
    uint8_t buf[3];

    if (dst >= self_p->size) {
        return (-EINVAL);
    }

    if ((dst + size) > self_p->size) {
        return (-EINVAL);
    }

    u8_src_p = src_p;

    /* Write one byte at a time. No page write support for now as it
       makes the implementation more complicated. */
    for (i = 0; i < size; i++, dst++) {
        buf[0] = (uint8_t)(dst >> 8);
        buf[1] = (uint8_t)dst;
        buf[2] = u8_src_p[i];

        res = try_write(self_p, &buf[0], sizeof(buf));

        if (res != sizeof(buf)) {
            return (res);
        }
    }

    return (size);
}

#endif
