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

#include "simba.h"

#if CONFIG_NRF24L01 == 1

/* Maximum packet payload size. */
#define PAYLOAD_MAX 32

/* SPI commands. */
#define SPI_CMD_R_REGISTER          0x00
#define SPI_CMD_W_REGISTER          0x20
#define SPI_CMD_R_RX_PAYLOAD        0x61
#define SPI_CMD_W_TX_PAYLOAD        0xa0
#define SPI_CMD_FLUSH_TX            0xe1
#define SPI_CMD_FLUSH_RX            0xe2
#define SPI_CMD_REUSE_TX_PL         0xe3
#define SPI_CMD_R_RX_PL_WID         0x60
#define SPI_CMD_W_ACK_PAYLOAD       0xa8
#define SPI_CMD_W_TX_PAYLOAD_NO_ACK 0xb0
#define SPI_CMD_NOP                 0xff

/* Registers. */
#define REG_CONFIG        0x00
#define REG_EN_AA         0x01
#define REG_EN_RXADDR     0x02
#define REG_SETUP_AW      0x03
#define REG_SETUP_RETR    0x04
#define REG_RF_CH         0x05
#define REG_RF_SETUP      0x06
#define REG_STATUS        0x07
#define REG_OBSERVE_TX    0x08
#define REG_RPD           0x09
#define REG_RX_ADDR_P0    0x0a
#define REG_RX_ADDR_P1    0x0b
#define REG_RX_ADDR_P2    0x0c
#define REG_RX_ADDR_P3    0x0d
#define REG_RX_ADDR_P4    0x0e
#define REG_RX_ADDR_P5    0x0f
#define REG_TX_ADDR       0x10
#define REG_RX_PW_P0      0x11
#define REG_RX_PW_P1      0x12
#define REG_RX_PW_P2      0x13
#define REG_RX_PW_P3      0x14
#define REG_RX_PW_P4      0x15
#define REG_RX_PW_P5      0x16
#define REG_FIFO_STATUS   0x17
#define REG_DYNPD         0x1c
#define REG_FEATURE       0x1d

#define REG_CONFIG_EN_CRC   0x08
#define REG_CONFIG_CRCO     0x04
#define REG_CONFIG_PWR_UP   0x02
#define REG_CONFIG_PRIM_RX  0x01

#define REG_SETUP_AW_3BYTES 0x01
#define REG_SETUP_AW_4BYTES 0x02
#define REG_SETUP_AW_5BYTES 0x03

#define REG_STATUS_RX_DR    0x40
#define REG_STATUS_TX_DS    0x20

static void isr(void *arg_p)
{
    char c;
    struct nrf24l01_driver_t *self_p = arg_p;

    queue_write_isr(&self_p->irqchan, &c, sizeof(c));
}

static void *isr_main(void *arg_p)
{
    char c;
    struct nrf24l01_driver_t *self_p = arg_p;
    uint8_t status;
    uint8_t buf[33];

    while (1) {
        /* Wait for interrupt. */
        queue_read(&self_p->irqchan, &c, sizeof(c));

        /* Read status. */
        status = SPI_CMD_NOP;
        spi_transfer(&self_p->spi, &status, &status, sizeof(status));

        /* Handle packet reception. */
        if (status & REG_STATUS_RX_DR) {
            buf[0] = SPI_CMD_R_RX_PAYLOAD;
            spi_transfer(&self_p->spi, buf, buf, sizeof(buf));
            queue_write(&self_p->chin, &buf[1], 32);
        }

        /* Handle packet transmission completion. */
        if (status & REG_STATUS_TX_DS) {
            thrd_resume(self_p->thrd_p, 0);
        }

        /* Clear interrupt flags. */
        buf[0] = (SPI_CMD_W_REGISTER | REG_STATUS);
        buf[1] = (REG_STATUS_RX_DR | REG_STATUS_TX_DS);
        spi_write(&self_p->spi, buf, 2);
    }

    return (NULL);
}

int nrf24l01_module_init(void)
{
    return (0);
}

int nrf24l01_init(struct nrf24l01_driver_t *self_p,
                  struct spi_device_t *spi_p,
                  struct pin_device_t *cs_p,
                  struct pin_device_t *ce_p,
                  struct exti_device_t *exti_p,
                  uint32_t address)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(spi_p != NULL, EINVAL);
    ASSERTN(cs_p != NULL, EINVAL);
    ASSERTN(ce_p != NULL, EINVAL);
    ASSERTN(exti_p != NULL, EINVAL);

    self_p->address = address;

    queue_init(&self_p->irqchan, self_p->irqbuf, sizeof(self_p->irqbuf));
    queue_init(&self_p->chin, self_p->chinbuf, sizeof(self_p->chinbuf));

    thrd_spawn(isr_main,
               self_p,
               0,
               self_p->stack,
               sizeof(self_p->stack));

    spi_init(&self_p->spi,
             spi_p,
             cs_p,
             SPI_MODE_MASTER,
             SPI_SPEED_250KBPS,
             0,
             0);

    exti_init(&self_p->exti,
              exti_p,
              EXTI_TRIGGER_FALLING_EDGE,
              isr,
              self_p);
    exti_start(&self_p->exti);

    pin_init(&self_p->ce, ce_p, PIN_OUTPUT);

    return (0);
}

