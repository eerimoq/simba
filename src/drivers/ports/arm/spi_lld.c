/**
 * @file drivers/sh/spi_port.c
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

#include "drivers/spi.h"
#include <stdint.h>

struct spi_device_t spi_device[SPI_DEVICE_MAX];

int spi_module_init()
{
    return (0);
}

int spi_init(struct spi_driver_t *drv,
             struct spi_device_t *dev,
             const struct spi_config_t *config)
{
    drv_p->dev = dev;
    return (0);
}

int spi_start(struct spi_driver_t *drv)
{
    return (0);
}

int spi_stop(struct spi_driver_t *drv)
{
    return (0);
}

void spi_select(struct spi_driver_t *drv)
{
}

void spi_unselect(struct spi_driver_t *drv)
{
}

ssize_t spi_transfer(struct spi_driver_t *drv,
                     void *rxbuf,
                     const void *txbuf,
                     size_t n)
{
    return (n);
}

ssize_t spi_write(struct spi_driver_t *drv,
                  const void *txbuf,
                  size_t n)
{
    return (spi_transfer(drv, NULL, txbuf, n));
}

ssize_t spi_read(struct spi_driver_t *drv,
                 void *rxbuf,
                 size_t n)
{
    return (spi_transfer(drv, rxbuf, NULL, n));
}
