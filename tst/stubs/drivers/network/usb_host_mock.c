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

#include "simba.h"
#include "usb_host_mock.h"

int mock_write_usb_host_module_init(int res)
{
    harness_mock_write("usb_host_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_module_init)()
{
    int res;

    harness_mock_read("usb_host_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_init(struct usb_device_t *dev_p,
                             struct usb_host_device_t *devices_p,
                             size_t length,
                             int res)
{
    harness_mock_write("usb_host_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("usb_host_init(devices_p)",
                       devices_p,
                       sizeof(*devices_p));

    harness_mock_write("usb_host_init(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("usb_host_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_init)(struct usb_host_driver_t *self_p,
                                               struct usb_device_t *dev_p,
                                               struct usb_host_device_t *devices_p,
                                               size_t length)
{
    int res;

    harness_mock_assert("usb_host_init(dev_p)",
                        &dev_p);

    harness_mock_assert("usb_host_init(devices_p)",
                        devices_p);

    harness_mock_assert("usb_host_init(length)",
                        &length);

    harness_mock_read("usb_host_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_start(int res)
{
    harness_mock_write("usb_host_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_start)(struct usb_host_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_host_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_stop(int res)
{
    harness_mock_write("usb_host_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_stop)(struct usb_host_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_host_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_driver_add(struct usb_host_device_driver_t *driver_p,
                                   void *arg_p,
                                   int res)
{
    harness_mock_write("usb_host_driver_add(driver_p)",
                       driver_p,
                       sizeof(*driver_p));

    harness_mock_write("usb_host_driver_add(): return (arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("usb_host_driver_add(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_driver_add)(struct usb_host_driver_t *self_p,
                                                     struct usb_host_device_driver_t *driver_p,
                                                     void *arg_p)
{
    int res;

    harness_mock_assert("usb_host_driver_add(driver_p)",
                        driver_p);

    harness_mock_read("usb_host_driver_add(): return (arg_p)",
                      arg_p,
                      -1);

    harness_mock_read("usb_host_driver_add(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_driver_remove(struct usb_host_device_driver_t *driver_p,
                                      int res)
{
    harness_mock_write("usb_host_driver_remove(driver_p)",
                       driver_p,
                       sizeof(*driver_p));

    harness_mock_write("usb_host_driver_remove(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_driver_remove)(struct usb_host_driver_t *self_p,
                                                        struct usb_host_device_driver_t *driver_p)
{
    int res;

    harness_mock_assert("usb_host_driver_remove(driver_p)",
                        driver_p);

    harness_mock_read("usb_host_driver_remove(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_device_open(int device,
                                    struct usb_host_device_t *
res)
{
    harness_mock_write("usb_host_device_open(device)",
                       &device,
                       sizeof(device));

    harness_mock_write("usb_host_device_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct usb_host_device_t *
__attribute__ ((weak)) STUB(usb_host_device_open)(struct usb_host_driver_t *self_p,
                                                                             int device)
{
    struct usb_host_device_t *
res;

    harness_mock_assert("usb_host_device_open(device)",
                        &device);

    harness_mock_read("usb_host_device_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_device_close(int device,
                                     int res)
{
    harness_mock_write("usb_host_device_close(device)",
                       &device,
                       sizeof(device));

    harness_mock_write("usb_host_device_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_device_close)(struct usb_host_driver_t *self_p,
                                                       int device)
{
    int res;

    harness_mock_assert("usb_host_device_close(device)",
                        &device);

    harness_mock_read("usb_host_device_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_device_read(struct usb_host_device_t *device_p,
                                    int endpoint,
                                    void *buf_p,
                                    size_t size,
                                    ssize_t res)
{
    harness_mock_write("usb_host_device_read(device_p)",
                       device_p,
                       sizeof(*device_p));

    harness_mock_write("usb_host_device_read(endpoint)",
                       &endpoint,
                       sizeof(endpoint));

    harness_mock_write("usb_host_device_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("usb_host_device_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("usb_host_device_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(usb_host_device_read)(struct usb_host_device_t *device_p,
                                                          int endpoint,
                                                          void *buf_p,
                                                          size_t size)
{
    ssize_t res;

    harness_mock_assert("usb_host_device_read(device_p)",
                        device_p);

    harness_mock_assert("usb_host_device_read(endpoint)",
                        &endpoint);

    harness_mock_assert("usb_host_device_read(buf_p)",
                        buf_p);

    harness_mock_assert("usb_host_device_read(size)",
                        &size);

    harness_mock_read("usb_host_device_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_device_write(struct usb_host_device_t *device_p,
                                     int endpoint,
                                     const void *buf_p,
                                     size_t size,
                                     ssize_t res)
{
    harness_mock_write("usb_host_device_write(device_p)",
                       device_p,
                       sizeof(*device_p));

    harness_mock_write("usb_host_device_write(endpoint)",
                       &endpoint,
                       sizeof(endpoint));

    harness_mock_write("usb_host_device_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("usb_host_device_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("usb_host_device_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(usb_host_device_write)(struct usb_host_device_t *device_p,
                                                           int endpoint,
                                                           const void *buf_p,
                                                           size_t size)
{
    ssize_t res;

    harness_mock_assert("usb_host_device_write(device_p)",
                        device_p);

    harness_mock_assert("usb_host_device_write(endpoint)",
                        &endpoint);

    harness_mock_assert("usb_host_device_write(buf_p)",
                        buf_p);

    harness_mock_assert("usb_host_device_write(size)",
                        &size);

    harness_mock_read("usb_host_device_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_device_control_transfer(struct usb_host_device_t *device_p,
                                                struct usb_setup_t *setup_p,
                                                void *buf_p,
                                                size_t size,
                                                ssize_t res)
{
    harness_mock_write("usb_host_device_control_transfer(device_p)",
                       device_p,
                       sizeof(*device_p));

    harness_mock_write("usb_host_device_control_transfer(setup_p)",
                       setup_p,
                       sizeof(*setup_p));

    harness_mock_write("usb_host_device_control_transfer(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("usb_host_device_control_transfer(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("usb_host_device_control_transfer(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(usb_host_device_control_transfer)(struct usb_host_device_t *device_p,
                                                                      struct usb_setup_t *setup_p,
                                                                      void *buf_p,
                                                                      size_t size)
{
    ssize_t res;

    harness_mock_assert("usb_host_device_control_transfer(device_p)",
                        device_p);

    harness_mock_assert("usb_host_device_control_transfer(setup_p)",
                        setup_p);

    harness_mock_assert("usb_host_device_control_transfer(buf_p)",
                        buf_p);

    harness_mock_assert("usb_host_device_control_transfer(size)",
                        &size);

    harness_mock_read("usb_host_device_control_transfer(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_host_device_set_configuration(struct usb_host_device_t *device_p,
                                                 uint8_t configuration,
                                                 int res)
{
    harness_mock_write("usb_host_device_set_configuration(device_p)",
                       device_p,
                       sizeof(*device_p));

    harness_mock_write("usb_host_device_set_configuration(configuration)",
                       &configuration,
                       sizeof(configuration));

    harness_mock_write("usb_host_device_set_configuration(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_host_device_set_configuration)(struct usb_host_device_t *device_p,
                                                                   uint8_t configuration)
{
    int res;

    harness_mock_assert("usb_host_device_set_configuration(device_p)",
                        device_p);

    harness_mock_assert("usb_host_device_set_configuration(configuration)",
                        &configuration);

    harness_mock_read("usb_host_device_set_configuration(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
