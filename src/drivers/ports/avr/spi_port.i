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

#include <avr/interrupt.h>

/**
 * SPI TX complete interrupt.
 */
ISR(SPI_STC_vect)
{
    struct spi_driver_t *drv_p = spi_device[0].drv_p;

    if (drv_p == NULL) {
        return;
    }

    /* Read incoming byte. */
    if (drv_p->rxbuf_p != NULL) {
        *drv_p->rxbuf_p++ = SPDR;
    }

    /* Resume on complete transfer. */
    if (drv_p->size == 0) {
        thrd_resume_isr(drv_p->thrd_p, 0);
    } else {
        /* Write next byte. */
        if (drv_p->txbuf_p != NULL) {
            SPDR = *drv_p->txbuf_p++;
        } else {
            SPDR = 0xff;
        }

        drv_p->size--;
    }
}

static int spi_port_module_init(void)
{
    return (0);
}

static int spi_port_init(struct spi_driver_t *self_p,
                         struct spi_device_t *dev_p,
                         struct pin_device_t *ss_pin_p,
                         int mode,
                         int speed,
                         int polarity,
                         int phase)
{
    self_p->spcr = (_BV(SPIE)
                    | _BV(SPE)
                    | (mode << 4)
                    | (polarity << 3)
                    | (phase << 2)
                    | (speed & 0x3));
    self_p->spsr = (speed >> 2);

    /* Pin initialization. */
    if (mode == SPI_MODE_MASTER) {
        pin_init(&self_p->sck, dev_p->sck_p, PIN_OUTPUT);
        pin_init(&self_p->mosi, dev_p->mosi_p, PIN_OUTPUT);
        pin_init(&self_p->miso, dev_p->miso_p, PIN_INPUT);
        pin_init(&self_p->ss, ss_pin_p, PIN_OUTPUT);
        pin_write(&self_p->ss, 1);
        /* The SS pin must be set as output and set to high in master
           mode, even if it is not used as select pin. */
        pin_init(&self_p->hw_ss, dev_p->ss_p, PIN_OUTPUT);
        pin_write(&self_p->hw_ss, 1);
    } else {
        pin_init(&self_p->sck, dev_p->sck_p, PIN_INPUT);
        pin_init(&self_p->mosi, dev_p->mosi_p, PIN_INPUT);
        pin_init(&self_p->miso, dev_p->miso_p, PIN_OUTPUT);
        pin_init(&self_p->ss, ss_pin_p, PIN_INPUT);
    }

    return (0);
}

static int spi_port_start(struct spi_driver_t *self_p)
{
    SPSR = self_p->spsr;
    SPCR = self_p->spcr;

    return (0);
}

static int spi_port_stop(struct spi_driver_t *self_p)
{
    return (0);
}

static ssize_t spi_port_transfer(struct spi_driver_t *self_p,
                                 void *rxbuf_p,
                                 const void *txbuf_p,
                                 size_t n)
{
    self_p->rxbuf_p = rxbuf_p;
    self_p->txbuf_p = txbuf_p;
    self_p->size = (n - 1);
    self_p->thrd_p = thrd_self();

    sys_lock();

    /* Write first byte. The rest are written from isr. */
    if (self_p->txbuf_p != NULL) {
        SPDR = *self_p->txbuf_p++;
    } else {
        SPDR = 0xff;
    }

    thrd_suspend_isr(NULL);

    sys_unlock();

    return (n);
}
