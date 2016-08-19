/**
 * @file drivers/ports/esp/spi_port.h
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

/* Speed configuration (the clock register value). */
#define SPI_PORT_SPEED_8MBPS    0x0101001
#define SPI_PORT_SPEED_4MBPS    0x0241001
#define SPI_PORT_SPEED_2MBPS    0x04c1001
#define SPI_PORT_SPEED_1MBPS    0x09c1001
#define SPI_PORT_SPEED_500KBPS  0x13c1001
#define SPI_PORT_SPEED_250KBPS  0x27c1001
#define SPI_PORT_SPEED_125KBPS  0x4fc1001

struct spi_driver_t;

struct spi_device_t {
    struct spi_driver_t *drv_p;
    volatile struct esp8266_spi_t *regs_p;
    struct sem_t sem;
};

struct spi_driver_t {
    struct spi_device_t *dev_p;
    struct pin_driver_t ss;
    int mode;
    int speed;
    int cpol;
    int cpha;
    uint8_t *rxbuf_p;               /* Transfer receive buffer or NULL. */
    const uint8_t *txbuf_p;         /* Transfer transmit buffer or NULL. */
    size_t size;                    /* Number of bytes left to transfer. */
    size_t block_size;
    struct thrd_t *thrd_p;          /* Waiting thread. */
};

#endif
