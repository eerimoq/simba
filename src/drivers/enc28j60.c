/**
 * @file enc28j60.c
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
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

/* Register Masks*/
#define ADDR_MASK    0x1f
#define BANK_MASK    0x60
#define MAC_MII_MASK 0x80

/* Bank Common Registers*/
#define EIE       0x1b
#define EIR       0x1c
#define ESTAT     0x1d
#define ECON2     0x1e
#define ECON1     0x1f

/* Bank 0 Control Registers*/
#define ERDPTL    0x00
#define ERDPTH    0x01
#define EWRPTL    0x02
#define EWRPTH    0x03
#define ETXSTL    0x04
#define ETXSTH    0x05
#define ETXNDL    0x06
#define ETXNDH    0x07
#define ERXSTL    0x08
#define ERXSTH    0x09
#define ERXNDL    0x0a
#define ERXNDH    0x0b
#define ERXRDPTL  0x0c
#define ERXRDPTH  0x0d
#define ERXWRPTL  0x0e
#define ERXWRPTH  0x0f
#define EDMASTL   0x10
#define EDMASTH   0x11
#define EDMANDL   0x12
#define EDMANDH   0x13
#define EDMADSTL  0x14
#define EDMADSTH  0x15
#define EDMACSL   0x16
#define EDMACSH   0x17

/* Bank 1 Control Registers*/
#define EHT0      0x20
#define EHT1      0x21
#define EHT2      0x22
#define EHT3      0x23
#define EHT4      0x24
#define EHT5      0x25
#define EHT6      0x26
#define EHT7      0x27
#define EPMM0     0x28
#define EPMM1     0x29
#define EPMM2     0x2a
#define EPMM3     0x2b
#define EPMM4     0x2c
#define EPMM5     0x2d
#define EPMM6     0x2e
#define EPMM7     0x2f
#define EPMCSL    0x30
#define EPMCSH    0x31
#define EPMOL     0x34
#define EPMOH     0x35
#define EWOLIE    0x36
#define EWOLIR    0x37
#define ERXFCON   0x38
#define EPKTCNT   0x39

/* Bank 2 Control Register*/
#define MACON1    0xc0
#define MACON2    0xc1
#define MACON3    0xc2
#define MACON4    0xc3
#define MABBIPG   0xc4
#define MAIPGL    0xc6
#define MAIPGH    0xc7
#define MACLCON1  0xc8
#define MACLCON2  0xc9
#define MAMXFLL   0xca
#define MAMXFLH   0xcb
#define MAPHSUP   0xcd
#define MICON     0xd1
#define MICMD     0xd2
#define MIREGADR  0xd4
#define MIWRL     0xd6
#define MIWRH     0xd7
#define MIRDL     0xd8
#define MIRDH     0xd9

/* Bank 3 Control Registers*/
#define MAADR1    0xe0
#define MAADR0    0xe1
#define MAADR3    0xe2
#define MAADR2    0xe3
#define MAADR5    0xe4
#define MAADR4    0xe5
#define EBSTSD    0x66
#define EBSTCON   0x67
#define EBSTCSL   0x68
#define EBSTCSH   0x69
#define MISTAT    0xea
#define EREVID    0x72
#define ECOCON    0x75
#define EFLOCON   0x77
#define EPAUSL    0x78
#define EPAUSH    0x79

/* PHY Registers*/
#define PHCON1    0x00
#define PHSTAT1   0x01
#define PHHID1    0x02
#define PHHID2    0x03
#define PHCON2    0x10
#define PHSTAT2   0x11
#define PHIE      0x12
#define PHIR      0x13
#define PHLCON    0x14

/* ERXFCON bit definitions*/
#define UCEN      0x80
#define ANDOR     0x40
#define CRCEN     0x20
#define PMEN      0x10
#define MPEN      0x08
#define HTEN      0x04
#define MCEN      0x02
#define BCEN      0x01

/* EIE bit definitions*/
#define INTIE     0x80
#define PKTIE     0x40
#define DMAIE     0x20
#define LINKIE    0x10
#define TXIE      0x08
#define WOLIE     0x04
#define TXERIE    0x02
#define RXERIE    0x01

/* EIR bit definitions*/
#define PKTIF     0x40
#define DMAIF     0x20
#define LINKIF    0x10
#define TXIF      0x08
#define WOLIF     0x04
#define TXERIF    0x02
#define RXERIF    0x01

