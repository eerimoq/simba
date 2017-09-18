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
#include "cdc_mock.h"

int mock_write_usb_device_class_cdc_module_init(int res)
{
    harness_mock_write("usb_device_class_cdc_module_init()",
                       NULL,
                       0);

    harness_mock_write("usb_device_class_cdc_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_class_cdc_module_init)()
{
    int res;

    harness_mock_assert("usb_device_class_cdc_module_init()",
                        NULL);

    harness_mock_read("usb_device_class_cdc_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_class_cdc_init(int control_interface,
                                         int endpoint_in,
                                         int endpoint_out,
                                         void *rxbuf_p,
                                         size_t size,
                                         int res)
{
    harness_mock_write("usb_device_class_cdc_init(control_interface)",
                       &control_interface,
                       sizeof(control_interface));

    harness_mock_write("usb_device_class_cdc_init(endpoint_in)",
                       &endpoint_in,
                       sizeof(endpoint_in));

    harness_mock_write("usb_device_class_cdc_init(endpoint_out)",
                       &endpoint_out,
                       sizeof(endpoint_out));

    harness_mock_write("usb_device_class_cdc_init(rxbuf_p)",
                       rxbuf_p,
                       size);

    harness_mock_write("usb_device_class_cdc_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("usb_device_class_cdc_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_class_cdc_init)(struct usb_device_class_cdc_driver_t *self_p,
                                                           int control_interface,
                                                           int endpoint_in,
                                                           int endpoint_out,
                                                           void *rxbuf_p,
                                                           size_t size)
{
    int res;

    harness_mock_assert("usb_device_class_cdc_init(control_interface)",
                        &control_interface);

    harness_mock_assert("usb_device_class_cdc_init(endpoint_in)",
                        &endpoint_in);

    harness_mock_assert("usb_device_class_cdc_init(endpoint_out)",
                        &endpoint_out);

    harness_mock_assert("usb_device_class_cdc_init(rxbuf_p)",
                        rxbuf_p);

    harness_mock_assert("usb_device_class_cdc_init(size)",
                        &size);

    harness_mock_read("usb_device_class_cdc_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_class_cdc_input_isr(int res)
{
    harness_mock_write("usb_device_class_cdc_input_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_class_cdc_input_isr)(struct usb_device_class_cdc_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_device_class_cdc_input_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_usb_device_class_cdc_is_connected(int res)
{
    harness_mock_write("usb_device_class_cdc_is_connected(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(usb_device_class_cdc_is_connected)(struct usb_device_class_cdc_driver_t *self_p)
{
    int res;

    harness_mock_read("usb_device_class_cdc_is_connected(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
