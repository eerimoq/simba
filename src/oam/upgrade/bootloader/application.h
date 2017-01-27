/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __OAM_UPGRADE_BOOTLOADER_APPLICATION_H__
#define __OAM_UPGRADE_BOOTLOADER_APPLICATION_H__

#include "simba.h"

/**
 * Initialize the application module.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_application_module_init(void);

/**
 * Erase the application area.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_application_erase(void);

/**
 *  Begin an application write transaction.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_application_write_begin(void);

/**
 * Write a chunk of data in an application write transaction.
 *
 * @param[in] buf_p Buffer to write.
 * @param[in] size Size of the buffer.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_application_write_chunk(const void *buf_p,
                                               size_t size);

/**
 * End an application write transaction.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_application_write_end(void);

/**
 * Returns true(1) if there is a valid application in the application
 * area.
 *
 * @return true(1) if a valid application exists in the memory
 *          region, otherwise false(0).
 */
int upgrade_bootloader_application_is_valid(void);

/**
 * Jump to the application.
 */
void upgrade_bootloader_application_jump(void);

#endif