/* ESTAT bit definitions*/
#define INT       0x80
#define LATECOL   0x10
#define RXBUSY    0x04
#define TXABRT    0x02
#define CLKRDY    0x01

/* ECON2 bit definitions*/
#define AUTOINC   0x80
#define PKTDEC    0x40
#define PWRSV     0x20
#define VRPS      0x08

/* ECON1 bit definitions*/
#define TXRST     0x80
#define RXRST     0x40
#define DMAST     0x20
#define CSUMEN    0x10
#define TXRTS     0x08
#define RXEN      0x04
#define BSEL1     0x02
#define BSEL0     0x01

/* MACON1 bit definitions*/
#define LOOPBK    0x10
#define TXPAUS    0x08
#define RXPAUS    0x04
#define PASSALL   0x02
#define MARXEN    0x01

/* MACON2 bit definitions*/
#define MARST     0x80
#define RNDRST    0x40
#define MARXRST   0x08
#define RFUNRST   0x04
#define MATXRST   0x02
#define TFUNRST   0x01

/* MACON3 bit definitions*/
#define PADCFG2   0x80
#define PADCFG1   0x40
#define PADCFG0   0x20
#define TXCRCEN   0x10
#define PHDRLEN   0x08
#define HFRMLEN   0x04
#define FRMLNEN   0x02
#define FULDPX    0x01

/* MICMD bit definitions*/
#define MIISCAN   0x02
#define MIIRD     0x01

/* MISTAT bit definitions*/
#define NVALID    0x04
#define SCAN      0x02
#define BUSY      0x01

/* PHCON1 bit definitions*/
#define PRST      0x8000
#define PLOOPBK   0x4000
#define PPWRSV    0x0800
#define PDPXMD    0x0100

/* PHSTAT1 bit definitions*/
#define PFDPX     0x1000
#define PHDPX     0x0800
#define LLSTAT    0x0004
#define JBSTAT    0x0002

/* PHCON2 bit definitions*/
#define FRCLINK   0x4000
#define TXDIS     0x2000
#define JABBER    0x0400
#define HDLDIS    0x0100

/* Packet Control bit Definitions*/
#define PHUGEEN   0x08
#define PPADEN    0x04
#define PCRCEN    0x02
#define POVERRIDE 0x01

/* SPI OP codes.*/
#define RCR       0x00
#define RBM       0x3a
#define WCR       0x40
#define WBM       0x7a
#define BFS       0x80
#define BFC       0xa0
#define SC        0xff

/* Buffer*/
#define HWMEM_SIZE   (0x2000)
#define HWMEM_BEGIN  (0x0000)
#define HWMEM_END    (0x1fff)
#define RXBUF_SIZE   (0x0800)
#define RXBUF_BEGIN  (HWMEM_BEGIN)
#define RXBUF_END    (RXBUF_BEGIN + RXBUF_SIZE - 1)
#define TXBUF_SIZE   (HWMEM_SIZE - RXBUF_SIZE)
#define TXBUF_BEGIN  (RXBUF_END + 1)
#define TXBUF_END    (HWMEM_END)

#define RECEIVED_OK  0x80
#define CRC_SIZE     4

static void enc28j60_spi_write_read(struct spi_driver_t *drv_p,
                                   uint8_t byte0,
                                   void *buf_p,
                                   int size)
{
    spi_write(drv_p, &byte0, sizeof(byte0));

    if (size > 0) {
        spi_write(drv, buf, size);
    } else {
        spi_read(drv, buf, -size);
    }
}

static void enc28j60_spi_write(struct spi_driver_t *spi_p,
                              uint8_t byte0,
                              const void *buf_p,
                              size_t size)
{
    enc28j60_spi_write_read(spi_p, byte0, (void *)buf, size);
}

static void enc28j60_spi_read(struct spi_driver_t *spi_p,
                              uint8_t byte0,
                              void *buf_p,
                              size_t size)
{
    enc28j60_spi_write_read(spi_p, byte0, buf, -(int16_t)size);
}

static void enc28j60_write_set_ctrl_reg(struct spi_driver_t *spi_p,
                                        uint8_t addr,
                                        uint8_t bits,
                                        uint8_t op)
{
    uint8_t bank = ((addr & BANK_MASK) >> 5);

    enc28j60_spi_write(spi_p, (BFS | ECON1), &bank, sizeof(bank));
    enc28j60_spi_write(spi_p, (op | (addr & ADDR_MASK)), &bits, sizeof(bits));
    enc28j60_spi_write(spi_p, (BFC | ECON1), &bank, sizeof(bank));
}

