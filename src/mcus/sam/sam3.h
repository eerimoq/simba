/**
 * @file sam3.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015, Erik Moqvist
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

#ifndef __SAM3_H__
#define __SAM3_H__

/* See "SAM3X / SAM3A Series, Atmel | SMART ARM-based MCU, DATASHEET"
   for details. */

#define BIT(n) (0x1 << (n))

#define BITFIELD(name, value) (((value) << name ## _POS) & name ## _MASK)

/* Peripheral identifiers. */
#define PERIPHERAL_ID_SUPC              0 /* Supply Controller */
#define PERIPHERAL_ID_RSTC              1 /* Reset Controller */
#define PERIPHERAL_ID_RTC               2 /* Real-time Clock */
#define PERIPHERAL_ID_RTT               3 /* Real-time Timer */
#define PERIPHERAL_ID_WDG               4 /* Watchdog Timer */
#define PERIPHERAL_ID_PMC               5 /* Power Management Controller */
#define PERIPHERAL_ID_EEFC0             6 /* Enhanced Embedded Flash Controller 0 */
#define PERIPHERAL_ID_EEFC1             7 /* Enhanced Embedded Flash Controller 1 */
#define PERIPHERAL_ID_UART              8 /* Universal Asynchronous Receiver Transceiver */
#define PERIPHERAL_ID_SMC_SDRAMC        9 /* Static Memory Controller / Synchronous Dynamic RAM Controller */
#define PERIPHERAL_ID_SDRAMC           10 /* Synchronous Dynamic RAM Controller */
#define PERIPHERAL_ID_PIOA             11 /* Parallel I/O Controller A */
#define PERIPHERAL_ID_PIOB             12 /* Parallel I/O Controller B */
#define PERIPHERAL_ID_PIOC             13 /* Parallel I/O Controller C */
#define PERIPHERAL_ID_PIOD             14 /* Parallel I/O Controller D */
#define PERIPHERAL_ID_PIOE             15 /* Parallel I/O Controller E */
#define PERIPHERAL_ID_PIOF             16 /* Parallel I/O Controller F */
#define PERIPHERAL_ID_USART0           17 /* Universal Synchronous Asynchronous Receiver Transmitter 0 */
#define PERIPHERAL_ID_USART1           18 /* Universal Synchronous Asynchronous Receiver Transmitter 1 */
#define PERIPHERAL_ID_USART2           19 /* Universal Synchronous Asynchronous Receiver Transmitter 2 */
#define PERIPHERAL_ID_USART3           20 /* Universal Synchronous Asynchronous Receiver Transmitter 3 */
#define PERIPHERAL_ID_HSMCI            21 /* High Speed Multimedia Card Interface */
#define PERIPHERAL_ID_TWI0             22 /* Two-Wire Interface 0 */
#define PERIPHERAL_ID_TWI1             23 /* Two-Wire Interface 1 */
#define PERIPHERAL_ID_SPI0             24 /* Serial Peripheral Interface 0 */
#define PERIPHERAL_ID_SPI1             25 /* Serial Peripheral Interface 1 */
#define PERIPHERAL_ID_SSC              26 /* Synchronous Serial Controller */
#define PERIPHERAL_ID_TC0              27 /* Timer Counter Channel 0 */
#define PERIPHERAL_ID_TC1              28 /* Timer Counter Channel 1 */
#define PERIPHERAL_ID_TC2              29 /* Timer Counter Channel 2 */
#define PERIPHERAL_ID_TC3              30 /* Timer Counter Channel 3 */
#define PERIPHERAL_ID_TC4              31 /* Timer Counter Channel 4 */
#define PERIPHERAL_ID_TC5              32 /* Timer Counter Channel 5 */
#define PERIPHERAL_ID_TC6              33 /* Timer Counter Channel 6 */
#define PERIPHERAL_ID_TC7              34 /* Timer Counter Channel 7 */
#define PERIPHERAL_ID_TC8              35 /* Timer Counter Channel 8 */
#define PERIPHERAL_ID_PWM              36 /* Pulse Width Modulation Controller */
#define PERIPHERAL_ID_ADC              37 /* ADC Controller */
#define PERIPHERAL_ID_DACC             38 /* DAC Controller */
#define PERIPHERAL_ID_DMAC             39 /* DMA Controller */
#define PERIPHERAL_ID_UOTGHS           40 /* USB OTG High Speed */
#define PERIPHERAL_ID_TRNG             41 /* True Random Number Generator */
#define PERIPHERAL_ID_EMAC             42 /* Ethernet MAC */
#define PERIPHERAL_ID_CAN0             43 /* CAN Controller 0 */
#define PERIPHERAL_ID_CAN1             44 /* CAN Controller 1 */

/*AHB DMA Controller. */
struct sam_dmac_t {
    uint32_t GCFG;
    uint32_t EN;
    uint32_t SREQ;
    uint32_t CREQ;
    uint32_t LAST;
    uint32_t reserved1;
    uint32_t EBCIER;
    uint32_t EBCIDR;
    uint32_t EBCIMR;
    uint32_t EBCISR;
    uint32_t CHER;
    uint32_t CHDR;
    uint32_t CHSR;
    uint32_t reserved2[2];
    struct {
        uint32_t SADDR;
        uint32_t DADDR;
        uint32_t DSCR;
        uint32_t CTRLA;
        uint32_t CTRLB;
        uint32_t CFG;
        uint32_t reserved1[4];
    } channel[6];
    uint32_t WPMR;
    uint32_t WPSR;
};

/* DMAC Global Configuration Register. */
#define DMAC_GCFG_ARB_CFG               BIT(4)

/* DMAC Enable register. */
#define DMAC_EN_ENABLE                  BIT(0)

/* DMAC Software Single Request Register. */
#define DMAC_SREQ_SSREQ0                BIT(0)
#define DMAC_SREQ_DSREQ0                BIT(1)
#define DMAC_SREQ_SSREQ1                BIT(2)
#define DMAC_SREQ_DSREQ1                BIT(3)
#define DMAC_SREQ_SSREQ2                BIT(4)
#define DMAC_SREQ_DSREQ2                BIT(5)
#define DMAC_SREQ_SSREQ3                BIT(6)
#define DMAC_SREQ_DSREQ3                BIT(7)
#define DMAC_SREQ_SSREQ4                BIT(8)
#define DMAC_SREQ_DSREQ4                BIT(9)
#define DMAC_SREQ_SSREQ5                BIT(10)
#define DMAC_SREQ_DSREQ5                BIT(11)

/* DMAC Software Chunk Transfer Request Register. */
#define DMAC_CREQ_SCREQ0                BIT(0)
#define DMAC_CREQ_DCREQ0                BIT(1)
#define DMAC_CREQ_SCREQ1                BIT(2)
#define DMAC_CREQ_DCREQ1                BIT(3)
#define DMAC_CREQ_SCREQ2                BIT(4)
#define DMAC_CREQ_DCREQ2                BIT(5)
#define DMAC_CREQ_SCREQ3                BIT(6)
#define DMAC_CREQ_DCREQ3                BIT(7)
#define DMAC_CREQ_SCREQ4                BIT(8)
#define DMAC_CREQ_DCREQ4                BIT(9)
#define DMAC_CREQ_SCREQ5                BIT(10)
#define DMAC_CREQ_DCREQ5                BIT(11)

/* DMAC Software Last Transfer Flag Register. */
#define DMAC_LAST_SLAST0                BIT(0)
#define DMAC_LAST_DLAST0                BIT(1)
#define DMAC_LAST_SLAST1                BIT(2)
#define DMAC_LAST_DLAST1                BIT(3)
#define DMAC_LAST_SLAST2                BIT(4)
#define DMAC_LAST_DLAST2                BIT(5)
#define DMAC_LAST_SLAST3                BIT(6)
#define DMAC_LAST_DLAST3                BIT(7)
#define DMAC_LAST_SLAST4                BIT(8)
#define DMAC_LAST_DLAST4                BIT(9)
#define DMAC_LAST_SLAST5                BIT(10)
#define DMAC_LAST_DLAST5                BIT(11)

/* DMAC Error, Buffer Transfer and Chained Buffer Transfer Interrupt
 * Enable Register. */
#define DMAC_EBCIER_BTC0                BIT(0)
#define DMAC_EBCIER_BTC1                BIT(1)
#define DMAC_EBCIER_BTC2                BIT(2)
#define DMAC_EBCIER_BTC3                BIT(3)
#define DMAC_EBCIER_BTC4                BIT(4)
#define DMAC_EBCIER_BTC5                BIT(5)
#define DMAC_EBCIER_CBTC0               BIT(8)
#define DMAC_EBCIER_CBTC1               BIT(9)
#define DMAC_EBCIER_CBTC2               BIT(10)
#define DMAC_EBCIER_CBTC3               BIT(11)
#define DMAC_EBCIER_CBTC4               BIT(12)
#define DMAC_EBCIER_CBTC5               BIT(13)
#define DMAC_EBCIER_ERR0                BIT(16)
#define DMAC_EBCIER_ERR1                BIT(17)
#define DMAC_EBCIER_ERR2                BIT(18)
#define DMAC_EBCIER_ERR3                BIT(19)
#define DMAC_EBCIER_ERR4                BIT(20)
#define DMAC_EBCIER_ERR5                BIT(21)

/* DMAC Error, Buffer Transfer and Chained Buffer Transfer Interrupt
 * Disable Register. */
#define DMAC_EBCIDR_BTC0                BIT(0)
#define DMAC_EBCIDR_BTC1                BIT(1)
#define DMAC_EBCIDR_BTC2                BIT(2)
#define DMAC_EBCIDR_BTC3                BIT(3)
#define DMAC_EBCIDR_BTC4                BIT(4)
#define DMAC_EBCIDR_BTC5                BIT(5)
#define DMAC_EBCIDR_CBTC0               BIT(8)
#define DMAC_EBCIDR_CBTC1               BIT(9)
#define DMAC_EBCIDR_CBTC2               BIT(10)
#define DMAC_EBCIDR_CBTC3               BIT(11)
#define DMAC_EBCIDR_CBTC4               BIT(12)
#define DMAC_EBCIDR_CBTC5               BIT(13)
#define DMAC_EBCIDR_ERR0                BIT(16)
#define DMAC_EBCIDR_ERR1                BIT(17)
#define DMAC_EBCIDR_ERR2                BIT(18)
#define DMAC_EBCIDR_ERR3                BIT(19)
#define DMAC_EBCIDR_ERR4                BIT(20)
#define DMAC_EBCIDR_ERR5                BIT(21)

/* DMAC Error, Buffer Transfer and Chained Buffer Transfer Interrupt
 * Mask Register. */
#define DMAC_EBCIMR_BTC0                BIT(0)
#define DMAC_EBCIMR_BTC1                BIT(1)
#define DMAC_EBCIMR_BTC2                BIT(2)
#define DMAC_EBCIMR_BTC3                BIT(3)
#define DMAC_EBCIMR_BTC4                BIT(4)
#define DMAC_EBCIMR_BTC5                BIT(5)
#define DMAC_EBCIMR_CBTC0               BIT(8)
#define DMAC_EBCIMR_CBTC1               BIT(9)
#define DMAC_EBCIMR_CBTC2               BIT(10)
#define DMAC_EBCIMR_CBTC3               BIT(11)
#define DMAC_EBCIMR_CBTC4               BIT(12)
#define DMAC_EBCIMR_CBTC5               BIT(13)
#define DMAC_EBCIMR_ERR0                BIT(16)
#define DMAC_EBCIMR_ERR1                BIT(17)
#define DMAC_EBCIMR_ERR2                BIT(18)
#define DMAC_EBCIMR_ERR3                BIT(19)
#define DMAC_EBCIMR_ERR4                BIT(20)
#define DMAC_EBCIMR_ERR5                BIT(21)

