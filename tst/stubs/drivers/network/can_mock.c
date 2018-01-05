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
#include "can_mock.h"

int mock_write_can_module_init(int res)
{
    harness_mock_write("can_module_init()",
                       NULL,
                       0);

    harness_mock_write("can_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(can_module_init)()
{
    int res;

    harness_mock_assert("can_module_init()",
                        NULL,
                        0);

    harness_mock_read("can_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_can_init(struct can_device_t *dev_p,
                        uint32_t speed,
                        void *rxbuf_p,
                        size_t size,
                        int res)
{
    harness_mock_write("can_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("can_init(speed)",
                       &speed,
                       sizeof(speed));

    harness_mock_write("can_init(rxbuf_p)",
                       rxbuf_p,
                       size);

    harness_mock_write("can_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("can_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(can_init)(struct can_driver_t *self_p,
                                          struct can_device_t *dev_p,
                                          uint32_t speed,
                                          void *rxbuf_p,
                                          size_t size)
{
    int res;

    harness_mock_assert("can_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("can_init(speed)",
                        &speed,
                        sizeof(speed));

    harness_mock_assert("can_init(rxbuf_p)",
                        rxbuf_p,
                        size);

    harness_mock_assert("can_init(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("can_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_can_start(int res)
{
    harness_mock_write("can_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(can_start)(struct can_driver_t *self_p)
{
    int res;

    harness_mock_read("can_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_can_stop(int res)
{
    harness_mock_write("can_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(can_stop)(struct can_driver_t *self_p)
{
    int res;

    harness_mock_read("can_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_can_read(struct can_frame_t *frame_p,
                        size_t size,
                        ssize_t res)
{
    harness_mock_write("can_read(): return (frame_p)",
                       frame_p,
                       sizeof(*frame_p));

    harness_mock_write("can_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("can_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(can_read)(struct can_driver_t *self_p,
                                              struct can_frame_t *frame_p,
                                              size_t size)
{
    ssize_t res;

    harness_mock_read("can_read(): return (frame_p)",
                      frame_p,
                      sizeof(*frame_p));

    harness_mock_assert("can_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("can_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_can_write(const struct can_frame_t *frame_p,
                         size_t size,
                         ssize_t res)
{
    harness_mock_write("can_write(frame_p)",
                       frame_p,
                       sizeof(*frame_p));

    harness_mock_write("can_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("can_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(can_write)(struct can_driver_t *self_p,
                                               const struct can_frame_t *frame_p,
                                               size_t size)
{
    ssize_t res;

    harness_mock_assert("can_write(frame_p)",
                        frame_p,
                        sizeof(*frame_p));

    harness_mock_assert("can_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("can_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
