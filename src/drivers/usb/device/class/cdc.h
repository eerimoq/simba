/**
 * @file drivers/usb/device/class/cdc.h
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

#ifndef __DRIVERS_USB_DEVICE_CLASS_CDC_H__
#define __DRIVERS_USB_DEVICE_CLASS_CDC_H__

#include "simba.h"

struct usb_device_class_cdc_driver_t {
};

struct usb_device_class_cdc_device_t {
};

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_init(struct usb_device_class_cdc_driver_t *self_p,
                              struct usb_device_class_cdc_device_t *dev_p,
                              void *rxbuf_p,
                              size_t size);

/**
 * Starts the CDC device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_start(struct usb_device_class_cdc_driver_t *self_p);

/**
 * Stops the CDC device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_stop(struct usb_device_class_cdc_driver_t *self_p);

/**
 * Read data from the CDC.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of bytes read or negative error code.
 */
#define usb_device_class_cdc_read(self_p, buf_p, size)  \
    queue_read(&(self_p)->chin, buf_p, size)

/**
 * Write data to the CDC.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
#define usb_device_class_cdc_write(self_p, buf_p, size)         \
    (self_p)->chout.write(&(self_p)->chout, buf_p, size)

#endif
