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

/**
 * @return true(1) if the device class is HID, otherwise false(0)
 */
static int supports(struct usb_host_device_t *device_p)
{
    int class;

    class = usb_desc_get_class(device_p->descriptors.buf,
                               device_p->descriptors.size,
                               1,
                               0);

    return (class == USB_CLASS_HID);
}

/**
 * HID-specific device enumeration.
 *
 * @return zero(0) or negative error code.
 */
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

int usb_host_class_hid_init(struct usb_host_class_hid_driver_t *self_p,
                            struct usb_host_driver_t *usb_p,
                            struct usb_host_class_hid_device_t *devices_p,
                            size_t length)
{
    self_p->usb_p = usb_p;
    self_p->devices_p = devices_p;
    self_p->length = length;
    self_p->device_driver.supports = supports;
    self_p->device_driver.enumerate = enumerate;

    return (0);
}

int usb_host_class_hid_start(struct usb_host_class_hid_driver_t *self_p)
{
    return (usb_host_driver_add(self_p->usb_p,
                                &self_p->device_driver,
                                self_p));
}

int usb_host_class_hid_stop(struct usb_host_class_hid_driver_t *self_p)
{
    return (usb_host_driver_remove(self_p->usb_p,
                                   &self_p->device_driver));
}

#endif
