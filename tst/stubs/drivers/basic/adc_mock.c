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
#include "adc_mock.h"

int mock_write_adc_module_init(int res)
{
    harness_mock_write("adc_module_init()",
                       NULL,
                       0);

    harness_mock_write("adc_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(adc_module_init)()
{
    int res;

    harness_mock_assert("adc_module_init()",
                        NULL);

    harness_mock_read("adc_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_adc_init(struct adc_device_t *dev_p,
                        struct pin_device_t *pin_dev_p,
                        int reference,
                        long sampling_rate,
                        int res)
{
    harness_mock_write("adc_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("adc_init(pin_dev_p)",
                       &pin_dev_p,
                       sizeof(pin_dev_p));

    harness_mock_write("adc_init(reference)",
                       &reference,
                       sizeof(reference));

    harness_mock_write("adc_init(sampling_rate)",
                       &sampling_rate,
                       sizeof(sampling_rate));

    harness_mock_write("adc_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(adc_init)(struct adc_driver_t *self_p,
                                          struct adc_device_t *dev_p,
                                          struct pin_device_t *pin_dev_p,
                                          int reference,
                                          long sampling_rate)
{
    int res;

    harness_mock_assert("adc_init(dev_p)",
                        &dev_p);

    harness_mock_assert("adc_init(pin_dev_p)",
                        &pin_dev_p);

    harness_mock_assert("adc_init(reference)",
                        &reference);

    harness_mock_assert("adc_init(sampling_rate)",
                        &sampling_rate);

    harness_mock_read("adc_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_adc_async_convert(uint16_t *samples_p,
                                 size_t length,
                                 int res)
{
    harness_mock_write("adc_async_convert(): return (samples_p)",
                       samples_p,
                       sizeof(*samples_p));

    harness_mock_write("adc_async_convert(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("adc_async_convert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(adc_async_convert)(struct adc_driver_t *self_p,
                                                   uint16_t *samples_p,
                                                   size_t length)
{
    int res;

    harness_mock_read("adc_async_convert(): return (samples_p)",
                      samples_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("adc_async_convert(length)",
                        &length);

    harness_mock_read("adc_async_convert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_adc_async_wait(int res)
{
    harness_mock_write("adc_async_wait(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(adc_async_wait)(struct adc_driver_t *self_p)
{
    int res;

    harness_mock_read("adc_async_wait(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_adc_convert(uint16_t *samples_p,
                           size_t length,
                           int res)
{
    harness_mock_write("adc_convert(): return (samples_p)",
                       samples_p,
                       sizeof(*samples_p));

    harness_mock_write("adc_convert(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("adc_convert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(adc_convert)(struct adc_driver_t *self_p,
                                             uint16_t *samples_p,
                                             size_t length)
{
    int res;

    harness_mock_read("adc_convert(): return (samples_p)",
                      samples_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("adc_convert(length)",
                        &length);

    harness_mock_read("adc_convert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_adc_convert_isr(uint16_t *sample_p,
                               int res)
{
    harness_mock_write("adc_convert_isr(): return (sample_p)",
                       sample_p,
                       sizeof(*sample_p));

    harness_mock_write("adc_convert_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(adc_convert_isr)(struct adc_driver_t *self_p,
                                                 uint16_t *sample_p)
{
    int res;

    harness_mock_read("adc_convert_isr(): return (sample_p)",
                      sample_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_read("adc_convert_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_adc_is_valid_device(struct adc_device_t *dev_p,
                                   int res)
{
    harness_mock_write("adc_is_valid_device(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("adc_is_valid_device(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(adc_is_valid_device)(struct adc_device_t *dev_p)
{
    int res;

    harness_mock_assert("adc_is_valid_device(dev_p)",
                        &dev_p);

    harness_mock_read("adc_is_valid_device(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
