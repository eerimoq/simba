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

#undef BIT
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_intr.h"
#include "soc/dport_reg.h"

/**
 * Start a block transfer of up to 64 bytes.
 */
static void start_block_transfer_isr(struct spi_driver_t *drv_p)
{
    size_t i;
    size_t size;
    uint32_t *txbuf_p;
    volatile struct esp32_spi_t *regs_p;

    regs_p = drv_p->dev_p->regs_p;

    /* Block size. */
    if (drv_p->size > sizeof(regs_p->DATA)) {
        size = sizeof(regs_p->DATA);
    } else {
        size = drv_p->size;
    }

    drv_p->block_size = size;
    drv_p->size -= size;

    /* Set number of bits to transfer. */
    regs_p->MOSI_DLEN = (8 * size - 1);
    regs_p->MISO_DLEN = (8 * size - 1);

    /* Fill the transmit buffer. */
    if (drv_p->txbuf_p != NULL) {
        txbuf_p = (uint32_t *)drv_p->txbuf_p;

        for (i = 0; i < DIV_CEIL(size, 4); i++) {
            regs_p->DATA[i] = *txbuf_p++;
        }

        drv_p->txbuf_p += size;
    } else {
        for (i = 0; i < DIV_CEIL(size, 4); i++) {
            regs_p->DATA[i] = 0xffffffff;
        }
    }

    /* Start the transfer. */
    regs_p->COMMAND = ESP32_SPI_COMMAND_USR;
}

static void isr(void *arg_p)
{
    struct spi_device_t *dev_p;
    struct spi_driver_t *drv_p;
    volatile struct esp32_spi_t *regs_p;
    uint8_t *buf_p;
    int i, j;

    for (i = 0; i < membersof(spi_device); i++) {
        dev_p = &spi_device[i];
        drv_p = dev_p->drv_p;

        /* Skip devices without active transfers. */
        if ((drv_p == NULL) || (drv_p->thrd_p == NULL)) {
            continue;
        }

        regs_p = dev_p->regs_p;

        /* Clear the interrupt flags. */
        regs_p->SLAVE &= 0xffffffe0;

        /* Read incoming byte(s). */
        if (drv_p->rxbuf_p != NULL) {
            buf_p = (uint8_t *)(&regs_p->DATA[0]);

            for (j = 0; j < drv_p->block_size; j++) {
                *drv_p->rxbuf_p++ = *buf_p++;
            }
        }

        /* Resume on complete transfer. */
        if (drv_p->size == 0) {
            thrd_resume_isr(drv_p->thrd_p, 0);
            drv_p->thrd_p = NULL;
        } else {
            start_block_transfer_isr(drv_p);
        }
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
    if (mode == SPI_MODE_MASTER) {
        pin_init(&self_p->ss, ss_pin_p, PIN_OUTPUT);
        pin_write(&self_p->ss, 1);
    } else {
        pin_init(&self_p->ss, ss_pin_p, PIN_INPUT);
    }

    return (0);
}

static int spi_port_start(struct spi_driver_t *self_p)
{
    struct spi_device_t *dev_p;
    volatile struct esp32_spi_t *regs_p;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;

    /* Configure pins for direct IO, bypassing the GPIO matrix for
       now. */

    /* Configure MISO pin. */
    ESP32_IO_MUX->PIN[dev_p->miso_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_FUNC2
                                               | ESP32_IO_MUX_PIN_FUNC_IE);
    ESP32_GPIO->FUNC_IN_SEL_CFG[dev_p->gpio.miso_func_in_sel] =
        ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL;

    /* Configure MOSI pin. */
    ESP32_IO_MUX->PIN[dev_p->mosi_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_FUNC2
                                               | ESP32_IO_MUX_PIN_FUNC_IE);
    ESP32_GPIO->FUNC_IN_SEL_CFG[dev_p->gpio.mosi_func_in_sel] =
        ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL;

    /* Configure SCK pin. */
    ESP32_IO_MUX->PIN[dev_p->sck_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_FUNC2
                                              | ESP32_IO_MUX_PIN_FUNC_IE);
    ESP32_GPIO->FUNC_IN_SEL_CFG[dev_p->gpio.sck_func_in_sel] =
        ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL;

    /* Enable the device. */
    SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, dev_p->dport_mask);
    CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, dev_p->dport_mask);

    /* Set mode. */
    if (self_p->mode == SPI_MODE_MASTER) {
        regs_p->SLAVE = 0;
    } else {
        regs_p->SLAVE = ESP32_SPI_SLAVE_SLAVE_MODE;
    }

    /* Phase and polarity. */
    regs_p->PIN = (ESP32_SPI_PIN_CK_IDLE_EDGE * self_p->polarity);
    regs_p->USER = (ESP32_SPI_USER_CK_OUT_EDGE * self_p->phase);

    /* Speed. */
    regs_p->CLOCK = self_p->speed;

    /* Duplex data transfer. */
    regs_p->USER |= (ESP32_SPI_USER_USR_MOSI
                     | ESP32_SPI_USER_USR_MISO
                     | ESP32_SPI_USER_DOUTDIN);

    /* Install the interrupt handler. */
    esp_xt_set_interrupt_handler(dev_p->interrupt.cpu, isr, NULL);
    esp_xt_ints_on(BIT(dev_p->interrupt.cpu));

    /* Map the SPI peripheral interrupt to the SPI CPU interrupt. */
    intr_matrix_set(xPortGetCoreID(),
                    dev_p->interrupt.source,
                    dev_p->interrupt.cpu);

    /* Enable interrupts. */
    regs_p->SLAVE |= ESP32_SPI_SLAVE_INT_EN_TRANS_DONE;

    return (0);
}

static int spi_port_stop(struct spi_driver_t *self_p)
{
    struct spi_device_t *dev_p = self_p->dev_p;

    /* Disable the device. */
    CLEAR_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, dev_p->dport_mask);
    SET_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, dev_p->dport_mask);

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

    sys_lock();
    start_block_transfer_isr(self_p);
    thrd_suspend_isr(NULL);
    sys_unlock();

    return (n);
}
