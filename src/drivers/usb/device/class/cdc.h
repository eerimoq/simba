/**
 * @file drivers/usb/device/class/cdc.h
 * @version 3.1.0
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
    struct usb_device_driver_base_t base;
    struct usb_device_driver_t *drv_p;
    int control_interface;
    int endpoint_in;
    int endpoint_out;
    int line_state;
    struct usb_cdc_line_info_t line_info;
    struct chan_t chout;
    struct queue_t chin;
};

/**
 * Initialize the CDC module.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_init(struct usb_device_class_cdc_driver_t *self_p,
                              int control_interface,
                              int endpoint_in,
                              int endpoint_out,
                              void *rxbuf_p,
                              size_t size);

/**
 * Read data from the CDC driver.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
#define usb_device_class_cdc_read(self_p, buf_p, size)  \
    queue_read(&(self_p)->chin, buf_p, size)

/**
 * Write data to the CDC driver.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
#define usb_device_class_cdc_write(self_p, buf_p, size)         \
    (self_p)->chout.write(&(self_p)->chout, buf_p, size)

/**
 * Called by the USB device driver periodically to let the CDC driver
 * read received data from the hardware.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_input_isr(struct usb_device_class_cdc_driver_t *self_p);

/**
 * Check if the CDC is connected to the remote endpoint.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return true(1) if connected, false(0) if disconnected, otherwise
 *         negative error code.
 */
int usb_device_class_cdc_is_connected(struct usb_device_class_cdc_driver_t *self_p);

#endif
