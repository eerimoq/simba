/**
 * @file drivers/nrf24l01.h
 * @version 0.1.0
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

#ifndef __DRIVERS_NRF24L01_H__
#define __DRIVERS_NRF24L01_H__

#include "simba.h"

struct nrf24l01_driver_t {
    struct spi_driver_t spi;
    struct exti_driver_t exti;
    struct pin_driver_t ce;
    struct queue_t irqchan;
    struct queue_t chin;
    struct thrd_t *thrd_p;
    uint32_t address;
    char irqbuf[8];
    char chinbuf[32];
    char stack[256];
};

/**
 * Initialize NRF24L01 module.
 */
int nrf24l01_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] spi_p SPI device.
 * @param[in] cs_p Chip select pin device.
 * @param[in] ce_p CE pin device.
 * @param[in] exti_p External interrupt flagdevice.
 * @param[in] address 4 MSB:s of RX pipes. LSB is set
 *                    to 0 through 5 for the 6 pipes.
 *
 * @return zero(0) or negative error code.
 */
int nrf24l01_init(struct nrf24l01_driver_t *self_p,
                  struct spi_device_t *spi_p,
                  struct pin_device_t *cs_p,
                  struct pin_device_t *ce_p,
                  struct exti_device_t *exti_p,
                  uint32_t address);

/**
 * Starts the NRF24L01 device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int nrf24l01_start(struct nrf24l01_driver_t *self_p);

/**
 * Stops the NRF24L01 device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int nrf24l01_stop(struct nrf24l01_driver_t *self_p);

/**
 * Read data from the NRF24L01 device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read (must be 32).
 *
 * @return Number of received bytes or negative error code.
 */
ssize_t nrf24l01_read(struct nrf24l01_driver_t *self_p,
                      void *buf_p,
                      size_t size);

/**
 * Write data to the NRF24L01 device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] address 4 MSB:s of TX address.
 * @param[in] pipe LSB of TX address.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write (must be 32).
 *
 * @return number of sent bytes or negative error code.
 */
ssize_t nrf24l01_write(struct nrf24l01_driver_t *self_p,
                       uint32_t address,
                       uint8_t pipe,
                       const void *buf_p,
                       size_t size);

#endif
