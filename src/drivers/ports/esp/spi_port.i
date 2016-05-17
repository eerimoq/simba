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

/**
 * Start a block transfer of up to 64 bytes.
 */
static void start_block_transfer_isr(struct spi_driver_t *drv_p)
{
    size_t i;
    size_t size;

    /* Block size. */
    if (drv_p->size > 64) {
        size = 64;
    } else {
        size = drv_p->size;
    }

    drv_p->block_size = size;
    drv_p->size -= size;

    /* Set bits to transfer. */
    drv_p->dev_p->regs_p->USER1 =
        (SPI_USER1_REG_USR_MISO_BITLEN(8 * size)
         | SPI_USER1_REG_USR_MOSI_BITLEN(8 * size));

    /* Fill the transmit buffer. */
    if (drv_p->txbuf_p != NULL) {
        for (i = 0; i < (size + 3) / 4; i++) {
            drv_p->dev_p->regs_p->W_0_15[i] = *drv_p->txbuf_p++;
        }
    } else {
        for (i = 0; i < (size + 3) / 4; i++) {
            drv_p->dev_p->regs_p->W_0_15[i] = 0xffffffff;
        }
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
    uint8_t *buf_p;

    /* Read incoming byte(s). */
    if (drv_p->rxbuf_p != NULL) {
        buf_p = (uint8_t *)(&dev_p->regs_p->W_0_15[0]);

        for (i = 0; i < drv_p->block_size; i++) {
            *drv_p->rxbuf_p++ = *buf_p++;
        }
    }

    /* Resume on complete transfer. */
    if (drv_p->size == 0) {
        thrd_resume_isr(drv_p->thrd_p, 0);
    } else {
        start_block_transfer_isr(drv_p);
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

    if (mode == SPI_MODE_MASTER) {
        dev_p->regs_p->USER = (SPI_USER_SPI_USR_MOSI);
    } else {
        return (-1);
    }

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
    self_p->size = n;
    self_p->thrd_p = thrd_self();

    /* Write first block. The rest are written from isr. */
    sys_lock();
    start_block_transfer_isr(self_p);
    thrd_suspend_isr(NULL);
    sys_unlock();

    return (n);
}
