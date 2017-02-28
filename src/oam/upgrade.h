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

#ifndef __OAM_UPGRADE_H__
#define __OAM_UPGRADE_H__

#include "simba.h"

/**
 * Initialize the upgrade module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_module_init(void);

/**
 * Enter the bootloader. This function does not return if all
 * preconditions for entering the bootloader are met.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_enter(void);

/**
 * Stay in the bootloader after next system reboot.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_stay_set(void);

/**
 * Do not stay in the bootloader after next system reboot.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_bootloader_stay_clear(void);

/**
 * Check if the bootlaoder is forced to enter its main loop instead of
 * calling any valid application.
 *
 * @return true(1) if the bootloder shall not call the application,
 *         otherwise false(0).
 */
int upgrade_bootloader_stay_get(void);

/**
 * Enter the application. This function does not return if all
 * preconditions for entering the application are met.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_application_enter(void);

/**
 * Erase the application area.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_application_erase(void);

/**
 * Returns true(1) if there is a valid application in the application
 * area.
 *
 * @param[in] quick Perform a quick validation. The quick validation
 *                  is port specific, while the non-quick validation
 *                  always calculates a checksum of the application
 *                  and compares it to the expected checksum.
 *
 * @return true(1) if a valid application exists in the memory region,
 *         otherwise false(0).
 */
int upgrade_application_is_valid(int quick);

/**
 * Begin an upload transaction of a .ubin file.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_binary_upload_begin(void);

/**
 * Add data to current upload transaction.
 *
 * @param[in] buf_p Buffer to write.
 * @param[in] size Size of the buffer.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_binary_upload(const void *buf_p,
                          size_t size);

/**
 * End current upload transaction.
 *
 * @return zero(0) or negative error code.
 */
int upgrade_binary_upload_end(void);

#endif
