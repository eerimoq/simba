/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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
#include "hx711_mock.h"

int mock_write_hx711_module_init(int res)
{
    harness_mock_write("hx711_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_module_init)()
{
    int res;

    harness_mock_read("hx711_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hx711_init(struct pin_device_t *pd_sck_p,
                          struct pin_device_t *dout_p,
                          float scale,
                          float offset,
                          int res)
{
    harness_mock_write("hx711_init(pd_sck_p)",
                       pd_sck_p,
                       sizeof(*pd_sck_p));

    harness_mock_write("hx711_init(dout_p)",
                       dout_p,
                       sizeof(*dout_p));

    harness_mock_write("hx711_init(scale)",
                       &scale,
                       sizeof(scale));

    harness_mock_write("hx711_init(offset)",
                       &offset,
                       sizeof(offset));

    harness_mock_write("hx711_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_init)(struct hx711_driver_t *self_p,
                                            struct pin_device_t *pd_sck_p,
                                            struct pin_device_t *dout_p,
                                            float scale,
                                            float offset)
{
    int res;

    harness_mock_assert("hx711_init(pd_sck_p)",
                        pd_sck_p);

    harness_mock_assert("hx711_init(dout_p)",
                        dout_p);

    harness_mock_assert("hx711_init(scale)",
                        &scale);

    harness_mock_assert("hx711_init(offset)",
                        &offset);

    harness_mock_read("hx711_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hx711_start(int res)
{
    harness_mock_write("hx711_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_start)(struct hx711_driver_t *self_p)
{
    int res;

    harness_mock_read("hx711_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hx711_stop(int res)
{
    harness_mock_write("hx711_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_stop)(struct hx711_driver_t *self_p)
{
    int res;

    harness_mock_read("hx711_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hx711_read(float *weight_p,
                          enum hx711_channel_gain_t channel_gain,
                          int res)
{
    harness_mock_write("hx711_read(): return (weight_p)",
                       weight_p,
                       sizeof(*weight_p));

    harness_mock_write("hx711_read(channel_gain)",
                       &channel_gain,
                       sizeof(channel_gain));

    harness_mock_write("hx711_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_read)(struct hx711_driver_t *self_p,
                                            float *weight_p,
                                            enum hx711_channel_gain_t channel_gain)
{
    int res;

    harness_mock_read("hx711_read(): return (weight_p)",
                      weight_p,
                      -1);

    harness_mock_assert("hx711_read(channel_gain)",
                        &channel_gain);

    harness_mock_read("hx711_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hx711_read_raw(int32_t *sample_p,
                              enum hx711_channel_gain_t channel_gain,
                              int res)
{
    harness_mock_write("hx711_read_raw(): return (sample_p)",
                       sample_p,
                       sizeof(*sample_p));

    harness_mock_write("hx711_read_raw(channel_gain)",
                       &channel_gain,
                       sizeof(channel_gain));

    harness_mock_write("hx711_read_raw(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_read_raw)(struct hx711_driver_t *self_p,
                                                int32_t *sample_p,
                                                enum hx711_channel_gain_t channel_gain)
{
    int res;

    harness_mock_read("hx711_read_raw(): return (sample_p)",
                      sample_p,
                      -1);

    harness_mock_assert("hx711_read_raw(channel_gain)",
                        &channel_gain);

    harness_mock_read("hx711_read_raw(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hx711_set_scale(float scale,
                               int res)
{
    harness_mock_write("hx711_set_scale(scale)",
                       &scale,
                       sizeof(scale));

    harness_mock_write("hx711_set_scale(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_set_scale)(struct hx711_driver_t *self_p,
                                                 float scale)
{
    int res;

    harness_mock_assert("hx711_set_scale(scale)",
                        &scale);

    harness_mock_read("hx711_set_scale(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hx711_set_offset(float offset,
                                int res)
{
    harness_mock_write("hx711_set_offset(offset)",
                       &offset,
                       sizeof(offset));

    harness_mock_write("hx711_set_offset(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hx711_set_offset)(struct hx711_driver_t *self_p,
                                                  float offset)
{
    int res;

    harness_mock_assert("hx711_set_offset(offset)",
                        &offset);

    harness_mock_read("hx711_set_offset(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
