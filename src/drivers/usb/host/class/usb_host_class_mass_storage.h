/**
 * @file usb_host_class_mass_storage.h
 * @version 0.6.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
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
