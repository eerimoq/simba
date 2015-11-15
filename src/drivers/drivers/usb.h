/**
 * @file drivers/usb.h
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

#ifndef __DRIVERS_USB_H__
#define __DRIVERS_USB_H__

#include "simba.h"

#include "usb_port.h"

extern struct usb_device_t usb_device[USB_DEVICE_MAX];

/**
 * Initialize USB module.
 */
int usb_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] drv_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] baudrate Baudrate.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 *
 * @return zero(0) or negative error code.
 */
int usb_init(struct usb_driver_t *drv_p,
             struct usb_device_t *dev_p);

/**
 * Starts the USB device using given driver object.
 *
 * @param[in] drv_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_start(struct usb_driver_t *drv_p);

/**
 * Stops the USB device referenced by driver object.
 *
 * @param[in] drv_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_stop(struct usb_driver_t *drv_p);

ssize_t usb_read(struct usb_driver_t *drv_p);

ssize_t usb_write(struct usb_driver_t *drv_p);

#endif
