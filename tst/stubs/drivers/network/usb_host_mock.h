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

#ifndef __USB_HOST_MOCK_H__
#define __USB_HOST_MOCK_H__

#include "simba.h"

int mock_write_usb_host_module_init(int res);

int mock_write_usb_host_init(struct usb_device_t *dev_p,
                             struct usb_host_device_t *devices_p,
                             size_t length,
                             int res);

int mock_write_usb_host_start(int res);

int mock_write_usb_host_stop(int res);

int mock_write_usb_host_driver_add(struct usb_host_device_driver_t *driver_p,
                                   void *arg_p,
                                   int res);

int mock_write_usb_host_driver_remove(struct usb_host_device_driver_t *driver_p,
                                      int res);

int mock_write_usb_host_device_open(int device,
                                    struct usb_host_device_t *
res);

int mock_write_usb_host_device_close(int device,
                                     int res);

int mock_write_usb_host_device_read(struct usb_host_device_t *device_p,
                                    int endpoint,
                                    void *buf_p,
                                    size_t size,
                                    ssize_t res);

int mock_write_usb_host_device_write(struct usb_host_device_t *device_p,
                                     int endpoint,
                                     const void *buf_p,
                                     size_t size,
                                     ssize_t res);

int mock_write_usb_host_device_control_transfer(struct usb_host_device_t *device_p,
                                                struct usb_setup_t *setup_p,
                                                void *buf_p,
                                                size_t size,
                                                ssize_t res);

int mock_write_usb_host_device_set_configuration(struct usb_host_device_t *device_p,
                                                 uint8_t configuration,
                                                 int res);

#endif
