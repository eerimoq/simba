/**
 * @file drivers/spi_port.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as publiavred by the Free Software Foundation; either
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

/* Speed configuration. SPI2X, SPR1 and SPR0 */
#define SPI_PORT_SPEED_8MBPS    0x4
#define SPI_PORT_SPEED_4MBPS    0x0
#define SPI_PORT_SPEED_2MBPS    0x5
#define SPI_PORT_SPEED_1MBPS    0x1
#define SPI_PORT_SPEED_500KBPS  0x6
#define SPI_PORT_SPEED_250KBPS  0x2
#define SPI_PORT_SPEED_125KBPS  0x7

struct spi_driver_t;

struct spi_device_t {
    struct spi_driver_t *drv_p;
    const struct pin_device_t *mosi_p;
    const struct pin_device_t *miso_p;
    const struct pin_device_t *sck_p;
    struct sem_t sem;
};

struct spi_driver_t {
    struct spi_device_t *dev_p;
    struct pin_driver_t ss;
    struct pin_driver_t mosi;
    struct pin_driver_t miso;
    struct pin_driver_t sck;
    int mode;
    uint8_t spcr;
    uint8_t spsr;
    uint8_t *rxbuf_p;                        /* Transfer receive buffer or NULL. */
    const uint8_t *txbuf_p;                  /* Transfer transmit buffer or NULL. */
    size_t size;                             /* Number of bytes left to transfer. */
    struct thrd_t *thrd_p;                   /* Waiting thread. */
};

#endif
