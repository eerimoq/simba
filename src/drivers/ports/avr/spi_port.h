/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
    struct pin_device_t *mosi_p;
    struct pin_device_t *miso_p;
    struct pin_device_t *sck_p;
    struct pin_device_t *ss_p;
    struct sem_t sem;
};

struct spi_driver_t {
    struct spi_device_t *dev_p;
    struct pin_driver_t hw_ss;
    struct pin_driver_t mosi;
    struct pin_driver_t miso;
    struct pin_driver_t sck;
    struct pin_driver_t ss;
    int mode;
    int speed;
    int polarity;
    int phase;
    uint8_t spcr;
    uint8_t spsr;
    uint8_t *rxbuf_p;                        /* Transfer receive buffer or NULL. */
    const uint8_t *txbuf_p;                  /* Transfer transmit buffer or NULL. */
    size_t size;                             /* Number of bytes left to transfer. */
    struct thrd_t *thrd_p;                   /* Waiting thread. */
};

#endif
