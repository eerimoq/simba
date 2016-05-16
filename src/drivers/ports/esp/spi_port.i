/**
 * @file drivers/ports/esp/spi_port.i
 * @version 0.4.0
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

#include "esp_common.h"

static void start_block_transfer(struct spi_driver_t *drv_p)
{
    int i;

    for (i = 0; i < 2; i++) {
        drv_p->dev_p->regs_p->W_0_15[i] = *drv_p->txbuf_p++;
    }
}

/**
 * SPI interrupt handler.
 */
static void isr(void *arg_p)
{
    int i;
    struct spi_driver_t *drv_p = NULL;
    struct spi_device_t *dev_p = drv_p->dev_p;

    if (0) {
        return;
    }

    /* Read incoming byte(s). */
    if (drv_p->rxbuf_p != NULL) {
        for (i = 0; i < 4; i++) {
            *drv_p->rxbuf_p++ = dev_p->regs_p->W_0_15[i];
        }
    }

    /* Resume on complete transfer. */
    if (drv_p->size == 0) {
        thrd_resume_isr(drv_p->thrd_p, 0);
    } else {
        start_block_transfer(drv_p);
    }

    drv_p->size--;
}

static int spi_port_init(struct spi_driver_t *self_p,
                         struct spi_device_t *dev_p,
                         struct pin_device_t *ss_pin_p,
                         int mode,
                         int speed,
                         int cpol,
                         int cpha)
{
    /* Install the interrupt handler. */
    _xt_isr_attach(ESP8266_IRQ_NUM_SPI, (_xt_isr)isr, NULL);
    _xt_isr_unmask(1 << ESP8266_IRQ_NUM_SPI);

    return (0);
}

static int spi_port_start(struct spi_driver_t *self_p)
{
    /* Enable the transfer complete interrupt. */

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

    /* Write first block. The rest are written from isr. */
    start_block_transfer(self_p);

    thrd_suspend(NULL);

    return (n);
}
