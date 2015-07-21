/**
 * @file drivers/spi.h
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

#ifndef __DRIVERS_SPI_H__
#define __DRIVERS_SPI_H__

#include "simba.h"

#include "spi_port.h"

/* SPI modes. */
#define SPI_MODE_SLAVE  0
#define SPI_MODE_MASTER 1

/* SPI speeds. */
#define SPI_SPEED_8MBPS SPI_PORT_SPEED_8MBPS
#define SPI_SPEED_4MBPS SPI_PORT_SPEED_4MBPS
#define SPI_SPEED_2MBPS SPI_PORT_SPEED_2MBPS
#define SPI_SPEED_1MBPS SPI_PORT_SPEED_1MBPS
#define SPI_SPEED_500KBPS SPI_PORT_SPEED_500KBPS
#define SPI_SPEED_250KBPS SPI_PORT_SPEED_250KBPS
#define SPI_SPEED_125KBPS SPI_PORT_SPEED_125KBPS

extern struct spi_device_t spi_device[SPI_DEVICE_MAX];

/**
 * Initialize SPI module.
 */
int spi_module_init(void);

/**
 * Initialize driver object.
 * @param[out] drv_p Driver object to initialize.
 * @param[in] dev_p Device to use.
 * @param[in] ss_pin_p Slave select pin device.
 * @param[in] mode Master or slave mode.
 * @param[in] speed Speed in kbps.
 * @param[in] cpol Polarity, 0 or 1.
 * @param[in] cpha Phase, 0 or 1.
 * @return zero(0) or negative error code.
 */
int spi_init(struct spi_driver_t *drv_p,
             struct spi_device_t *dev_p,
             struct pin_device_t *ss_pin_p,
             int mode,
             int speed,
             int cpol,
             int cpha);

/**
 * Simultaniuos read/write operation over the SPI bus.
 * @param[in] drv_p Initialized driver object.
 * @param[in] rxbuf_p Buffer to read into.
 * @param[in] txbuf_p Buffer to write.
 * @param[in] size Number of bytes to transfer.
 * @return Number of transferred bytes or negative error code.
 */
ssize_t spi_transfer(struct spi_driver_t *drv_p,
                     void *rxbuf_p,
                     const void *txbuf_p,
                     size_t size);

/**
 * Read data from the SPI bus.
 * @param[in] drv_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 * @return Number of read bytes or negative error code.
 */
ssize_t spi_read(struct spi_driver_t *drv_p,
                 void *buf_p,
                 size_t size);

/**
 * Write data to the SPI bus.
 * @param[in] drv_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 * @return Number of written bytes or negative error code.
 */
ssize_t spi_write(struct spi_driver_t *drv_p,
                  const void *buf_p,
                  size_t size);

#endif