/* DMAC Error, Buffer Transfer and Chained Buffer Transfer Interrupt
 * Status Register. */
#define DMAC_EBCISR_BTC0                BIT(0)
#define DMAC_EBCISR_BTC1                BIT(1)
#define DMAC_EBCISR_BTC2                BIT(2)
#define DMAC_EBCISR_BTC3                BIT(3)
#define DMAC_EBCISR_BTC4                BIT(4)
#define DMAC_EBCISR_BTC5                BIT(5)
#define DMAC_EBCISR_CBTC0               BIT(8)
#define DMAC_EBCISR_CBTC1               BIT(9)
#define DMAC_EBCISR_CBTC2               BIT(10)
#define DMAC_EBCISR_CBTC3               BIT(11)
#define DMAC_EBCISR_CBTC4               BIT(12)
#define DMAC_EBCISR_CBTC5               BIT(13)
#define DMAC_EBCISR_ERR0                BIT(16)
#define DMAC_EBCISR_ERR1                BIT(17)
#define DMAC_EBCISR_ERR2                BIT(18)
#define DMAC_EBCISR_ERR3                BIT(19)
#define DMAC_EBCISR_ERR4                BIT(20)
#define DMAC_EBCISR_ERR5                BIT(21)

/* DMAC Channel Handler Enable Register */
#define DMAC_CHER_ENA0                  BIT(0)
#define DMAC_CHER_ENA1                  BIT(1)
#define DMAC_CHER_ENA2                  BIT(2)
#define DMAC_CHER_ENA3                  BIT(3)
#define DMAC_CHER_ENA4                  BIT(4)
#define DMAC_CHER_ENA5                  BIT(5)
#define DMAC_CHER_SUSP0                 BIT(8)
#define DMAC_CHER_SUSP1                 BIT(9)
#define DMAC_CHER_SUSP2                 BIT(10)
#define DMAC_CHER_SUSP3                 BIT(11)
#define DMAC_CHER_SUSP4                 BIT(12)
#define DMAC_CHER_SUSP5                 BIT(13)
#define DMAC_CHER_KEEP0                 BIT(24)
#define DMAC_CHER_KEEP1                 BIT(25)
#define DMAC_CHER_KEEP2                 BIT(26)
#define DMAC_CHER_KEEP3                 BIT(27)
#define DMAC_CHER_KEEP4                 BIT(28)
#define DMAC_CHER_KEEP5                 BIT(29)

/* DMAC Channel Handler Disable Register */
#define DMAC_CHDR_DIS0                  BIT(0)
#define DMAC_CHDR_DIS1                  BIT(1)
#define DMAC_CHDR_DIS2                  BIT(2)
#define DMAC_CHDR_DIS3                  BIT(3)
#define DMAC_CHDR_DIS4                  BIT(4)
#define DMAC_CHDR_DIS5                  BIT(5)
#define DMAC_CHDR_RES0                  BIT(8)
#define DMAC_CHDR_RES1                  BIT(9)
#define DMAC_CHDR_RES2                  BIT(10)
#define DMAC_CHDR_RES3                  BIT(11)
#define DMAC_CHDR_RES4                  BIT(12)
#define DMAC_CHDR_RES5                  BIT(13)

/* DMAC Channel Handler Status Register */
#define DMAC_CHSR_ENA0                  BIT(0)
#define DMAC_CHSR_ENA1                  BIT(1)
#define DMAC_CHSR_ENA2                  BIT(2)
#define DMAC_CHSR_ENA3                  BIT(3)
#define DMAC_CHSR_ENA4                  BIT(4)
#define DMAC_CHSR_ENA5                  BIT(5)
#define DMAC_CHSR_SUSP0                 BIT(8)
#define DMAC_CHSR_SUSP1                 BIT(9)
#define DMAC_CHSR_SUSP2                 BIT(10)
#define DMAC_CHSR_SUSP3                 BIT(11)
#define DMAC_CHSR_SUSP4                 BIT(12)
#define DMAC_CHSR_SUSP5                 BIT(13)
#define DMAC_CHSR_EMPT0                 BIT(16)
#define DMAC_CHSR_EMPT1                 BIT(17)
#define DMAC_CHSR_EMPT2                 BIT(18)
#define DMAC_CHSR_EMPT3                 BIT(19)
#define DMAC_CHSR_EMPT4                 BIT(20)
#define DMAC_CHSR_EMPT5                 BIT(21)
#define DMAC_CHSR_STAL0                 BIT(24)
#define DMAC_CHSR_STAL1                 BIT(25)
#define DMAC_CHSR_STAL2                 BIT(26)
#define DMAC_CHSR_STAL3                 BIT(27)
#define DMAC_CHSR_STAL4                 BIT(28)
#define DMAC_CHSR_STAL5                 BIT(29)

/* DMAC Channel x [x = 0..5] Control A Register */
#define DMAC_CTRLA_BTSIZE_POS           (0)
#define DMAC_CTRLA_BTSIZE_MASK          (0xffff << DMAC_CTRLA_BTSIZE_POS)
#define DMAC_CTRLA_BTSIZE(value)        BITFIELD(DMAC_CTRLA_BTSIZE, (value))
#define DMAC_CTRLA_SCSIZE_POS           (16)
#define DMAC_CTRLA_SCSIZE_MASK          (0x7 << DMAC_CTRLA_SCSIZE_POS)
#define DMAC_CTRLA_SCSIZE(value)        BITFIELD(DMAC_CTRLA_SCSIZE, (value))
#define DMAC_CTRLA_DCSIZE_POS           (20)
#define DMAC_CTRLA_DCSIZE_MASK          (0x7 << DMAC_CTRLA_DCSIZE_POS)
#define DMAC_CTRLA_DCSIZE(value)        BITFIELD(DMAC_CTRLA_DCSIZE, (value))
#define DMAC_CTRLA_SRC_WIDTH_POS        (24)
#define DMAC_CTRLA_SRC_WIDTH_MASK       (0x3 << DMAC_CTRLA_SRC_WIDTH_POS)
#define DMAC_CTRLA_SRC_WIDTH(value)     BITFIELD(DMAC_CTRLA_SRC_WIDTH, (value))
#define DMAC_CTRLA_DST_WIDTH_POS        (28)
#define DMAC_CTRLA_DST_WIDTH_MASK       (0x3 << DMAC_CTRLA_DST_WIDTH_POS)
#define DMAC_CTRLA_DST_WIDTH(value)     BITFIELD(DMAC_CTRLA_DST_WIDTH, (value))
#define DMAC_CTRLA_DONE                 BIT(31)

/* DMAC Channel x [x = 0..5] Control B Register */
#define DMAC_CTRLB_SRC_DSCR             (16)
#define DMAC_CTRLB_DST_DSCR             (20)
#define DMAC_CTRLB_FC_POS               (21)
#define DMAC_CTRLB_FC_MASK              (0x7 << DMAC_CTRLB_FC_POS)
#define DMAC_CTRLB_FC(value)            BITFIELD(DMAC_CTRLB_FC, (value))
#define DMAC_CTRLB_SRC_INCR_POS         (24)
#define DMAC_CTRLB_SRC_INCR_MASK        (0x3 << DMAC_CTRLB_SRC_INCR_POS)
#define DMAC_CTRLB_SRC_INCR(value)      BITFIELD(DMAC_CTRLB_SRC_INCR, (value))
#define DMAC_CTRLB_DST_INCR_POS         (28)
#define DMAC_CTRLB_DST_INCR_MASK        (0x3 << DMAC_CTRLB_DST_INCR_POS)
#define DMAC_CTRLB_DST_INCR(value)      BITFIELD(DMAC_CTRLB_DST_INCR, (value))
#define DMAC_CTRLB_IEN                  BIT(30)

/* DMAC Channel x [x = 0..5] Configuration Register */
#define DMAC_CFG_SRC_PER_POS            (0)
#define DMAC_CFG_SRC_PER_MASK           (0xf << DMAC_CFG_SRC_PER_POS)
#define DMAC_CFG_SRC_PER(value)         BITFIELD(DMAC_CFG_SRC_PER, (value))
#define DMAC_CFG_DST_PER_POS            (4)
#define DMAC_CFG_DST_PER_MASK           (0xf << DMAC_CFG_DST_PER_POS)
#define DMAC_CFG_DST_PER(value)         BITFIELD(DMAC_CFG_DST_PER, (value))
#define DMAC_CFG_SRC_H2SEL              BIT(9)
#define DMAC_CFG_DST_H2SEL              BIT(13)
#define DMAC_CFG_SOD                    BIT(16)
#define DMAC_CFG_LOCK_IF                BIT(20)
#define DMAC_CFG_LOCK_B                 BIT(21)
#define DMAC_CFG_LOCK_IF_L              BIT(22)
#define DMAC_CFG_AHB_PROT_POS           (24)
#define DMAC_CFG_AHB_PROT_MASK          (0x7 << DMAC_CFG_AHB_PROT_POS)
#define DMAC_CFG_AHB_PROT(value)        BITFIELD(DMAC_CFG_AHB_PROT, (value))
#define DMAC_CFG_FIFOCFG_POS            (28)
#define DMAC_CFG_FIFOCFG_MASK           (0x3 << DMAC_CFG_FIFOCFG_POS)
#define DMAC_CFG_FIFOCFG(value)         BITFIELD(DMAC_CFG_FIFOCFG, (value))

/* DMAC Write Protect Mode Register */
#define DMAC_WPMR_WPEN                  BIT(0)

/* DMAC Write Protect Status Register */
#define DMAC_WPSR_WPVS                  BIT(0)

/* Serial peripheral interface. */
struct sam_spi_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t RDR;
    uint32_t TDR;
    uint32_t SR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t reserved1[4];
    uint32_t CSR[4];
    uint32_t reserved2[38];
    uint32_t WPMR;
    uint32_t WPSR;
};

/* Control register. */
#define SPI_CR_SPIEN                    BIT(0)
#define SPI_CR_SPIDIS                   BIT(1)
#define SPI_CR_SWRST                    BIT(7)
#define SPI_CR_LASTXFER                 BIT(24)

/* Mode register. */
#define SPI_MR_MSTR                     BIT(0)
#define SPI_MR_PS                       BIT(1)
#define SPI_MR_PCSDEC                   BIT(2)
#define SPI_MR_MODFDIS                  BIT(4)
#define SPI_MR_WDRBT                    BIT(5)
#define SPI_MR_LLB                      BIT(7)
#define SPI_MR_PCS_POS                  (16)
#define SPI_MR_PCS_MASK                 (0xf << SPI_MR_PCS_POS)
#define SPI_MR_PCS(value)               BITFIELD(SPI_MR_PCS, value)
#define SPI_MR_DLTBCS_POS               (24)
#define SPI_MR_DLTBCS_MASK              (0xff << SPI_MR_DLTBCS_POS)
#define SPI_MR_DLTBCS(value)            BITFIELD(SPI_MR_DLTBCS, value)