static void enc28j60_set_ctrl_reg(struct spi_driver_t *drv_p,
                                  uint8_t addr,
                                  uint8_t bits)
{
    enc28j60_write_set_ctrl_reg(drv_p, addr, bits, BFS);
}

static void enc28j60_write_ctrl_reg(struct spi_driver_t *spi_p,
                                    uint8_t addr,
                                    uint8_t value)
{
    enc28j60_write_set_ctrl_reg(spi_p, addr, value, WCR);
}

static void enc28j60_write_ctrl_reg_pair(struct spi_driver_t *spi_p,
                                         uint8_t addr,
                                         uint16_t value)
{
    enc28j60_write_set_ctrl_reg(spi_p, addr, value, WCR);
    enc28j60_write_set_ctrl_reg(spi_p, addr + 1, value >> 8, WCR);
}

static uint8_t enc28j60_read_ctrl_reg(struct spi_driver_t *spi_p,
                                      uint8_t addr)
{
    uint8_t value[2];
    uint8_t offset = ((addr & MAC_MII_MASK) != 0);
    uint8_t bank = ((addr & BANK_MASK) >> 5);

    enc28j60_spi_write(spi, (BFS | ECON1), &bank, sizeof(bank));
    enc28j60_spi_read(spi, (RCR | (addr & ADDR_MASK)), value, 1 + offset);
    enc28j60_spi_write(spi, (BFC | ECON1), &bank, sizeof(bank));

    return (value[offset]);
}

static void enc28j60_write_phy(struct spi_driver_t *spi_p,
                               uint8_t addr,
                               uint16_t data)
{
    enc28j60_write_ctrl_reg(spi_p, MIREGADR, addr);
    enc28j60_write_ctrl_reg_pair(spi_p, MIWRL, data);

    while (enc28j60_read_ctrl_reg(spi_p, MISTAT) & BUSY) {
    }
}

int enc28j60_init(struct enc28j60_driver_t *drv_p,
                  struct spi_driver_t *spi_p,
                  struct exti_driver_t *exti_p,
                  const uint8_t* macaddr_p,
                  void (*on_interrupt)(struct enc28j60_driver_t *drv_p,
                                       uint8_t status))
{
    uint8_t bits, sc;
    int i;
    static const FAR struct {
        uint8_t addr;
        uint8_t value;
    } init[17] = {
        /* RX & TX buffer initialization. */
        { ERXSTL, RXBUF_BEGIN & 0xff },
        { ERXSTH, RXBUF_BEGIN >> 8 },
        { ERXRDPTL, RXBUF_BEGIN & 0xff },
        { ERXRDPTH, RXBUF_BEGIN >> 8 },
        { ERXNDL, RXBUF_END & 0xff },
        { ERXNDH, RXBUF_END >> 8 },
        { ETXSTL, TXBUF_BEGIN & 0xff },
        { ETXSTH, TXBUF_BEGIN >> 8 },
        { ETXNDL, TXBUF_END & 0xff },
        { ETXNDH, TXBUF_END >> 8 },

        /* Receive filter control. */
        { ERXFCON, (UCEN | CRCEN | PMEN) },

        /* Enable receive and pause frames for fullduplex flow control. */
        { MACON1, (TXPAUS | RXPAUS | MARXEN) },

        /* Pull MAC out of reset. */
        { MACON2, 0 },

        /* Pad short packets. */
        { MACON3, (PADCFG0 | TXCRCEN | FRMLNEN) },

        /* Inter-packet gap. */
        { MABBIPG, 0x12 },
        { MAIPGL, 0x12 },
        { MAIPGH, 0x0c },
    };

    drv_p->spi_p = spi_p;
    drv_p->on_interrupt = on_interrupt;

    /* Use bank 0 as default. */
    bits = (BSEL1 | BSEL0);
    enc28j60_spi_write(spi_p, (BFC | ECON1), &bits, sizeof(bits));

    /* System reset. */
    sc = SC;
    spi_write(spi_p, &sc, sizeof(sc));

    /* Loop for smaller memory footprint. */
    for (i = 0; i < membersof(init); i++) {
        enc28j60_write_ctrl_reg(spi_p, init[i].addr, init[i].value);
    }

    /* HW MAC adress. */
    enc28j60_write_ctrl_reg_pair(spi_p,
                                 MAADR5,
                                 (((uint16_t)macaddr[1] << 8) | macaddr[0]));
    enc28j60_write_ctrl_reg_pair(spi_p,
                                 MAADR3,
                                 (((uint16_t)macaddr[3] << 8) | macaddr[2]));
    enc28j60_write_ctrl_reg_pair(spi_p,
                                 MAADR1,
                                 (((uint16_t)macaddr[5] << 8) | macaddr[4]));

    /* PHY configuration. */
    enc28j60_write_phy(spi_p, PHCON2, HDLDIS);

    return (0);
}

