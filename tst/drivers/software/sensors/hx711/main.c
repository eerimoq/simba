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

#define PD_SCK_DEV_P  &pin_device[0]
#define DOUT_DEV_P    &pin_device[1]

struct hx711_driver_t hx711;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(hx711_module_init() == 0);
    BTASSERT(hx711_module_init() == 0);
    BTASSERT(hx711_init(&hx711,
                        PD_SCK_DEV_P,
                        DOUT_DEV_P,
                        1,
                        0) == 0);

    return (0);
}

static int test_start_timeout(struct harness_t *harness_p)
{
    int mode;
    struct pin_device_t *dev_p;
    enum time_compare_t res;

    /* Timeout waiting for data ready. */
    res = time_compare_equal_t;
    harness_mock_write("time_compare(): return (res)",
                       &res,
                       sizeof(res));

    /* Call the start function. */
    BTASSERTI(hx711_start(&hx711), ==, -ETIMEDOUT);

    /* PD_SCK configured as output and set low. */
    harness_mock_read("pin_port_device_set_mode(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == PD_SCK_DEV_P);

    harness_mock_read("pin_port_device_set_mode(mode)",
                      &mode,
                      sizeof(mode));
    BTASSERT(mode == PIN_OUTPUT);

    harness_mock_read("pin_port_device_write_low(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == PD_SCK_DEV_P);

    /* DOUT configured as input. */
    harness_mock_read("pin_port_device_set_mode(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == DOUT_DEV_P);

    harness_mock_read("pin_port_device_set_mode(mode)",
                      &mode,
                      sizeof(mode));
    BTASSERT(mode == PIN_INPUT);

    return (0);
}

static int test_start(struct harness_t *harness_p)
{
    int i;
    int mode;
    int value;
    struct pin_device_t *dev_p;

    /* DOUT ready. */
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));

    /* Call the start function. */
    BTASSERT(hx711_start(&hx711) == 0);

    /* PD_SCK configured as output and set low. */
    harness_mock_read("pin_port_device_set_mode(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == PD_SCK_DEV_P);

    harness_mock_read("pin_port_device_set_mode(mode)",
                      &mode,
                      sizeof(mode));
    BTASSERT(mode == PIN_OUTPUT);

    harness_mock_read("pin_port_device_write_low(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == PD_SCK_DEV_P);

    /* DOUT configured as input. */
    harness_mock_read("pin_port_device_set_mode(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == DOUT_DEV_P);

    harness_mock_read("pin_port_device_set_mode(mode)",
                      &mode,
                      sizeof(mode));
    BTASSERT(mode == PIN_INPUT);

    /* A dummy sample has been read. */
    for (i = 0; i < 24; i++) {
        harness_mock_read("pin_port_device_write_high(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);

        harness_mock_read("pin_port_device_write_low(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);
    }

    return (0);
}

static int test_read_raw(struct harness_t *harness_p)
{
    int i;
    int value;
    int32_t sample;
    struct pin_device_t *dev_p;

    /* First read bit (MSB) as one(1). First read is channel & gain
       config, then read the ready bit, then one sample bit. */
    value = 1;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));
    value = 0;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));
    value = 1;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));

    sample = 0xffffffff;

    BTASSERT(hx711_read_raw(&hx711,
                            &sample,
                            hx711_channel_gain_a_128_t) == 0);
    BTASSERTI(sample, ==, (int32_t)0xff800000);

    /* A channel-gain of length 2 and sample has been read. */
    for (i = 0; i < 24 + 2; i++) {
        harness_mock_read("pin_port_device_write_high(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);

        harness_mock_read("pin_port_device_write_low(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);
    }

    return (0);
}

static int test_read_raw_timeout(struct harness_t *harness_p)
{
    int32_t sample;
    struct pin_device_t *dev_p;
    enum time_compare_t res;

    /* Timeout waiting for data ready. */
    res = time_compare_greater_than_t;
    harness_mock_write("time_compare(): return (res)",
                       &res,
                       sizeof(res));

    sample = 0xffffffff;

    BTASSERTI(hx711_read_raw(&hx711,
                             &sample,
                             hx711_channel_gain_a_128_t), ==, -ETIMEDOUT);
    BTASSERTI(sample, ==, (int32_t)0xffffffff);

    /* A channel-gain of length 1 has been read. */
    harness_mock_read("pin_port_device_write_high(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == PD_SCK_DEV_P);

    harness_mock_read("pin_port_device_write_low(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == PD_SCK_DEV_P);

    return (0);
}

static int test_read(struct harness_t *harness_p)
{
    int i;
    int value;
    float weight;
    struct pin_device_t *dev_p;

    /* First read bit (MSB) as one(1). First read is channel & gain
       config, then read the ready bit, then one sample bit. */
    value = 1;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));
    value = 0;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));
    value = 1;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));

    weight = 0.0;

    BTASSERT(hx711_read(&hx711,
                        &weight,
                        hx711_channel_gain_a_128_t) == 0);
    BTASSERT(weight== -8388608.0);

    /* A channel-gain of length 2 and sample has been read. */
    for (i = 0; i < 24 + 2; i++) {
        harness_mock_read("pin_port_device_write_high(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);

        harness_mock_read("pin_port_device_write_low(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);
    }

    return (0);
}

static int test_read_scale_offset(struct harness_t *harness_p)
{
    int i;
    int value;
    float weight;
    struct pin_device_t *dev_p;

    /* Set scale and offset. */
    BTASSERT(hx711_set_scale(&hx711, 0.1) == 0);
    BTASSERT(hx711_set_offset(&hx711, 10.0) == 0);

    /* First read bit (MSB) as one(1). First read is channel & gain
       config, then read the ready bit, then one sample bit. */
    value = 1;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));
    value = 0;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));
    value = 1;
    harness_mock_write("pin_port_device_read(): return (value)",
                       &value,
                       sizeof(value));

    weight = 0.0;

    BTASSERT(hx711_read(&hx711,
                        &weight,
                        hx711_channel_gain_a_128_t) == 0);
    BTASSERT(weight== -838859.8125);

    /* A channel-gain of length 2 and sample has been read. */
    for (i = 0; i < 24 + 2; i++) {
        harness_mock_read("pin_port_device_write_high(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);

        harness_mock_read("pin_port_device_write_low(dev_p)",
                          &dev_p,
                          sizeof(dev_p));
        BTASSERT(dev_p == PD_SCK_DEV_P);
    }

    return (0);
}

