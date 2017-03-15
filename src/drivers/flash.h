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

#ifndef __DRIVERS_FLASH_H__
#define __DRIVERS_FLASH_H__

#include "simba.h"

#include "flash_port.h"

extern struct flash_device_t flash_device[FLASH_DEVICE_MAX];

/**
 * Initialize the flash module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int flash_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] dev_p Device to use.
 *
 * @return zero(0) or negative error code.
 */
int flash_init(struct flash_driver_t *self_p,
               struct flash_device_t *dev_p);

/**
 * Read data from given flash memory.
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
                   uintptr_t src,
                   size_t size);

/**
 * Write data to given flash memory.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] dst Address in flash memory to write to.
 * @param[in] src_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t flash_write(struct flash_driver_t *self_p,
                    uintptr_t dst,
                    const void *src_p,
                    size_t size);

/**
 * Erase all sectors part of given memory range.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] dst Address in flash memory to erase from.
 * @param[in] size Number of bytes to erase.
 *
 * @return zero(0) or negative error code.
 */
int flash_erase(struct flash_driver_t *self_p,
                uintptr_t addr,
                size_t size);

#endif
