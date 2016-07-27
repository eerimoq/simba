/**
 * @file drivers/spi_port.h
 * @version 3.1.1
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

#ifndef __DRIVERS_SPI_PORT_H__
#define __DRIVERS_SPI_PORT_H__

#include <io.h>

/* Speed configuration is not used in this port. */
#define SPI_PORT_SPEED_8MBPS    0
#define SPI_PORT_SPEED_4MBPS    0
#define SPI_PORT_SPEED_2MBPS    0
#define SPI_PORT_SPEED_1MBPS    0
#define SPI_PORT_SPEED_500KBPS  0
#define SPI_PORT_SPEED_250KBPS  0
#define SPI_PORT_SPEED_125KBPS  0

struct spi_driver_t;

struct spi_device_t {
    struct spi_driver_t *drv_p;
    struct sem_t sem;
};

struct spi_driver_t {
    struct spi_device_t *dev_p;
    struct pin_driver_t ss;
    int mode;
    int speed;
    int cpol;
    int cpha;
};

#endif