/* Receive data register. */
#define SPI_RDR_RD_POS                  (0)
#define SPI_RDR_RD_MASK                 (0xffff << SPI_RDR_RD_POS)
#define SPI_RDR_RD(value)               BITFIELD(SPI_RDR_RD, value)
#define SPI_RDR_PCS_POS                 (16)
#define SPI_RDR_PCS_MASK                (0xf << SPI_RDR_PCS_POS)
#define SPI_RDR_PCS(value)              BITFIELD(SPI_RDR_PCS, value)

/* Transmit data register. */
#define SPI_TDR_TD_POS                  (0)
#define SPI_TDR_TD_MASK                 (0xffff << SPI_TDR_TD_POS)
#define SPI_TDR_TD(value)               BITFIELD(SPI_TDR_TD, value)
#define SPI_TDR_PCS_POS                 (16)
#define SPI_TDR_PCS_MASK                (0xf << SPI_TDR_PCS_POS)
#define SPI_TDR_PCS(value)              BITFIELD(SPI_TDR_PCS, value)
#define SPI_TDR_LASTXFER                BIT(24)

/* Status register. */
#define SPI_SR_RDRF                     BIT(0)
#define SPI_SR_TDRE                     BIT(1)
#define SPI_SR_MODF                     BIT(2)
#define SPI_SR_OVRES                    BIT(3)
#define SPI_SR_NSSR                     BIT(8)
#define SPI_SR_TXEMPTY                  BIT(9)
#define SPI_SR_UNDES                    BIT(10)
#define SPI_SR_SPIENS                   BIT(16)

/* Interrupt enable register. */
#define SPI_IER_RDRF                    BIT(0)
#define SPI_IER_TDRE                    BIT(1)
#define SPI_IER_MODF                    BIT(2)
#define SPI_IER_OVRES                   BIT(3)
#define SPI_IER_NSSR                    BIT(8)
#define SPI_IER_TXEMPTY                 BIT(9)
#define SPI_IER_UNDES                   BIT(10)

/* Interrupt disable register. */
#define SPI_IDR_RDRF                    BIT(0)
#define SPI_IDR_TDRE                    BIT(1)
#define SPI_IDR_MODF                    BIT(2)
#define SPI_IDR_OVRES                   BIT(3)
#define SPI_IDR_NSSR                    BIT(8)
#define SPI_IDR_TXEMPTY                 BIT(9)
#define SPI_IDR_UNDES                   BIT(10)

/* Interrupt mask register. */
#define SPI_IMR_RDRF                    BIT(0)
#define SPI_IMR_TDRE                    BIT(1)
#define SPI_IMR_MODF                    BIT(2)
#define SPI_IMR_OVRES                   BIT(3)
#define SPI_IMR_NSSR                    BIT(8)
#define SPI_IMR_TXEMPTY                 BIT(9)
#define SPI_IMR_UNDES                   BIT(10)

/* Chip select register. */
#define SPI_CSR_CPOL                    BIT(0)
#define SPI_CSR_NCPHA                   BIT(1)
#define SPI_CSR_CSNAAT                  BIT(2)
#define SPI_CSR_CSAAT                   BIT(3)
#define SPI_CSR_BITS_POS                (4)
#define SPI_CSR_BITS_MASK               (0xf << SPI_CSR_BITS_POS)
#define SPI_CSR_BITS(value)             BITFIELD(SPI_CSR_BITS, value)
#define SPI_CSR_SCBR_POS                (8)
#define SPI_CSR_SCBR_MASK               (0xff << SPI_CSR_SCBR_POS)
#define SPI_CSR_SCBR(value)             BITFIELD(SPI_CSR_SCBR, value)
#define SPI_CSR_DLYBS_POS               (16)
#define SPI_CSR_DLYBS_MASK              (0xff << SPI_CSR_DLYBS_POS)
#define SPI_CSR_DLYBS(value)            BITFIELD(SPI_CSR_DLYBS, value)
#define SPI_CSR_DLYBCT_POS              (24)
#define SPI_CSR_DLYBCT_MASK             (0xff << SPI_CSR_DLYBCT_POS)
#define SPI_CSR_DLYBCT(value)           BITFIELD(SPI_CSR_DLYBCT, value)

/* Power management controller. */
struct sam_pmc_t {
    uint32_t SCER;
    uint32_t SCDR;
    uint32_t SCSR;
    uint32_t reserved1;
    uint32_t PCER0;
    uint32_t PCDR0;
    uint32_t PCSR0;
    uint32_t CKGR_UCKR;
    uint32_t CKGR_MOR;
    uint32_t CKGR_MCFR;
    uint32_t CKGR_PLLAR;
    uint32_t reserved2;
    uint32_t MCKR;
    uint32_t reserved3;
    uint32_t USB;
    uint32_t reserved4;
    uint32_t PCK[3];
    uint32_t reserved5[5];
    uint32_t IER;
    uint32_t IDR;
    uint32_t SR;
    uint32_t IMR;
    uint32_t FSMR;
    uint32_t FSPR;
    uint32_t FOCR;
    uint32_t reserved6[100];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t reserved7[4];
    uint32_t PCER1;
    uint32_t PCDR1;
    uint32_t PCSR1;
    uint32_t PCR;
};

#define PMC_UOTGCLK                     BIT(5)
#define PMC_PCK0                        BIT(8)
#define PMC_PCK1                        BIT(9)
#define PMC_PCK2                        BIT(10)

#define PMC_PID2                        BIT(2)
#define PMC_PID3                        BIT(3)
#define PMC_PID4                        BIT(4)
#define PMC_PID5                        BIT(5)
#define PMC_PID6                        BIT(6)
#define PMC_PID7                        BIT(7)
#define PMC_PID8                        BIT(8)
#define PMC_PID9                        BIT(9)
#define PMC_PID10                       BIT(10)
#define PMC_PID11                       BIT(11)
#define PMC_PID12                       BIT(12)
#define PMC_PID13                       BIT(13)
#define PMC_PID14                       BIT(14)
#define PMC_PID15                       BIT(15)
#define PMC_PID16                       BIT(16)
#define PMC_PID17                       BIT(17)
#define PMC_PID18                       BIT(18)
#define PMC_PID19                       BIT(19)
#define PMC_PID20                       BIT(20)
#define PMC_PID21                       BIT(21)
#define PMC_PID22                       BIT(22)
#define PMC_PID23                       BIT(23)
#define PMC_PID24                       BIT(24)
#define PMC_PID25                       BIT(25)
#define PMC_PID26                       BIT(26)
#define PMC_PID27                       BIT(27)
#define PMC_PID28                       BIT(28)
#define PMC_PID29                       BIT(29)
#define PMC_PID30                       BIT(30)
#define PMC_PID31                       BIT(31)

#define PMC_CKGR_UCKR_UPLLEN            BIT(16)
#define PMC_CKGR_UCKR_UPLLCOUNT_POS     20
#define PMC_CKGR_UCKR_UPLLCOUNT_MASK    (0xf << PMC_CKGR_UCKR_UPLLCOUNT_POS)
#define PMC_CKGR_UCKR_UPLLCOUNT(value)  BITFIELD(PMC_CKGR_UCKR_UPLLCOUNT, value)

#define PMC_CKGR_MOR_MOSCXTEN           BIT(0)
#define PMC_CKGR_MOR_MOSCXTBY           BIT(1)
#define PMC_CKGR_MOR_MOSCRCEN           BIT(3)
#define PMC_CKGR_MOR_MOSCRCF_POS        (4)
#define PMC_CKGR_MOR_MOSCRCF_MASK       (0x7 << PMC_CKGR_MOR_MOSCRCF_POS)
#define PMC_CKGR_MOR_MOSCRCF(value)     BITFIELD(PMC_CKGR_MOR_MOSCRCF, value)
#define PMC_CKGR_MOR_MOSCRCF_4_MHz      PMC_CKGR_MOR_MOSCRCF(0)
#define PMC_CKGR_MOR_MOSCRCF_8_MHz      PMC_CKGR_MOR_MOSCRCF(1)
#define PMC_CKGR_MOR_MOSCRCF_12_MHz     PMC_CKGR_MOR_MOSCRCF(2)
#define PMC_CKGR_MOR_MOSCXTST_POS       (8)
#define PMC_CKGR_MOR_MOSCXTST_MASK      (0xff << PMC_CKGR_MOR_MOSCXTST_POS)
#define PMC_CKGR_MOR_MOSCXTST(value)    BITFIELD(PMC_CKGR_MOR_MOSCXTST, value)
#define PMC_CKGR_MOR_KEY_POS            (16)
#define PMC_CKGR_MOR_KEY_MASK           (0xff << PMC_CKGR_MOR_KEY_POS)
#define PMC_CKGR_MOR_KEY(value)         BITFIELD(PMC_CKGR_MOR_KEY, value)
#define PMC_CKGR_MOR_MOSCSEL            BIT(24)
#define PMC_CKGR_MOR_CFDEN              BIT(25)

#define PMC_CKGR_MCFR_MAINF_POS         (0)
#define PMC_CKGR_MCFR_MAINF_MASK        (0xffff << PMC_CKGR_MCFR_MAINF_POS)
#define PMC_CKGR_MCFR_MAINF(value)      BITFIELD(PMC_CKGR_MCFR_MAINF, value)
#define PMC_CKGR_MCFR_MAINFRDY          BIT(16)

#define PMC_CKGR_PLLAR_DIVA_POS         (0)
#define PMC_CKGR_PLLAR_DIVA_MASK        (0xff << PMC_CKGR_PLLAR_DIVA_POS)
#define PMC_CKGR_PLLAR_DIVA(value)      BITFIELD(PMC_CKGR_PLLAR_DIVA, value)
#define PMC_CKGR_PLLAR_PLLACOUNT_POS    (8)
#define PMC_CKGR_PLLAR_PLLACOUNT_MASK   (0x3f << PMC_CKGR_PLLAR_PLLACOUNT_POS)
#define PMC_CKGR_PLLAR_PLLACOUNT(value) BITFIELD(PMC_CKGR_PLLAR_PLLACOUNT, value)
#define PMC_CKGR_PLLAR_MULA_POS         (16)
#define PMC_CKGR_PLLAR_MULA_MASK        (0x7ff << PMC_CKGR_PLLAR_MULA_POS)
#define PMC_CKGR_PLLAR_MULA(value)      BITFIELD(PMC_CKGR_PLLAR_MULA, value)
#define PMC_CKGR_PLLAR_ONE              BIT(29)

