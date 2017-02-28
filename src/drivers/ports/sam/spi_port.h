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
