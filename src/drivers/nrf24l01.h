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
 * Initialize NRF24L01 module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
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