#define PMC_MCKR_CSS_POS                (0)
#define PMC_MCKR_CSS_MASK               (0x3 << PMC_MCKR_CSS_POS)
#define PMC_MCKR_CSS(value)             BITFIELD(PMC_MCKR_CSS, value)
#define PMC_MCKR_CSS_SLOW_CLK           PMC_MCKR_CSS(0)
#define PMC_MCKR_CSS_MAIN_CLK           PMC_MCKR_CSS(1)
#define PMC_MCKR_CSS_PLLA_CLK           PMC_MCKR_CSS(2)
#define PMC_MCKR_CSS_UPLL_CLK           PMC_MCKR_CSS(3)
#define PMC_MCKR_PRES_POS               (4)
#define PMC_MCKR_PRES_MASK              (0x7 << PMC_MCKR_PRES_POS)
#define PMC_MCKR_PRES(value)            BITFIELD(PMC_MCKR_PRES, value)
#define PMC_MCKR_PRES_CLK               PMC_MCKR_PRES(0)
#define PMC_MCKR_PRES_CLK_2             PMC_MCKR_PRES(1)
#define PMC_MCKR_PRES_CLK_4             PMC_MCKR_PRES(2)
#define PMC_MCKR_PRES_CLK_8             PMC_MCKR_PRES(3)
#define PMC_MCKR_PRES_CLK_16            PMC_MCKR_PRES(4)
#define PMC_MCKR_PRES_CLK_32            PMC_MCKR_PRES(5)
#define PMC_MCKR_PRES_CLK_64            PMC_MCKR_PRES(6)
#define PMC_MCKR_PRES_CLK_3             PMC_MCKR_PRES(7)
#define PMC_MCKR_PLLADIV2               BIT(12)
#define PMC_MCKR_UPLLDIV2               BIT(13)

#define PMC_USB_USBS                    BIT(0)
#define PMC_USB_USBDIV_POS              (8)
#define PMC_USB_USBDIV_MASK             (0xf << PMC_USB_USBDIV_POS)
#define PMC_USB_USBDIV(value)           BITFIELD(PMC_USB_USBDIV, value)

#define PMC_PCK_CSS_POS                 (0)
#define PMC_PCK_CSS_MASK                (0x7 << PMC_PCK_CSS_POS)
#define PMC_PCK_CSS(value)              BITFIELD(PMC_PCK_CSS, value)
#define PMC_PCK_CSS_SLOW_CLK            PMC_PCK_CSS(0)
#define PMC_PCK_CSS_MAIN_CLK            PMC_PCK_CSS(1)
#define PMC_PCK_CSS_PLLA_CLK            PMC_PCK_CSS(2)
#define PMC_PCK_CSS_UPLL_CLK            PMC_PCK_CSS(3)
#define PMC_PCK_CSS_MCK                 PMC_PCK_CSS(4)
#define PMC_PCK_PRES_POS                (4)
#define PMC_PCK_PRES_MASK               (0x7 << PMC_PCK_PRES_POS)
#define PMC_PCK_PRES(value)             BITFIELD(PMC_PCK_PRES, value)
#define PMC_PCK_PRES_CLK                PMC_PCK_PRES(0)
#define PMC_PCK_PRES_CLK_2              PMC_PCK_PRES(1)
#define PMC_PCK_PRES_CLK_4              PMC_PCK_PRES(2)
#define PMC_PCK_PRES_CLK_8              PMC_PCK_PRES(3)
#define PMC_PCK_PRES_CLK_16             PMC_PCK_PRES(4)
#define PMC_PCK_PRES_CLK_32             PMC_PCK_PRES(5)
#define PMC_PCK_PRES_CLK_64             PMC_PCK_PRES(6)

#define PMC_IER_MOSCXTS                 BIT(0)
#define PMC_IER_LOCKA                   BIT(1)
#define PMC_IER_MCKRDY                  BIT(3)
#define PMC_IER_LOCK                    BIT(6)
#define PMC_IER_PCKRDY0                 BIT(8)
#define PMC_IER_PCKRDY1                 BIT(9)
#define PMC_IER_PCKRDY2                 BIT(10)
#define PMC_IER_MOSCSELS                BIT(16)
#define PMC_IER_MOSCRCS                 BIT(17)
#define PMC_IER_CFDEV                   BIT(18)

#define PMC_IDR_MOSCXTS                 BIT(0)
#define PMC_IDR_LOCKA                   BIT(1)
#define PMC_IDR_MCKRDY                  BIT(3)
#define PMC_IDR_LOCK                    BIT(6)
#define PMC_IDR_PCKRDY0                 BIT(8)
#define PMC_IDR_PCKRDY1                 BIT(9)
#define PMC_IDR_PCKRDY2                 BIT(10)
#define PMC_IDR_MOSCSELS                BIT(16)
#define PMC_IDR_MOSCRCS                 BIT(17)
#define PMC_IDR_CFDEV                   BIT(18)

#define PMC_SR_MOSCXTS                  BIT(0)
#define PMC_SR_LOCKA                    BIT(1)
#define PMC_SR_MCKRDY                   BIT(3)
#define PMC_SR_LOCK                     BIT(6)
#define PMC_SR_OSCSELS                  BIT(7)
#define PMC_SR_PCKRDY0                  BIT(8)
#define PMC_SR_PCKRDY1                  BIT(9)
#define PMC_SR_PCKRDY2                  BIT(10)
#define PMC_SR_MOSCSELS                 BIT(16)
#define PMC_SR_MOSCRCS                  BIT(17)
#define PMC_SR_CFDEV                    BIT(18)
#define PMC_SR_CFDS                     BIT(19)
#define PMC_SR_FOS                      BIT(20)

#define PMC_IMR_MOSCXTS                 BIT(0)
#define PMC_IMR_LOCKA                   BIT(1)
#define PMC_IMR_MCKRDY                  BIT(3)
#define PMC_IMR_LOCK                    BIT(6)
#define PMC_IMR_PCKRDY0                 BIT(8)
#define PMC_IMR_PCKRDY1                 BIT(9)
#define PMC_IMR_PCKRDY2                 BIT(10)
#define PMC_IMR_MOSCSELS                BIT(16)
#define PMC_IMR_MOSCRCS                 BIT(17)
#define PMC_IMR_CFDEV                   BIT(18)

#define PMC_FSMR_FSTT0                  BIT(0)
#define PMC_FSMR_FSTT1                  BIT(1)
#define PMC_FSMR_FSTT2                  BIT(2)
#define PMC_FSMR_FSTT3                  BIT(3)
#define PMC_FSMR_FSTT4                  BIT(4)
#define PMC_FSMR_FSTT5                  BIT(5)
#define PMC_FSMR_FSTT6                  BIT(6)
#define PMC_FSMR_FSTT7                  BIT(7)
#define PMC_FSMR_FSTT8                  BIT(8)
#define PMC_FSMR_FSTT9                  BIT(9)
#define PMC_FSMR_FSTT10                 BIT(10)
#define PMC_FSMR_FSTT11                 BIT(11)
#define PMC_FSMR_FSTT12                 BIT(12)
#define PMC_FSMR_FSTT13                 BIT(13)
#define PMC_FSMR_FSTT14                 BIT(14)
#define PMC_FSMR_FSTT15                 BIT(15)
#define PMC_FSMR_RTTAL                  BIT(16)
#define PMC_FSMR_RTCAL                  BIT(17)
#define PMC_FSMR_USBAL                  BIT(18)
#define PMC_FSMR_LPM                    BIT(20)

#define PMC_FSPR_FSTP0                  BIT(0)
#define PMC_FSPR_FSTP1                  BIT(1)
#define PMC_FSPR_FSTP2                  BIT(2)
#define PMC_FSPR_FSTP3                  BIT(3)
#define PMC_FSPR_FSTP4                  BIT(4)
#define PMC_FSPR_FSTP5                  BIT(5)
#define PMC_FSPR_FSTP6                  BIT(6)
#define PMC_FSPR_FSTP7                  BIT(7)
#define PMC_FSPR_FSTP8                  BIT(8)
#define PMC_FSPR_FSTP9                  BIT(9)
#define PMC_FSPR_FSTP10                 BIT(10)
#define PMC_FSPR_FSTP11                 BIT(11)
#define PMC_FSPR_FSTP12                 BIT(12)
#define PMC_FSPR_FSTP13                 BIT(13)
#define PMC_FSPR_FSTP14                 BIT(14)
#define PMC_FSPR_FSTP15                 BIT(15)

#define PMC_FOCR_FOCLR                  BIT(0)

#define PMC_WPMR_WPEN                   BIT(0)
#define PMC_WPMR_WPKEY_POS              (8)
#define PMC_WPMR_WPKEY_MASK             (0xffffff << PMC_WPMR_WPKEY_POS)
#define PMC_WPMR_WPKEY(value)           BITFIELD(PMC_WPMR_WPKEY, value)

#define PMC_WPSR_WPVS                   BIT(0)
#define PMC_WPSR_WPVSRC_POS             (8)
#define PMC_WPSR_WPVSRC_MASK            (0xffff << PMC_WPSR_WPVSRC_POS)
#define PMC_WPSR_WPVSRC(value)          BITFIELD(PMC_WPSR_WPVSRC, value)

#define PMC_PCER1_PID32                 BIT(0)
#define PMC_PCER1_PID33                 BIT(1)
#define PMC_PCER1_PID34                 BIT(2)
#define PMC_PCER1_PID35                 BIT(3)
#define PMC_PCER1_PID36                 BIT(4)
#define PMC_PCER1_PID37                 BIT(5)
#define PMC_PCER1_PID38                 BIT(6)
#define PMC_PCER1_PID39                 BIT(7)
#define PMC_PCER1_PID40                 BIT(8)
#define PMC_PCER1_PID41                 BIT(9)
#define PMC_PCER1_PID42                 BIT(10)
#define PMC_PCER1_PID43                 BIT(11)
#define PMC_PCER1_PID44                 BIT(12)

#define PMC_PCDR1_PID32                 BIT(0)
#define PMC_PCDR1_PID33                 BIT(1)
#define PMC_PCDR1_PID34                 BIT(2)
#define PMC_PCDR1_PID35                 BIT(3)
#define PMC_PCDR1_PID36                 BIT(4)
#define PMC_PCDR1_PID37                 BIT(5)
#define PMC_PCDR1_PID38                 BIT(6)
#define PMC_PCDR1_PID39                 BIT(7)
#define PMC_PCDR1_PID40                 BIT(8)
#define PMC_PCDR1_PID41                 BIT(9)
#define PMC_PCDR1_PID42                 BIT(10)
#define PMC_PCDR1_PID43                 BIT(11)
#define PMC_PCDR1_PID44                 BIT(12)

#define PMC_PCSR1_PID32                 BIT(0)
#define PMC_PCSR1_PID33                 BIT(1)
#define PMC_PCSR1_PID34                 BIT(2)
#define PMC_PCSR1_PID35                 BIT(3)
#define PMC_PCSR1_PID36                 BIT(4)
#define PMC_PCSR1_PID37                 BIT(5)
#define PMC_PCSR1_PID38                 BIT(6)
#define PMC_PCSR1_PID39                 BIT(7)
#define PMC_PCSR1_PID40                 BIT(8)
#define PMC_PCSR1_PID41                 BIT(9)
#define PMC_PCSR1_PID42                 BIT(10)
#define PMC_PCSR1_PID43                 BIT(11)
#define PMC_PCSR1_PID44                 BIT(12)

#define PMC_PCR_PID_POS                 (0)
#define PMC_PCR_PID_MASK                (0x3f << PMC_PCR_PID_POS)
#define PMC_PCR_PID(value)              BITFIELD(PMC_PCR_PID, value)
#define PMC_PCR_CMD                     BIT(12)
#define PMC_PCR_DIV_POS                 (16)
#define PMC_PCR_DIV_MASK                (0x3 << PMC_PCR_DIV_POS)
#define PMC_PCR_DIV(value)              BITFIELD(PMC_PCR_DIV, value)
#define PMC_PCR_DIV_PERIPH_DIV_MCK      PMC_PCR_DIV(0)
#define PMC_PCR_DIV_PERIPH_DIV2_MCK     PMC_PCR_DIV(1)
#define PMC_PCR_DIV_PERIPH_DIV4_MCK     PMC_PCR_DIV(2)
#define PMC_PCR_EN                      BIT(28)

