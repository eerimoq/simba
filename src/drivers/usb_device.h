/**
 * @file drivers/usb_device.h
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

#ifndef __DRIVERS_USB_DEVICE_H__
#define __DRIVERS_USB_DEVICE_H__

#include "simba.h"

#include "usb_device_port.h"

/**
 * Initialize the USB device module.
 */
int usb_device_module_init(void);

/**
 * Initialize the USB device driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p USB device to use.
 * @param[in] drivers_pp An array of initialized drivers.
 * @param[in] drivers_max Length of the drivers array.
 * @param[in] descriptors_pp A NULL terminated array of USB
 *                           descriptors.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_init(struct usb_device_driver_t *self_p,
                    struct usb_device_t *dev_p,
                    struct usb_device_driver_base_t **drivers_pp,
                    int drivers_max,
                    FAR const union usb_descriptor_t **descriptors_pp);

/**
 * Start the USB device device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_start(struct usb_device_driver_t *self_p);

/**
 * Stop the USB device device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_stop(struct usb_device_driver_t *self_p);

/**
 * Write data to given endpoint.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] endpoint Endpoint to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t usb_device_write(struct usb_device_driver_t *self_p,
                         int endpoint,
                         const void *buf_p,
                         size_t size);

/**
 * Read data from given endpoint from an isr or with the system lock
 * taken.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] endpoint Endpoint to read data from.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t usb_device_read_isr(struct usb_device_driver_t *self_p,
                            int endpoint,
                            void *buf_p,
                            size_t size);

/**
 * Write data to given endpoint from an isr or with the system lock
 * taken.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] endpoint Endpoint to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t usb_device_write_isr(struct usb_device_driver_t *self_p,
                             int endpoint,
                             const void *buf_p,
                             size_t size);

#endif
