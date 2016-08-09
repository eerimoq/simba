/**
 * @file drivers/ports/sam/usb_host_port.h
 * @version 6.0.0
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

#ifndef __DRIVERS_USB_HOST_PORT_H__
#define __DRIVERS_USB_HOST_PORT_H__

#include "simba.h"

struct usb_host_driver_t {
    int id;                               /* The driver id. */
    struct usb_device_t *dev_p;
    size_t length;
    struct usb_host_device_t *devices_p;
    struct queue_t control;
    struct event_t event;
    struct usb_host_driver_t *next_p;
    THRD_STACK(stack, 1024);
};

#endif
