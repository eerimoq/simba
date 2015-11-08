/**
 * @file dac.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#include "simba.h"

#include "dac_port.i"

int dac_module_init(void)
{
    return (dac_port_module_init());
}

int dac_init(struct dac_driver_t *drv_p,
             struct dac_device_t *dev_p,
             struct pin_device_t *pin_dev_p,
             int sampling_rate)
{
    return (dac_port_init(drv_p,
                          dev_p,
                          pin_dev_p,
                          sampling_rate));
}

int dac_async_convert(struct dac_driver_t *drv_p,
                      int *samples,
                      size_t length)
{
    return (dac_port_async_convert(drv_p, samples, length));
}

int dac_async_wait(struct dac_driver_t *drv_p)
{
    return (dac_port_async_wait(drv_p));
}

int dac_convert(struct dac_driver_t *drv_p,
                int *samples,
                size_t length)
{
    return (dac_port_convert(drv_p, samples, length));
}