/* Table 9-1. Peripheral identifiers. */
#define PID_SUPC                        (0)
#define PID_RSTC                        (1)
#define PID_RTC                         (2)
#define PID_RTT                         (3)
#define PID_WDT                         (4)
#define PID_PMC                         (5)
#define PID_EFC0                        (6)
#define PID_EFC1                        (7)
#define PID_UART                        (8)
#define PID_SMC                         (9)
#define PID_PIOA                        (11)
#define PID_PIOB                        (12)
#define PID_PIOC                        (13)
#define PID_PIOD                        (14)
#define PID_USART0                      (17)
#define PID_USART1                      (18)
#define PID_USART2                      (19)
#define PID_USART3                      (20)
#define PID_HSMCI                       (21)
#define PID_TWI0                        (22)
#define PID_TWI1                        (23)
#define PID_SPI0                        (24)
#define PID_SSC                         (26)
#define PID_TC0                         (27)
#define PID_TC1                         (28)
#define PID_TC2                         (29)
#define PID_TC3                         (30)
#define PID_TC4                         (31)
#define PID_TC5                         (32)
#define PID_TC6                         (33)
#define PID_TC7                         (34)
#define PID_TC8                         (35)
#define PID_PWM                         (36)
#define PID_ADC                         (37)
#define PID_DACC                        (38)
#define PID_DMAC                        (39)
#define PID_UOTGHS                      (40)
#define PID_TRNG                        (41)
#define PID_EMAC                        (42)
#define PID_CAN0                        (43)
#define PID_CAN1                        (44)

/* Embedded flash controller. */
struct sam_eefc_t {
    uint32_t FMR;
    uint32_t FCR;
    uint32_t FSR;
    uint32_t FRR;
};

#define EEFC_FMR_FRDY                   BIT(0)
#define EEFC_FMR_FWS_POS                (8)
#define EEFC_FMR_FWS_MASK               (0xf << EEFC_FMR_FWS_POS)
#define EEFC_FMR_FWS(value)             BITFIELD(EEFC_FMR_FWS, value)
#define EEFC_FMR_FAM                    BIT(24)
#define EEFC_FCR_FCMD_POS               (0)
#define EEFC_FCR_FCMD_MASK              (0xff << EEFC_FCR_FCMD_POS)
#define EEFC_FCR_FCMD(value)            BITFIELD(EEFC_FCR_FCMD, value)
#define EEFC_FCR_FARG_POS               (8)
#define EEFC_FCR_FARG_MASK              (0xffff << EEFC_FCR_FARG_POS)
#define EEFC_FCR_FARG(value)            BITFIELD(EEFC_FCR_FARG, value)
#define EEFC_FCR_FKEY_POS               (24)
#define EEFC_FCR_FKEY_MASK              (0xff << EEFC_FCR_FKEY_POS)
#define EEFC_FCR_FKEY(value)            BITFIELD(EEFC_FCR_FKEY, value)
#define EEFC_FSR_FRDY                   BIT(0)
#define EEFC_FSR_FCMDE                  BIT(1)
#define EEFC_FSR_FLOCKE                 BIT(2)
#define EEFC_FRR_FVALUE_POS             (0)
#define EEFC_FRR_FVALUE_MASK            (0xffffffff << EEFC_FRR_FVALUE_POS)
#define EEFC_FRR_FVALUE(value)          BITFIELD(EEFC_FRR_FVALUE, value)

/* System control block. */
struct sam_system_control_block_t {
    uint32_t ACTLR;
    uint32_t reserved1[3316];
    uint32_t CPUID;
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHPR1;
    uint32_t SHPR2;
    uint32_t SHPR3;
    uint32_t SHCSR;
    uint32_t CFSR;
    uint32_t HFSR;
    uint32_t MMAR;
    uint32_t BFAR;
};

/* Auxiliary Control Register */
#define SCB_ACTLR_DISMCYCINT            BIT(0)
#define SCB_ACTLR_DISDEFWBUF            BIT(1)
#define SCB_ACTLR_DISFOLD               BIT(2)

/* CPUID Base Register */
#define SCB_CPUID_REVISION_POS          (0)
#define SCB_CPUID_REVISION_MASK         (0xf << SCB_CPUID_REVISION_POS)
#define SCB_CPUID_REVISION(value)       BITFIELD(SCB_CPUID_REVISION, value)
#define SCB_CPUID_PARTNO_POS            (4)
#define SCB_CPUID_PARTNO_MASK           (0xfff << SCB_CPUID_PARTNO_POS)
#define SCB_CPUID_PARTNO(value)         BITFIELD(SCB_CPUID_PARTNO, value)
#define SCB_CPUID_CONSTANT_POS          (16)
#define SCB_CPUID_CONSTANT_MASK         (0xf << SCB_CPUID_CONSTANT_POS)
#define SCB_CPUID_CONSTANT(value)       BITFIELD(SCB_CPUID_CONSTANT, value)
#define SCB_CPUID_VARIANT_POS           (20)
#define SCB_CPUID_VARIANT_MASK          (0xf << SCB_CPUID_VARIANT_POS)
#define SCB_CPUID_VARIANT(value)        BITFIELD(SCB_CPUID_VARIANT, value)
#define SCB_CPUID_IMPLEMENTER_POS       (20)
#define SCB_CPUID_IMPLEMENTER_MASK      (0xf << SCB_CPUID_IMPLEMENTER_POS)
#define SCB_CPUID_IMPLEMENTER(value)    BITFIELD(SCB_CPUID_IMPLEMENTER, value)

/* Interrupt Control and State Register */
#define SCB_ICSR_VECTACTIVE_POS         (0)
#define SCB_ICSR_VECTACTIVE_MASK        (0x1ff << SCB_ICSR_VECTACTIVE_POS)
#define SCB_ICSR_VECTACTIVE(value)      BITFIELD(SCB_ICSR_VECTACTIVE, value)
#define SCB_ICSR_RETTOBASE              BIT(11)
#define SCB_ICSR_VECTPENDING_POS        (12)
#define SCB_ICSR_VECTPENDING_MASK       (0x3ff << SCB_ICSR_VECTPENDING_POS)
#define SCB_ICSR_VECTPENDING(value)     BITFIELD(SCB_ICSR_VECTPENDING, value)
#define SCB_ICSR_ISRPENDING             BIT(22)
#define SCB_ICSR_PENDSTCLR              BIT(25)
#define SCB_ICSR_PENDSTSET              BIT(26)
#define SCB_ICSR_PENDSVCLR              BIT(27)
#define SCB_ICSR_PENDSVSET              BIT(28)

/* Vector Table Offset Register */
#define SCB_VTOR_TBLOFF_POS             (7)
#define SCB_VTOR_TBLOFF_MASK            (0x7fffff << SCB_VTOR_TBLOFF_POS)
#define SCB_VTOR_TBLOFF(value)          BITFIELD(SCB_VTOR_TBLOFF, value)

/* Application Interrupt and Reset Control Register */
#define SCB_AIRCR_VECTRESET             BIT(0)
#define SCB_AIRCR_VECTCLRACTIVE         BIT(1)
#define SCB_AIRCR_SYSRESETREQ           BIT(2)
#define SCB_AIRCR_PRIGROUP_POS          (8)
#define SCB_AIRCR_PRIGROUP_MASK         (0x7 << SCB_AIRCR_PRIGROUP_POS)
#define SCB_AIRCR_PRIGROUP(value)       BITFIELD(SCB_AIRCR_PRIGROUP, value)
#define SCB_AIRCR_ENDIANESS             BIT(15)
#define SCB_AIRCR_VECTKEY_POS           (16)
#define SCB_AIRCR_VECTKEY_MASK          (0xffff << SCB_AIRCR_VECTKEY_POS)
#define SCB_AIRCR_VECTKEY(value)        BITFIELD(SCB_AIRCR_VECTKEY, value)

/* System Control Register */
#define SCB_SCR_SLEEONEXIT              BIT(1)
#define SCB_SCR_SLEEPDEEP               BIT(2)
#define SCB_SCR_SEVONPEND               BIT(4)

/* Configuration and Control Register */
#define SCB_CCR_NONEBASETHRDENA         BIT(0)
#define SCB_CCR_USERSETMPEND            BIT(1)
#define SCB_CCR_UNALIGN_TRP             BIT(3)
#define SCB_CCR_DIV_0_TRP               BIT(4)
#define SCB_CCR_BFHFNMIGN               BIT(8)
#define SCB_CCR_STKALIGN                BIT(9)

/* System Handler Priority Register 1 */
#define SCB_SHPR1_PRI_4_POS             (0)
#define SCB_SHPR1_PRI_4_MASK            (0xff <<  SCB_SHPR1_PRI_4_POS)
#define SCB_SHPR1_PRI_4(value)          BITFIELD(SHPR1_PRI_4, value)
#define SCB_SHPR1_PRI_5_POS             (8)
#define SCB_SHPR1_PRI_5_MASK            (0xff << SCB_SHPR1_PRI_5_POS)
#define SCB_SHPR1_PRI_5(value)          BITFIELD(SHPR1_PRI_5, value)
#define SCB_SHPR1_PRI_6_POS             (16)
#define SCB_SHPR1_PRI_6_MASK            (0xff << SCB_SHPR1_PRI_6_POS)
#define SCB_SHPR1_PRI_6(value)          BITFIELD(SHPR1_PRI_6, value)

/* System Handler Priority Register 2 */
#define SCB_SHPR2_PRI_11_POS            (24)
#define SCB_SHPR2_PRI_11_MASK           (0xff << SCB_SHPR2_PRI_11_POS)
#define SCB_SHPR2_PRI_11(value)         BITFIELD(SCB_SHPR2_PRI_11, value)

/* System Handler Priority Register 3 */
#define SCB_SHPR3_PRI_14_POS            (16)
#define SCB_SHPR3_PRI_14_MASK           (0xff << SCB_SHPR3_PRI_14_POS)
#define SCB_SHPR3_PRI_14(value)         BITFIELD(SCB_SHPR3_PRI_14, value)
#define SCB_SHPR3_PRI_15_POS            (24)
#define SCB_SHPR3_PRI_15_MASK           (0xff << SCB_SHPR3_PRI_15_POS)
#define SCB_SHPR3_PRI_15(value)         BITFIELD(SCB_SHPR3_PRI_15, value)

/* System Handler Control and State Register */
#define SCB_SHCSR_MEMFAULTACT           BIT(0)
#define SCB_SHCSR_BUSFAULTACT           BIT(1)
#define SCB_SHCSR_USGFAULTACT           BIT(3)
#define SCB_SHCSR_SVCALLAVCT            BIT(7)
#define SCB_SHCSR_MONITORACT            BIT(8)
#define SCB_SHCSR_PENDSVACT             BIT(10)
#define SCB_SHCSR_SYSTICKACT            BIT(11)
#define SCB_SHCSR_USGFAULTPENDED        BIT(12)
#define SCB_SHCSR_MEMFAULTPENDED        BIT(13)
#define SCB_SHCSR_BUSFAULTPENDED        BIT(14)
#define SCB_SHCSR_SVCALLPENDED          BIT(15)
#define SCB_SHCSR_MEMFAULTENA           BIT(16)
#define SCB_SHCSR_BUSFAULTENA           BIT(17)
#define SCB_SHCSR_USGFAULTENA           BIT(18)

