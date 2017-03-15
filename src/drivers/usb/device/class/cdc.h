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