int enc28j60_start(struct enc28j60_driver_t *drv_p)
{
    /* Enable RX packet interrupt. */
    enc28j60_set_ctrl_reg(drv_p->spi_p, EIE, (INTIE | PKTIE));
    enc28j60_set_ctrl_reg(drv_p->spi_p, ECON1, RXEN);

    spi_start(drv_p->spi_p);
    exti_start(drv_p->exti);

    return (0);
}

int enc28j60_stop(struct enc28j60_driver_t *drv_p)
{
    exti_stop(drv_p->exti);
    spi_stop(drv_p->spi_p);

    return (0);
}

/**
 * Packet layout in enc28j60:
 *   [0-1]: low and high byte of next packet pointer
 *   [2-5]: packet receive status vector
 *      [0-15]: total frame length
 *   [6-m]: packet data (DMAC, SMAC, type/length, payload, padding, CRC)
 */
size_t enc28j60_read_begin(struct enc28j60_driver_t *drv_p)
{
    struct {
        uint16_t next_packet_addr;
        uint16_t size;
        uint16_t status;
    } header;

    /* Set read address.*/
    enc28j60_write_ctrl_reg_pair(drv_p->spi_p, ERDPTL, drv_p->next_packet_addr);

    /* Read header bytes 0-5.*/
    enc28j60_spi_read(drv_p->spi_p, RBM, &header, sizeof(header));
    drv_p->next_packet_addr = header.next_packet_addr;

    if ((header.status & RECEIVED_OK) == 0) {
        return (0);
    }

    return (header.size - CRC_SIZE);
}

size_t enc28j60_read(struct enc28j60_driver_t *drv_p, void *buf_p, size_t size)
{
    enc28j60_spi_read(drv_p->spi_p, RBM, buf, size);

    return (size);
}

void enc28j60_read_end(struct enc28j60_driver_t *drv_p)
{
    enc28j60_write_ctrl_reg_pair(drv_p->spi_p, ERXRDPTL, drv_p->next_packet_addr);
    enc28j60_set_ctrl_reg(drv_p->spi_p, ECON2, PKTDEC);
}

/**
 * Packet layout in enc28j60:
 *   [0]: control
 *   [1-m]: packet data (DMAC, SMAC, type/length, payload)
 *   [m+1-m+7]: status vector
 */
void enc28j60_write_begin(struct enc28j60_driver_t *drv_p)
{
    uint8_t control = 0;

    /* Wait for previous transmission to end.*/
    while (enc28j60_read_ctrl_reg(drv_p->spi_p, ECON1) & TXRTS) {
    }

    enc28j60_write_ctrl_reg_pair(drv_p->spi_p, EWRPTL, TXBUF_BEGIN);
    enc28j60_spi_write(drv_p->spi_p, WBM, &control, sizeof(control));
    drv_p->txsize = 0;
}

size_t enc28j60_write(struct enc28j60_driver_t *drv_p,
                      const void *buf_p,
                      size_t size)
{
    enc28j60_spi_write(drv_p->spi_p, WBM, buf, size);
    drv_p->txsize += size;

    return (size);
}

void enc28j60_write_end(struct enc28j60_driver_t *drv_p)
{
    enc28j60_write_ctrl_reg_pair(drv_p->spi_p, ETXNDL, TXBUF_BEGIN + drv_p->txsize);

    /* Start transmission.*/
    enc28j60_set_ctrl_reg(drv_p->spi_p, ECON1, TXRTS);
}

/* void enc28j60_isr(EXTDriver *extp, expchannel_t channel) */
/* { */
/*     (void)extp; */
/*     (void)channel; */
/*     drv_p->on_interrupt(0); */
/* } */

uint8_t enc28j60_get_pkt_cnt(struct enc28j60_driver_t *drv_p)
{
    return (enc28j60_read_ctrl_reg(drv_p->spi_p, EPKTCNT));
}
