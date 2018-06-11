/*
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

#ifndef __USB_MOCK_H__
#define __USB_MOCK_H__

#include "simba.h"

int mock_write_usb_format_descriptors(void *out_p,
                                      uint8_t *buf_p,
                                      size_t size,
                                      int res);

int mock_write_usb_desc_get_configuration(uint8_t *desc_p,
                                          size_t size,
                                          int configuration,
                                          struct usb_descriptor_configuration_t *
res);

int mock_write_usb_desc_get_interface(uint8_t *desc_p,
                                      size_t size,
                                      int configuration,
                                      int interface,
                                      struct usb_descriptor_interface_t *
res);

int mock_write_usb_desc_get_endpoint(uint8_t *desc_p,
                                     size_t size,
                                     int configuration,
                                     int interface,
                                     int endpoint,
                                     struct usb_descriptor_endpoint_t *
res);

int mock_write_usb_desc_get_class(uint8_t *buf_p,
                                  size_t size,
                                  int configuration,
                                  int interface,
                                  int res);

#endif