static int test_stop(struct harness_t *harness_p)
{
    int mode;
    struct pin_device_t *dev_p;

    BTASSERT(hx711_stop(&hx711) == 0);

    /* PD_SCK configured as input. */
    harness_mock_read("pin_port_device_set_mode(dev_p)",
                      &dev_p,
                      sizeof(dev_p));
    BTASSERT(dev_p == PD_SCK_DEV_P);

    harness_mock_read("pin_port_device_set_mode(mode)",
                      &mode,
                      sizeof(mode));
    BTASSERT(mode == PIN_INPUT);

    return (0);
}

int STUB(pin_port_device_set_mode)(const struct pin_device_t *dev_p,
                              int mode)
{
    harness_mock_write("pin_port_device_set_mode(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("pin_port_device_set_mode(mode)",
                       &mode,
                       sizeof(mode));

    return (0);
}

int STUB(pin_port_device_read)(const struct pin_device_t *dev_p)
{
    int value;

    BTASSERT(dev_p == DOUT_DEV_P);

    if (harness_mock_read("pin_port_device_read(): return (value)",
                          &value,
                          sizeof(value)) == -1) {
        value = 0;
    }

    return (value);
}

int STUB(pin_port_device_write_high)(const struct pin_device_t *dev_p)
{
    harness_mock_write("pin_port_device_write_high(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    return (0);
}

int STUB(pin_port_device_write_low)(const struct pin_device_t *dev_p)
{
    harness_mock_write("pin_port_device_write_low(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    return (0);
}

void STUB(sys_lock)()
{
}

void STUB(sys_unlock)()
{
}

enum time_compare_t STUB(time_compare)(struct time_t *left_p,
                                       struct time_t *right_p)
{
    enum time_compare_t res;

    if (harness_mock_read("time_compare(): return (res)",
                          &res,
                          sizeof(res)) == -1) {
        res = time_compare_less_than_t;
    }

    return (res);

}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_start_timeout, "test_start_timeout" },
        { test_start, "test_start" },
        { test_read_raw, "test_read_raw" },
        { test_read_raw_timeout, "test_read_raw_timeout" },
        { test_read, "test_read" },
        { test_read_scale_offset, "test_read_scale_offset" },
        { test_stop, "test_stop" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