int nrf24l01_start(struct nrf24l01_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    uint8_t buf[6];
    int i;

    spi_start(&self_p->spi);
    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    /* Use 4 bytes address. */
    buf[0] = (SPI_CMD_W_REGISTER | REG_SETUP_AW);
    buf[1] = REG_SETUP_AW_5BYTES;
    spi_write(&self_p->spi, buf, 2);

    /* Disable acknoledgements. */
    buf[0] = (SPI_CMD_W_REGISTER | REG_EN_AA);
    buf[1] = 0;
    spi_write(&self_p->spi, buf, 2);

    /* Set RX address for pipes. */
    for (i = 0; i < 2; i++) {
        buf[0] = (SPI_CMD_W_REGISTER | (REG_RX_ADDR_P0 + i));
        buf[1] = self_p->address >> 24;
        buf[2] = self_p->address >> 16;
        buf[3] = self_p->address >> 8;
        buf[4] = self_p->address;
        buf[5] = i;
        spi_write(&self_p->spi, buf, 6);
    }

    for (; i < 6; i++) {
        /* Set RX address for pipe. */
        buf[0] = (SPI_CMD_W_REGISTER |
                  (REG_RX_ADDR_P0 + i));
        buf[1] = i;
        spi_write(&self_p->spi, buf, 2);
    }

    /* Receive 32 bytes. */
    for (i = 0; i < 6; i++) {
        buf[0] = (SPI_CMD_W_REGISTER | (REG_RX_PW_P0 + i));
        buf[1] = 32;
        spi_write(&self_p->spi, buf, 2);
    }

    /* Enable RX pipes. */
    buf[0] = (SPI_CMD_W_REGISTER | REG_EN_RXADDR);
    buf[1] = 0x3f;
    spi_write(&self_p->spi, buf, 2);

    /* Power up. */
    pin_write(&self_p->ce, 0);

    buf[0] = (SPI_CMD_W_REGISTER | REG_CONFIG);
    buf[1] = (REG_CONFIG_EN_CRC |
              REG_CONFIG_CRCO |
              REG_CONFIG_PWR_UP);
    spi_write(&self_p->spi, buf, 2);

    time_busy_wait_us(3000);

    /* Clear status flags. */
    buf[0] = (SPI_CMD_W_REGISTER | REG_STATUS);
    buf[1] = (REG_STATUS_RX_DR | REG_STATUS_TX_DS);
    spi_write(&self_p->spi, buf, 2);

    /* Flush TX and RX fifos. */
    buf[0] = SPI_CMD_FLUSH_TX;
    spi_write(&self_p->spi, buf, 1);

    buf[0] = SPI_CMD_FLUSH_RX;
    spi_write(&self_p->spi, buf, 1);

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    return (0);
}

int nrf24l01_stop(struct nrf24l01_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (0);
}

ssize_t nrf24l01_read(struct nrf24l01_driver_t *self_p,
                      void *buf_p,
                      size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    uint8_t buf[2];

    ASSERT(size == PAYLOAD_MAX);

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    /* Setup RX mode. */
    buf[0] = (SPI_CMD_W_REGISTER | REG_CONFIG);
    buf[1] = (REG_CONFIG_EN_CRC |
              REG_CONFIG_CRCO |
              REG_CONFIG_PWR_UP |
              REG_CONFIG_PRIM_RX);
    spi_write(&self_p->spi, buf, sizeof(buf));

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    /* Activate RX mode. */
    pin_write(&self_p->ce, 1);

    /* Wait for packet. */
    return (queue_read(&self_p->chin, buf_p, size));
}

ssize_t nrf24l01_write(struct nrf24l01_driver_t *self_p,
                       uint32_t address,
                       uint8_t pipe,
                       const void *buf_p,
                       size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    uint8_t buf[33];

    ASSERT(size == PAYLOAD_MAX);

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    /* Set TX address. */
    buf[0] = (SPI_CMD_W_REGISTER | REG_TX_ADDR);
    buf[1] = address >> 24;
    buf[2] = address >> 16;
    buf[3] = address >> 8;
    buf[4] = address;
    buf[5] = pipe;
    spi_write(&self_p->spi, buf, 6);

    /* Write payload. */
    buf[0] = SPI_CMD_W_TX_PAYLOAD;
    memcpy(&buf[1], buf_p, size);
    spi_write(&self_p->spi, buf, 33);

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    self_p->thrd_p = thrd_self();

    /* Pulse CE high to start transmission. */
    pin_write(&self_p->ce, 1);
    time_busy_wait_us(15);
    pin_write(&self_p->ce, 0);

    /* Wait for completion. */
    thrd_suspend(NULL);

    return (size);
}

#endif
