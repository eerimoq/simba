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

/* Speed configuration. */
#define SPI_PORT_SPEED_8MBPS    (0x00101001)
#define SPI_PORT_SPEED_4MBPS    (0x00241001)
#define SPI_PORT_SPEED_2MBPS    (0x004c1001)
#define SPI_PORT_SPEED_1MBPS    (0x009c1001)
#define SPI_PORT_SPEED_500KBPS  (0x013c1001)
#define SPI_PORT_SPEED_250KBPS  (0x027c1001)
#define SPI_PORT_SPEED_125KBPS  (0x04fc1001)

struct spi_driver_t;

struct spi_device_t {
    struct spi_driver_t *drv_p;
    volatile struct esp32_spi_t *regs_p;
    uint32_t dport_mask;
    struct {
        int miso_func_in_sel;
        int mosi_func_in_sel;
        int sck_func_in_sel;
    } gpio;
    struct pin_device_t *mosi_p;
    struct pin_device_t *miso_p;
    struct pin_device_t *sck_p;
    struct pin_device_t *ss_p;
    struct {
        int source;
        int cpu;
    } interrupt;
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
    size_t size;
    size_t block_size;
    struct thrd_t *thrd_p;                   /* Waiting thread. */
};

#endif
