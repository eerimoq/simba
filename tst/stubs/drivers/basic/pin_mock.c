/**
 * @section License
 *
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
#include "pin_mock.h"

int mock_write_pin_module_init(int res)
{
    harness_mock_write("pin_module_init()",
                       NULL,
                       0);

    harness_mock_write("pin_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pin_module_init)()
{
    int res;

    harness_mock_assert("pin_module_init()",
                        NULL,
                        0);

    harness_mock_read("pin_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pin_init(struct pin_device_t *dev_p,
                        int mode,
                        int res)
{
    harness_mock_write("pin_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("pin_init(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("pin_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pin_init)(struct pin_driver_t *self_p,
                                          struct pin_device_t *dev_p,
                                          int mode)
{
    int res;

    harness_mock_assert("pin_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("pin_init(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_read("pin_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pin_write(int value,
                         int res)
{
    harness_mock_write("pin_write(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("pin_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pin_write)(struct pin_driver_t *self_p,
                                           int value)
{
    int res;

    harness_mock_assert("pin_write(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("pin_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pin_read(int res)
{
    harness_mock_write("pin_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pin_read)(struct pin_driver_t *self_p)
{
    int res;

    harness_mock_read("pin_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pin_toggle(int res)
{
    harness_mock_write("pin_toggle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pin_toggle)(struct pin_driver_t *self_p)
{
    int res;

    harness_mock_read("pin_toggle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pin_set_mode(int mode,
                            int res)
{
    harness_mock_write("pin_set_mode(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("pin_set_mode(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pin_set_mode)(struct pin_driver_t *self_p,
                                              int mode)
{
    int res;

    harness_mock_assert("pin_set_mode(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_read("pin_set_mode(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pin_is_valid_device(struct pin_device_t *dev_p,
                                   int res)
{
    harness_mock_write("pin_is_valid_device(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("pin_is_valid_device(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pin_is_valid_device)(struct pin_device_t *dev_p)
{
    int res;

    harness_mock_assert("pin_is_valid_device(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_read("pin_is_valid_device(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
