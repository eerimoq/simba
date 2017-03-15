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

#ifndef __DRIVERS_USB_DEVICE_H__
#define __DRIVERS_USB_DEVICE_H__

#include "simba.h"

#include "usb_device_port.h"

/**
 * Initialize the USB device module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
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
