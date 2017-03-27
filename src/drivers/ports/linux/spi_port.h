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
    int polarity;
    int phase;
};

#endif
