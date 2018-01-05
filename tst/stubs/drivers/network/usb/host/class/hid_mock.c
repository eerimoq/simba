/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "hid_mock.h"

int mock_write_usb_host_class_hid_init(struct usb_host_driver_t *usb_p,
                                       struct usb_host_class_hid_device_t *devices_p,
                                       size_t length,
                                       int res)
{
    harness_mock_write("usb_host_class_hid_init(usb_p)",
                       usb_p,
                       sizeof(*usb_p));

    harness_mock_write("usb_host_class_hid_init(devices_p)",
                       devices_p,
                       sizeof(*devices_p));

    harness_mock_write("usb_host_class_hid_init(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("usb_host_class_hid_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_class_hid_init)(struct usb_host_class_hid_driver_t *self_p,
                                                         struct usb_host_driver_t *usb_p,
                                                         struct usb_host_class_hid_device_t *devices_p,
                                                         size_t length)
{
    int res;

    harness_mock_assert("usb_host_class_hid_init(usb_p)",
                        usb_p,
                        sizeof(*usb_p));

    harness_mock_assert("usb_host_class_hid_init(devices_p)",
                        devices_p,
                        sizeof(*devices_p));

    harness_mock_assert("usb_host_class_hid_init(length)",
                        &length,
                        sizeof(length));

    harness_mock_read("usb_host_class_hid_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_class_hid_start(int res)
{
    harness_mock_write("usb_host_class_hid_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_class_hid_start)(struct usb_host_class_hid_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_host_class_hid_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_class_hid_stop(int res)
{
    harness_mock_write("usb_host_class_hid_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_class_hid_stop)(struct usb_host_class_hid_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_host_class_hid_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
