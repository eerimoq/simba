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

#include "freertos/FreeRTOS.h"

/**
 * Start a block transfer of up to 64 bytes.
 */
static void start_block_transfer_isr(struct spi_driver_t *drv_p)
{
    size_t i;
    size_t size;
    uint32_t *txbuf_p;

    /* Block size. */
    if (drv_p->size > 64) {
        size = 64;
    } else {
        size = drv_p->size;
    }

    drv_p->block_size = size;
    drv_p->size -= size;

    /* Set number of bits to transfer. */
    drv_p->dev_p->regs_p->USER1 =
        (ESP8266_SPI_USER1_MISO_BITLEN(8 * size - 1)
         | ESP8266_SPI_USER1_MOSI_BITLEN(8 * size - 1));

    /* Fill the transmit buffer. */
    if (drv_p->txbuf_p != NULL) {
        txbuf_p = (uint32_t *)drv_p->txbuf_p;

        for (i = 0; i < (size + 3) / 4; i++) {
            drv_p->dev_p->regs_p->W_0_15[i] = *txbuf_p++;
        }

        drv_p->txbuf_p += size;
    } else {
        for (i = 0; i < (size + 3) / 4; i++) {
            drv_p->dev_p->regs_p->W_0_15[i] = 0xffffffff;
        }
    }

    /* Start the transfer. */
    drv_p->dev_p->regs_p->CMD = ESP8266_SPI_CMD_USR;
}

/**
 * SPI interrupt handler. It handles interrupts for all SPI
 * peripherals. Currently only HSPI is implemented.
 */
static void isr(void *arg_p)
{
    int i;
    struct spi_device_t *dev_p = &spi_device[0];
    struct spi_driver_t *drv_p = dev_p->drv_p;
    uint8_t *buf_p;

    /* Clear the interrupt flags. */
    dev_p->regs_p->SLAVE &= 0xffffffe0;

    /* Read incoming byte(s). */
    if (drv_p->rxbuf_p != NULL) {
        buf_p = (uint8_t *)(&dev_p->regs_p->W_0_15[0]);

        for (i = 0; i < drv_p->block_size; i++) {
            *drv_p->rxbuf_p++ = *buf_p++;
        }
    }

    /* Resume the waiting thread when the transfer is complete. */
    if (drv_p->size == 0) {
        thrd_resume_isr(drv_p->thrd_p, 0);
    } else {
        start_block_transfer_isr(drv_p);
    }
}

static int spi_port_module_init(void)
{
    /* Disable interrupts for SPI0 and SPI1. */
    ESP8266_SPI0->SLAVE = 0;
    ESP8266_SPI1->SLAVE = 0;

    /* Install the interrupt handler. */
    _xt_isr_attach(ESP8266_IRQ_NUM_SPI, (_xt_isr)isr, NULL);
    _xt_isr_unmask(1 << ESP8266_IRQ_NUM_SPI);

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
    /* Configure the SPI pins (12, 13 and 14). */
    ESP8266_IOMUX->PIN[0] = ESP8266_IOMUX_PIN_FUNC(2);
    ESP8266_IOMUX->PIN[1] = ESP8266_IOMUX_PIN_FUNC(2);
    ESP8266_IOMUX->PIN[2] = ESP8266_IOMUX_PIN_FUNC(2);

    /* Should be moved to the start function. */
    if (mode == SPI_MODE_MASTER) {
        pin_init(&self_p->ss, ss_pin_p, PIN_OUTPUT);
        pin_write(&self_p->ss, 1);
    } else {
        return (-1);
    }

    return (0);
}

static int spi_port_start(struct spi_driver_t *self_p)
{
    volatile struct esp8266_spi_t *regs_p = self_p->dev_p->regs_p;

    regs_p->CTRL = 0;
    regs_p->SLAVE = ESP8266_SPI_SLAVE_INT_EN_TRANS_DONE;
    ESP8266_IOMUX->CONF &= ~ESP8266_IOMUX_CONF_SPI0_CLK_EQU_SYS_CLK;
    regs_p->CLOCK = self_p->speed;
    regs_p->USER = (ESP8266_SPI_USER_MOSI | 1);

    return (0);
}

static int spi_port_stop(struct spi_driver_t *self_p)
{
    volatile struct esp8266_spi_t *regs_p = self_p->dev_p->regs_p;

    regs_p->SLAVE = 0;

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

    /* Write first block. The rest are written from the isr. */
    sys_lock();
    start_block_transfer_isr(self_p);
    thrd_suspend_isr(NULL);
    sys_unlock();

    return (n);
}