/* Configurable Fault Status Register */
#define SCB_CFSR

/* Hard Fault Status Register */
#define SCB_HFSR

/* Memory Management Fault Address Register */
#define SCB_MMAR

/* Bus Fault Address Register */
#define SCB_BFAR

/* System timer. */
struct sam_system_timer_t {
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;
};

/* SysTick Control and Status Register */
#define SYSTEM_TIMER_CTRL_ENABLE        BIT(0)
#define SYSTEM_TIMER_CTRL_TICKINT       BIT(1)
#define SYSTEM_TIMER_CTRL_CLKSOURCE     BIT(2)
#define SYSTEM_TIMER_CTRL_COUNTFLAG     BIT(16)

/* SysTick Reload Value Register */
#define SYSTEM_TIMER_LOAD_RELOAD_POS    (0)
#define SYSTEM_TIMER_LOAD_RELOAD_MASK   (0xffffff << SYSTEM_TIMER_LOAD_RELOAD_POS)
#define SYSTEM_TIMER_LOAD_RELOAD(value) BITFIELD(SYSTEM_TIMER_LOAD_RELOAD, value)

/* SysTick Current Value Register */
#define SYSTEM_TIMER_VAL_CURRENT_POS    (0)
#define SYSTEM_TIMER_VAL_CURRENT_MASK   (0xffffff << SYSTEM_TIMER_VAL_CURRENT_POS)
#define SYSTEM_TIMER_VAL_CURRENT(value) BITFIELD(SYSTEM_TIMER_VAL_CURRENT, value)

    /* SysTick Calibration Value Register */
#define SYSTEM_TIMER_CALIB_TENMS_POS    (0)
#define SYSTEM_TIMER_CALIB_TENMS_MASK   (0xffffff << SYSTEM_TIMER_CALIB_TENMS_POS)
#define SYSTEM_TIMER_CALIB_TENMS(value) BITFIELD(SYSTEM_TIMER_CALIB_TENMS, value)
#define SYSTEM_TIMER_CALIB_SKEW         BIT(30)
#define SYSTEM_TIMER_CALIB_NOREF        BIT(31)

/* System nested vectored interrupt controller. */
struct sam_nvic_t {
    uint32_t ISE[2];
    uint32_t reserved1[30];
    uint32_t ICE[2];
    uint32_t reserved2[30];
    uint32_t ISP[2];
    uint32_t reserved3[30];
    uint32_t ICP[2];
    uint32_t reserved4[30];
    uint32_t IAB[2];
    uint32_t reserved5[62];
    uint32_t IP[8];
    uint32_t reserved6[697];
    uint32_t STIR;
};

/* High Speed MultiMedia Card Interface (HSMCI). */
struct sam_hsmci_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t DTOR;
    uint32_t SDCR;
    uint32_t ARGR;
    uint32_t CMDR;
    uint32_t BLKR;
    uint32_t CSTOR;
    uint32_t RSPR[4];
    uint32_t RDR;
    uint32_t TDR;
    uint32_t reserved1[2];
    uint32_t SR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t DMA;
    uint32_t CFG;
    uint32_t reserved2[35];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t reserved3[69];
    uint32_t FIFO[256];
};

struct sam_pdc_t {
    uint32_t RPR;
    uint32_t RCR;
    uint32_t TPR;
    uint32_t TCR;
    uint32_t RNPR;
    uint32_t RNCR;
    uint32_t TNPR;
    uint32_t TNCR;
    uint32_t PTCR;
    uint32_t PTSR;
};

/* Receive Pointer Register */
#define PERIPH_RPR_RXPTR_POS            (0)
#define PERIPH_RPR_RXPTR_MASK           (0xffffffff << PERIPH_RPR_RXPTR_POS)
#define PERIPH_RPR_RXPTR(value)         BITFIELD(PERIPH_RPR_RXPTR, value)

/* Receive Counter Register */
#define PERIPH_RCR_RXCTR_POS            (0)
#define PERIPH_RCR_RXCTR_MASK           (0xffff << PERIPH_RCR_RXCTR_POS)
#define PERIPH_RCR_RXCTR(value)         BITFIELD(PERIPH_RCR_RXCTR, value)

/* Transmit Pointer Register */
#define PERIPH_TPR_TXPTR_POS            (0)
#define PERIPH_TPR_TXPTR_MASK           (0xffffffff << PERIPH_TPR_TXPTR_POS)
#define PERIPH_TPR_TXPTR(value)         BITFIELD(PERIPH_TPR_TXPTR, value)

/* Transmit Counter Register */
#define PERIPH_TCR_TXCTR_POS            (0)
#define PERIPH_TCR_TXCTR_MASK           (0xffff << PERIPH_TCR_TXCTR_POS)
#define PERIPH_TCR_TXCTR(value)         BITFIELD(PERIPH_TCR_TXCTR, value)

/* Receive Next Pointer Register */
#define PERIPH_RNPR_RXNPTR_POS          (0)
#define PERIPH_RNPR_RXNPTR_MASK         (0xffffffff << PERIPH_RNPR_RXNPTR_POS)
#define PERIPH_RNPR_RXNPTR(value)       BITFIELD(PERIPH_RNPR_RXNPTR, value)

/* Receive Next Counter Register */
#define PERIPH_RNCR_RXNCTR_POS          (0)
#define PERIPH_RNCR_RXNCTR_MASK         (0xffff << PERIPH_RNCR_RXNCTR_POS)
#define PERIPH_RNCR_RXNCTR(value)       BITFIELD(PERIPH_RNCR_RXNCTR, value)

/* Transmit Next Pointer Register */
#define PERIPH_TNPR_TXNPTR_POS          (0)
#define PERIPH_TNPR_TXNPTR_MASK         (0xffffffff << PERIPH_TNPR_TXNPTR_POS)
#define PERIPH_TNPR_TXNPTR(value)       BITFIELD(PERIPH_TNPR_TXNPTR, value)

/* Transmit Next Counter Register */
#define PERIPH_TNCR_TXNCTR_POS          (0)
#define PERIPH_TNCR_TXNCTR_MASK         (0xffff << PERIPH_TNCR_TXNCTR_POS)
#define PERIPH_TNCR_TXNCTR(value)       BITFIELD(PERIPH_TNCR_TXNCTR, value)

/* Transfer Control Register */
#define PERIPH_PTCR_RXTEN               BIT(0)
#define PERIPH_PTCR_RXTDIS              BIT(1)
#define PERIPH_PTCR_TXTEN               BIT(8)
#define PERIPH_PTCR_TXTDIS              BIT(9)

/* Transfer Status Register */
#define PERIPH_PTSR_RXTEN               BIT(0)
#define PERIPH_PTSR_TXTEN               BIT(8)

struct sam_uart_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t CSR;
    uint32_t RHR;
    uint32_t THR;
    uint32_t BRGR;
    uint32_t reserved1[55];
    struct sam_pdc_t PDC;
};

struct sam_usart_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t CSR;
    uint32_t RHR;
    uint32_t THR;
    uint32_t BRGR;
    uint32_t RTOR;
    uint32_t TTGR;
    uint32_t reserved1[5];
    uint32_t FIDI;
    uint32_t NER;
    uint32_t reserved2[1];
    uint32_t IF;
    uint32_t MAN;
    uint32_t LINMR;
    uint32_t LINIR;
    uint32_t reserved3[34];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t reserved4[5];
    struct sam_pdc_t PDC;
};

/* USART Control Register */
#define US_CR_RSTRX                     BIT(2)
#define US_CR_RSTTX                     BIT(3)
#define US_CR_RXEN                      BIT(4)
#define US_CR_RXDIS                     BIT(5)
#define US_CR_TXEN                      BIT(6)
#define US_CR_TXDIS                     BIT(7)
#define US_CR_RSTSTA                    BIT(8)
#define US_CR_STTBRK                    BIT(9)
#define US_CR_STPBRK                    BIT(10)
#define US_CR_STTTO                     BIT(11)
#define US_CR_SENDA                     BIT(12)
#define US_CR_RSTIT                     BIT(13)
#define US_CR_RSTNACK                   BIT(14)
#define US_CR_RETTO                     BIT(15)
#define US_CR_RTSEN_FCS                 BIT(18)
#define US_CR_RTSDIS_RCS                BIT(19)
#define US_CR_LINABT                    BIT(20)
#define US_CR_LINWKUP                   BIT(21)

/* USART Mode Register */
#define US_MR_USART_MODE_POS            (0)
#define US_MR_USART_MODE_MASK           (0xf << US_MR_USART_MODE_POS)
#define US_MR_USART_MODE(value)         BITFIELD(US_MR_USART_MODE, value)
#define US_MR_USART_MODE_NORMAL         (0x0 << 0)
#define US_MR_USART_MODE_RS485          BIT(0)
#define US_MR_USART_MODE_HW_HANDSHAKING (0x2 << 0)
#define US_MR_USART_MODE_IS07816_T_0    (0x4 << 0)
#define US_MR_USART_MODE_IS07816_T_1    (0x6 << 0)
#define US_MR_USART_MODE_IRDA           (0x8 << 0)
#define US_MR_USART_MODE_LIN_MASTER     (0xa << 0)
#define US_MR_USART_MODE_LIN_SLAVE      (0xb << 0)
#define US_MR_USART_MODE_SPI_MASTER     (0xe << 0)
#define US_MR_USART_MODE_SPI_SLAVE      (0xf << 0)
#define US_MR_USCLKS_POS                (4)
#define US_MR_USCLKS_MASK               (0x3 << US_MR_USCLKS_POS)
#define US_MR_USCLKS(value)             BITFIELD(US_MR_USCLKS, value)
#define US_MR_USCLKS_MCK                (0x0 << 4)
#define US_MR_USCLKS_DIV                BIT(4)
#define US_MR_USCLKS_SCK                (0x3 << 4)
#define US_MR_CHRL_POS                  (6)
#define US_MR_CHRL_MASK                 (0x3 << US_MR_CHRL_POS)
#define US_MR_CHRL(value)               BITFIELD(US_MR_CHRL, value)
#define US_MR_CHRL_5_BIT                (0x0 << 6)
#define US_MR_CHRL_6_BIT                BIT(6)
#define US_MR_CHRL_7_BIT                (0x2 << 6)
#define US_MR_CHRL_8_BIT                (0x3 << 6)
#define US_MR_SYNC_CPHA                 BIT(8)
#define US_MR_PAR_POS                   (9)
#define US_MR_PAR_MASK                  (0x7 << US_MR_PAR_POS)
#define US_MR_PAR(value)                BITFIELD(US_MR_PAR, value)
#define US_MR_PAR_EVEN                  (0x0 << 9)
#define US_MR_PAR_ODD                   BIT(9)
#define US_MR_PAR_SPACE                 (0x2 << 9)
#define US_MR_PAR_MARK                  (0x3 << 9)
#define US_MR_PAR_NO                    (0x4 << 9)
#define US_MR_PAR_MULTIDROP             (0x6 << 9)
#define US_MR_NBSTOP_POS                (12)
#define US_MR_NBSTOP_MASK               (0x3 << US_MR_NBSTOP_POS)
#define US_MR_NBSTOP(value)             BITFIELD(US_MR_NBSTOP, value)
#define US_MR_NBSTOP_1_BIT              (0x0 << 12)
#define US_MR_NBSTOP_1_5_BIT            BIT(12)
#define US_MR_NBSTOP_2_BIT              (0x2 << 12)
#define US_MR_CHMODE_POS                (14)
#define US_MR_CHMODE_MASK               (0x3 << US_MR_CHMODE_POS)
#define US_MR_CHMODE(value)             BITFIELD(US_MR_CHMODE, value)
#define US_MR_CHMODE_NORMAL             (0x0 << 14)
#define US_MR_CHMODE_AUTOMATIC          BIT(14)
#define US_MR_CHMODE_LOCAL_LOOPBACK     (0x2 << 14)
#define US_MR_CHMODE_REMOTE_LOOPBACK    (0x3 << 14)
#define US_MR_MSBF                      BIT(16)
#define US_MR_CPOL                      BIT(16)
#define US_MR_MODE9                     BIT(17)
#define US_MR_CLKO                      BIT(18)
#define US_MR_OVER                      BIT(19)
#define US_MR_INACK                     BIT(20)
#define US_MR_DSNACK                    BIT(21)
#define US_MR_VAR_SYNC                  BIT(22)
#define US_MR_INVDATA                   BIT(23)
#define US_MR_MAX_ITERATION_POS         (24)
#define US_MR_MAX_ITERATION_MASK        (0x7 << US_MR_MAX_ITERATION_POS)
#define US_MR_MAX_ITERATION(value)      BITFIELD(US_MR_MAX_ITERATION, value)
#define US_MR_FILTER                    BIT(28)
#define US_MR_MAN                       BIT(29)
#define US_MR_MODSYNC                   BIT(30)
#define US_MR_ONEBIT                    BIT(31)

