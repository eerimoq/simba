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
#include "bus_mock.h"

int mock_write_bus_module_init(int res)
{
    harness_mock_write("bus_module_init()",
                       NULL,
                       0);

    harness_mock_write("bus_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bus_module_init)()
{
    int res;

    harness_mock_assert("bus_module_init()",
                        NULL,
                        0);

    harness_mock_read("bus_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bus_init(int res)
{
    harness_mock_write("bus_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bus_init)(struct bus_t *self_p)
{
    int res;

    harness_mock_read("bus_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bus_listener_init(int id,
                                 void *chan_p,
                                 int res)
{
    harness_mock_write("bus_listener_init(id)",
                       &id,
                       sizeof(id));

    harness_mock_write("bus_listener_init(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("bus_listener_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bus_listener_init)(struct bus_listener_t *self_p,
                                                   int id,
                                                   void *chan_p)
{
    int res;

    harness_mock_assert("bus_listener_init(id)",
                        &id,
                        sizeof(id));

    harness_mock_assert("bus_listener_init(chan_p)",
                        chan_p,
                        sizeof(*chan_p));

    harness_mock_read("bus_listener_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bus_attach(struct bus_listener_t *listener_p,
                          int res)
{
    harness_mock_write("bus_attach(listener_p)",
                       listener_p,
                       sizeof(*listener_p));

    harness_mock_write("bus_attach(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bus_attach)(struct bus_t *self_p,
                                            struct bus_listener_t *listener_p)
{
    int res;

    harness_mock_assert("bus_attach(listener_p)",
                        listener_p,
                        sizeof(*listener_p));

    harness_mock_read("bus_attach(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bus_detach(struct bus_listener_t *listener_p,
                          int res)
{
    harness_mock_write("bus_detach(listener_p)",
                       listener_p,
                       sizeof(*listener_p));

    harness_mock_write("bus_detach(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bus_detach)(struct bus_t *self_p,
                                            struct bus_listener_t *listener_p)
{
    int res;

    harness_mock_assert("bus_detach(listener_p)",
                        listener_p,
                        sizeof(*listener_p));

    harness_mock_read("bus_detach(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bus_write(int id,
                         const void *buf_p,
                         size_t size,
                         int res)
{
    harness_mock_write("bus_write(id)",
                       &id,
                       sizeof(id));

    harness_mock_write("bus_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("bus_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("bus_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bus_write)(struct bus_t *self_p,
                                           int id,
                                           const void *buf_p,
                                           size_t size)
{
    int res;

    harness_mock_assert("bus_write(id)",
                        &id,
                        sizeof(id));

    harness_mock_assert("bus_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("bus_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("bus_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
