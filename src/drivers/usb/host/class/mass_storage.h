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

#ifndef __DRIVERS_USB_HOST_CLASS_MASS_STORAGE_H__
#define __DRIVERS_USB_HOST_CLASS_MASS_STORAGE_H__

#include "simba.h"

struct usb_host_class_mass_storage_device_t {
    uint8_t buf[1];
};

struct usb_host_class_mass_storage_driver_t {
    struct usb_host_driver_t *usb_p;
    struct usb_host_class_mass_storage_device_t *devices_p;
    size_t length;
    struct {
        size_t size;
    } report;
    struct usb_host_device_driver_t device_driver;
};

int usb_host_class_mass_storage_init(struct usb_host_class_mass_storage_driver_t *self_p,
                                     struct usb_host_driver_t *usb_p,
                                     struct usb_host_class_mass_storage_device_t *devices_p,
                                     size_t length);

int usb_host_class_mass_storage_start(struct usb_host_class_mass_storage_driver_t *self_p);

int usb_host_class_mass_storage_stop(struct usb_host_class_mass_storage_driver_t *self_p);

ssize_t usb_host_class_mass_storage_device_read(
    struct usb_host_device_t *device_p,
    void *buf_p,
    size_t address,
    size_t size);

#endif
