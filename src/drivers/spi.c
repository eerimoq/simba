/**
 * @file spi.c
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

#include "spi_port.i"

int spi_module_init()
{
    return (0);
}

int spi_init(struct spi_driver_t *drv_p,
             struct spi_device_t *dev_p,
             struct pin_device_t *ss_pin_p,
             int mode,
             int speed,
             int cpol,
             int cpha)
{
    drv_p->dev_p = dev_p;
    drv_p->mode = mode;

    return (spi_port_init(drv_p, dev_p, ss_pin_p, mode, speed, cpol, cpha));
}

ssize_t spi_transfer(struct spi_driver_t *drv_p,
                     void *rxbuf_p,
                     const void *txbuf_p,
                     size_t size)
{
    sem_get(&drv_p->dev_p->sem, NULL);

    /* Configure and start SPI hardware with driver configuration. */
    if (drv_p->dev_p->drv_p != drv_p) {
        if (drv_p->dev_p->drv_p != NULL) {
            spi_port_stop(drv_p->dev_p->drv_p);
        }

        drv_p->dev_p->drv_p = drv_p;
        spi_port_start(drv_p);
    }

    if (drv_p->mode == SPI_MODE_MASTER) {
        pin_write(&drv_p->ss, 0);
    }

    size = spi_port_transfer(drv_p, rxbuf_p, txbuf_p, size);

    if (drv_p->mode == SPI_MODE_MASTER) {
        pin_write(&drv_p->ss, 1);
    }

    sem_put(&drv_p->dev_p->sem, 1);

    return (size);
}

ssize_t spi_write(struct spi_driver_t *drv_p,
                  const void *txbuf_p,
                  size_t size)
{
    return (spi_transfer(drv_p, NULL, txbuf_p, size));
}

ssize_t spi_read(struct spi_driver_t *drv_p,
                 void *rxbuf_p,
                 size_t size)
{
    return (spi_transfer(drv_p, rxbuf_p, NULL, size));
}