/* USART Interrupt Enable Register */
#define US_IER_RXRDY                    BIT(0)
#define US_IER_TXRDY                    BIT(1)
#define US_IER_RXBRK                    BIT(2)
#define US_IER_ENDRX                    BIT(3)
#define US_IER_ENDTX                    BIT(4)
#define US_IER_OVRE                     BIT(5)
#define US_IER_FRAME                    BIT(6)
#define US_IER_PARE                     BIT(7)
#define US_IER_TIMEOUT                  BIT(8)
#define US_IER_TXEMPTY                  BIT(9)
#define US_IER_ITER_UNRE                BIT(10)
#define US_IER_TXBUFE                   BIT(11)
#define US_IER_RXBUFF                   BIT(12)
#define US_IER_NACK_LINBK               BIT(13)
#define US_IER_LINID                    BIT(14)
#define US_IER_LINTC                    BIT(15)
#define US_IER_CTSIC                    BIT(19)
#define US_IER_MANE                     BIT(24)
#define US_IER_LINBE                    BIT(25)
#define US_IER_LINISFE                  BIT(26)
#define US_IER_LINIPE                   BIT(27)
#define US_IER_LINCE                    BIT(28)
#define US_IER_LINSNRE                  BIT(29)

/* USART Interrupt Disable Register */
#define US_IDR_RXRDY                    BIT(0)
#define US_IDR_TXRDY                    BIT(1)
#define US_IDR_RXBRK                    BIT(2)
#define US_IDR_ENDRX                    BIT(3)
#define US_IDR_ENDTX                    BIT(4)
#define US_IDR_OVRE                     BIT(5)
#define US_IDR_FRAME                    BIT(6)
#define US_IDR_PARE                     BIT(7)
#define US_IDR_TIMEOUT                  BIT(8)
#define US_IDR_TXEMPTY                  BIT(9)
#define US_IDR_ITER_UNRE                BIT(10)
#define US_IDR_TXBUFE                   BIT(11)
#define US_IDR_RXBUFF                   BIT(12)
#define US_IDR_NACK_LINBK               BIT(13)
#define US_IDR_LINID                    BIT(14)
#define US_IDR_LINTC                    BIT(15)
#define US_IDR_CTSIC                    BIT(19)
#define US_IDR_MANE                     BIT(24)
#define US_IDR_LINBE                    BIT(25)
#define US_IDR_LINISFE                  BIT(26)
#define US_IDR_LINIPE                   BIT(27)
#define US_IDR_LINCE                    BIT(28)
#define US_IDR_LINSNRE                  BIT(29)

/* USART Interrupt Mask Register */
#define US_IMR_RXRDY                    BIT(0)
#define US_IMR_TXRDY                    BIT(1)
#define US_IMR_RXBRK                    BIT(2)
#define US_IMR_ENDRX                    BIT(3)
#define US_IMR_ENDTX                    BIT(4)
#define US_IMR_OVRE                     BIT(5)
#define US_IMR_FRAME                    BIT(6)
#define US_IMR_PARE                     BIT(7)
#define US_IMR_TIMEOUT                  BIT(8)
#define US_IMR_TXEMPTY                  BIT(9)
#define US_IMR_ITER_UNRE                BIT(10)
#define US_IMR_TXBUFE                   BIT(11)
#define US_IMR_RXBUFF                   BIT(12)
#define US_IMR_NACK_LINBK               BIT(13)
#define US_IMR_LINID                    BIT(14)
#define US_IMR_LINTC                    BIT(15)
#define US_IMR_CTSIC                    BIT(19)
#define US_IMR_MANE                     BIT(24)
#define US_IMR_LINBE                    BIT(25)
#define US_IMR_LINISFE                  BIT(26)
#define US_IMR_LINIPE                   BIT(27)
#define US_IMR_LINCE                    BIT(28)
#define US_IMR_LINSNRE                  BIT(29)

/* USART Channel Status Register */
#define US_CSR_RXRDY                    BIT(0)
#define US_CSR_TXRDY                    BIT(1)
#define US_CSR_RXBRK                    BIT(2)
#define US_CSR_ENDRX                    BIT(3)
#define US_CSR_ENDTX                    BIT(4)
#define US_CSR_OVRE                     BIT(5)
#define US_CSR_FRAME                    BIT(6)
#define US_CSR_PARE                     BIT(7)
#define US_CSR_TIMEOUT                  BIT(8)
#define US_CSR_TXEMPTY                  BIT(9)
#define US_CSR_ITER_UNRE                BIT(10)
#define US_CSR_TXBUFE                   BIT(11)
#define US_CSR_RXBUFF                   BIT(12)
#define US_CSR_NACK_LINBK               BIT(13)
#define US_CSR_LINID                    BIT(14)
#define US_CSR_LINTC                    BIT(15)
#define US_CSR_CTSIC                    BIT(19)
#define US_CSR_CTS_LINBLS               BIT(23)
#define US_CSR_MANERR                   BIT(24)
#define US_CSR_LINBE                    BIT(25)
#define US_CSR_LINISFE                  BIT(26)
#define US_CSR_LINIPE                   BIT(27)
#define US_CSR_LINCE                    BIT(28)
#define US_CSR_LINSNRE                  BIT(29)

/* USART Receive Holding Register */
#define US_RHR_RXCHR_POS                (0)
#define US_RHR_RXCHR_MASK               (0x1ff << US_RHR_RXCHR_POS)
#define US_RHR_RXCHR(value)             BITFIELD(US_RHR_RXCHR, value)
#define US_RHR_RXSYNH                   BIT(15)

/* USART Transmit Holding Register */
#define US_THR_TXCHR_POS                (0)
#define US_THR_TXCHR_MASK               (0x1ff << US_THR_TXCHR_POS)
#define US_THR_TXCHR(value)             BITFIELD(US_THR_TXCHR, value)
#define US_THR_TXSYNH                   BIT(15)

/* USART Baud Rate Generator Register */
#define US_BRGR_CD_POS                  (0)
#define US_BRGR_CD_MASK                 (0xffff << US_BRGR_CD_POS)
#define US_BRGR_CD(value)               BITFIELD(US_BRGR_CD, value)
#define US_BRGR_FP_POS                  (16)
#define US_BRGR_FP_MASK                 (0x7 << US_BRGR_FP_POS)
#define US_BRGR_FP(value)               BITFIELD(US_BRGR_FP, value)

/* USART Receiver Time-out Register */
#define US_RTOR_TO_POS                  (0)
#define US_RTOR_TO_MASK                 (0x1ffff << US_RTOR_TO_POS)
#define US_RTOR_TO(value)               BITFIELD(US_RTOR_TO, value)

/* USART Transmitter Timeguard Register */
#define US_TTGR_TG_POS                  (0)
#define US_TTGR_TG_MASK                 (0xff << US_TTGR_TG_POS)
#define US_TTGR_TG(value)               BITFIELD(US_TTGR_TG, value)

/* USART FI DI RATIO Register */
#define US_FIDI_FI_DI_RATIO_POS         (0)
#define US_FIDI_FI_DI_RATIO_MASK        (0x7ff << US_FIDI_FI_DI_RATIO_POS)
#define US_FIDI_FI_DI_RATIO(value)      BITFIELD(US_FIDI_FI_DI_RATIO, value)

/* USART Number of Errors Register */
#define US_NER_NB_ERRORS_POS            (0)
#define US_NER_NB_ERRORS_MASK           (0xff << US_NER_NB_ERRORS_POS)
#define US_NER_NB_ERRORS(value)         BITFIELD(US_NER_NB_ERRORS, value)

/* USART IrDA FILTER Register */
#define US_IF_IRDA_FILTER_POS           (0)
#define US_IF_IRDA_FILTER_MASK          (0xff << US_IF_IRDA_FILTER_POS)
#define US_IF_IRDA_FILTER(value)        BITFIELD(US_IF_IRDA_FILTER, value)

/* USART Manchester Configuration Register */
#define US_MAN_TX_PL_POS                (0)
#define US_MAN_TX_PL_MASK               (0xf << US_MAN_TX_PL_POS)
#define US_MAN_TX_PL(value)             BITFIELD(US_MAN_TX_PL, value)
#define US_MAN_TX_PP_POS                (8)
#define US_MAN_TX_PP_MASK               (0x3 << US_MAN_TX_PP_POS)
#define US_MAN_TX_PP(value)             BITFIELD(US_MAN_TX_PP, value)
#define US_MAN_TX_PP_ALL_ONE            (0x0 << 8)
#define US_MAN_TX_PP_ALL_ZERO           BIT(8)
#define US_MAN_TX_PP_ZERO_ONE           (0x2 << 8)
#define US_MAN_TX_PP_ONE_ZERO           (0x3 << 8)
#define US_MAN_TX_MPOL                  BIT(12)
#define US_MAN_RX_PL_POS                (16)
#define US_MAN_RX_PL_MASK               (0xf << US_MAN_RX_PL_POS)
#define US_MAN_RX_PL(value)             BITFIELD(US_MAN_RX_PL, value)
#define US_MAN_RX_PP_POS                (24)
#define US_MAN_RX_PP_MASK               (0x3 << US_MAN_RX_PP_POS)
#define US_MAN_RX_PP(value)             BITFIELD(US_MAN_RX_PP, value)
#define US_MAN_RX_PP_ALL_ONE            (0x0 << 24)
#define US_MAN_RX_PP_ALL_ZERO           BIT(24)
#define US_MAN_RX_PP_ZERO_ONE           (0x2 << 24)
#define US_MAN_RX_PP_ONE_ZERO           (0x3 << 24)
#define US_MAN_RX_MPOL                  BIT(28)
#define US_MAN_STUCKTO1                 BIT(29)
#define US_MAN_DRIFT                    BIT(30)

