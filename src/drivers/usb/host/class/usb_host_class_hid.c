/**
 * @file usb_host_class_hid.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#include "simba.h"

static int supports(struct usb_host_device_t *device_p)
{
    int class;

    class = usb_desc_get_class(device_p->descriptors.buf,
                               device_p->descriptors.size,
                               1,
                               0);

    return (class == USB_CLASS_HID);
}

static int enumerate(struct usb_host_device_t *device_p)
{
    struct usb_descriptor_interface_t *int_desc_p;

    std_printf(FSTR("Enumerating a HID device.\r\n"));

    int_desc_p = usb_desc_get_interface(device_p->descriptors.buf,
                                        device_p->descriptors.size,
                                        1,
                                        0);

    if (int_desc_p == NULL) {
        return (-1);
    }

    return (usb_host_device_set_configuration(device_p, 1));
}

int usb_host_class_hid_init(struct usb_host_class_hid_driver_t *drv_p,
                            struct usb_host_driver_t *usb_p,
                            struct usb_host_class_hid_device_t *devices_p,
                            size_t length)
{
    drv_p->usb_p = usb_p;
    drv_p->devices_p = devices_p;
    drv_p->length = length;
    drv_p->device_driver.supports = supports;
    drv_p->device_driver.enumerate = enumerate;

    return (0);
}

int usb_host_class_hid_start(struct usb_host_class_hid_driver_t *drv_p)
{
    return (usb_host_driver_add(drv_p->usb_p,
                                &drv_p->device_driver,
                                drv_p));
}

int usb_host_class_hid_stop(struct usb_host_class_hid_driver_t *drv_p)
{
    return (-1);
}
