/**
 * @file spi.c
 * @version 0.2.0
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

#include "simba.h"

#include "spi_port.i"

int spi_module_init(void)
{
    return (0);
}

int spi_init(struct spi_driver_t *self_p,
             struct spi_device_t *dev_p,
             struct pin_device_t *ss_pin_p,
             int mode,
             int speed,
             int cpol,
             int cpha)
{
    self_p->dev_p = dev_p;
    self_p->mode = mode;
    self_p->speed = speed;
    self_p->cpol = cpol;
    self_p->cpha = cpha;

    return (spi_port_init(self_p, dev_p, ss_pin_p, mode, speed, cpol, cpha));
}

ssize_t spi_transfer(struct spi_driver_t *self_p,
                     void *rxbuf_p,
                     const void *txbuf_p,
                     size_t size)
{
    sem_get(&self_p->dev_p->sem, NULL);

    /* Configure and start SPI hardware with driver configuration. */
    if (self_p->dev_p->drv_p != self_p) {
        if (self_p->dev_p->drv_p != NULL) {
            spi_port_stop(self_p->dev_p->drv_p);
        }

        self_p->dev_p->drv_p = self_p;
        spi_port_start(self_p);
    }

    if (self_p->mode == SPI_MODE_MASTER) {
        pin_write(&self_p->ss, 0);
    }

    size = spi_port_transfer(self_p, rxbuf_p, txbuf_p, size);

    if (self_p->mode == SPI_MODE_MASTER) {
        pin_write(&self_p->ss, 1);
    }

    sem_put(&self_p->dev_p->sem, 1);

    return (size);
}

ssize_t spi_read(struct spi_driver_t *self_p,
                 void *rxbuf_p,
                 size_t size)
{
    return (spi_transfer(self_p, rxbuf_p, NULL, size));
}

ssize_t spi_write(struct spi_driver_t *self_p,
                  const void *txbuf_p,
                  size_t size)
{
    return (spi_transfer(self_p, NULL, txbuf_p, size));
}

ssize_t spi_get(struct spi_driver_t *self_p,
                uint8_t *data_p)
{
    return (spi_read(self_p, data_p, 1));
}

ssize_t spi_put(struct spi_driver_t *self_p,
                uint8_t data)
{
    return (spi_write(self_p, &data, 1));
}
