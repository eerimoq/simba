/**
 * @file drivers/linux/spi_port.i
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

#include <avr/interrupt.h>

/**
 * SPI TX complete interrupt.
 */
ISR(SPI_STC_vect)
{
    struct spi_driver_t *drv_p = spi_device[0].drv_p;

    /* Read incoming byte. */
    if (drv_p->rxbuf_p != NULL) {
        *drv_p->rxbuf_p++ = SPDR;
    }

    /* Resume on complete transfer. */
    if (drv_p->size == 0) {
        thrd_resume_irq(drv_p->thrd_p, 0);
    } else {
        /* Write next byte. */
        if (drv_p->txbuf_p != NULL) {
            SPDR = *drv_p->txbuf_p++;
        } else {
            SPDR = 0;
        }
        
        drv_p->size--;
    }
}

static int spi_port_init(struct spi_driver_t *drv_p,
                         struct spi_device_t *dev_p,
                         struct pin_device_t *ss_pin_p,
                         int mode,
                         int speed,
                         int cpol,
                         int cpha)
{
    drv_p->spcr = (_BV(SPIE) |
                   _BV(SPE) |
                   (mode << 4) |
                   (cpol << 3) |
                   (cpha << 2) |
                   (speed & 0x3));
    drv_p->spsr = (speed >> 2);

    /* Pin initialization. */
    if (mode == SPI_MODE_MASTER) {
        pin_init(&drv_p->sck, dev_p->sck_p, PIN_OUTPUT);
        pin_init(&drv_p->mosi, dev_p->mosi_p, PIN_OUTPUT);
        pin_init(&drv_p->miso, dev_p->miso_p, PIN_INPUT);
        pin_init(&drv_p->ss, ss_pin_p, PIN_OUTPUT);
        pin_write(&drv_p->ss, 1);
    } else {
        pin_init(&drv_p->sck, dev_p->sck_p, PIN_INPUT);
        pin_init(&drv_p->mosi, dev_p->mosi_p, PIN_INPUT);
        pin_init(&drv_p->miso, dev_p->miso_p, PIN_OUTPUT);
        pin_init(&drv_p->ss, ss_pin_p, PIN_INPUT);
    }

    return (0);
}

static int spi_port_start(struct spi_driver_t *drv_p)
{
    SPSR = drv_p->spsr;
    SPCR = drv_p->spcr;

    return (0);
}

static int spi_port_stop(struct spi_driver_t *drv_p)
{
    return (0);
}

static ssize_t spi_port_transfer(struct spi_driver_t *drv_p,
                                 void *rxbuf_p,
                                 const void *txbuf_p,
                                 size_t n)
{
    drv_p->rxbuf_p = rxbuf_p;
    drv_p->txbuf_p = txbuf_p;
    drv_p->size = (n - 1);
    drv_p->thrd_p = thrd_self();

    /* Write first byte. The rest are written from isr. */
    if (drv_p->txbuf_p != NULL) {
        SPDR = *drv_p->txbuf_p++;
    } else {
        SPDR = 0;
    }

    thrd_suspend(NULL);

    return (n);
}
