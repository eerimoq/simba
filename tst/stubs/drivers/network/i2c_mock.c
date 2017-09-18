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
#include "i2c_mock.h"

int mock_write_i2c_module_init(int res)
{
    harness_mock_write("i2c_module_init()",
                       NULL,
                       0);

    harness_mock_write("i2c_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(i2c_module_init)()
{
    int res;

    harness_mock_assert("i2c_module_init()",
                        NULL);

    harness_mock_read("i2c_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_init(struct i2c_device_t *dev_p,
                        int baudrate,
                        int address,
                        int res)
{
    harness_mock_write("i2c_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("i2c_init(baudrate)",
                       &baudrate,
                       sizeof(baudrate));

    harness_mock_write("i2c_init(address)",
                       &address,
                       sizeof(address));

    harness_mock_write("i2c_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(i2c_init)(struct i2c_driver_t *self_p,
                                          struct i2c_device_t *dev_p,
                                          int baudrate,
                                          int address)
{
    int res;

    harness_mock_assert("i2c_init(dev_p)",
                        &dev_p);

    harness_mock_assert("i2c_init(baudrate)",
                        &baudrate);

    harness_mock_assert("i2c_init(address)",
                        &address);

    harness_mock_read("i2c_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_start(int res)
{
    harness_mock_write("i2c_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(i2c_start)(struct i2c_driver_t *self_p)
{
    int res;

    harness_mock_read("i2c_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_stop(int res)
{
    harness_mock_write("i2c_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(i2c_stop)(struct i2c_driver_t *self_p)
{
    int res;

    harness_mock_read("i2c_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_read(int address,
                        void *buf_p,
                        size_t size,
                        ssize_t res)
{
    harness_mock_write("i2c_read(address)",
                       &address,
                       sizeof(address));

    harness_mock_write("i2c_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("i2c_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("i2c_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(i2c_read)(struct i2c_driver_t *self_p,
                                              int address,
                                              void *buf_p,
                                              size_t size)
{
    ssize_t res;

    harness_mock_assert("i2c_read(address)",
                        &address);

    harness_mock_read("i2c_read(): return (buf_p)",
                      buf_p,
                      -1);

    harness_mock_assert("i2c_read(size)",
                        &size);

    harness_mock_read("i2c_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_write(int address,
                         const void *buf_p,
                         size_t size,
                         ssize_t res)
{
    harness_mock_write("i2c_write(address)",
                       &address,
                       sizeof(address));

    harness_mock_write("i2c_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("i2c_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("i2c_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(i2c_write)(struct i2c_driver_t *self_p,
                                               int address,
                                               const void *buf_p,
                                               size_t size)
{
    ssize_t res;

    harness_mock_assert("i2c_write(address)",
                        &address);

    harness_mock_assert("i2c_write(buf_p)",
                        buf_p);

    harness_mock_assert("i2c_write(size)",
                        &size);

    harness_mock_read("i2c_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_scan(int address,
                        int res)
{
    harness_mock_write("i2c_scan(address)",
                       &address,
                       sizeof(address));

    harness_mock_write("i2c_scan(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(i2c_scan)(struct i2c_driver_t *self_p,
                                          int address)
{
    int res;

    harness_mock_assert("i2c_scan(address)",
                        &address);

    harness_mock_read("i2c_scan(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_slave_start(int res)
{
    harness_mock_write("i2c_slave_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(i2c_slave_start)(struct i2c_driver_t *self_p)
{
    int res;

    harness_mock_read("i2c_slave_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_slave_stop(int res)
{
    harness_mock_write("i2c_slave_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(i2c_slave_stop)(struct i2c_driver_t *self_p)
{
    int res;

    harness_mock_read("i2c_slave_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_slave_read(void *buf_p,
                              size_t size,
                              ssize_t res)
{
    harness_mock_write("i2c_slave_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("i2c_slave_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("i2c_slave_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(i2c_slave_read)(struct i2c_driver_t *self_p,
                                                    void *buf_p,
                                                    size_t size)
{
    ssize_t res;

    harness_mock_read("i2c_slave_read(): return (buf_p)",
                      buf_p,
                      -1);

    harness_mock_assert("i2c_slave_read(size)",
                        &size);

    harness_mock_read("i2c_slave_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_i2c_slave_write(const void *buf_p,
                               size_t size,
                               ssize_t res)
{
    harness_mock_write("i2c_slave_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("i2c_slave_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("i2c_slave_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(i2c_slave_write)(struct i2c_driver_t *self_p,
                                                     const void *buf_p,
                                                     size_t size)
{
    ssize_t res;

    harness_mock_assert("i2c_slave_write(buf_p)",
                        buf_p);

    harness_mock_assert("i2c_slave_write(size)",
                        &size);

    harness_mock_read("i2c_slave_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
