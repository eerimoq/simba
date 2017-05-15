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

#ifndef __OAM_NVM_H__
#define __OAM_NVM_H__

#include "simba.h"

/**
 * Initialize the NVM module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int nvm_module_init(void);

/**
 * Mount the non-volatile memory.
 *
 * @return zero(0) if the memory was successfully mounted, otherwise
 *         negative error code.
 */
int nvm_mount(void);

/**
 * Format the non-volatile memory, writing 0xff/erasing to the whole
 * memory. A formatted NVM can always be mounted with `nvm_mount()`.
 *
 * @return zero(0) or negative error code.
 */
int nvm_format(void);

/**
 * Read into given buffer from given NVM address.
 *
 * @param[out] dst_p Buffer to read into.
 * @param[in] src Address in NVM to read from. Addressing starts at
 *                zero(0).
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t nvm_read(void *dst_p, uint32_t src, size_t size);

/**
 * Write given buffer to given NVM address.
 *
 * @param[in] dst Address in NVM to write to. Addressing starts at
 *                zero(0).
 * @param[in] src_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t nvm_write(uint32_t dst, const void *src_p, size_t size);

#endif
