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

#include "simba.h"

#if CONFIG_USB_HOST == 1

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

#endif
