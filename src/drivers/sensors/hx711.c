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

/* Timeout of 2 s. */
#define READY_TIMEOUT_S 2

/**
 * Wait for the sensor to be ready.
 */
static int wait_ready(struct hx711_driver_t *self_p)
{
    struct time_t stop;
    struct time_t now;

    stop.seconds = READY_TIMEOUT_S;
    stop.nanoseconds = 0;

    sys_uptime(&now);
    time_add(&stop, &stop, &now);

    while (time_compare(&now, &stop) == time_compare_less_than_t) {
        if (pin_device_read(self_p->dout_p) == 0) {
            return (0);
        }

        sys_uptime(&now);
        thrd_yield();
    }

    return (-ETIMEDOUT);
}

/**
 * Read a single bit from the sensor.
 */
static int read_bit(struct hx711_driver_t *self_p)
{
    int value;

    /* Beginning of the clock pulse. */
    sys_lock();
    pin_device_write_high(self_p->pd_sck_p);
    time_busy_wait_us(1);

    /* Read the data pin. */
    value = pin_device_read(self_p->dout_p);

    /* End of the clock pulse. */
    pin_device_write_low(self_p->pd_sck_p);
    sys_unlock();
    time_busy_wait_us(1);

    return (value);
}

static int32_t read_sample(struct hx711_driver_t *self_p)
{
    int i;
    int32_t sample;

    sample = 0;

    for (i = 0; i < 24; i++) {
        sample <<= 1;
        sample |= read_bit(self_p);
    }

    /* Sign extension. */
    if (0x800000 & sample) {
        sample |= 0xff000000;
    }

    return (sample);
}

int hx711_module_init()
{
    return (0);
}

int hx711_init(struct hx711_driver_t *self_p,
               struct pin_device_t *pd_sck_p,
               struct pin_device_t *dout_p,
               float scale,
               float offset)
{
    self_p->pd_sck_p = pd_sck_p;
    self_p->dout_p = dout_p;
    self_p->scale = scale;
    self_p->offset = offset;

    return (0);
}

int hx711_start(struct hx711_driver_t *self_p)
{
    int res;

    /* Initialize PD_SCK pin as output and set it low. */
    pin_device_set_mode(self_p->pd_sck_p, PIN_OUTPUT);
    pin_device_write_low(self_p->pd_sck_p);

    /* Initialize DOUT pin input. */
    pin_device_set_mode(self_p->dout_p, PIN_INPUT);

    /* Read a dummy sample to setup for channel and gain configuration
       on first read. */
    res = wait_ready(self_p);

    if (res != 0) {
        return (res);
    }

    (void)read_sample(self_p);

    return (0);
}

int hx711_stop(struct hx711_driver_t *self_p)
{
    return (pin_device_set_mode(self_p->pd_sck_p, PIN_INPUT));
}

int hx711_read(struct hx711_driver_t *self_p,
               float *weight_p,
               enum hx711_channel_gain_t channel_gain)
{
    int res;
    int32_t sample;

    res = hx711_read_raw(self_p, &sample, channel_gain);

    if (res != 0) {
        return (res);
    }

    *weight_p = (((float)sample + self_p->offset) * self_p->scale);

    return (0);
}

int hx711_read_raw(struct hx711_driver_t *self_p,
                   int32_t *sample_p,
                   enum hx711_channel_gain_t channel_gain)
{
    int res;
    int i;

    /* Setup the correct channel and gain by sending 1-3 pulses (read
       bit ignored). */
    for (i = 0; i < channel_gain; i++) {
        (void)read_bit(self_p);
    }

    /* Wait for the module to be ready. */
    res = wait_ready(self_p);

    if (res != 0) {
        return (res);
    }

    /* Read the sample. */
    *sample_p = read_sample(self_p);

    return (0);
}

int hx711_set_scale(struct hx711_driver_t *self_p,
                    float scale)
{
    self_p->scale = scale;

    return (0);
}

int hx711_set_offset(struct hx711_driver_t *self_p,
                     float offset)
{
    self_p->offset = offset;

    return (0);
}
