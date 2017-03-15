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

#ifndef __DRIVERS_USB_DEVICE_PORT_H__
#define __DRIVERS_USB_DEVICE_PORT_H__

#include "simba.h"

struct usb_device_driver_base_t;

typedef int (*usb_device_start_of_frame_cb_t)(struct usb_device_driver_base_t *self_p);
typedef int (*usb_device_setup_cb_t)(struct usb_device_driver_base_t *self_p,
                                     struct usb_setup_t *setup_p);
typedef int (*usb_device_print_cb_t)(struct usb_device_driver_base_t *self_p,
                                     void *chout_p);

struct usb_device_driver_base_t {
    struct usb_device_driver_base_t *next_p;
    usb_device_start_of_frame_cb_t start_of_frame_isr;
    usb_device_setup_cb_t setup_isr;
#if CONFIG_FS_CMD_USB_DEVICE_LIST == 1
    usb_device_print_cb_t print;
#endif
};

struct usb_device_driver_t {
    struct usb_device_t *dev_p;
    int configuration;
    struct usb_device_driver_base_t **drivers_pp;
    int drivers_max;
    FAR const union usb_descriptor_t **descriptors_pp;
};

#endif
