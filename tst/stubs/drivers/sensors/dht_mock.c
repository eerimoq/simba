/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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
#include "dht_mock.h"

int mock_write_dht_module_init(int res)
{
    harness_mock_write("dht_module_init()",
                       NULL,
                       0);

    harness_mock_write("dht_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dht_module_init)()
{
    int res;

    harness_mock_assert("dht_module_init()",
                        NULL,
                        0);

    harness_mock_read("dht_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_dht_init(struct pin_device_t *pin_p,
                        int res)
{
    harness_mock_write("dht_init(pin_p)",
                       pin_p,
                       sizeof(*pin_p));

    harness_mock_write("dht_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dht_init)(struct dht_driver_t *self_p,
                                          struct pin_device_t *pin_p)
{
    int res;

    harness_mock_assert("dht_init(pin_p)",
                        pin_p,
                        sizeof(*pin_p));

    harness_mock_read("dht_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_dht_read(float *temperature_p,
                        float *humidty_p,
                        int res)
{
    harness_mock_write("dht_read(): return (temperature_p)",
                       temperature_p,
                       sizeof(*temperature_p));

    harness_mock_write("dht_read(): return (humidty_p)",
                       humidty_p,
                       sizeof(*humidty_p));

    harness_mock_write("dht_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(dht_read)(struct dht_driver_t *self_p,
                                          float *temperature_p,
                                          float *humidty_p)
{
    int res;

    harness_mock_read("dht_read(): return (temperature_p)",
                      temperature_p,
                      sizeof(*temperature_p));

    harness_mock_read("dht_read(): return (humidty_p)",
                      humidty_p,
                      sizeof(*humidty_p));

    harness_mock_read("dht_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
