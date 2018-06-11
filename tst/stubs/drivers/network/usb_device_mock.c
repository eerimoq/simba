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

#include "simba.h"
#include "usb_device_mock.h"

int mock_write_usb_device_module_init(int res)
{
    harness_mock_write("usb_device_module_init()",
                       NULL,
                       0);

    harness_mock_write("usb_device_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_module_init)()
{
    int res;

    harness_mock_assert("usb_device_module_init()",
                        NULL,
                        0);

    harness_mock_read("usb_device_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_init(struct usb_device_t *dev_p,
                               struct usb_device_driver_base_t **drivers_pp,
                               int drivers_max,
                               FAR const union usb_descriptor_t **descriptors_pp,
                               int res)
{
    harness_mock_write("usb_device_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("usb_device_init(drivers_pp)",
                       drivers_pp,
                       sizeof(*drivers_pp));

    harness_mock_write("usb_device_init(drivers_max)",
                       &drivers_max,
                       sizeof(drivers_max));

    harness_mock_write("usb_device_init(descriptors_pp)",
                       descriptors_pp,
                       sizeof(*descriptors_pp));

    harness_mock_write("usb_device_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_init)(struct usb_device_driver_t *self_p,
                                                 struct usb_device_t *dev_p,
                                                 struct usb_device_driver_base_t **drivers_pp,
                                                 int drivers_max,
                                                 FAR const union usb_descriptor_t **descriptors_pp)
{
    int res;

    harness_mock_assert("usb_device_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("usb_device_init(drivers_pp)",
                        drivers_pp,
                        sizeof(*drivers_pp));

    harness_mock_assert("usb_device_init(drivers_max)",
                        &drivers_max,
                        sizeof(drivers_max));

    harness_mock_assert("usb_device_init(descriptors_pp)",
                        descriptors_pp,
                        sizeof(*descriptors_pp));

    harness_mock_read("usb_device_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_start(int res)
{
    harness_mock_write("usb_device_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_start)(struct usb_device_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_device_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_stop(int res)
{
    harness_mock_write("usb_device_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_stop)(struct usb_device_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_device_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_write(int endpoint,
                                const void *buf_p,
                                size_t size,
                                ssize_t res)
{
    harness_mock_write("usb_device_write(endpoint)",
                       &endpoint,
                       sizeof(endpoint));

    harness_mock_write("usb_device_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("usb_device_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("usb_device_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(usb_device_write)(struct usb_device_driver_t *self_p,
                                                      int endpoint,
                                                      const void *buf_p,
                                                      size_t size)
{
    ssize_t res;

    harness_mock_assert("usb_device_write(endpoint)",
                        &endpoint,
                        sizeof(endpoint));

    harness_mock_assert("usb_device_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("usb_device_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("usb_device_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_read_isr(int endpoint,
                                   void *buf_p,
                                   size_t size,
                                   ssize_t res)
{
    harness_mock_write("usb_device_read_isr(endpoint)",
                       &endpoint,
                       sizeof(endpoint));

    harness_mock_write("usb_device_read_isr(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("usb_device_read_isr(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("usb_device_read_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(usb_device_read_isr)(struct usb_device_driver_t *self_p,
                                                         int endpoint,
                                                         void *buf_p,
                                                         size_t size)
{
    ssize_t res;

    harness_mock_assert("usb_device_read_isr(endpoint)",
                        &endpoint,
                        sizeof(endpoint));

    harness_mock_read("usb_device_read_isr(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("usb_device_read_isr(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("usb_device_read_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_write_isr(int endpoint,
                                    const void *buf_p,
                                    size_t size,
                                    ssize_t res)
{
    harness_mock_write("usb_device_write_isr(endpoint)",
                       &endpoint,
                       sizeof(endpoint));

    harness_mock_write("usb_device_write_isr(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("usb_device_write_isr(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("usb_device_write_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(usb_device_write_isr)(struct usb_device_driver_t *self_p,
                                                          int endpoint,
                                                          const void *buf_p,
                                                          size_t size)
{
    ssize_t res;

    harness_mock_assert("usb_device_write_isr(endpoint)",
                        &endpoint,
                        sizeof(endpoint));

    harness_mock_assert("usb_device_write_isr(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("usb_device_write_isr(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("usb_device_write_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
