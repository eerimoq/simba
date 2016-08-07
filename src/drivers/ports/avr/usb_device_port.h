/**
 * @file drivers/ports/avr/usb_device_port.h
 * @version 5.0.0
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

#ifndef __DRIVERS_USB_DEVICE_PORT_H__
#define __DRIVERS_USB_DEVICE_PORT_H__

#include "simba.h"

struct usb_device_driver_base_t;

typedef int (*usb_device_start_of_frame_cb_t)(struct usb_device_driver_base_t *self_p);
typedef int (*usb_device_setup_cb_t)(struct usb_device_driver_base_t *self_p,
                                     struct usb_setup_t *setup_p);
typedef int (*usb_device_print_cb_t)(struct usb_device_driver_base_t *self_p,
                                     chan_t *chout_p);

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
