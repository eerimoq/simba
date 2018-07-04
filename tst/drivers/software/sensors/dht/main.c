/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

#define DHT_DEV_P    &pin_device[0]

struct dht_driver_t dht;

static void mock_dht_response(int edge_time);
static void mock_read_bit(int value);
static void mock_read_byte(int value);

static int test_init(void)
{
    BTASSERT(dht_module_init() == 0);
    BTASSERT(dht_module_init() == 0);
    BTASSERT(dht_init(&dht, DHT_DEV_P) == 0);

    return (0);
}

static int test_dht_read(void)
{
    float temperature;
    float humidity;

    mock_dht_response(60);

    mock_read_byte(2);   /* humidity MSB */
    mock_read_byte(143); /* humidity LSB */
    mock_read_byte(1);   /* temperature MSB */
    mock_read_byte(29);  /* temperature LSB */
    mock_read_byte(175); /* parity */

    BTASSERT(dht_read(&dht, &temperature, &humidity) == 0);
    BTASSERT(temperature == 28.5);
    BTASSERT(humidity == 65.5);

    return (0);
}

static int test_dht_11_read(void)
{
    float temperature;
    float humidity;

    mock_dht_response(60);

    mock_read_byte(65); /* humidity MSB */
    mock_read_byte(0);  /* humidity LSB */
    mock_read_byte(28); /* temperature MSB */
    mock_read_byte(5);  /* temperature LSB */
    mock_read_byte(98); /* parity */

    BTASSERT(dht_11_read(&dht, &temperature, &humidity) == 0);
    BTASSERT(temperature == 28.5);
    BTASSERT(humidity == 65);

    return (0);
}

static void mock_write_pin_port_device_read(int value)
{
    harness_mock_write("pin_port_device_read(): return(value)",
                       &value,
                       sizeof(value));
}

int STUB(pin_port_device_read)(const struct pin_device_t *dev_p)
{
    int value;

    harness_mock_read("pin_port_device_read(): return(value)",
                      &value,
                      sizeof(value));

    return (value);
}

static void mock_write_time_micros_elapsed(int value)
{
    harness_mock_write("time_micros_elapsed(): return(value)",
                       &value,
                       sizeof(value));
}

int STUB(time_micros_elapsed)(int start, int stop)
{
    int value;

    harness_mock_read("time_micros_elapsed(): return(value)",
                      &value,
                      sizeof(value));

    return (value);
}

void STUB(sys_lock)(void)
{
}

void STUB(sys_unlock)(void)
{
}

static void mock_dht_response(int edge_time)
{
    mock_write_pin_port_device_read(1);
    mock_write_time_micros_elapsed(edge_time);

    mock_write_pin_port_device_read(0);
    mock_write_time_micros_elapsed(edge_time);
}

static void mock_read_bit(int value)
{
    mock_dht_response(value > 0 ? 60 : 50);
}

static void mock_read_byte(int value)
{
    int i;

    for (i = 7; i >= 0; i--) {
        mock_read_bit(value & (1 << i));
    }
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_dht_read, "test_dht_read" },
        { test_dht_11_read, "test_dht_11_read" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
