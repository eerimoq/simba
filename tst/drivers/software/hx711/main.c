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
                        DOUT_DEV_P) == 0);

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

static int test_read(struct harness_t *harness_p)
{
    int i;
    int value;
    uint32_t sample;
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

    BTASSERT(hx711_read(&hx711,
                        &sample,
                        hx711_channel_gain_a_128_t) == 0);
    BTASSERTI(sample, ==, 0x00800000);

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

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_start, "test_start" },
        { test_read, "test_read" },
        { test_stop, "test_stop" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
