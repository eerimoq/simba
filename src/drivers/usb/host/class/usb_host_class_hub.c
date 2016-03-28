/**
 * @file usb_class_hub.c
 * @version 0.3.0
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

int usb_class_hub_enumerate(struct usb_host_device_t *device_p)
{
    /* Setup the interrupt endpoint. */

    return (0);
}

ssize_t usb_class_hub_read(struct usb_class_hub_t *hub_p,
                           uint8_t *report_p,
                           size_t size)
{
    /* Get an interrupt from the HUB. */

    /* Use GET_STATUS to get the status of all USB hub ports. The status
     * PORT_CONNECTION means a device has been attached. */

    /* CLEAR_FEATURE C_PORT_CONNECTION on the port with the connection. */

    /* Wait for an interrupt that tells us what?? */

    /* Use SET_FEATURE to set the device reset on the device connected
     * to a USB hub port. */

    /* Use CLEAR_FEATURE to clear the device reset. */

    return (-1);
}
