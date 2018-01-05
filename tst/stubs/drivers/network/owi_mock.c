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
#include "owi_mock.h"

int mock_write_owi_init(struct pin_device_t *dev_p,
                        struct owi_device_t *devices_p,
                        size_t nmemb,
                        int res)
{
    harness_mock_write("owi_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("owi_init(devices_p)",
                       devices_p,
                       sizeof(*devices_p));

    harness_mock_write("owi_init(nmemb)",
                       &nmemb,
                       sizeof(nmemb));

    harness_mock_write("owi_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(owi_init)(struct owi_driver_t *self_p,
                                          struct pin_device_t *dev_p,
                                          struct owi_device_t *devices_p,
                                          size_t nmemb)
{
    int res;

    harness_mock_assert("owi_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("owi_init(devices_p)",
                        devices_p,
                        sizeof(*devices_p));

    harness_mock_assert("owi_init(nmemb)",
                        &nmemb,
                        sizeof(nmemb));

    harness_mock_read("owi_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_owi_reset(int res)
{
    harness_mock_write("owi_reset(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(owi_reset)(struct owi_driver_t *self_p)
{
    int res;

    harness_mock_read("owi_reset(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_owi_search(int res)
{
    harness_mock_write("owi_search(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(owi_search)(struct owi_driver_t *self_p)
{
    int res;

    harness_mock_read("owi_search(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_owi_read(void *buf_p,
                        size_t size,
                        ssize_t res)
{
    harness_mock_write("owi_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("owi_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("owi_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(owi_read)(struct owi_driver_t *self_p,
                                              void *buf_p,
                                              size_t size)
{
    ssize_t res;

    harness_mock_assert("owi_read(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("owi_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("owi_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_owi_write(const void *buf_p,
                         size_t size,
                         ssize_t res)
{
    harness_mock_write("owi_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("owi_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("owi_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(owi_write)(struct owi_driver_t *self_p,
                                               const void *buf_p,
                                               size_t size)
{
    ssize_t res;

    harness_mock_assert("owi_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("owi_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("owi_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
