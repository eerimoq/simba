/**
 * @file usb_device.c
 * @version 2.0.0
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

#include "simba.h"

#include "usb_device_port.i"

int usb_device_module_init(void)
{
    return (0);
}

int usb_device_init(struct usb_device_driver_t *self_p,
                    struct usb_device_t *dev_p)
{
    return (-1);
}

int usb_device_start(struct usb_device_driver_t *self_p)
{
    return (0);
}

int usb_device_stop(struct usb_device_driver_t *self_p)
{
    return (0);
}
