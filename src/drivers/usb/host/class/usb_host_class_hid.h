/**
 * @file usb_host_class_hid.h
 * @version 0.2.0
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

#ifndef __DRIVERS_USB_HOST_CLASS_HID_H__
#define __DRIVERS_USB_HOST_CLASS_HID_H__

#include "simba.h"

/* Subclasses. */
#define USB_CLASS_HID_SUBCLASS_NONE            0
#define USB_CLASS_HID_SUBCLASS_BOOT_INTERFACE  1

/* Protocols. */
#define USB_CLASS_HID_PROTOCOL_NONE            0
#define USB_CLASS_HID_PROTOCOL_KEYBOARD        1
#define USB_CLASS_HID_PROTOCOL_MOUSE           2

struct usb_host_class_hid_device_t {
    uint8_t buf[1];
};

struct usb_host_class_hid_driver_t {
    struct usb_host_driver_t *usb_p;
    struct usb_host_class_hid_device_t *devices_p;
    size_t length;
    struct {
        size_t size;
    } report;
    struct usb_host_device_driver_t device_driver;
};

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] usb_p USB driver to use.
 * @param[in] devices_p Array of devices. One entry in this array is
                        allocated for each HID device that is connected to
                        the host.
 * @param[in] length Length of the devices array.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_class_hid_init(struct usb_host_class_hid_driver_t *self_p,
                            struct usb_host_driver_t *usb_p,
                            struct usb_host_class_hid_device_t *devices_p,
                            size_t length);

/**
 * Starts the HID driver.
 *
 * @param[in] self_p Initialized driver object to start.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_class_hid_start(struct usb_host_class_hid_driver_t *self_p);

/**
 * Stops the HID driver.
 *
 * @param[in] self_p Initialized to stop.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_class_hid_stop(struct usb_host_class_hid_driver_t *self_p);

#endif
