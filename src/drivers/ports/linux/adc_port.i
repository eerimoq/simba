/**
 * @file drivers/linux/adc_port.i
 * @version 0.3.0
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

static int adc_port_module_init(void)
{
    return (0);
}

static int adc_port_init(struct adc_driver_t *self_p,
                         struct adc_device_t *dev_p,
                         struct pin_device_t *pin_dev_p,
                         int reference,
                         int sampling_rate)
{
    return (0);
}

static int adc_port_async_convert(struct adc_driver_t *self_p,
                                  uint16_t *samples_p,
                                  size_t length)
{
    return (0);
}

static int adc_port_async_wait(struct adc_driver_t *self_p)
{
    return (0);
}
