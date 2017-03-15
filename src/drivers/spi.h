/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
 * Initialize SPI module. This function must be called before calling
 * any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int spi_module_init(void);

/**
 * Initialize driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] dev_p Device to use.
 * @param[in] ss_pin_p Slave select pin device.
 * @param[in] mode Master or slave mode.
 * @param[in] speed Speed in kbps.
 * @param[in] polarity Set to 0 or 1.
 * @param[in] phase Set to 0 or 1.
 *
 * @return zero(0) or negative error code.
 */
int spi_init(struct spi_driver_t *self_p,
             struct spi_device_t *dev_p,
             struct pin_device_t *ss_pin_p,
             int mode,
             int speed,
             int polarity,
             int phase);

/**
 * Start given SPI driver. Configures the SPI hardware.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int spi_start(struct spi_driver_t *self_p);

/**
 * Stop given SPI driver. Deconfigures the SPI hardware if given
 * driver currently ownes the bus.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int spi_stop(struct spi_driver_t *self_p);

/**
 * In multi master application the driver must take ownership of the
 * SPI bus before performing data transfers. Will re-configure the SPI
 * hardware if configured by another driver.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int spi_take_bus(struct spi_driver_t *self_p);

/**
 * In multi master application the driver must give ownership of the
 * SPI bus to let other masters take it.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int spi_give_bus(struct spi_driver_t *self_p);

/**
 * Select the slave by asserting the slave select pin.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int spi_select(struct spi_driver_t *self_p);

/**
 * Deselect the slave by de-asserting the slave select pin.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int spi_deselect(struct spi_driver_t *self_p);

/**
 * Simultaniuos read/write operation over the SPI bus.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] rxbuf_p Buffer to read into.
 * @param[in] txbuf_p Buffer to write.
 * @param[in] size Number of bytes to transfer.
 *
 * @return Number of transferred bytes or negative error code.
 */
ssize_t spi_transfer(struct spi_driver_t *self_p,
                     void *rxbuf_p,
                     const void *txbuf_p,
                     size_t size);

/**
 * Read data from the SPI bus.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t spi_read(struct spi_driver_t *self_p,
                 void *buf_p,
                 size_t size);

/**
 * Write data to the SPI bus.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t spi_write(struct spi_driver_t *self_p,
                  const void *buf_p,
                  size_t size);

/**
 * Get one byte of data from the SPI bus.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] data_p Read data.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t spi_get(struct spi_driver_t *self_p, uint8_t *data_p);

/**
 * Put one byte of data to the SPI bus.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] data data to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t spi_put(struct spi_driver_t *self_p, uint8_t data);

#endif
