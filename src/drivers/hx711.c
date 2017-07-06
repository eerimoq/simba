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

static void wait_ready(struct hx711_driver_t *self_p)
{
    /* Wait for the module to be ready. */
    while (pin_device_read(self_p->dout_p) == 1);
}

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

static uint32_t read_sample(struct hx711_driver_t *self_p)
{
    int i;
    uint32_t sample;

    sample = 0;

    for (i = 0; i < 24; i++) {
        sample <<= 1;
        sample |= read_bit(self_p);
    }

    return (sample);
}

int hx711_module_init()
{
    return (0);
}

int hx711_init(struct hx711_driver_t *self_p,
               struct pin_device_t *pd_sck_p,
               struct pin_device_t *dout_p)
{
    self_p->pd_sck_p = pd_sck_p;
    self_p->dout_p = dout_p;

    return (0);
}

int hx711_start(struct hx711_driver_t *self_p)
{
    /* Initialize PD_SCK pin as output and set it low. */
    pin_device_set_mode(self_p->pd_sck_p, PIN_OUTPUT);
    pin_device_write_low(self_p->pd_sck_p);

    /* Initialize DOUT pin input. */
    pin_device_set_mode(self_p->dout_p, PIN_INPUT);

    /* Read a dummy sample to setup for channel and gain configuration
       on first read. */
    wait_ready(self_p);
    (void)read_sample(self_p);

    return (0);
}

int hx711_stop(struct hx711_driver_t *self_p)
{
    pin_device_set_mode(self_p->pd_sck_p, PIN_INPUT);

    return (0);
}

int hx711_read(struct hx711_driver_t *self_p,
               uint32_t *sample_p,
               enum hx711_channel_gain_t channel_gain)
{
    int i;

    /* Setup the correct channel and gain by sending 2-4 pulses (read
       bit ignored). */
    for (i = 0; i < channel_gain; i++) {
        (void)read_bit(self_p);
    }

    /* Wait for the module to be ready. */
    wait_ready(self_p);

    /* Read the sample. */
    *sample_p = read_sample(self_p);

    return (0);
}
