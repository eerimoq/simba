/**
 * @file usb.c
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

#include "usb_port.i"

/**
 * Enumerate a detected device. After the enumeration the device has
 * been configured and is ready to use by the application.
 */
int usb_enumerate()
{
    std_printf(FSTR("Starting the USB enumeration process for a device.\r\n"));

    /* Reset the device. */

    /* Assign the device an address. */
    set_address(1);

    /* Get various descriptors. */
    get_descriptor(DESCRIPTOR_DEVICE, 1);
    get_descriptor(DESCRIPTOR_CONFIGURATION, 9);

    if (length > 9) {
        get_descriptor(DESCRIPTOR_CONFIGURATION, length);
    }

    get_descriptor(STRING, 0, 255);
    get_descriptor(STRING, 1, 255);
    get_descriptor(STRING, 2, 255);

    /* Set the configuration. */
    get_descriptor(DESCRIPTOR_CONFIGURATION, SET, 9);

    /* Call the driver for the device class. */
    switch (device_p->class) {

    case CLASS_HID:
        usb_class_hid_enumerate(device_p);
        break;
        
    default:
        std_printf(FSTR("Unsupported USB device class %d.\r\n"),
                   device_p->class);
        break;
    }
    
    return (0);
}

int usb_module_init(void)
{
    return (0);
}

int usb_init(struct usb_driver_t *drv_p,
             struct usb_device_t *dev_p)
{
    return (usb_port_init(drv_p, dev_p));
}

int usb_start(struct usb_driver_t *drv_p)
{
    return (usb_port_start(drv_p));
}

int usb_stop(struct usb_driver_t *drv_p)
{
    return (usb_port_stop(drv_p));
}