/* USART LIN Mode Register */
#define US_LINMR_NACT_POS               (0)
#define US_LINMR_NACT_MASK              (0x3 << US_LINMR_NACT_POS)
#define US_LINMR_NACT(value)            BITFIELD(US_LINMR_NACT, value)
#define US_LINMR_NACT_PUBLISH           (0x0 << 0)
#define US_LINMR_NACT_SUBSCRIBE         BIT(0)
#define US_LINMR_NACT_IGNORE            (0x2 << 0)
#define US_LINMR_PARDIS                 BIT(2)
#define US_LINMR_CHKDIS                 BIT(3)
#define US_LINMR_CHKTYP                 BIT(4)
#define US_LINMR_DLM                    BIT(5)
#define US_LINMR_FSDIS                  BIT(6)
#define US_LINMR_WKUPTYP                BIT(7)
#define US_LINMR_DLC_POS                (8)
#define US_LINMR_DLC_MASK               (0xff << US_LINMR_DLC_POS)
#define US_LINMR_DLC(value)             BITFIELD(US_LINMR_DLC, value)
#define US_LINMR_PDCM                   BIT(16)

/* USART LIN Identifier Register */
#define US_LINIR_IDCHR_POS              (0)
#define US_LINIR_IDCHR_MASK             (0xff << US_LINIR_IDCHR_POS)
#define US_LINIR_IDCHR(value)           BITFIELD(US_LINIR_IDCHR, value)

/* USART Write Protect Mode Register */
#define US_WPMR_WPEN                    BIT(0)
#define US_WPMR_WPKEY_POS               (8)
#define US_WPMR_WPKEY_MASK              (0xffffff << US_WPMR_WPKEY_POS)
#define US_WPMR_WPKEY(value)            BITFIELD(US_WPMR_WPKEY, value)

/* USART Write Protect Status Register */
#define US_WPSR_WPVS                    BIT(0)
#define US_WPSR_WPVSRC_POS              (8)
#define US_WPSR_WPVSRC_MASK             (0xffff << US_WPSR_WPVSRC_POS)
#define US_WPSR_WPVSRC(value)           BITFIELD(US_WPSR_WPVSRC, value)

/* Base addresses of private peripherals. */
#define SAM_SCB        ((volatile struct sam_system_control_block_t *)0xe000e008u)
#define SAM_ST         ((volatile struct sam_system_timer_t         *)0xe000e010u)
#define SAM_NVIC       ((volatile struct sam_nvic_t                 *)0xe000e100u)

struct sam_pio_t {
    uint32_t PER;
    uint32_t PDR;
    uint32_t PSR;
    uint32_t reserved1;
    uint32_t OER;
    uint32_t ODR;
    uint32_t OSR;
    uint32_t reserved2;
    uint32_t IFER;
    uint32_t IFDR;
    uint32_t IFSR;
    uint32_t reserved3;
    uint32_t SODR;
    uint32_t CODR;
    uint32_t ODSR;
    uint32_t PDSR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t ISR;
    uint32_t MDER;
    uint32_t MDDR;
    uint32_t MDSR;
    uint32_t reserved4;
    uint32_t PUDR;
    uint32_t PUER;
    uint32_t PUSR;
    uint32_t reserved5;
    uint32_t ABSR;
    uint32_t reserved6[3];
    uint32_t SCIFSR;
    uint32_t DIFSR;
    uint32_t IFDGSR;
    uint32_t SCDR;
    uint32_t reserved7[4];
    uint32_t OWER;
    uint32_t OWDR;
    uint32_t OWSR;
    uint32_t reserved8;
    uint32_t AIMER;
    uint32_t AIMDR;
    uint32_t AIMMR;
    uint32_t reserved9;
    uint32_t ESR;
    uint32_t LSR;
    uint32_t ELSR;
    uint32_t reserved10;
    uint32_t FELLSR;
    uint32_t REHLSR;
    uint32_t FRLHSR;
    uint32_t reserved11;
    uint32_t LOCKSR;
    uint32_t WPMR;
    uint32_t WPSR;
};

/* Port definitions. */
#define SAM_PIO_P0                      BIT(0)
#define SAM_PIO_P1                      BIT(1)
#define SAM_PIO_P2                      BIT(2)
#define SAM_PIO_P3                      BIT(3)
#define SAM_PIO_P4                      BIT(4)
#define SAM_PIO_P5                      BIT(5)
#define SAM_PIO_P6                      BIT(6)
#define SAM_PIO_P7                      BIT(7)
#define SAM_PIO_P8                      BIT(8)
#define SAM_PIO_P9                      BIT(9)
#define SAM_PIO_P10                     BIT(10)
#define SAM_PIO_P11                     BIT(11)
#define SAM_PIO_P12                     BIT(12)
#define SAM_PIO_P13                     BIT(13)
#define SAM_PIO_P14                     BIT(14)
#define SAM_PIO_P15                     BIT(15)
#define SAM_PIO_P16                     BIT(16)
#define SAM_PIO_P17                     BIT(17)
#define SAM_PIO_P18                     BIT(18)
#define SAM_PIO_P19                     BIT(19)
#define SAM_PIO_P20                     BIT(20)
#define SAM_PIO_P21                     BIT(21)
#define SAM_PIO_P22                     BIT(22)
#define SAM_PIO_P23                     BIT(23)
#define SAM_PIO_P24                     BIT(24)
#define SAM_PIO_P25                     BIT(25)
#define SAM_PIO_P26                     BIT(26)
#define SAM_PIO_P27                     BIT(27)
#define SAM_PIO_P28                     BIT(28)
#define SAM_PIO_P29                     BIT(29)
#define SAM_PIO_P30                     BIT(30)
#define SAM_PIO_P31                     BIT(31)

struct sam_wdt_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t SR;
};

/* Base addresses of peripherals. */
#define SAM_HSMCI      ((volatile struct sam_hsmci_t  *)0x40000000u)
#define SAM_SSC        ((volatile struct sam_ssc_t    *)0x40004000u)
#define SAM_SPI0       ((volatile struct sam_spi_t    *)0x40008000u)
#define SAM_TC0        ((volatile struct sam_tc_t     *)0x40080000u)
#define SAM_TC1        ((volatile struct sam_tc_t     *)0x40084000u)
#define SAM_TC2        ((volatile struct sam_tc_t     *)0x40088000u)
#define SAM_TWI0       ((volatile struct sam_twi_t    *)0x4008c000u)
#define SAM_PDC_TWI0   ((volatile struct sam_pdc_t    *)0x4008c100u)
#define SAM_TWI1       ((volatile struct sam_twi_t    *)0x40090000u)
#define SAM_PDC_TWI1   ((volatile struct sam_pdc_t    *)0x40090100u)
#define SAM_PWM        ((volatile struct sam_pwm_t    *)0x40094000u)
#define SAM_PDC_PWM    ((volatile struct sam_pdc_t    *)0x40094100u)
#define SAM_USART0     ((volatile struct sam_usart_t  *)0x40098000u)
#define SAM_USART1     ((volatile struct sam_usart_t  *)0x4009c000u)
#define SAM_USART2     ((volatile struct sam_usart_t  *)0x400a0000u)
#define SAM_USART3     ((volatile struct sam_usart_t  *)0x400a4000u)
#define SAM_UOTGHS     ((volatile struct sam_uotghs_t *)0x400ac000u)
#define SAM_EMAC       ((volatile struct sam_emac_t   *)0x400b0000u)
#define SAM_CAN0       ((volatile struct sam_can_t    *)0x400b4000u)
#define SAM_CAN1       ((volatile struct sam_can_t    *)0x400b8000u)
#define SAM_TRNG       ((volatile struct sam_trng_t   *)0x400bc000u)
#define SAM_ADC        ((volatile struct sam_adc_t    *)0x400c0000u)
#define SAM_PDC_ADC    ((volatile struct sam_pdc_t    *)0x400c0100u)
#define SAM_DMAC       ((volatile struct sam_dmac_t   *)0x400c4000u)
#define SAM_DACC       ((volatile struct sam_dacc_t   *)0x400c8000u)
#define SAM_PDC_DACC   ((volatile struct sam_pdc_t    *)0x400c8100u)
#define SAM_SMC        ((volatile struct sam_smc_t    *)0x400e0000u)
#define SAM_MATRIX     ((volatile struct sam_matrix_t *)0x400e0400u)
#define SAM_PMC        ((volatile struct sam_pmc_t    *)0x400e0600u)
#define SAM_UART       ((volatile struct sam_uart_t   *)0x400e0800u)
#define SAM_PDC_UART   ((volatile struct sam_pdc_t    *)0x400e0900u)
#define SAM_CHIPID     ((volatile struct sam_chipid_t *)0x400e0940u)
#define SAM_EEFC0      ((volatile struct sam_eefc_t   *)0x400e0a00u)
#define SAM_EEFC1      ((volatile struct sam_eefc_t   *)0x400e0c00u)
#define SAM_PIOA       ((volatile struct sam_pio_t    *)0x400e0e00u)
#define SAM_PIOB       ((volatile struct sam_pio_t    *)0x400e1000u)
#define SAM_PIOC       ((volatile struct sam_pio_t    *)0x400e1200u)
#define SAM_PIOD       ((volatile struct sam_pio_t    *)0x400e1400u)
#define SAM_RSTC       ((volatile struct sam_rstc_t   *)0x400e1a00u)
#define SAM_SUPC       ((volatile struct sam_supc_t   *)0x400e1a10u)
#define SAM_RTT        ((volatile struct sam_rtt_t    *)0x400e1a30u)
#define SAM_WDT        ((volatile struct sam_wdt_t    *)0x400e1a50u)
#define SAM_RTC        ((volatile struct sam_rtc_t    *)0x400e1a60u)
#define SAM_GPBR       ((volatile struct sam_gpbr_t   *)0x400e1a90u)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

/* Internal flash memory. */
#define FLASH0_SIZE      0x40000
#define FLASH0_BEGIN     0x80000
#define FLASH0_END       (FLASH0_BEGIN + FLASH0_SIZE)
#define FLASH0_PAGE_SIZE 0x100

#define FLASH1_SIZE      0x40000
#define FLASH1_BEGIN     (FLASH0_END)
#define FLASH1_END       (FLASH1_BEGIN + FLASH1_SIZE)
#define FLASH1_PAGE_SIZE 0x100

#define FLASH_IAP_ADDRESS (0x00100008)

static inline void nvic_enable_interrupt(int id)
{
    SAM_NVIC->ICP[id / 32] = (1 << (id % 32));
    SAM_NVIC->ISE[id / 32] = (1 << (id % 32));
}

static inline void pmc_peripheral_clock_enable(int id)
{
    if (id < 32) {
        SAM_PMC->PCER0 = (1 << (id % 32));
    } else {
        SAM_PMC->PCER1 = (1 << (id % 32));
    }
}

#endif
