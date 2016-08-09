/**
 * @file drivers/esp/adc_port.i
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "esp_system.h"

static int adc_port_module_init(void)
{
    return (0);
}

static int adc_port_init(struct adc_driver_t *self_p,
                         struct adc_device_t *dev_p,
                         struct pin_device_t *pin_dev_p,
                         int reference,
                         long sampling_rate)
{
    return (0);
}

static int adc_port_async_convert(struct adc_driver_t *self_p,
                                  uint16_t *samples_p,
                                  size_t length)
{
    self_p->samples_p = samples_p;
    self_p->length = length;

    return (0);
}

static int adc_port_async_wait(struct adc_driver_t *self_p)
{
    size_t i;

    for (i = 0; i < self_p->length; i++) {
        self_p->samples_p[i] = system_adc_read();
    }

    return (1);
}

int adc_port_convert_isr(struct adc_driver_t *self_p,
                         uint16_t *sample_p)
{
    return (-1);
}
