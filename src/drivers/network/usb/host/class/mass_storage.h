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
