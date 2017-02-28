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

#ifndef __DRIVERS_OWI_H__
#define __DRIVERS_OWI_H__

#include "simba.h"

/* One wire commands. */
#define OWI_SEARCH_ROM   0xf0
#define OWI_READ_ROM     0x33
#define OWI_MATCH_ROM    0x55
#define OWI_SKIP_ROM     0xcc
#define OWI_ALARM_SEARCH 0xec

struct owi_device_t {
    uint8_t id[8];
};

struct owi_driver_t {
    struct pin_driver_t pin;
    struct owi_device_t *devices_p;  /* Device identity list. */
    size_t nmemb;                    /* Maximum length of identity list. */
    size_t len;                      /* Number of devices in identity list. */
};

/**
 * Initialize driver object.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Pin device to use.
 * @param[in] devices_p Storage for devices found when searching.
 * @param[in] nmemb Number of members in devices.
 *
 * @return zero(0) or negative error code.
 */
int owi_init(struct owi_driver_t *self_p,
             struct pin_device_t *dev_p,
             struct owi_device_t *devices_p,
             size_t nmemb);

/**
 * Send reset on one wire bus.
 *
 * @param[in] self_p Driver object.
 *
 * @return true(1) if one or more devices are connected to the bus,
 *         false(0) if no devices were found,
 *         otherwise negative error code.
 */
int owi_reset(struct owi_driver_t *self_p);

/**
 * Search for devices on given one wire bus. The device id of all
 * found devices are stored in the devices array passed to
 * `owi_init()`.
 *
 * @param[in] self_p Driver object.
 *
 * @return Number of devices found or negative error code.
 */
int owi_search(struct owi_driver_t *self_p);

/**
 * Read into buffer from one wire bus.
 *
 * @param[in] self_p Driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bits to read.
 *
 * @return Number of bits read or negative error code.
 */
ssize_t owi_read(struct owi_driver_t *self_p,
                 void *buf_p,
                 size_t size);

/**
 * Write buffer to given one wire bus.
 *
 * @param[in] self_p Driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bits to write.
 *
 * @return Number of bits written or negative error code.
 */
ssize_t owi_write(struct owi_driver_t *self_p,
                  const void *buf_p,
                  size_t size);

#endif
