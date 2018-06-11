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
#include "dac_mock.h"

int mock_write_dac_module_init(int res)
{
    harness_mock_write("dac_module_init()",
                       NULL,
                       0);

    harness_mock_write("dac_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dac_module_init)()
{
    int res;

    harness_mock_assert("dac_module_init()",
                        NULL,
                        0);

    harness_mock_read("dac_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_dac_init(struct dac_device_t *dev_p,
                        struct pin_device_t *pin0_dev_p,
                        struct pin_device_t *pin1_dev_p,
                        int sampling_rate,
                        int res)
{
    harness_mock_write("dac_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("dac_init(pin0_dev_p)",
                       pin0_dev_p,
                       sizeof(*pin0_dev_p));

    harness_mock_write("dac_init(pin1_dev_p)",
                       pin1_dev_p,
                       sizeof(*pin1_dev_p));

    harness_mock_write("dac_init(sampling_rate)",
                       &sampling_rate,
                       sizeof(sampling_rate));

    harness_mock_write("dac_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dac_init)(struct dac_driver_t *self_p,
                                          struct dac_device_t *dev_p,
                                          struct pin_device_t *pin0_dev_p,
                                          struct pin_device_t *pin1_dev_p,
                                          int sampling_rate)
{
    int res;

    harness_mock_assert("dac_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("dac_init(pin0_dev_p)",
                        pin0_dev_p,
                        sizeof(*pin0_dev_p));

    harness_mock_assert("dac_init(pin1_dev_p)",
                        pin1_dev_p,
                        sizeof(*pin1_dev_p));

    harness_mock_assert("dac_init(sampling_rate)",
                        &sampling_rate,
                        sizeof(sampling_rate));

    harness_mock_read("dac_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_dac_async_convert(void *samples_p,
                                 size_t length,
                                 int res)
{
    harness_mock_write("dac_async_convert(): return (samples_p)",
                       samples_p,
                       sizeof(samples_p));

    harness_mock_write("dac_async_convert(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("dac_async_convert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dac_async_convert)(struct dac_driver_t *self_p,
                                                   void *samples_p,
                                                   size_t length)
{
    int res;

    harness_mock_read("dac_async_convert(): return (samples_p)",
                      samples_p,
                      sizeof(*samples_p));

    harness_mock_assert("dac_async_convert(length)",
                        &length,
                        sizeof(length));

    harness_mock_read("dac_async_convert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_dac_async_wait(int res)
{
    harness_mock_write("dac_async_wait(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dac_async_wait)(struct dac_driver_t *self_p)
{
    int res;

    harness_mock_read("dac_async_wait(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_dac_convert(void *samples_p,
                           size_t length,
                           int res)
{
    harness_mock_write("dac_convert(): return (samples_p)",
                       samples_p,
                       sizeof(samples_p));

    harness_mock_write("dac_convert(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("dac_convert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dac_convert)(struct dac_driver_t *self_p,
                                             void *samples_p,
                                             size_t length)
{
    int res;

    harness_mock_read("dac_convert(): return (samples_p)",
                      samples_p,
                      sizeof(*samples_p));

    harness_mock_assert("dac_convert(length)",
                        &length,
                        sizeof(length));

    harness_mock_read("dac_convert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
