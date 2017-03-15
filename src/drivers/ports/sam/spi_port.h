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

#ifndef __DRIVERS_SPI_PORT_H__
#define __DRIVERS_SPI_PORT_H__

#include <io.h>

/* Speed configuration. SPI2X, SPR1 and SPR0 */
#define SPI_PORT_SPEED_8MBPS    (F_CPU / 8000000)
#define SPI_PORT_SPEED_4MBPS    (F_CPU / 4000000)
#define SPI_PORT_SPEED_2MBPS    (F_CPU / 2000000)
#define SPI_PORT_SPEED_1MBPS    (F_CPU / 1000000)
#define SPI_PORT_SPEED_500KBPS  (F_CPU /  500000)
#define SPI_PORT_SPEED_250KBPS  (F_CPU /  250000)
#define SPI_PORT_SPEED_125KBPS  (F_CPU /  120000)

struct spi_driver_t;

struct spi_device_t {
    struct spi_driver_t *drv_p;
    volatile struct sam_spi_t *regs_p;
    struct pin_device_t *mosi_p;
    struct pin_device_t *miso_p;
    struct pin_device_t *sck_p;
    int id;
    struct sem_t sem;
};

struct spi_driver_t {
    struct spi_device_t *dev_p;
    struct pin_driver_t ss;
    int mode;
    int speed;
    int polarity;
    int phase;
    uint8_t *rxbuf_p;                        /* Transfer receive buffer or NULL. */
    const uint8_t *txbuf_p;                  /* Transfer transmit buffer or NULL. */
    size_t size;                             /* Number of bytes left to transfer. */
    struct thrd_t *thrd_p;                   /* Waiting thread. */
};

#endif
