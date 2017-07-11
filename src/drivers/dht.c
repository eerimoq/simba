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

#define FALLING                                     0
#define RISING                                      1

/* Time definitions. */
#define TIMEOUT_NS                                  100000
#define MINIMUM_RESPONSE_LOW_NS                     50000
#define MINIMUM_RESPONSE_HIGH_NS                    60000

/* Data definitions. */
#define DATA_SIZE                                   5

#define HUMID_MSB_INDEX                             0
#define HUMID_LSB_INDEX                             1
#define TEMP_MSB_INDEX                              2
#define TEMP_LSB_INDEX                              3
#define PARITY_INDEX                                4

/**
 * Check if given buffer contains valid data.
 */
static int is_valid(uint8_t *buf_p)
{
    uint8_t actual_parity;
    uint8_t expected_parity;

    actual_parity = (buf_p[HUMID_MSB_INDEX]
                     + buf_p[HUMID_LSB_INDEX]
                     + buf_p[TEMP_MSB_INDEX]
                     + buf_p[TEMP_LSB_INDEX]);
    expected_parity = buf_p[PARITY_INDEX];

    return (actual_parity == expected_parity);
}

/**
 * Wait for a rising or falling edge.
 *
 * @return Wait time in nanoseconds, or negative error code.
 */
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

        if (stop.nanoseconds > TIMEOUT_NS) {
            return (-ETIMEDOUT);
        }
    }

    return (stop.nanoseconds);
}

/**
 * Read a bit from the device.
 *
 * @return zero(0) or negative error code.
 */
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

    *value_p = (res > 50000);

    return (0);
}

/**
 * Read data from the device by setting the start signal high, wait
 * for the response and then read all 40 bits.
 *
 * @return zero(0), positive number on failure, or negative error code.
 */
static int read_isr(struct dht_driver_t *self_p, uint8_t *buf_p)
{
    int i;
    int res;
    int value;

    pin_device_write_high(self_p->pin_p);
    time_busy_wait_us(20);
    pin_device_set_mode(self_p->pin_p, PIN_INPUT);

    /* Wait for the response signal. */
    time_busy_wait_us(10);

    res = wait_for_edge(self_p, RISING);

    if (res < MINIMUM_RESPONSE_LOW_NS) {
        return (res);
    }

    res = wait_for_edge(self_p, FALLING);

    if (res < MINIMUM_RESPONSE_HIGH_NS) {
        return (res);
    }

    /* Read temperature, humidty and parity. */
    for (i = DATA_SIZE * 8 - 1; i >= 0; i--) {
        res = read_bit(self_p, &value);

        if (res != 0) {
            break;
        }

        buf_p[i / 8] |= value;
        buf_p[i / 8] <<= 1;
    }

    return (res);
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
    int negative;
    uint8_t buf[DATA_SIZE];

    memset(&buf[0], 0, sizeof(buf));

    /* Device communication. Start start signal by setting the pin
       low. */
    pin_device_write_low(self_p->pin_p);
    thrd_sleep_ms(1);

    sys_lock();
    res = read_isr(self_p, &buf[0]);
    sys_unlock();

    pin_device_set_mode(self_p->pin_p, PIN_OUTPUT);
    pin_device_write_high(self_p->pin_p);

    if (res != 0) {
        return (res < 0 ? res : -EPROTO);
    }

    /* Check the parity bits. */
    if (!is_valid(&buf[0])) {
        return (-EPROTO);
    }

    /* Temperature and humidty unpacking and convertion. */
    negative = (buf[TEMP_MSB_INDEX] >> 7);
    buf[TEMP_MSB_INDEX] &= 0x7f;
    *temperature_p = ((buf[TEMP_MSB_INDEX] << 8) | buf[TEMP_LSB_INDEX]);
    *temperature_p /= 10.0f;

    if (negative == 1) {
        *temperature_p *= -1.0f;
    }

    *humidty_p = ((buf[HUMID_MSB_INDEX] << 8) | buf[HUMID_LSB_INDEX]);
    *humidty_p /= 10.0f;

    return (0);
}

#endif
