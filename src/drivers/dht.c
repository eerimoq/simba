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

#if CONFIG_DHT == 1

#define FALLING 0
#define RISING  1
#define TIMEOUT 100000L

static int is_valid(uint8_t *buf_p)
{
    uint8_t actual_parity;
    uint8_t expected_parity;

    actual_parity = (buf_p[0] + buf_p[1] + buf_p[2] + buf_p[3]);
    expected_parity = buf_p[4];

    return (actual_parity == expected_parity);
}

static long wait_for_edge(struct dht_driver_t *self_p,
                          int target_level)
{
    struct time_t start;
    struct time_t stop;

    sys_uptime_isr(&start);

    while (1) {
        sys_uptime_isr(&stop);
        time_subtract(&stop, &stop, &start);

        if (pin_device_read(self_p->pin_p) == target_level) {
            break;
        }

        if (stop.nanoseconds > TIMEOUT) {
            return (-ETIMEDOUT);
        }
    }

    return (stop.nanoseconds);
}

static int read_bit(struct dht_driver_t *self_p,
                    int *value_p)
{
    long res;

    res = wait_for_edge(self_p, RISING);

    if (res < 0) {
        return (res);
    }

    res = wait_for_edge(self_p, FALLING);

    if (res < 0) {
        return (res);
    }

    *value_p = (res > 50);

    return (0);
}

int dht_module_init()
{
    return (0);
}

int dht_init(struct dht_driver_t *self_p,
             struct pin_device_t *pin_p)
{
    pin_device_set_mode(self_p->pin_p, PIN_OUTPUT);
    pin_device_write_high(self_p->pin_p);

    return (0);
}

int dht_read(struct dht_driver_t *self_p,
             float *temperature_p,
             float *humidty_p)
{
    int res;
    int i;
    int value;
    int negative;
    uint8_t buf[5];

    memset(&buf[0], 0, sizeof(buf));
    value = 0;

    /* Send start signal. */
    pin_device_write_low(self_p->pin_p);
    thrd_sleep_ms(1);

    sys_lock();

    pin_device_write_high(self_p->pin_p);
    time_busy_wait_us(20);
    pin_device_set_mode(self_p->pin_p, PIN_INPUT);

    /* Wait for the response signal. */
    time_busy_wait_us(10);

    res = wait_for_edge(self_p, RISING);

    if (res > 50) {
        res = wait_for_edge(self_p, FALLING);

        if (res > 60) {
            /* Read the 40 bits of data. */
            for (i = 39; i >= 0; i--) {
                res = read_bit(self_p, &value);

                if (res != 0) {
                    break;
                }

                buf[i / 8] |= value;
                buf[i / 8] <<= 1;
            }
        }
    }

    sys_unlock();

    pin_device_set_mode(self_p->pin_p, PIN_OUTPUT);
    pin_device_write_high(self_p->pin_p);

    if (res == 0) {
        /* Check the parity bits. */
        if (is_valid(&buf[0])) {
            negative = (buf[2] >> 7);
            buf[2] &= 0x7f;
            *temperature_p = ((buf[2] << 8)| buf[3]) / 10.0f;

            if (negative == 1) {
                *temperature_p *= -1.0f;
            }

            *humidty_p = (((buf[0] << 8)| buf[1]) / 10.0f);
            res = 0;
        } else {
            res = -EPROTO;
        }
    }

    return (res);
}

#endif
