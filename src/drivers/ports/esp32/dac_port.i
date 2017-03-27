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

#include "driver/dac.h"

static int dac_port_module_init(void)
{
    return (0);
}

static int dac_port_init(struct dac_driver_t *self_p,
                         struct dac_device_t *dev_p,
                         struct pin_device_t *pin0_dev_p,
                         struct pin_device_t *pin1_dev_p,
                         long sampling_rate)
{
    self_p->dev_p = dev_p;

    /* Pin 0 channel. */
    if (pin0_dev_p == &pin_device[25]) {
        self_p->pin0_channel = DAC_CHANNEL_1;
    } else {
        self_p->pin0_channel = DAC_CHANNEL_2;
    }

    /* Pin 1 channel. */
    if (pin1_dev_p == &pin_device[25]) {
        self_p->pin1_channel = DAC_CHANNEL_1;
    } else if (pin1_dev_p == &pin_device[26]) {
        self_p->pin1_channel = DAC_CHANNEL_2;
    } else {
        self_p->pin1_channel = DAC_CHANNEL_MAX;
    }
    
    return (0);
}

static int dac_port_async_convert(struct dac_driver_t *self_p,
                                  uint8_t *samples_p,
                                  size_t length)
{
    ASSERTN(length <= 2, EINVAL);
    
    esp_dac_out_voltage(self_p->pin0_channel, *samples_p++);

    if (self_p->pin1_channel != DAC_CHANNEL_MAX) {
        esp_dac_out_voltage(self_p->pin1_channel, *samples_p);
    }
    
    return (0);
}

static int dac_port_async_wait(struct dac_driver_t *self_p)
{
    return (0);
}

static int dac_port_convert(struct dac_driver_t *self_p,
                            uint8_t *samples_p,
                            size_t length)
{
    ASSERTN(length <= 2, EINVAL);

    dac_port_async_convert(self_p, samples_p, length);
    dac_port_async_wait(self_p);

    return (0);
}
