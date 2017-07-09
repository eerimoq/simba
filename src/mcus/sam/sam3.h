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

#ifndef __SAM3_H__
#define __SAM3_H__

/* See "SAM3X / SAM3A Series, Atmel | SMART ARM-based MCU, DATASHEET"
   for details. */

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

/* 12. Reset controller. */
struct sam_rstc_t {
    uint32_t CR;
    uint32_t SR;
    uint32_t MR;
};

#define RSTC_CR_PROCRST                               BIT(0)
#define RSTC_CR_PERRST                                BIT(2)
#define RSTC_CR_EXTRST                                BIT(3)
#define RSTC_CR_KEY_POS                                 (24)
#define RSTC_CR_KEY_MASK           (0xff << RSTC_CR_KEY_POS)
#define RSTC_CR_KEY(value)  BITFIELD_SET(RSTC_CR_KEY, value)

/* 15. Watchdog Timer. */
struct sam_wdt_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t SR;
};

/* 18. Embedded flash controller. */
struct sam_eefc_t {
    uint32_t FMR;
    uint32_t FCR;
    uint32_t FSR;
    uint32_t FRR;
};

#define EEFC_FMR_FRDY                   BIT(0)
#define EEFC_FMR_FWS_POS                (8)
#define EEFC_FMR_FWS_MASK               (0xf << EEFC_FMR_FWS_POS)
#define EEFC_FMR_FWS(value)             BITFIELD_SET(EEFC_FMR_FWS, value)
#define EEFC_FMR_FAM                    BIT(24)
#define EEFC_FCR_FCMD_POS               (0)
#define EEFC_FCR_FCMD_MASK              (0xff << EEFC_FCR_FCMD_POS)
#define EEFC_FCR_FCMD(value)            BITFIELD_SET(EEFC_FCR_FCMD, value)
#define EEFC_FCR_FARG_POS               (8)
#define EEFC_FCR_FARG_MASK              (0xffff << EEFC_FCR_FARG_POS)
#define EEFC_FCR_FARG(value)            BITFIELD_SET(EEFC_FCR_FARG, value)
#define EEFC_FCR_FKEY_POS               (24)
#define EEFC_FCR_FKEY_MASK              (0xff << EEFC_FCR_FKEY_POS)
#define EEFC_FCR_FKEY(value)            BITFIELD_SET(EEFC_FCR_FKEY, value)
#define EEFC_FSR_FRDY                   BIT(0)
#define EEFC_FSR_FCMDE                  BIT(1)
#define EEFC_FSR_FLOCKE                 BIT(2)
#define EEFC_FRR_FVALUE_POS             (0)
#define EEFC_FRR_FVALUE_MASK            (0xffffffff << EEFC_FRR_FVALUE_POS)
#define EEFC_FRR_FVALUE(value)          BITFIELD_SET(EEFC_FRR_FVALUE, value)

/* 22. AHB DMA Controller. */
struct sam_dmac_t {
    uint32_t GCFG;
    uint32_t EN;
    uint32_t SREQ;
    uint32_t CREQ;
    uint32_t LAST;
    uint32_t RESERVED1;
    uint32_t EBCIER;
    uint32_t EBCIDR;
    uint32_t EBCIMR;
    uint32_t EBCISR;
    uint32_t CHER;
    uint32_t CHDR;
    uint32_t CHSR;
    uint32_t RESERVED2[2];
    struct {
        uint32_t SADDR;
        uint32_t DADDR;
        uint32_t DSCR;
        uint32_t CTRLA;
        uint32_t CTRLB;
        uint32_t CFG;
        uint32_t RESERVED1[4];
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
#define DMAC_CTRLA_BTSIZE(value)        BITFIELD_SET(DMAC_CTRLA_BTSIZE, (value))
#define DMAC_CTRLA_SCSIZE_POS           (16)
#define DMAC_CTRLA_SCSIZE_MASK          (0x7 << DMAC_CTRLA_SCSIZE_POS)
#define DMAC_CTRLA_SCSIZE(value)        BITFIELD_SET(DMAC_CTRLA_SCSIZE, (value))
#define DMAC_CTRLA_DCSIZE_POS           (20)
#define DMAC_CTRLA_DCSIZE_MASK          (0x7 << DMAC_CTRLA_DCSIZE_POS)
#define DMAC_CTRLA_DCSIZE(value)        BITFIELD_SET(DMAC_CTRLA_DCSIZE, (value))
#define DMAC_CTRLA_SRC_WIDTH_POS        (24)
#define DMAC_CTRLA_SRC_WIDTH_MASK       (0x3 << DMAC_CTRLA_SRC_WIDTH_POS)
#define DMAC_CTRLA_SRC_WIDTH(value)     BITFIELD_SET(DMAC_CTRLA_SRC_WIDTH, (value))
#define DMAC_CTRLA_DST_WIDTH_POS        (28)
#define DMAC_CTRLA_DST_WIDTH_MASK       (0x3 << DMAC_CTRLA_DST_WIDTH_POS)
#define DMAC_CTRLA_DST_WIDTH(value)     BITFIELD_SET(DMAC_CTRLA_DST_WIDTH, (value))
#define DMAC_CTRLA_DONE                 BIT(31)

/* DMAC Channel x [x = 0..5] Control B Register */
#define DMAC_CTRLB_SRC_DSCR             (16)
#define DMAC_CTRLB_DST_DSCR             (20)
#define DMAC_CTRLB_FC_POS               (21)
#define DMAC_CTRLB_FC_MASK              (0x7 << DMAC_CTRLB_FC_POS)
#define DMAC_CTRLB_FC(value)            BITFIELD_SET(DMAC_CTRLB_FC, (value))
#define DMAC_CTRLB_SRC_INCR_POS         (24)
#define DMAC_CTRLB_SRC_INCR_MASK        (0x3 << DMAC_CTRLB_SRC_INCR_POS)
#define DMAC_CTRLB_SRC_INCR(value)      BITFIELD_SET(DMAC_CTRLB_SRC_INCR, (value))
#define DMAC_CTRLB_DST_INCR_POS         (28)
#define DMAC_CTRLB_DST_INCR_MASK        (0x3 << DMAC_CTRLB_DST_INCR_POS)
#define DMAC_CTRLB_DST_INCR(value)      BITFIELD_SET(DMAC_CTRLB_DST_INCR, (value))
#define DMAC_CTRLB_IEN                  BIT(30)

/* DMAC Channel x [x = 0..5] Configuration Register */
#define DMAC_CFG_SRC_PER_POS            (0)
#define DMAC_CFG_SRC_PER_MASK           (0xf << DMAC_CFG_SRC_PER_POS)
#define DMAC_CFG_SRC_PER(value)         BITFIELD_SET(DMAC_CFG_SRC_PER, (value))
#define DMAC_CFG_DST_PER_POS            (4)
#define DMAC_CFG_DST_PER_MASK           (0xf << DMAC_CFG_DST_PER_POS)
#define DMAC_CFG_DST_PER(value)         BITFIELD_SET(DMAC_CFG_DST_PER, (value))
#define DMAC_CFG_SRC_H2SEL              BIT(9)
#define DMAC_CFG_DST_H2SEL              BIT(13)
#define DMAC_CFG_SOD                    BIT(16)
#define DMAC_CFG_LOCK_IF                BIT(20)
#define DMAC_CFG_LOCK_B                 BIT(21)
#define DMAC_CFG_LOCK_IF_L              BIT(22)
#define DMAC_CFG_AHB_PROT_POS           (24)
#define DMAC_CFG_AHB_PROT_MASK          (0x7 << DMAC_CFG_AHB_PROT_POS)
#define DMAC_CFG_AHB_PROT(value)        BITFIELD_SET(DMAC_CFG_AHB_PROT, (value))
#define DMAC_CFG_FIFOCFG_POS            (28)
#define DMAC_CFG_FIFOCFG_MASK           (0x3 << DMAC_CFG_FIFOCFG_POS)
#define DMAC_CFG_FIFOCFG(value)         BITFIELD_SET(DMAC_CFG_FIFOCFG, (value))

/* DMAC Write Protect Mode Register */
#define DMAC_WPMR_WPEN                  BIT(0)

/* DMAC Write Protect Status Register */
#define DMAC_WPSR_WPVS                  BIT(0)

/* 26. Peripheral DMA Controller. */
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
#define PERIPH_RPR_RXPTR(value)         BITFIELD_SET(PERIPH_RPR_RXPTR, value)

/* Receive Counter Register */
#define PERIPH_RCR_RXCTR_POS            (0)
#define PERIPH_RCR_RXCTR_MASK           (0xffff << PERIPH_RCR_RXCTR_POS)
#define PERIPH_RCR_RXCTR(value)         BITFIELD_SET(PERIPH_RCR_RXCTR, value)

/* Transmit Pointer Register */
#define PERIPH_TPR_TXPTR_POS            (0)
#define PERIPH_TPR_TXPTR_MASK           (0xffffffff << PERIPH_TPR_TXPTR_POS)
#define PERIPH_TPR_TXPTR(value)         BITFIELD_SET(PERIPH_TPR_TXPTR, value)

/* Transmit Counter Register */
#define PERIPH_TCR_TXCTR_POS            (0)
#define PERIPH_TCR_TXCTR_MASK           (0xffff << PERIPH_TCR_TXCTR_POS)
#define PERIPH_TCR_TXCTR(value)         BITFIELD_SET(PERIPH_TCR_TXCTR, value)

/* Receive Next Pointer Register */
#define PERIPH_RNPR_RXNPTR_POS          (0)
#define PERIPH_RNPR_RXNPTR_MASK         (0xffffffff << PERIPH_RNPR_RXNPTR_POS)
#define PERIPH_RNPR_RXNPTR(value)       BITFIELD_SET(PERIPH_RNPR_RXNPTR, value)

/* Receive Next Counter Register */
#define PERIPH_RNCR_RXNCTR_POS          (0)
#define PERIPH_RNCR_RXNCTR_MASK         (0xffff << PERIPH_RNCR_RXNCTR_POS)
#define PERIPH_RNCR_RXNCTR(value)       BITFIELD_SET(PERIPH_RNCR_RXNCTR, value)

/* Transmit Next Pointer Register */
#define PERIPH_TNPR_TXNPTR_POS          (0)
#define PERIPH_TNPR_TXNPTR_MASK         (0xffffffff << PERIPH_TNPR_TXNPTR_POS)
#define PERIPH_TNPR_TXNPTR(value)       BITFIELD_SET(PERIPH_TNPR_TXNPTR, value)

/* Transmit Next Counter Register */
#define PERIPH_TNCR_TXNCTR_POS          (0)
#define PERIPH_TNCR_TXNCTR_MASK         (0xffff << PERIPH_TNCR_TXNCTR_POS)
#define PERIPH_TNCR_TXNCTR(value)       BITFIELD_SET(PERIPH_TNCR_TXNCTR, value)

/* Transfer Control Register */
#define PERIPH_PTCR_RXTEN               BIT(0)
#define PERIPH_PTCR_RXTDIS              BIT(1)
#define PERIPH_PTCR_TXTEN               BIT(8)
#define PERIPH_PTCR_TXTDIS              BIT(9)

/* Transfer Status Register */
#define PERIPH_PTSR_RXTEN               BIT(0)
#define PERIPH_PTSR_TXTEN               BIT(8)

/* 28. Power management controller. */
struct sam_pmc_t {
    uint32_t SCER;
    uint32_t SCDR;
    uint32_t SCSR;
    uint32_t RESERVED1;
    uint32_t PCER0;
    uint32_t PCDR0;
    uint32_t PCSR0;
    uint32_t CKGR_UCKR;
    uint32_t CKGR_MOR;
    uint32_t CKGR_MCFR;
    uint32_t CKGR_PLLAR;
    uint32_t RESERVED2;
    uint32_t MCKR;
    uint32_t RESERVED3;
    uint32_t USB;
    uint32_t RESERVED4;
    uint32_t PCK[3];
    uint32_t RESERVED5[5];
    uint32_t IER;
    uint32_t IDR;
    uint32_t SR;
    uint32_t IMR;
    uint32_t FSMR;
    uint32_t FSPR;
    uint32_t FOCR;
    uint32_t RESERVED6[26];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t RESERVED7[5];
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
#define PMC_CKGR_UCKR_UPLLCOUNT(value)  BITFIELD_SET(PMC_CKGR_UCKR_UPLLCOUNT, value)

#define PMC_CKGR_MOR_MOSCXTEN           BIT(0)
#define PMC_CKGR_MOR_MOSCXTBY           BIT(1)
#define PMC_CKGR_MOR_MOSCRCEN           BIT(3)
#define PMC_CKGR_MOR_MOSCRCF_POS        (4)
#define PMC_CKGR_MOR_MOSCRCF_MASK       (0x7 << PMC_CKGR_MOR_MOSCRCF_POS)
#define PMC_CKGR_MOR_MOSCRCF(value)     BITFIELD_SET(PMC_CKGR_MOR_MOSCRCF, value)
#define PMC_CKGR_MOR_MOSCRCF_4_MHz      PMC_CKGR_MOR_MOSCRCF(0)
#define PMC_CKGR_MOR_MOSCRCF_8_MHz      PMC_CKGR_MOR_MOSCRCF(1)
#define PMC_CKGR_MOR_MOSCRCF_12_MHz     PMC_CKGR_MOR_MOSCRCF(2)
#define PMC_CKGR_MOR_MOSCXTST_POS       (8)
#define PMC_CKGR_MOR_MOSCXTST_MASK      (0xff << PMC_CKGR_MOR_MOSCXTST_POS)
#define PMC_CKGR_MOR_MOSCXTST(value)    BITFIELD_SET(PMC_CKGR_MOR_MOSCXTST, value)
#define PMC_CKGR_MOR_KEY_POS            (16)
#define PMC_CKGR_MOR_KEY_MASK           (0xff << PMC_CKGR_MOR_KEY_POS)
#define PMC_CKGR_MOR_KEY(value)         BITFIELD_SET(PMC_CKGR_MOR_KEY, value)
#define PMC_CKGR_MOR_MOSCSEL            BIT(24)
#define PMC_CKGR_MOR_CFDEN              BIT(25)

#define PMC_CKGR_MCFR_MAINF_POS         (0)
#define PMC_CKGR_MCFR_MAINF_MASK        (0xffff << PMC_CKGR_MCFR_MAINF_POS)
#define PMC_CKGR_MCFR_MAINF(value)      BITFIELD_SET(PMC_CKGR_MCFR_MAINF, value)
#define PMC_CKGR_MCFR_MAINFRDY          BIT(16)

#define PMC_CKGR_PLLAR_DIVA_POS         (0)
#define PMC_CKGR_PLLAR_DIVA_MASK        (0xff << PMC_CKGR_PLLAR_DIVA_POS)
#define PMC_CKGR_PLLAR_DIVA(value)      BITFIELD_SET(PMC_CKGR_PLLAR_DIVA, value)
#define PMC_CKGR_PLLAR_PLLACOUNT_POS    (8)
#define PMC_CKGR_PLLAR_PLLACOUNT_MASK   (0x3f << PMC_CKGR_PLLAR_PLLACOUNT_POS)
#define PMC_CKGR_PLLAR_PLLACOUNT(value) BITFIELD_SET(PMC_CKGR_PLLAR_PLLACOUNT, value)
#define PMC_CKGR_PLLAR_MULA_POS         (16)
#define PMC_CKGR_PLLAR_MULA_MASK        (0x7ff << PMC_CKGR_PLLAR_MULA_POS)
#define PMC_CKGR_PLLAR_MULA(value)      BITFIELD_SET(PMC_CKGR_PLLAR_MULA, value)
#define PMC_CKGR_PLLAR_ONE              BIT(29)

#define PMC_MCKR_CSS_POS                (0)
#define PMC_MCKR_CSS_MASK               (0x3 << PMC_MCKR_CSS_POS)
#define PMC_MCKR_CSS(value)             BITFIELD_SET(PMC_MCKR_CSS, value)
#define PMC_MCKR_CSS_SLOW_CLK           PMC_MCKR_CSS(0)
#define PMC_MCKR_CSS_MAIN_CLK           PMC_MCKR_CSS(1)
#define PMC_MCKR_CSS_PLLA_CLK           PMC_MCKR_CSS(2)
#define PMC_MCKR_CSS_UPLL_CLK           PMC_MCKR_CSS(3)
#define PMC_MCKR_PRES_POS               (4)
#define PMC_MCKR_PRES_MASK              (0x7 << PMC_MCKR_PRES_POS)
#define PMC_MCKR_PRES(value)            BITFIELD_SET(PMC_MCKR_PRES, value)
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
#define PMC_USB_USBDIV(value)           BITFIELD_SET(PMC_USB_USBDIV, value)

#define PMC_PCK_CSS_POS                 (0)
#define PMC_PCK_CSS_MASK                (0x7 << PMC_PCK_CSS_POS)
#define PMC_PCK_CSS(value)              BITFIELD_SET(PMC_PCK_CSS, value)
#define PMC_PCK_CSS_SLOW_CLK            PMC_PCK_CSS(0)
#define PMC_PCK_CSS_MAIN_CLK            PMC_PCK_CSS(1)
#define PMC_PCK_CSS_PLLA_CLK            PMC_PCK_CSS(2)
#define PMC_PCK_CSS_UPLL_CLK            PMC_PCK_CSS(3)
#define PMC_PCK_CSS_MCK                 PMC_PCK_CSS(4)
#define PMC_PCK_PRES_POS                (4)
#define PMC_PCK_PRES_MASK               (0x7 << PMC_PCK_PRES_POS)
#define PMC_PCK_PRES(value)             BITFIELD_SET(PMC_PCK_PRES, value)
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
#define PMC_IER_LOCKU                   BIT(6)
#define PMC_IER_PCKRDY0                 BIT(8)
#define PMC_IER_PCKRDY1                 BIT(9)
#define PMC_IER_PCKRDY2                 BIT(10)
#define PMC_IER_MOSCSELS                BIT(16)
#define PMC_IER_MOSCRCS                 BIT(17)
#define PMC_IER_CFDEV                   BIT(18)

#define PMC_IDR_MOSCXTS                 BIT(0)
#define PMC_IDR_LOCKA                   BIT(1)
#define PMC_IDR_MCKRDY                  BIT(3)
#define PMC_IDR_LOCKU                   BIT(6)
#define PMC_IDR_PCKRDY0                 BIT(8)
#define PMC_IDR_PCKRDY1                 BIT(9)
#define PMC_IDR_PCKRDY2                 BIT(10)
#define PMC_IDR_MOSCSELS                BIT(16)
#define PMC_IDR_MOSCRCS                 BIT(17)
#define PMC_IDR_CFDEV                   BIT(18)

#define PMC_SR_MOSCXTS                  BIT(0)
#define PMC_SR_LOCKA                    BIT(1)
#define PMC_SR_MCKRDY                   BIT(3)
#define PMC_SR_LOCKU                    BIT(6)
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
#define PMC_IMR_LOCKU                   BIT(6)
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
#define PMC_WPMR_WPKEY(value)           BITFIELD_SET(PMC_WPMR_WPKEY, value)

#define PMC_WPSR_WPVS                   BIT(0)
#define PMC_WPSR_WPVSRC_POS             (8)
#define PMC_WPSR_WPVSRC_MASK            (0xffff << PMC_WPSR_WPVSRC_POS)
#define PMC_WPSR_WPVSRC(value)          BITFIELD_SET(PMC_WPSR_WPVSRC, value)

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
#define PMC_PCR_PID(value)              BITFIELD_SET(PMC_PCR_PID, value)
#define PMC_PCR_CMD                     BIT(12)
#define PMC_PCR_DIV_POS                 (16)
#define PMC_PCR_DIV_MASK                (0x3 << PMC_PCR_DIV_POS)
#define PMC_PCR_DIV(value)              BITFIELD_SET(PMC_PCR_DIV, value)
#define PMC_PCR_DIV_PERIPH_DIV_MCK      PMC_PCR_DIV(0)
#define PMC_PCR_DIV_PERIPH_DIV2_MCK     PMC_PCR_DIV(1)
#define PMC_PCR_DIV_PERIPH_DIV4_MCK     PMC_PCR_DIV(2)
#define PMC_PCR_EN                      BIT(28)

/* 29. Chip Identifier. */
struct sam_chipid_t {
    uint32_t CIDR;
    uint32_t EXID;
};

#define CHIPID_CIDR_VERSION_POS         (0)
#define CHIPID_CIDR_VERSION_MASK        (0x1f << CHIPID_CIDR_VERSION_POS)
#define CHIPID_CIDR_VERSION(value)      BITFIELD_SET(CHIPID_CIDR_VERSION, value)
#define CHIPID_CIDR_VERSION_GET(value)  BITFIELD_GET(CHIPID_CIDR_VERSION, value)
#define CHIPID_CIDR_EPROC_POS           (5)
#define CHIPID_CIDR_EPROC_MASK          (0x7 << CHIPID_CIDR_EPROC_POS)
#define CHIPID_CIDR_EPROC(value)        BITFIELD_SET(CHIPID_CIDR_EPROC, value)
#define CHIPID_CIDR_EPROC_GET(value)    BITFIELD_GET(CHIPID_CIDR_EPROC, value)
#define CHIPID_CIDR_NVPSIZ_POS          (8)
#define CHIPID_CIDR_NVPSIZ_MASK         (0xf << CHIPID_CIDR_NVPSIZ_POS)
#define CHIPID_CIDR_NVPSIZ(value)       BITFIELD_SET(CHIPID_CIDR_NVPSIZ, value)
#define CHIPID_CIDR_NVPSIZ_GET(value)   BITFIELD_GET(CHIPID_CIDR_NVPSIZ, value)
#define CHIPID_CIDR_NVPSIZ2_POS         (12)
#define CHIPID_CIDR_NVPSIZ2_MASK        (0xf << CHIPID_CIDR_NVPSIZ2_POS)
#define CHIPID_CIDR_NVPSIZ2(value)      BITFIELD_SET(CHIPID_CIDR_NVPSIZ2, value)
#define CHIPID_CIDR_NVPSIZ2_GET(value)  BITFIELD_GET(CHIPID_CIDR_NVPSIZ2, value)
#define CHIPID_CIDR_SRAMSIZ_POS         (16)
#define CHIPID_CIDR_SRAMSIZ_MASK        (0xf << CHIPID_CIDR_SRAMSIZ_POS)
#define CHIPID_CIDR_SRAMSIZ(value)      BITFIELD_SET(CHIPID_CIDR_SRAMSIZ, value)
#define CHIPID_CIDR_SRAMSIZ_GET(value)  BITFIELD_GET(CHIPID_CIDR_SRAMSIZ, value)
#define CHIPID_CIDR_ARCH_POS            (20)
#define CHIPID_CIDR_ARCH_MASK           (0xff << CHIPID_CIDR_ARCH_POS)
#define CHIPID_CIDR_ARCH(value)         BITFIELD_SET(CHIPID_CIDR_ARCH, value)
#define CHIPID_CIDR_ARCH_GET(value)     BITFIELD_GET(CHIPID_CIDR_ARCH, value)
#define CHIPID_CIDR_NVPTYP_POS          (28)
#define CHIPID_CIDR_NVPTYP_MASK         (0xf << CHIPID_CIDR_NVPTYP_POS)
#define CHIPID_CIDR_NVPTYP(value)       BITFIELD_SET(CHIPID_CIDR_NVPTYP, value)
#define CHIPID_CIDR_NVPTYP_GET(value)   BITFIELD_GET(CHIPID_CIDR_NVPTYP, value)
#define SAM_CHIPID_EXT                  BIT(31)

/* 31. Parallel Input/Output Controller. */
struct sam_pio_t {
    uint32_t PER;
    uint32_t PDR;
    uint32_t PSR;
    uint32_t RESERVED1;
    uint32_t OER;
    uint32_t ODR;
    uint32_t OSR;
    uint32_t RESERVED2;
    uint32_t IFER;
    uint32_t IFDR;
    uint32_t IFSR;
    uint32_t RESERVED3;
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
    uint32_t RESERVED4;
    uint32_t PUDR;
    uint32_t PUER;
    uint32_t PUSR;
    uint32_t RESERVED5;
    uint32_t ABSR;
    uint32_t RESERVED6[3];
    uint32_t SCIFSR;
    uint32_t DIFSR;
    uint32_t IFDGSR;
    uint32_t SCDR;
    uint32_t RESERVED7[4];
    uint32_t OWER;
    uint32_t OWDR;
    uint32_t OWSR;
    uint32_t RESERVED8;
    uint32_t AIMER;
    uint32_t AIMDR;
    uint32_t AIMMR;
    uint32_t RESERVED9;
    uint32_t ESR;
    uint32_t LSR;
    uint32_t ELSR;
    uint32_t RESERVED10;
    uint32_t FELLSR;
    uint32_t REHLSR;
    uint32_t FRLHSR;
    uint32_t RESERVED11;
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

/* 32. Serial peripheral interface. */
struct sam_spi_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t RDR;
    uint32_t TDR;
    uint32_t SR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t RESERVED1[4];
    uint32_t CSR[4];
    uint32_t RESERVED2[38];
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
#define SPI_MR_PCS(value)               BITFIELD_SET(SPI_MR_PCS, value)
#define SPI_MR_DLTBCS_POS               (24)
#define SPI_MR_DLTBCS_MASK              (0xff << SPI_MR_DLTBCS_POS)
#define SPI_MR_DLTBCS(value)            BITFIELD_SET(SPI_MR_DLTBCS, value)

/* Receive data register. */
#define SPI_RDR_RD_POS                  (0)
#define SPI_RDR_RD_MASK                 (0xffff << SPI_RDR_RD_POS)
#define SPI_RDR_RD(value)               BITFIELD_SET(SPI_RDR_RD, value)
#define SPI_RDR_PCS_POS                 (16)
#define SPI_RDR_PCS_MASK                (0xf << SPI_RDR_PCS_POS)
#define SPI_RDR_PCS(value)              BITFIELD_SET(SPI_RDR_PCS, value)

/* Transmit data register. */
#define SPI_TDR_TD_POS                  (0)
#define SPI_TDR_TD_MASK                 (0xffff << SPI_TDR_TD_POS)
#define SPI_TDR_TD(value)               BITFIELD_SET(SPI_TDR_TD, value)
#define SPI_TDR_PCS_POS                 (16)
#define SPI_TDR_PCS_MASK                (0xf << SPI_TDR_PCS_POS)
#define SPI_TDR_PCS(value)              BITFIELD_SET(SPI_TDR_PCS, value)
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
#define SPI_CSR_BITS(value)             BITFIELD_SET(SPI_CSR_BITS, value)
#define SPI_CSR_SCBR_POS                (8)
#define SPI_CSR_SCBR_MASK               (0xff << SPI_CSR_SCBR_POS)
#define SPI_CSR_SCBR(value)             BITFIELD_SET(SPI_CSR_SCBR, value)
#define SPI_CSR_DLYBS_POS               (16)
#define SPI_CSR_DLYBS_MASK              (0xff << SPI_CSR_DLYBS_POS)
#define SPI_CSR_DLYBS(value)            BITFIELD_SET(SPI_CSR_DLYBS, value)
#define SPI_CSR_DLYBCT_POS              (24)
#define SPI_CSR_DLYBCT_MASK             (0xff << SPI_CSR_DLYBCT_POS)
#define SPI_CSR_DLYBCT(value)           BITFIELD_SET(SPI_CSR_DLYBCT, value)

/* 33. Two-wire Interface (TWI) User Interface. */
struct sam_twi_t {
    uint32_t CR;
    uint32_t MMR;
    uint32_t SMR;
    uint32_t IADR;
    uint32_t CWGR;
    uint32_t RESERVED0[3];
    uint32_t SR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t RHR;
    uint32_t THR;
    uint32_t RESERVED1[50];
    struct sam_pdc_t PDC;
};

/* Control register. */
#define SAM_TWI_CR_START                BIT(0)
#define SAM_TWI_CR_STOP                 BIT(1)
#define SAM_TWI_CR_MSEN                 BIT(2)
#define SAM_TWI_CR_MSDIS                BIT(3)
#define SAM_TWI_CR_SVEN                 BIT(4)
#define SAM_TWI_CR_SVDIS                BIT(5)
#define SAM_TWI_CR_QUICK                BIT(6)
#define SAM_TWI_CR_SWRST                BIT(7)

/* Master mode register. */
#define SAM_TWI_MMR_IADRSZ_POS          (8)
#define SAM_TWI_MMR_IADRSZ_MASK         (0x3 << SAM_TWI_MMR_IADRSZ_POS)
#define SAM_TWI_MMR_IADRSZ(value)       BITFIELD_SET(SAM_TWI_MMR_IADRSZ, value)
#define SAM_TWI_MMR_IADRSZ_NONE         SAM_TWI_MMR_IADRSZ(0)
#define SAM_TWI_MMR_IADRSZ_1_BYTE       SAM_TWI_MMR_IADRSZ(1)
#define SAM_TWI_MMR_IADRSZ_2_BYTE       SAM_TWI_MMR_IADRSZ(2)
#define SAM_TWI_MMR_IADRSZ_3_BYTE       SAM_TWI_MMR_IADRSZ(3)
#define SAM_TWI_MMR_MREAD               BIT(12)
#define SAM_TWI_MMR_DADR_POS            (16)
#define SAM_TWI_MMR_DADR_MASK           (0x7f << SAM_TWI_MMR_DADR_POS)
#define SAM_TWI_MMR_DADR(value)         BITFIELD_SET(SAM_TWI_MMR_DADR, value)

/* Slave mode register. */
#define SAM_TWI_SMR_SADR_POS            (16)
#define SAM_TWI_SMR_SADR_MASK           (0x7f << SAM_TWI_MMR_SADR_POS)
#define SAM_TWI_SMR_SADR(value)         BITFIELD_SET(SAM_TWI_MMR_SADR, value)

/* Clock waveform generator register. */
#define SAM_TWI_CWGR_CLDIV_POS           (0)
#define SAM_TWI_CWGR_CLDIV_MASK          (0xff << SAM_TWI_CWGR_CLDIV_POS)
#define SAM_TWI_CWGR_CLDIV(value)        BITFIELD_SET(SAM_TWI_CWGR_CLDIV, value)
#define SAM_TWI_CWGR_CHDIV_POS           (8)
#define SAM_TWI_CWGR_CHDIV_MASK          (0xff << SAM_TWI_CWGR_CHDIV_POS)
#define SAM_TWI_CWGR_CHDIV(value)        BITFIELD_SET(SAM_TWI_CWGR_CHDIV, value)
#define SAM_TWI_CWGR_CKDIV_POS           (16)
#define SAM_TWI_CWGR_CKDIV_MASK          (0xff << SAM_TWI_CWGR_CKDIV_POS)
#define SAM_TWI_CWGR_CKDIV(value)        BITFIELD_SET(SAM_TWI_CWGR_CKDIV, value)

/* Status register. */
#define SAM_TWI_SR_TXCOMP                BIT(0)
#define SAM_TWI_SR_RXRDY                 BIT(1)
#define SAM_TWI_SR_TXRDY                 BIT(2)
#define SAM_TWI_SR_SVREAD                BIT(3)
#define SAM_TWI_SR_SVACC                 BIT(4)
#define SAM_TWI_SR_GACC                  BIT(5)
#define SAM_TWI_SR_OVRE                  BIT(6)
#define SAM_TWI_SR_NACK                  BIT(8)
#define SAM_TWI_SR_ARBLST                BIT(9)
#define SAM_TWI_SR_SCLWS                 BIT(10)
#define SAM_TWI_SR_EOSACC                BIT(11)
#define SAM_TWI_SR_ENDRX                 BIT(12)
#define SAM_TWI_SR_ENDTX                 BIT(13)
#define SAM_TWI_SR_RXBUFF                BIT(14)
#define SAM_TWI_SR_TXBUFE                BIT(15)

/* Interrupt enable register. */
#define SAM_TWI_IER_TXCOMP                BIT(0)
#define SAM_TWI_IER_RXRDY                 BIT(1)
#define SAM_TWI_IER_TXRDY                 BIT(2)
#define SAM_TWI_IER_SVACC                 BIT(4)
#define SAM_TWI_IER_GACC                  BIT(5)
#define SAM_TWI_IER_OVRE                  BIT(6)
#define SAM_TWI_IER_NACK                  BIT(8)
#define SAM_TWI_IER_ARBLST                BIT(9)
#define SAM_TWI_IER_SCLWS                 BIT(10)
#define SAM_TWI_IER_EOSACC                BIT(11)
#define SAM_TWI_IER_ENDRX                 BIT(12)
#define SAM_TWI_IER_ENDTX                 BIT(13)
#define SAM_TWI_IER_RXBUFF                BIT(14)
#define SAM_TWI_IER_TXBUFE                BIT(15)

/* Interrupt disable register. */
#define SAM_TWI_IDR_TXCOMP                BIT(0)
#define SAM_TWI_IDR_RXRDY                 BIT(1)
#define SAM_TWI_IDR_TXRDY                 BIT(2)
#define SAM_TWI_IDR_SVACC                 BIT(4)
#define SAM_TWI_IDR_GACC                  BIT(5)
#define SAM_TWI_IDR_OVRE                  BIT(6)
#define SAM_TWI_IDR_NACK                  BIT(8)
#define SAM_TWI_IDR_ARBLST                BIT(9)
#define SAM_TWI_IDR_SCLWS                 BIT(10)
#define SAM_TWI_IDR_EOSACC                BIT(11)
#define SAM_TWI_IDR_ENDRX                 BIT(12)
#define SAM_TWI_IDR_ENDTX                 BIT(13)
#define SAM_TWI_IDR_RXBUFF                BIT(14)
#define SAM_TWI_IDR_TXBUFE                BIT(15)

/* Interrupt mask register. */
#define SAM_TWI_IMR_TXCOMP                BIT(0)
#define SAM_TWI_IMR_RXRDY                 BIT(1)
#define SAM_TWI_IMR_TXRDY                 BIT(2)
#define SAM_TWI_IMR_SVACC                 BIT(4)
#define SAM_TWI_IMR_GACC                  BIT(5)
#define SAM_TWI_IMR_OVRE                  BIT(6)
#define SAM_TWI_IMR_NACK                  BIT(8)
#define SAM_TWI_IMR_ARBLST                BIT(9)
#define SAM_TWI_IMR_SCLWS                 BIT(10)
#define SAM_TWI_IMR_EOSACC                BIT(11)
#define SAM_TWI_IMR_ENDRX                 BIT(12)
#define SAM_TWI_IMR_ENDTX                 BIT(13)
#define SAM_TWI_IMR_RXBUFF                BIT(14)
#define SAM_TWI_IMR_TXBUFE                BIT(15)

/* 34. Universal Asynchronous Receiver Tranceiver. */
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
    uint32_t RESERVED1[55];
    struct sam_pdc_t PDC;
};

/* 35. Universal Synchrounous Asynchronous Receiver Tranceiver. */
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
    uint32_t RESERVED1[5];
    uint32_t FIDI;
    uint32_t NER;
    uint32_t RESERVED2[1];
    uint32_t IF;
    uint32_t MAN;
    uint32_t LINMR;
    uint32_t LINIR;
    uint32_t RESERVED3[34];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t RESERVED4[5];
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
#define US_MR_USART_MODE(value)         BITFIELD_SET(US_MR_USART_MODE, value)
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
#define US_MR_USCLKS(value)             BITFIELD_SET(US_MR_USCLKS, value)
#define US_MR_USCLKS_MCK                (0x0 << 4)
#define US_MR_USCLKS_DIV                BIT(4)
#define US_MR_USCLKS_SCK                (0x3 << 4)
#define US_MR_CHRL_POS                  (6)
#define US_MR_CHRL_MASK                 (0x3 << US_MR_CHRL_POS)
#define US_MR_CHRL(value)               BITFIELD_SET(US_MR_CHRL, value)
#define US_MR_CHRL_5_BIT                (0x0 << 6)
#define US_MR_CHRL_6_BIT                BIT(6)
#define US_MR_CHRL_7_BIT                (0x2 << 6)
#define US_MR_CHRL_8_BIT                (0x3 << 6)
#define US_MR_SYNC_CPHA                 BIT(8)
#define US_MR_PAR_POS                   (9)
#define US_MR_PAR_MASK                  (0x7 << US_MR_PAR_POS)
#define US_MR_PAR(value)                BITFIELD_SET(US_MR_PAR, value)
#define US_MR_PAR_EVEN                  (0x0 << 9)
#define US_MR_PAR_ODD                   BIT(9)
#define US_MR_PAR_SPACE                 (0x2 << 9)
#define US_MR_PAR_MARK                  (0x3 << 9)
#define US_MR_PAR_NO                    (0x4 << 9)
#define US_MR_PAR_MULTIDROP             (0x6 << 9)
#define US_MR_NBSTOP_POS                (12)
#define US_MR_NBSTOP_MASK               (0x3 << US_MR_NBSTOP_POS)
#define US_MR_NBSTOP(value)             BITFIELD_SET(US_MR_NBSTOP, value)
#define US_MR_NBSTOP_1_BIT              (0x0 << 12)
#define US_MR_NBSTOP_1_5_BIT            BIT(12)
#define US_MR_NBSTOP_2_BIT              (0x2 << 12)
#define US_MR_CHMODE_POS                (14)
#define US_MR_CHMODE_MASK               (0x3 << US_MR_CHMODE_POS)
#define US_MR_CHMODE(value)             BITFIELD_SET(US_MR_CHMODE, value)
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
#define US_MR_MAX_ITERATION(value)      BITFIELD_SET(US_MR_MAX_ITERATION, value)
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
#define US_RHR_RXCHR(value)             BITFIELD_SET(US_RHR_RXCHR, value)
#define US_RHR_RXSYNH                   BIT(15)

/* USART Transmit Holding Register */
#define US_THR_TXCHR_POS                (0)
#define US_THR_TXCHR_MASK               (0x1ff << US_THR_TXCHR_POS)
#define US_THR_TXCHR(value)             BITFIELD_SET(US_THR_TXCHR, value)
#define US_THR_TXSYNH                   BIT(15)

/* USART Baud Rate Generator Register */
#define US_BRGR_CD_POS                  (0)
#define US_BRGR_CD_MASK                 (0xffff << US_BRGR_CD_POS)
#define US_BRGR_CD(value)               BITFIELD_SET(US_BRGR_CD, value)
#define US_BRGR_FP_POS                  (16)
#define US_BRGR_FP_MASK                 (0x7 << US_BRGR_FP_POS)
#define US_BRGR_FP(value)               BITFIELD_SET(US_BRGR_FP, value)

/* USART Receiver Time-out Register */
#define US_RTOR_TO_POS                  (0)
#define US_RTOR_TO_MASK                 (0x1ffff << US_RTOR_TO_POS)
#define US_RTOR_TO(value)               BITFIELD_SET(US_RTOR_TO, value)

/* USART Transmitter Timeguard Register */
#define US_TTGR_TG_POS                  (0)
#define US_TTGR_TG_MASK                 (0xff << US_TTGR_TG_POS)
#define US_TTGR_TG(value)               BITFIELD_SET(US_TTGR_TG, value)

/* USART FI DI RATIO Register */
#define US_FIDI_FI_DI_RATIO_POS         (0)
#define US_FIDI_FI_DI_RATIO_MASK        (0x7ff << US_FIDI_FI_DI_RATIO_POS)
#define US_FIDI_FI_DI_RATIO(value)      BITFIELD_SET(US_FIDI_FI_DI_RATIO, value)

/* USART Number of Errors Register */
#define US_NER_NB_ERRORS_POS            (0)
#define US_NER_NB_ERRORS_MASK           (0xff << US_NER_NB_ERRORS_POS)
#define US_NER_NB_ERRORS(value)         BITFIELD_SET(US_NER_NB_ERRORS, value)

/* USART IrDA FILTER Register */
#define US_IF_IRDA_FILTER_POS           (0)
#define US_IF_IRDA_FILTER_MASK          (0xff << US_IF_IRDA_FILTER_POS)
#define US_IF_IRDA_FILTER(value)        BITFIELD_SET(US_IF_IRDA_FILTER, value)

/* USART Manchester Configuration Register */
#define US_MAN_TX_PL_POS                (0)
#define US_MAN_TX_PL_MASK               (0xf << US_MAN_TX_PL_POS)
#define US_MAN_TX_PL(value)             BITFIELD_SET(US_MAN_TX_PL, value)
#define US_MAN_TX_PP_POS                (8)
#define US_MAN_TX_PP_MASK               (0x3 << US_MAN_TX_PP_POS)
#define US_MAN_TX_PP(value)             BITFIELD_SET(US_MAN_TX_PP, value)
#define US_MAN_TX_PP_ALL_ONE            (0x0 << 8)
#define US_MAN_TX_PP_ALL_ZERO           BIT(8)
#define US_MAN_TX_PP_ZERO_ONE           (0x2 << 8)
#define US_MAN_TX_PP_ONE_ZERO           (0x3 << 8)
#define US_MAN_TX_MPOL                  BIT(12)
#define US_MAN_RX_PL_POS                (16)
#define US_MAN_RX_PL_MASK               (0xf << US_MAN_RX_PL_POS)
#define US_MAN_RX_PL(value)             BITFIELD_SET(US_MAN_RX_PL, value)
#define US_MAN_RX_PP_POS                (24)
#define US_MAN_RX_PP_MASK               (0x3 << US_MAN_RX_PP_POS)
#define US_MAN_RX_PP(value)             BITFIELD_SET(US_MAN_RX_PP, value)
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
#define US_LINMR_NACT(value)            BITFIELD_SET(US_LINMR_NACT, value)
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
#define US_LINMR_DLC(value)             BITFIELD_SET(US_LINMR_DLC, value)
#define US_LINMR_PDCM                   BIT(16)

/* USART LIN Identifier Register */
#define US_LINIR_IDCHR_POS              (0)
#define US_LINIR_IDCHR_MASK             (0xff << US_LINIR_IDCHR_POS)
#define US_LINIR_IDCHR(value)           BITFIELD_SET(US_LINIR_IDCHR, value)

/* USART Write Protect Mode Register */
#define US_WPMR_WPEN                    BIT(0)
#define US_WPMR_WPKEY_POS               (8)
#define US_WPMR_WPKEY_MASK              (0xffffff << US_WPMR_WPKEY_POS)
#define US_WPMR_WPKEY(value)            BITFIELD_SET(US_WPMR_WPKEY, value)

/* USART Write Protect Status Register */
#define US_WPSR_WPVS                    BIT(0)
#define US_WPSR_WPVSRC_POS              (8)
#define US_WPSR_WPVSRC_MASK             (0xffff << US_WPSR_WPVSRC_POS)
#define US_WPSR_WPVSRC(value)           BITFIELD_SET(US_WPSR_WPVSRC, value)

/* 36. Timer Counter. */
struct sam_tc_t {
    struct {
        uint32_t CCR;
        uint32_t CMR;
        uint32_t SMMR;
        uint32_t RESERVED0;
        uint32_t CV;
        uint32_t RA;
        uint32_t RB;
        uint32_t RC;
        uint32_t SR;
        uint32_t IER;
        uint32_t IDR;
        uint32_t IMR;
        uint32_t RESERVED1[4];
    } CHANNEL[3];
    uint32_t BCR;
    uint32_t BMR;
    uint32_t QIER;
    uint32_t QIDR;
    uint32_t QIMR;
    uint32_t QISR;
    uint32_t FMR;
    uint32_t WPMR;
};

/* Channel Control Register. */
#define TC_CCR_CLKEN                    BIT(0)
#define TC_CCR_CLKDIS                   BIT(1)
#define TC_CCR_SWTRG                    BIT(2)

/* Channel Mode Register, Capture. */
#define TC_CMR_CAPTURE_TCCLKS_POS       (0)
#define TC_CMR_CAPTURE_TCCLKS_MASK      (0x7 << TC_CMR_CAPTURE_TCCLKS_POS)
#define TC_CMR_CAPTURE_TCCLKS(value)    BITFIELD_SET(TC_CMR_CAPTURE_TCCLKS, (value))
#define TC_CMR_CAPTURE_CLKI             BIT(3)
#define TC_CMR_CAPTURE_BURST_POS        (4)
#define TC_CMR_CAPTURE_BURST_MASK       (0x3 << TC_CMR_CAPTURE_BURST_POS)
#define TC_CMR_CAPTURE_BURST(value)     BITFIELD_SET(TC_CMR_CAPTURE_BURST, (value))
#define TC_CMR_CAPTURE_LDBSTOP          BIT(6)
#define TC_CMR_CAPTURE_LDBDIS           BIT(7)
#define TC_CMR_CAPTURE_ETRGEDG_POS      (8)
#define TC_CMR_CAPTURE_ETRGEDG_MASK     (0x3 << TC_CMR_CAPTURE_ETRGEDG_POS)
#define TC_CMR_CAPTURE_ETRGEDG(value)   BITFIELD_SET(TC_CMR_CAPTURE_ETRGEDG, (value))
#define TC_CMR_CAPTURE_ABETRG           BIT(10)
#define TC_CMR_CAPTURE_CPCTRG           BIT(14)
#define TC_CMR_CAPTURE_WAVE             BIT(15)
#define TC_CMR_CAPTURE_LDRA_POS         (16)
#define TC_CMR_CAPTURE_LDRA_MASK        (0x3 << TC_CMR_CAPTURE_LDRA_POS)
#define TC_CMR_CAPTURE_LDRA(value)      BITFIELD_SET(TC_CMR_CAPTURE_LDRA, (value))
#define TC_CMR_CAPTURE_LDRB_POS         (18)
#define TC_CMR_CAPTURE_LDRB_MASK        (0x3 << TC_CMR_CAPTURE_LDRB_POS)
#define TC_CMR_CAPTURE_LDRB(value)      BITFIELD_SET(TC_CMR_CAPTURE_LDRB, (value))

/* Channel Mode Register, Waveform. */
#define TC_CMR_WAVEFORM_TCCLKS_POS      (0)
#define TC_CMR_WAVEFORM_TCCLKS_MASK     (0x7 << TC_CMR_WAVEFORM_TCCLKS_POS)
#define TC_CMR_WAVEFORM_TCCLKS(value)   BITFIELD_SET(TC_CMR_WAVEFORM_TCCLKS, (value))
#define TC_CMR_WAVEFORM_CLKI            BIT(3)
#define TC_CMR_WAVEFORM_BURST_POS       (4)
#define TC_CMR_WAVEFORM_BURST_MASK      (0x3 << TC_CMR_WAVEFORM_BURST_POS)
#define TC_CMR_WAVEFORM_BURST(value)    BITFIELD_SET(TC_CMR_WAVEFORM_BURST, (value))
#define TC_CMR_WAVEFORM_CPCSTOP         BIT(6)
#define TC_CMR_WAVEFORM_CPCDIS          BIT(7)
#define TC_CMR_WAVEFORM_EEVTEDG_POS     (8)
#define TC_CMR_WAVEFORM_EEVTEDG_MASK    (0x3 << TC_CMR_WAVEFORM_EEVTEDG_POS)
#define TC_CMR_WAVEFORM_EEVTEDG(value)  BITFIELD_SET(TC_CMR_WAVEFORM_EEVTEDG, (value))
#define TC_CMR_WAVEFORM_EEVT_POS        (10)
#define TC_CMR_WAVEFORM_EEVT_MASK       (0x3 << TC_CMR_WAVEFORM_EEVT_POS)
#define TC_CMR_WAVEFORM_EEVT(value)     BITFIELD_SET(TC_CMR_WAVEFORM_EEVT, (value))
#define TC_CMR_WAVEFORM_ENETRG          BIT(12)
#define TC_CMR_WAVEFORM_WAVSEL_POS      (13)
#define TC_CMR_WAVEFORM_WAVSEL_MASK     (0x3 << TC_CMR_WAVEFORM_WAVSEL_POS)
#define TC_CMR_WAVEFORM_WAVSEL(value)   BITFIELD_SET(TC_CMR_WAVEFORM_WAVSEL, (value))
#define TC_CMR_WAVEFORM_WAVSEL_UP       TC_CMR_WAVEFORM_WAVSEL(0)
#define TC_CMR_WAVEFORM_WAVSEL_UPDOWN   TC_CMR_WAVEFORM_WAVSEL(1)
#define TC_CMR_WAVEFORM_WAVSEL_UP_RC    TC_CMR_WAVEFORM_WAVSEL(2)
#define TC_CMR_WAVEFORM_WAVSEL_UPDOWN_RC TC_CMR_WAVEFORM_WAVSEL(3)
#define TC_CMR_WAVEFORM_WAVE            BIT(15)
#define TC_CMR_WAVEFORM_ACPA_POS        (16)
#define TC_CMR_WAVEFORM_ACPA_MASK       (0x3 << TC_CMR_WAVEFORM_ACPA_POS)
#define TC_CMR_WAVEFORM_ACPA(value)     BITFIELD_SET(TC_CMR_WAVEFORM_ACPA, (value))
#define TC_CMR_WAVEFORM_ACPC_POS        (18)
#define TC_CMR_WAVEFORM_ACPC_MASK       (0x3 << TC_CMR_WAVEFORM_ACPC_POS)
#define TC_CMR_WAVEFORM_ACPC(value)     BITFIELD_SET(TC_CMR_WAVEFORM_ACPC, (value))
#define TC_CMR_WAVEFORM_AEEVT_POS       (20)
#define TC_CMR_WAVEFORM_AEEVT_MASK      (0x3 << TC_CMR_WAVEFORM_AEEVT_POS)
#define TC_CMR_WAVEFORM_AEEVT(value)    BITFIELD_SET(TC_CMR_WAVEFORM_AEEVT, (value))
#define TC_CMR_WAVEFORM_ASWTRG_POS      (22)
#define TC_CMR_WAVEFORM_ASWTRG_MASK     (0x3 << TC_CMR_WAVEFORM_ASWTRG_POS)
#define TC_CMR_WAVEFORM_ASWTRG(value)   BITFIELD_SET(TC_CMR_WAVEFORM_ASWTRG, (value))
#define TC_CMR_WAVEFORM_BCPB_POS        (24)
#define TC_CMR_WAVEFORM_BCPB_MASK       (0x3 << TC_CMR_WAVEFORM_BCPB_POS)
#define TC_CMR_WAVEFORM_BCPB(value)     BITFIELD_SET(TC_CMR_WAVEFORM_BCPB, (value))
#define TC_CMR_WAVEFORM_BCPC_POS        (26)
#define TC_CMR_WAVEFORM_BCPC_MASK       (0x3 << TC_CMR_WAVEFORM_BCPC_POS)
#define TC_CMR_WAVEFORM_BCPC(value)     BITFIELD_SET(TC_CMR_WAVEFORM_BCPC, (value))
#define TC_CMR_WAVEFORM_BEEVT_POS       (28)
#define TC_CMR_WAVEFORM_BEEVT_MASK      (0x3 << TC_CMR_WAVEFORM_BEEVT_POS)
#define TC_CMR_WAVEFORM_BEEVT(value)    BITFIELD_SET(TC_CMR_WAVEFORM_BEEVT, (value))
#define TC_CMR_WAVEFORM_BSWTRG_POS      (30)
#define TC_CMR_WAVEFORM_BSWTRG_MASK     (0x3 << TC_CMR_WAVEFORM_BSWTRG_POS)
#define TC_CMR_WAVEFORM_BSWTRG(value)   BITFIELD_SET(TC_CMR_WAVEFORM_BSWTRG, (value))

/* Stepper Motor Mode Register. */
#define TC_SMMR_GCEN                    BIT(0)
#define TC_SMMR_DOWN                    BIT(1)

/* Status Register. */
#define TC_SR_COVFS                     BIT(0)
#define TC_SR_LOVRS                     BIT(1)
#define TC_SR_CPAS                      BIT(2)
#define TC_SR_CPBS                      BIT(3)
#define TC_SR_CPCS                      BIT(4)
#define TC_SR_LDRAS                     BIT(5)
#define TC_SR_LDRBS                     BIT(6)
#define TC_SR_ETRGS                     BIT(7)
#define TC_SR_CLKSTA                    BIT(16)
#define TC_SR_MTIOA                     BIT(17)
#define TC_SR_MTIOB                     BIT(18)

/* Interrupt Enable Register. */
#define TC_IER_COVFS                    BIT(0)
#define TC_IER_LOVRS                    BIT(1)
#define TC_IER_CPAS                     BIT(2)
#define TC_IER_CPBS                     BIT(3)
#define TC_IER_CPCS                     BIT(4)
#define TC_IER_LDRAS                    BIT(5)
#define TC_IER_LDRBS                    BIT(6)
#define TC_IER_ETRGS                    BIT(7)

/* Interrupt Disable Register. */
#define TC_IDR_COVFS                    BIT(0)
#define TC_IDR_LOVRS                    BIT(1)
#define TC_IDR_CPAS                     BIT(2)
#define TC_IDR_CPBS                     BIT(3)
#define TC_IDR_CPCS                     BIT(4)
#define TC_IDR_LDRAS                    BIT(5)
#define TC_IDR_LDRBS                    BIT(6)
#define TC_IDR_ETRGS                    BIT(7)

/* Interrupt Mask Register. */
#define TC_IMR_COVFS                    BIT(0)
#define TC_IMR_LOVRS                    BIT(1)
#define TC_IMR_CPAS                     BIT(2)
#define TC_IMR_CPBS                     BIT(3)
#define TC_IMR_CPCS                     BIT(4)
#define TC_IMR_LDRAS                    BIT(5)
#define TC_IMR_LDRBS                    BIT(6)
#define TC_IMR_ETRGS                    BIT(7)

/* 37. High Speed MultiMedia Card Interface (HSMCI). */
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
    uint32_t RESERVED1[2];
    uint32_t SR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t DMA;
    uint32_t CFG;
    uint32_t RESERVED2[35];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t RESERVED3[69];
    uint32_t FIFO[256];
};

/* 39. Universal Synchrounous Asynchronous Receiver Tranceiver. */
struct sam_uotghs_t {

    /* USB Device registers. */
    struct {
        uint32_t CTRL;
        uint32_t ISR;
        uint32_t ICR;
        uint32_t IFR;
        uint32_t IMR;
        uint32_t IDR;
        uint32_t IER;
        uint32_t EPT;
        uint32_t FNUM;
        uint32_t RESERVED0[55];
        uint32_t EPTCFG[10];
        uint32_t RESERVED1[2];
        uint32_t EPTISR[10];
        uint32_t RESERVED2[2];
        uint32_t EPTICR[10];
        uint32_t RESERVED3[2];
        uint32_t EPTIFR[10];
        uint32_t RESERVED4[2];
        uint32_t EPTIMR[10];
        uint32_t RESERVED5[2];
        uint32_t EPTIER[10];
        uint32_t RESERVED6[2];
        uint32_t EPTIDR[10];
        uint32_t RESERVED7[50];
        struct {
            uint32_t NXTDSC;
            uint32_t ADDRESS;
            uint32_t CONTROL;
            uint32_t STATUS;
        } DMA[7];
    } DEVICE;
    uint32_t RESERVED0[32];

    /* USB Host registers. */
    struct {
        uint32_t CTRL;
        uint32_t ISR;
        uint32_t ICR;
        uint32_t IFR;
        uint32_t IMR;
        uint32_t IDR;
        uint32_t IER;
        uint32_t PIP;
        uint32_t FNUM;
        uint32_t ADDR[3];
        uint32_t RESERVED0[52];
        uint32_t PIPCFG[10];
        uint32_t reverved1[2];
        uint32_t PIPISR[10];
        uint32_t reverved2[2];
        uint32_t PIPICR[10];
        uint32_t reverved3[2];
        uint32_t PIPIFR[10];
        uint32_t reverved4[2];
        uint32_t PIPIMR[10];
        uint32_t reverved5[2];
        uint32_t PIPIER[10];
        uint32_t reverved6[2];
        uint32_t PIPIDR[10];
        uint32_t reverved7[2];
        uint32_t PIPINRQ[10];
        uint32_t reverved8[2];
        uint32_t PIPERR[10];
        uint32_t RESERVED10[26];
        struct {
            uint32_t NXTDSC;
            uint32_t ADDRESS;
            uint32_t CONTROL;
            uint32_t STATUS;
        } DMA[7];
    } HOST;
    uint32_t RESERVED1[32];

    /* General USB registers. */
    uint32_t CTRL;
    uint32_t SR;
    uint32_t SCR;
    uint32_t SFR;
    uint32_t RESERVED2[7];
    uint32_t FSM;
};

/* Device */

/* Host. */

/* Host General Control Register. */
#define SAM_UOTGHS_HOST_CTRL_SOFE               BIT(8)
#define SAM_UOTGHS_HOST_CTRL_RESET              BIT(9)
#define SAM_UOTGHS_HOST_CTRL_RESUME             BIT(10)
#define SAM_UOTGHS_HOST_CTRL_SPDCONF_POS        (12)
#define SAM_UOTGHS_HOST_CTRL_SPDCONF_MASK       (0x3 << SAM_UOTGHS_HOST_CTRL_SPDCONF_POS)
#define SAM_UOTGHS_HOST_CTRL_SPDCONF(value)     BITFIELD_SET(SAM_UOTGHS_HOST_CTRL_SPDCONF, (value))
#define SAM_UOTGHS_HOST_CTRL_SPDCONF_NORMAL     SAM_UOTGHS_HOST_CTRL_SPDCONF(0)
#define SAM_UOTGHS_HOST_CTRL_SPDCONF_LOW_POWER  SAM_UOTGHS_HOST_CTRL_SPDCONF(1)
#define SAM_UOTGHS_HOST_CTRL_SPDCONF_HIGH_SPEED SAM_UOTGHS_HOST_CTRL_SPDCONF(2)
#define SAM_UOTGHS_HOST_CTRL_SPDCONF_FORCES_FS  SAM_UOTGHS_HOST_CTRL_SPDCONF(3)

/* Host Global Interrupt Status Register. */
#define SAM_UOTGHS_HOST_ISR_DCONNI      BIT(0)
#define SAM_UOTGHS_HOST_ISR_DDISCI      BIT(1)
#define SAM_UOTGHS_HOST_ISR_RSTI        BIT(2)
#define SAM_UOTGHS_HOST_ISR_RSMEDI      BIT(3)
#define SAM_UOTGHS_HOST_ISR_RXRSMI      BIT(4)
#define SAM_UOTGHS_HOST_ISR_HSOFI       BIT(5)
#define SAM_UOTGHS_HOST_ISR_HWUPI       BIT(6)
#define SAM_UOTGHS_HOST_ISR_PEP_0       BIT(8)
#define SAM_UOTGHS_HOST_ISR_PEP_1       BIT(9)
#define SAM_UOTGHS_HOST_ISR_PEP_2       BIT(10)
#define SAM_UOTGHS_HOST_ISR_PEP_3       BIT(11)
#define SAM_UOTGHS_HOST_ISR_PEP_4       BIT(12)
#define SAM_UOTGHS_HOST_ISR_PEP_5       BIT(13)
#define SAM_UOTGHS_HOST_ISR_PEP_6       BIT(14)
#define SAM_UOTGHS_HOST_ISR_PEP_7       BIT(15)
#define SAM_UOTGHS_HOST_ISR_PEP_8       BIT(16)
#define SAM_UOTGHS_HOST_ISR_PEP_9       BIT(17)
#define SAM_UOTGHS_HOST_ISR_DMA_1       BIT(25)
#define SAM_UOTGHS_HOST_ISR_DMA_2       BIT(26)
#define SAM_UOTGHS_HOST_ISR_DMA_3       BIT(27)
#define SAM_UOTGHS_HOST_ISR_DMA_4       BIT(28)
#define SAM_UOTGHS_HOST_ISR_DMA_5       BIT(29)
#define SAM_UOTGHS_HOST_ISR_DMA_6       BIT(30)

/* Host Global Interrupt Clear Register. */
#define SAM_UOTGHS_HOST_ICR_DCONNIC     BIT(0)
#define SAM_UOTGHS_HOST_ICR_DDISCIC     BIT(1)
#define SAM_UOTGHS_HOST_ICR_RSTIC       BIT(2)
#define SAM_UOTGHS_HOST_ICR_RSMEDIC     BIT(3)
#define SAM_UOTGHS_HOST_ICR_RXRSMIC     BIT(4)
#define SAM_UOTGHS_HOST_ICR_HSOFIC      BIT(5)
#define SAM_UOTGHS_HOST_ICR_HWUPIC      BIT(6)

/* Host Global Interrupt Set Register. */
#define SAM_UOTGHS_HOST_IFR_DCONNIS     BIT(0)
#define SAM_UOTGHS_HOST_IFR_DDISCIS     BIT(1)
#define SAM_UOTGHS_HOST_IFR_RSTIS       BIT(2)
#define SAM_UOTGHS_HOST_IFR_RSMEDIS     BIT(3)
#define SAM_UOTGHS_HOST_IFR_RXRSMIS     BIT(4)
#define SAM_UOTGHS_HOST_IFR_HSOFIS      BIT(5)
#define SAM_UOTGHS_HOST_IFR_HWUPIS      BIT(6)
#define SAM_UOTGHS_HOST_IFR_DMA_1       BIT(25)
#define SAM_UOTGHS_HOST_IFR_DMA_2       BIT(26)
#define SAM_UOTGHS_HOST_IFR_DMA_3       BIT(27)
#define SAM_UOTGHS_HOST_IFR_DMA_4       BIT(28)
#define SAM_UOTGHS_HOST_IFR_DMA_5       BIT(29)
#define SAM_UOTGHS_HOST_IFR_DMA_6       BIT(30)

/* Host Global Interrupt Mask Register. */
#define SAM_UOTGHS_HOST_IMR_DCONNIE     BIT(0)
#define SAM_UOTGHS_HOST_IMR_DDISCIE     BIT(1)
#define SAM_UOTGHS_HOST_IMR_RSTIE       BIT(2)
#define SAM_UOTGHS_HOST_IMR_RSMEDIE     BIT(3)
#define SAM_UOTGHS_HOST_IMR_RXRSMIE     BIT(4)
#define SAM_UOTGHS_HOST_IMR_HSOFIE      BIT(5)
#define SAM_UOTGHS_HOST_IMR_HWUPIE      BIT(6)
#define SAM_UOTGHS_HOST_IMR_PEP_0       BIT(8)
#define SAM_UOTGHS_HOST_IMR_PEP_1       BIT(9)
#define SAM_UOTGHS_HOST_IMR_PEP_2       BIT(10)
#define SAM_UOTGHS_HOST_IMR_PEP_3       BIT(11)
#define SAM_UOTGHS_HOST_IMR_PEP_4       BIT(12)
#define SAM_UOTGHS_HOST_IMR_PEP_5       BIT(13)
#define SAM_UOTGHS_HOST_IMR_PEP_6       BIT(14)
#define SAM_UOTGHS_HOST_IMR_PEP_7       BIT(15)
#define SAM_UOTGHS_HOST_IMR_PEP_8       BIT(16)
#define SAM_UOTGHS_HOST_IMR_PEP_9       BIT(17)
#define SAM_UOTGHS_HOST_IMR_DMA_1       BIT(25)
#define SAM_UOTGHS_HOST_IMR_DMA_2       BIT(26)
#define SAM_UOTGHS_HOST_IMR_DMA_3       BIT(27)
#define SAM_UOTGHS_HOST_IMR_DMA_4       BIT(28)
#define SAM_UOTGHS_HOST_IMR_DMA_5       BIT(29)
#define SAM_UOTGHS_HOST_IMR_DMA_6       BIT(30)

/* Host Global Interrupt Disable Register. */
#define SAM_UOTGHS_HOST_IDR_DCONNIED    BIT(0)
#define SAM_UOTGHS_HOST_IDR_DDISCIED    BIT(1)
#define SAM_UOTGHS_HOST_IDR_RSTIED      BIT(2)
#define SAM_UOTGHS_HOST_IDR_RSMEDIED    BIT(3)
#define SAM_UOTGHS_HOST_IDR_RXRSMIED    BIT(4)
#define SAM_UOTGHS_HOST_IDR_HSOFIED     BIT(5)
#define SAM_UOTGHS_HOST_IDR_HWUPIED     BIT(6)
#define SAM_UOTGHS_HOST_IDR_PEP_0       BIT(8)
#define SAM_UOTGHS_HOST_IDR_PEP_1       BIT(9)
#define SAM_UOTGHS_HOST_IDR_PEP_2       BIT(10)
#define SAM_UOTGHS_HOST_IDR_PEP_3       BIT(11)
#define SAM_UOTGHS_HOST_IDR_PEP_4       BIT(12)
#define SAM_UOTGHS_HOST_IDR_PEP_5       BIT(13)
#define SAM_UOTGHS_HOST_IDR_PEP_6       BIT(14)
#define SAM_UOTGHS_HOST_IDR_PEP_7       BIT(15)
#define SAM_UOTGHS_HOST_IDR_PEP_8       BIT(16)
#define SAM_UOTGHS_HOST_IDR_PEP_9       BIT(17)
#define SAM_UOTGHS_HOST_IDR_DMA_1       BIT(25)
#define SAM_UOTGHS_HOST_IDR_DMA_2       BIT(26)
#define SAM_UOTGHS_HOST_IDR_DMA_3       BIT(27)
#define SAM_UOTGHS_HOST_IDR_DMA_4       BIT(28)
#define SAM_UOTGHS_HOST_IDR_DMA_5       BIT(29)
#define SAM_UOTGHS_HOST_IDR_DMA_6       BIT(30)

/* Host Global Interrupt Enable Register. */
#define SAM_UOTGHS_HOST_IER_DCONNIES    BIT(0)
#define SAM_UOTGHS_HOST_IER_DDISCIES    BIT(1)
#define SAM_UOTGHS_HOST_IER_RSTIES      BIT(2)
#define SAM_UOTGHS_HOST_IER_RSMEDIES    BIT(3)
#define SAM_UOTGHS_HOST_IER_RXRSMIES    BIT(4)
#define SAM_UOTGHS_HOST_IER_HSOFIES     BIT(5)
#define SAM_UOTGHS_HOST_IER_HWUPIES     BIT(6)
#define SAM_UOTGHS_HOST_IER_PEP_0       BIT(8)
#define SAM_UOTGHS_HOST_IER_PEP_1       BIT(9)
#define SAM_UOTGHS_HOST_IER_PEP_2       BIT(10)
#define SAM_UOTGHS_HOST_IER_PEP_3       BIT(11)
#define SAM_UOTGHS_HOST_IER_PEP_4       BIT(12)
#define SAM_UOTGHS_HOST_IER_PEP_5       BIT(13)
#define SAM_UOTGHS_HOST_IER_PEP_6       BIT(14)
#define SAM_UOTGHS_HOST_IER_PEP_7       BIT(15)
#define SAM_UOTGHS_HOST_IER_PEP_8       BIT(16)
#define SAM_UOTGHS_HOST_IER_PEP_9       BIT(17)
#define SAM_UOTGHS_HOST_IER_DMA_1       BIT(25)
#define SAM_UOTGHS_HOST_IER_DMA_2       BIT(26)
#define SAM_UOTGHS_HOST_IER_DMA_3       BIT(27)
#define SAM_UOTGHS_HOST_IER_DMA_4       BIT(28)
#define SAM_UOTGHS_HOST_IER_DMA_5       BIT(29)
#define SAM_UOTGHS_HOST_IER_DMA_6       BIT(30)

/* Host Frame Number Register. */
#define SAM_UOTGHS_HOST_FNUM_MFNUM_POS       (0)
#define SAM_UOTGHS_HOST_FNUM_MFNUM_MASK      (0x7 << SAM_UOTGHS_HOST_FNUM_MFNUM_POS)
#define SAM_UOTGHS_HOST_FNUM_MFNUM(value)    BITFIELD_SET(SAM_UOTGHS_HOST_FNUM_MFNUM, (value))
#define SAM_UOTGHS_HOST_FNUM_FNUM_POS        (3)
#define SAM_UOTGHS_HOST_FNUM_FNUM_MASK       (0x7ff << SAM_UOTGHS_HOST_FNUM_FNUM_POS)
#define SAM_UOTGHS_HOST_FNUM_FNUM(value)     BITFIELD_SET(SAM_UOTGHS_HOST_FNUM_FNUM, (value))
#define SAM_UOTGHS_HOST_FNUM_FLENHIGH_POS    (16)
#define SAM_UOTGHS_HOST_FNUM_FLENHIGH_MASK   (0xff << SAM_UOTGHS_HOST_FNUM_FLENHIGH_POS)
#define SAM_UOTGHS_HOST_FNUM_FLENHIGH(value) BITFIELD_SET(SAM_UOTGHS_HOST_FNUM_FLENHIGH, (value))

/* Host Address 1 Register. */
#define SAM_UOTGHS_HOST_ADDR1_ADDRP0_POS     (0)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP0_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR1_ADDRP0_POS)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP0(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR1_ADDRP0, (value))
#define SAM_UOTGHS_HOST_ADDR1_ADDRP1_POS     (8)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP1_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR1_ADDRP1_POS)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP1(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR1_ADDRP1, (value))
#define SAM_UOTGHS_HOST_ADDR1_ADDRP2_POS     (16)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP2_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR1_ADDRP2_POS)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP2(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR1_ADDRP2, (value))
#define SAM_UOTGHS_HOST_ADDR1_ADDRP3_POS     (24)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP3_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR1_ADDRP3_POS)
#define SAM_UOTGHS_HOST_ADDR1_ADDRP3(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR1_ADDRP3, (value))

/* Host Address 2 Register. */
#define SAM_UOTGHS_HOST_ADDR2_ADDRP4_POS     (0)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP4_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR2_ADDRP4_POS)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP4(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR2_ADDRP4, (value))
#define SAM_UOTGHS_HOST_ADDR2_ADDRP5_POS     (8)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP5_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR2_ADDRP5_POS)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP5(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR2_ADDRP5, (value))
#define SAM_UOTGHS_HOST_ADDR2_ADDRP6_POS     (16)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP6_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR2_ADDRP6_POS)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP6(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR2_ADDRP6, (value))
#define SAM_UOTGHS_HOST_ADDR2_ADDRP7_POS     (24)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP7_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR2_ADDRP7_POS)
#define SAM_UOTGHS_HOST_ADDR2_ADDRP7(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR2_ADDRP7, (value))

/* Host Address 3 Register. */
#define SAM_UOTGHS_HOST_ADDR3_ADDRP8_POS     (0)
#define SAM_UOTGHS_HOST_ADDR3_ADDRP8_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR3_ADDRP8_POS)
#define SAM_UOTGHS_HOST_ADDR3_ADDRP8(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR3_ADDRP8, (value))
#define SAM_UOTGHS_HOST_ADDR3_ADDRP9_POS     (8)
#define SAM_UOTGHS_HOST_ADDR3_ADDRP9_MASK    (0x7f << SAM_UOTGHS_HOST_ADDR3_ADDRP9_POS)
#define SAM_UOTGHS_HOST_ADDR3_ADDRP9(value)  BITFIELD_SET(SAM_UOTGHS_HOST_ADDR3_ADDRP9, (value))

/* Host Pipe Register. */
#define SAM_UOTGHS_HOST_PIP_PEN0        BIT(0)
#define SAM_UOTGHS_HOST_PIP_PEN1        BIT(1)
#define SAM_UOTGHS_HOST_PIP_PEN2        BIT(2)
#define SAM_UOTGHS_HOST_PIP_PEN3        BIT(3)
#define SAM_UOTGHS_HOST_PIP_PEN4        BIT(4)
#define SAM_UOTGHS_HOST_PIP_PEN5        BIT(5)
#define SAM_UOTGHS_HOST_PIP_PEN6        BIT(6)
#define SAM_UOTGHS_HOST_PIP_PEN7        BIT(7)
#define SAM_UOTGHS_HOST_PIP_PEN8        BIT(8)
#define SAM_UOTGHS_HOST_PIP_PRST0       BIT(16)
#define SAM_UOTGHS_HOST_PIP_PRST1       BIT(17)
#define SAM_UOTGHS_HOST_PIP_PRST2       BIT(18)
#define SAM_UOTGHS_HOST_PIP_PRST3       BIT(19)
#define SAM_UOTGHS_HOST_PIP_PRST4       BIT(20)
#define SAM_UOTGHS_HOST_PIP_PRST5       BIT(21)
#define SAM_UOTGHS_HOST_PIP_PRST6       BIT(22)
#define SAM_UOTGHS_HOST_PIP_PRST7       BIT(23)
#define SAM_UOTGHS_HOST_PIP_PRST8       BIT(24)

/* Host Pipe x Configuration Register. */
#define SAM_UOTGHS_HOST_PIPCFG_ALLOC                   BIT(1)
#define SAM_UOTGHS_HOST_PIPCFG_PBK_POS                 (2)
#define SAM_UOTGHS_HOST_PIPCFG_PBK_MASK                (0x3 << SAM_UOTGHS_HOST_PIPCFG_PBK_POS)
#define SAM_UOTGHS_HOST_PIPCFG_PBK(value)              BITFIELD_SET(SAM_UOTGHS_HOST_PIPCFG_PBK, (value))
#define SAM_UOTGHS_HOST_PIPCFG_PBK_1_BANK              SAM_UOTGHS_HOST_PIPCFG_PBK(0)
#define SAM_UOTGHS_HOST_PIPCFG_PBK_2_BANK              SAM_UOTGHS_HOST_PIPCFG_PBK(1)
#define SAM_UOTGHS_HOST_PIPCFG_PBK_3_BANK              SAM_UOTGHS_HOST_PIPCFG_PBK(2)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_POS               (4)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_MASK              (0x7 << SAM_UOTGHS_HOST_PIPCFG_PSIZE_POS)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE(value)            BITFIELD_SET(SAM_UOTGHS_HOST_PIPCFG_PSIZE, (value))
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_8_BYTES           SAM_UOTGHS_HOST_PIPCFG_PSIZE(0)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_16_BYTES          SAM_UOTGHS_HOST_PIPCFG_PSIZE(1)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_32_BYTES          SAM_UOTGHS_HOST_PIPCFG_PSIZE(2)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_64_BYTES          SAM_UOTGHS_HOST_PIPCFG_PSIZE(3)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_128_BYTES         SAM_UOTGHS_HOST_PIPCFG_PSIZE(4)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_256_BYTES         SAM_UOTGHS_HOST_PIPCFG_PSIZE(5)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_512_BYTES         SAM_UOTGHS_HOST_PIPCFG_PSIZE(6)
#define SAM_UOTGHS_HOST_PIPCFG_PSIZE_1024_BYTES        SAM_UOTGHS_HOST_PIPCFG_PSIZE(7)
#define SAM_UOTGHS_HOST_PIPCFG_PTOKEN_POS              (8)
#define SAM_UOTGHS_HOST_PIPCFG_PTOKEN_MASK             (0x3 << SAM_UOTGHS_HOST_PIPCFG_PTOKEN_POS)
#define SAM_UOTGHS_HOST_PIPCFG_PTOKEN(value)           BITFIELD_SET(SAM_UOTGHS_HOST_PIPCFG_PTOKEN, (value))
#define SAM_UOTGHS_HOST_PIPCFG_PTOKEN_SET(reg, value)  (((reg) & ~SAM_UOTGHS_HOST_PIPCFG_PTOKEN_MASK) | \
                                                        SAM_UOTGHS_HOST_PIPCFG_PTOKEN_ ## value)
#define SAM_UOTGHS_HOST_PIPCFG_PTOKEN_SETUP            SAM_UOTGHS_HOST_PIPCFG_PTOKEN(0)
#define SAM_UOTGHS_HOST_PIPCFG_PTOKEN_IN               SAM_UOTGHS_HOST_PIPCFG_PTOKEN(1)
#define SAM_UOTGHS_HOST_PIPCFG_PTOKEN_OUT              SAM_UOTGHS_HOST_PIPCFG_PTOKEN(2)
#define SAM_UOTGHS_HOST_PIPCFG_AUTOSW                  BIT(10)
#define SAM_UOTGHS_HOST_PIPCFG_PTYPE_POS               (12)
#define SAM_UOTGHS_HOST_PIPCFG_PTYPE_MASK              (0x3 << SAM_UOTGHS_HOST_PIPCFG_PTYPE_POS)
#define SAM_UOTGHS_HOST_PIPCFG_PTYPE(value)            BITFIELD_SET(SAM_UOTGHS_HOST_PIPCFG_PTYPE, (value))
#define SAM_UOTGHS_HOST_PIPCFG_PTYPE_CTRL              SAM_UOTGHS_HOST_PIPCFG_PTYPE(0)
#define SAM_UOTGHS_HOST_PIPCFG_PTYPE_ISO               SAM_UOTGHS_HOST_PIPCFG_PTYPE(1)
#define SAM_UOTGHS_HOST_PIPCFG_PTYPE_BLK               SAM_UOTGHS_HOST_PIPCFG_PTYPE(2)
#define SAM_UOTGHS_HOST_PIPCFG_PTYPE_INTRPT            SAM_UOTGHS_HOST_PIPCFG_PTYPE(3)
#define SAM_UOTGHS_HOST_PIPCFG_PEPNUM_POS              (16)
#define SAM_UOTGHS_HOST_PIPCFG_PEPNUM_MASK             (0xf << SAM_UOTGHS_HOST_PIPCFG_PEPNUM_POS)
#define SAM_UOTGHS_HOST_PIPCFG_PEPNUM(value)           BITFIELD_SET(SAM_UOTGHS_HOST_PIPCFG_PEPNUM, (value))
#define SAM_UOTGHS_HOST_PIPCFG_PINGEN                  BIT(20)
#define SAM_UOTGHS_HOST_PIPCFG_INTFRQ_BINTERVAL_POS    (24)
#define SAM_UOTGHS_HOST_PIPCFG_INTFRQ_BINTERVAL_MASK   (0xff << SAM_UOTGHS_HOST_PIPCFG_INTFRQ_BINTERVAL_POS)
#define SAM_UOTGHS_HOST_PIPCFG_INTFRQ_BINTERVAL(value) BITFIELD_SET(SAM_UOTGHS_HOST_PIPCFG_INTFRQ_BINTERVAL, (value))

/* Host Pipe x Status Register. */
#define SAM_UOTGHS_HOST_PIPISR_RXINI              BIT(0)
#define SAM_UOTGHS_HOST_PIPISR_TXOUTI             BIT(1)
#define SAM_UOTGHS_HOST_PIPISR_TXSTPI_UNDERFI     BIT(2)
#define SAM_UOTGHS_HOST_PIPISR_PERRI              BIT(3)
#define SAM_UOTGHS_HOST_PIPISR_NAKEDI             BIT(4)
#define SAM_UOTGHS_HOST_PIPISR_OVERFI             BIT(5)
#define SAM_UOTGHS_HOST_PIPISR_RXSTALLDI_CRCERRI  BIT(6)
#define SAM_UOTGHS_HOST_PIPISR_SHORTPACKETI       BIT(7)
#define SAM_UOTGHS_HOST_PIPISR_DTSEQ_POS          (8)
#define SAM_UOTGHS_HOST_PIPISR_DTSEQ_MASK         (0x3 << SAM_UOTGHS_HOST_PIPISR_DTSEQ_POS)
#define SAM_UOTGHS_HOST_PIPISR_DTSEQ(value)       BITFIELD_SET(SAM_UOTGHS_HOST_PIPISR_DTSEQ, (value))
#define SAM_UOTGHS_HOST_PIPISR_NBUSYBK_POS        (12)
#define SAM_UOTGHS_HOST_PIPISR_NBUSYBK_MASK       (0x3 << SAM_UOTGHS_HOST_PIPISR_NBUSYBK_POS)
#define SAM_UOTGHS_HOST_PIPISR_NBUSYBK(value)     BITFIELD_SET(SAM_UOTGHS_HOST_PIPISR_NBUSYBK, (value))
#define SAM_UOTGHS_HOST_PIPISR_CURRBK_POS         (14)
#define SAM_UOTGHS_HOST_PIPISR_CURRBK_MASK        (0x3 << SAM_UOTGHS_HOST_PIPISR_CURRBK_POS)
#define SAM_UOTGHS_HOST_PIPISR_CURRBK(value)      BITFIELD_SET(SAM_UOTGHS_HOST_PIPISR_CURRBK, (value))
#define SAM_UOTGHS_HOST_PIPISR_RWALL              BIT(16)
#define SAM_UOTGHS_HOST_PIPISR_CFGOK              BIT(18)
#define SAM_UOTGHS_HOST_PIPISR_PBYCT_POS          (20)
#define SAM_UOTGHS_HOST_PIPISR_PBYCT_MASK         (0x7ff << SAM_UOTGHS_HOST_PIPISR_PBYCT_POS)
#define SAM_UOTGHS_HOST_PIPISR_PBYCT(value)       BITFIELD_SET(SAM_UOTGHS_HOST_PIPISR_PBYCT, (value))

/* Host Pipe x Clear Register. */
#define SAM_UOTGHS_HOST_PIPICR_RXINIC              BIT(0)
#define SAM_UOTGHS_HOST_PIPICR_TXOUTIC             BIT(1)
#define SAM_UOTGHS_HOST_PIPICR_TXSTPIC_UNDERFIC    BIT(2)
#define SAM_UOTGHS_HOST_PIPICR_NAKEDIC             BIT(4)
#define SAM_UOTGHS_HOST_PIPICR_OVERFIC             BIT(5)
#define SAM_UOTGHS_HOST_PIPICR_RXSTALLDIC_CRCERRIC BIT(6)
#define SAM_UOTGHS_HOST_PIPICR_SHORTPACKETIC       BIT(7)

/* Host Pipe x Set Register. */
#define SAM_UOTGHS_HOST_PIPIFR_RXINIS              BIT(0)
#define SAM_UOTGHS_HOST_PIPIFR_TXOUTIS             BIT(1)
#define SAM_UOTGHS_HOST_PIPIFR_TXSTPIS_UNDERFIS    BIT(2)
#define SAM_UOTGHS_HOST_PIPIFR_NAKEDIS             BIT(4)
#define SAM_UOTGHS_HOST_PIPIFR_OVERFIS             BIT(5)
#define SAM_UOTGHS_HOST_PIPIFR_RXSTALLDIS_CRCERRIS BIT(6)
#define SAM_UOTGHS_HOST_PIPIFR_SHORTPACKETIS       BIT(7)
#define SAM_UOTGHS_HOST_PIPIFR_NBUSYBKS            BIT(12)

/* Host Pipe x Mask Register. */
#define SAM_UOTGHS_HOST_PIPIMR_RXINE               BIT(0)
#define SAM_UOTGHS_HOST_PIPIMR_TXOUTE              BIT(1)
#define SAM_UOTGHS_HOST_PIPIMR_TXSTPE_UNDERFE      BIT(2)
#define SAM_UOTGHS_HOST_PIPIMR_NAKEDE              BIT(4)
#define SAM_UOTGHS_HOST_PIPIMR_OVERFE              BIT(5)
#define SAM_UOTGHS_HOST_PIPIMR_RXSTALLDE_CRCERRE   BIT(6)
#define SAM_UOTGHS_HOST_PIPIMR_SHORTPACKETE        BIT(7)
#define SAM_UOTGHS_HOST_PIPIMR_NBUSYBE             BIT(12)
#define SAM_UOTGHS_HOST_PIPIMR_FIFOCON             BIT(14)
#define SAM_UOTGHS_HOST_PIPIMR_PDISHDMA            BIT(16)
#define SAM_UOTGHS_HOST_PIPIMR_PFREEZE             BIT(17)
#define SAM_UOTGHS_HOST_PIPIMR_RSTDT               BIT(18)

/* Host Pipe x Disable Register. */
#define SAM_UOTGHS_HOST_PIPIDR_RXINEC               BIT(0)
#define SAM_UOTGHS_HOST_PIPIDR_TXOUTEC              BIT(1)
#define SAM_UOTGHS_HOST_PIPIDR_TXSTPEC_UNDERFEC     BIT(2)
#define SAM_UOTGHS_HOST_PIPIDR_NAKEDEC              BIT(4)
#define SAM_UOTGHS_HOST_PIPIDR_OVERFEC              BIT(5)
#define SAM_UOTGHS_HOST_PIPIDR_RXSTALLDEC_CRCERREC  BIT(6)
#define SAM_UOTGHS_HOST_PIPIDR_SHORTPACKETEC        BIT(7)
#define SAM_UOTGHS_HOST_PIPIDR_NBUSYBEC             BIT(12)
#define SAM_UOTGHS_HOST_PIPIDR_FIFOCONC             BIT(14)
#define SAM_UOTGHS_HOST_PIPIDR_PDISHDMAC            BIT(16)
#define SAM_UOTGHS_HOST_PIPIDR_PFREEZEC             BIT(17)

/* Host Pipe x Enable Register. */
#define SAM_UOTGHS_HOST_PIPIER_RXINES               BIT(0)
#define SAM_UOTGHS_HOST_PIPIER_TXOUTES              BIT(1)
#define SAM_UOTGHS_HOST_PIPIER_TXSTPES_UNDERFES     BIT(2)
#define SAM_UOTGHS_HOST_PIPIER_NAKEDES              BIT(4)
#define SAM_UOTGHS_HOST_PIPIER_OVERFES              BIT(5)
#define SAM_UOTGHS_HOST_PIPIER_RXSTALLDES_CRCERRES  BIT(6)
#define SAM_UOTGHS_HOST_PIPIER_SHORTPACKETES        BIT(7)
#define SAM_UOTGHS_HOST_PIPIER_NBUSYBES             BIT(12)
#define SAM_UOTGHS_HOST_PIPIER_PDISHDMAS            BIT(16)
#define SAM_UOTGHS_HOST_PIPIER_PFREEZES             BIT(17)
#define SAM_UOTGHS_HOST_PIPIER_RSTDTS               BIT(18)

/* Host Pipe x IN Request Register. */
#define SAM_UOTGHS_HOST_PIPINRQ_INRQ_POS          (0)
#define SAM_UOTGHS_HOST_PIPINRQ_INRQ_MASK         (0xff << SAM_UOTGHS_HOST_PIPINRQ_INRQ_POS)
#define SAM_UOTGHS_HOST_PIPINRQ_INRQ(value)       BITFIELD_SET(SAM_UOTGHS_HOST_PIPINRQ_INRQ, (value))
#define SAM_UOTGHS_HOST_PIPINRQ_INMODE            BIT(8)

/* Host Pipe x Error Register. */
#define SAM_UOTGHS_HOST_PIPERR_DATATGL            BIT(0)
#define SAM_UOTGHS_HOST_PIPERR_DATAPID            BIT(1)
#define SAM_UOTGHS_HOST_PIPERR_PID                BIT(2)
#define SAM_UOTGHS_HOST_PIPERR_TIMEOUT            BIT(3)
#define SAM_UOTGHS_HOST_PIPERR_CRC16              BIT(4)
#define SAM_UOTGHS_HOST_PIPERR_COUNTER_POS        (5)
#define SAM_UOTGHS_HOST_PIPERR_COUNTER_MASK       (0x3 << SAM_UOTGHS_HOST_PIPERR_COUNTER_POS)
#define SAM_UOTGHS_HOST_PIPERR_FS_COUNTER(value)     BITFIELD_SET(SAM_UOTGHS_HOST_PIPERR_COUNTER, (value))

/* Host DMA Channel x Control Register. */
#define SAM_UOTGHS_HOST_DMA_CONTROL_CHANN_ENB          BIT(0)
#define SAM_UOTGHS_HOST_DMA_CONTROL_LDNXT_DSC          BIT(1)
#define SAM_UOTGHS_HOST_DMA_CONTROL_END_TR_EN          BIT(2)
#define SAM_UOTGHS_HOST_DMA_CONTROL_END_B_EN           BIT(3)
#define SAM_UOTGHS_HOST_DMA_CONTROL_END_TR_IT          BIT(4)
#define SAM_UOTGHS_HOST_DMA_CONTROL_END_BUFFIT         BIT(5)
#define SAM_UOTGHS_HOST_DMA_CONTROL_DESC_LD_IT         BIT(6)
#define SAM_UOTGHS_HOST_DMA_CONTROL_BURST_LCK          BIT(7)
#define SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_LENGTH_POS    (16)
#define SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_LENGTH_MASK   (0xffff << SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_LENGTH_POS)
#define SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_LENGTH(value) BITFIELD_SET(SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_LENGTH, (value))

/* Host DMA Channel x Status Register. */
#define SAM_UOTGHS_HOST_DMA_CONTROL_CHANN_ENB         BIT(0)
#define SAM_UOTGHS_HOST_DMA_CONTROL_LDNXT_DSC         BIT(1)
#define SAM_UOTGHS_HOST_DMA_CONTROL_END_TR_IT         BIT(4)
#define SAM_UOTGHS_HOST_DMA_CONTROL_END_BUFFIT        BIT(5)
#define SAM_UOTGHS_HOST_DMA_CONTROL_DESC_LD_IT        BIT(6)
#define SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_COUNT_POS    (16)
#define SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_COUNT_MASK   (0xffff << SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_COUNT_POS)
#define SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_COUNT(value) BITFIELD_SET(SAM_UOTGHS_HOST_DMA_CONTROL_BUFF_COUNT, (value))

/* General. */

/* General Control Register. */
#define SAM_UOTGHS_CTRL_IDTE            BIT(0)
#define SAM_UOTGHS_CTRL_VBUSTE          BIT(1)
#define SAM_UOTGHS_CTRL_SRPE            BIT(2)
#define SAM_UOTGHS_CTRL_VBERRE          BIT(3)
#define SAM_UOTGHS_CTRL_BCERRE          BIT(4)
#define SAM_UOTGHS_CTRL_ROLEEXE         BIT(5)
#define SAM_UOTGHS_CTRL_HNPERRE         BIT(6)
#define SAM_UOTGHS_CTRL_STOE            BIT(7)
#define SAM_UOTGHS_CTRL_VBUSHWC         BIT(8)
#define SAM_UOTGHS_CTRL_SRPSEL          BIT(9)
#define SAM_UOTGHS_CTRL_SRPREQ          BIT(10)
#define SAM_UOTGHS_CTRL_HNPREQ          BIT(11)
#define SAM_UOTGHS_CTRL_OTGPADE         BIT(12)
#define SAM_UOTGHS_CTRL_VBUSPO          BIT(13)
#define SAM_UOTGHS_CTRL_FRZCLK          BIT(14)
#define SAM_UOTGHS_CTRL_USBE            BIT(15)
#define SAM_UOTGHS_CTRL_TIMVALUE_POS    (16)
#define SAM_UOTGHS_CTRL_TIMVALUE_MASK   (0x3 << SAM_UOTGHS_CTRL_TIMVALUE_POS)
#define SAM_UOTGHS_CTRL_TIMVALUE(value) BITFIELD_SET(SAM_UOTGHS_CTRL_TIMVALUE, (value))
#define SAM_UOTGHS_CTRL_TIMPAGE_POS     (20)
#define SAM_UOTGHS_CTRL_TIMPAGE_MASK    (0x3 << SAM_UOTGHS_CTRL_TIMPAGE_POS)
#define SAM_UOTGHS_CTRL_TIMPAGE(value)  BITFIELD_SET(SAM_UOTGHS_CTRL_TIMPAGE, (value))
#define SAM_UOTGHS_CTRL_UNLOCK          BIT(22)
#define SAM_UOTGHS_CTRL_UIDE            BIT(24)
#define SAM_UOTGHS_CTRL_UIMOD           BIT(25)

/* General Status Register. */
#define SAM_UOTGHS_SR_IDTI            BIT(0)
#define SAM_UOTGHS_SR_VBUSTI          BIT(1)
#define SAM_UOTGHS_SR_SRPI            BIT(2)
#define SAM_UOTGHS_SR_VBERRI          BIT(3)
#define SAM_UOTGHS_SR_BCERRI          BIT(4)
#define SAM_UOTGHS_SR_ROLEEXI         BIT(5)
#define SAM_UOTGHS_SR_HNPERRI         BIT(6)
#define SAM_UOTGHS_SR_STOI            BIT(7)
#define SAM_UOTGHS_SR_VBUSRQ          BIT(9)
#define SAM_UOTGHS_SR_ID              BIT(10)
#define SAM_UOTGHS_SR_VBUS            BIT(11)
#define SAM_UOTGHS_SR_SPEED_POS       (12)
#define SAM_UOTGHS_SR_SPEED_MASK      (0x3 << SAM_UOTGHS_SR_SPEED_POS)
#define SAM_UOTGHS_SR_SPEED(value)    BITFIELD_SET(SAM_UOTGHS_SR_SPEED, (value))
#define SAM_UOTGHS_SR_CLKUSABLE       BIT(14)

/* General Status Clear Register. */
#define SAM_UOTGHS_SR_IDTIC           BIT(0)
#define SAM_UOTGHS_SR_VBUSTIC         BIT(1)
#define SAM_UOTGHS_SR_SRPIC           BIT(2)
#define SAM_UOTGHS_SR_VBERRIC         BIT(3)
#define SAM_UOTGHS_SR_BCERRIC         BIT(4)
#define SAM_UOTGHS_SR_ROLEEXIC        BIT(5)
#define SAM_UOTGHS_SR_HNPERRIC        BIT(6)
#define SAM_UOTGHS_SR_STOIC           BIT(7)
#define SAM_UOTGHS_SR_VBUSRQC         BIT(9)

/* General Status Set Register. */
#define SAM_UOTGHS_SR_IDTIS           BIT(0)
#define SAM_UOTGHS_SR_VBUSTIS         BIT(1)
#define SAM_UOTGHS_SR_SRPIS           BIT(2)
#define SAM_UOTGHS_SR_VBERRIS         BIT(3)
#define SAM_UOTGHS_SR_BCERRIS         BIT(4)
#define SAM_UOTGHS_SR_ROLEEXIS        BIT(5)
#define SAM_UOTGHS_SR_HNPERRIS        BIT(6)
#define SAM_UOTGHS_SR_STOIS           BIT(7)
#define SAM_UOTGHS_SR_VBUSRQS         BIT(9)

/* General Finite State Machine. */
#define SAM_UOTGHS_FSM_DRDSTATE_POS       (0)
#define SAM_UOTGHS_FSM_DRDSTATE_MASK      (0xf << SAM_UOTGHS_FSM_DRDSTATE_POS)
#define SAM_UOTGHS_FSM_DRDSTATE(value)    BITFIELD_SET(SAM_UOTGHS_FSM_DRDSTATE, (value))

/* 40. Controller Area Network (CAN). */
struct sam_can_mailbox_t {
    uint32_t MMR;
    uint32_t MAM;
    uint32_t MID;
    uint32_t MFID;
    uint32_t MSR;
    uint32_t MDL;
    uint32_t MDH;
    uint32_t MCR;
};

struct sam_can_t {
    uint32_t MR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t SR;
    uint32_t BR;
    uint32_t TIM;
    uint32_t TIMESTP;
    uint32_t ECR;
    uint32_t TCR;
    uint32_t ACR;
    uint32_t RESERVED0[46];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t RESERVED1[69];
    struct sam_can_mailbox_t MAILBOX[8];
};

/* Mode Register. */
#define CAN_MR_CANEN                    BIT(0)
#define CAN_MR_LPM                      BIT(1)
#define CAN_MR_ABM                      BIT(2)
#define CAN_MR_OVL                      BIT(3)
#define CAN_MR_TEOF                     BIT(4)
#define CAN_MR_TTM                      BIT(5)
#define CAN_MR_TIMFRZ                   BIT(6)
#define CAN_MR_DRPT                     BIT(7)
#define CAN_MR_RXSYNC_POS               (24)
#define CAN_MR_RXSYNC_MASK              (0x7 << CAN_MR_RXSYNC_POS)
#define CAN_MR_RXSYNC(value)            BITFIELD_SET(CAN_MR_RXSYNC, value)

/* Interrupt Enable Register. */
#define CAN_IER_MB0                     BIT(0)
#define CAN_IER_MB1                     BIT(1)
#define CAN_IER_MB2                     BIT(2)
#define CAN_IER_MB3                     BIT(3)
#define CAN_IER_MB4                     BIT(4)
#define CAN_IER_MB5                     BIT(5)
#define CAN_IER_MB6                     BIT(6)
#define CAN_IER_MB7                     BIT(7)
#define CAN_IER_ERRA                    BIT(16)
#define CAN_IER_WARN                    BIT(17)
#define CAN_IER_ERRP                    BIT(18)
#define CAN_IER_BOFF                    BIT(19)
#define CAN_IER_SLEEP                   BIT(20)
#define CAN_IER_WAKEUP                  BIT(21)
#define CAN_IER_TOVF                    BIT(22)
#define CAN_IER_TSTP                    BIT(23)
#define CAN_IER_CERR                    BIT(24)
#define CAN_IER_SERR                    BIT(25)
#define CAN_IER_AERR                    BIT(26)
#define CAN_IER_FERR                    BIT(27)
#define CAN_IER_BERR                    BIT(28)

/* Interrupt Disable Register. */
#define CAN_IDR_MB0                     BIT(0)
#define CAN_IDR_MB1                     BIT(1)
#define CAN_IDR_MB2                     BIT(2)
#define CAN_IDR_MB3                     BIT(3)
#define CAN_IDR_MB4                     BIT(4)
#define CAN_IDR_MB5                     BIT(5)
#define CAN_IDR_MB6                     BIT(6)
#define CAN_IDR_MB7                     BIT(7)
#define CAN_IDR_ERRA                    BIT(16)
#define CAN_IDR_WARN                    BIT(17)
#define CAN_IDR_ERRP                    BIT(18)
#define CAN_IDR_BOFF                    BIT(19)
#define CAN_IDR_SLEEP                   BIT(20)
#define CAN_IDR_WAKEUP                  BIT(21)
#define CAN_IDR_TOVF                    BIT(22)
#define CAN_IDR_TSTP                    BIT(23)
#define CAN_IDR_CERR                    BIT(24)
#define CAN_IDR_SERR                    BIT(25)
#define CAN_IDR_AERR                    BIT(26)
#define CAN_IDR_FERR                    BIT(27)
#define CAN_IDR_BERR                    BIT(28)

/* Interrupt Mask Register. */
#define CAN_IMR_MB0                     BIT(0)
#define CAN_IMR_MB1                     BIT(1)
#define CAN_IMR_MB2                     BIT(2)
#define CAN_IMR_MB3                     BIT(3)
#define CAN_IMR_MB4                     BIT(4)
#define CAN_IMR_MB5                     BIT(5)
#define CAN_IMR_MB6                     BIT(6)
#define CAN_IMR_MB7                     BIT(7)
#define CAN_IMR_ERRA                    BIT(16)
#define CAN_IMR_WARN                    BIT(17)
#define CAN_IMR_ERRP                    BIT(18)
#define CAN_IMR_BOFF                    BIT(19)
#define CAN_IMR_SLEEP                   BIT(20)
#define CAN_IMR_WAKEUP                  BIT(21)
#define CAN_IMR_TOVF                    BIT(22)
#define CAN_IMR_TSTP                    BIT(23)
#define CAN_IMR_CERR                    BIT(24)
#define CAN_IMR_SERR                    BIT(25)
#define CAN_IMR_AERR                    BIT(26)
#define CAN_IMR_FERR                    BIT(27)
#define CAN_IMR_BERR                    BIT(28)

/* Status Register. */
#define CAN_SR_MB0                      BIT(0)
#define CAN_SR_MB1                      BIT(1)
#define CAN_SR_MB2                      BIT(2)
#define CAN_SR_MB3                      BIT(3)
#define CAN_SR_MB4                      BIT(4)
#define CAN_SR_MB5                      BIT(5)
#define CAN_SR_MB6                      BIT(6)
#define CAN_SR_MB7                      BIT(7)
#define CAN_SR_ERRA                     BIT(16)
#define CAN_SR_WARN                     BIT(17)
#define CAN_SR_ERRP                     BIT(18)
#define CAN_SR_BOFF                     BIT(19)
#define CAN_SR_SLEEP                    BIT(20)
#define CAN_SR_WAKEUP                   BIT(21)
#define CAN_SR_TOVF                     BIT(22)
#define CAN_SR_TSTP                     BIT(23)
#define CAN_SR_CERR                     BIT(24)
#define CAN_SR_SERR                     BIT(25)
#define CAN_SR_AERR                     BIT(26)
#define CAN_SR_FERR                     BIT(27)
#define CAN_SR_BERR                     BIT(28)
#define CAN_SR_RBSY                     BIT(29)
#define CAN_SR_TBSY                     BIT(30)
#define CAN_SR_OVLSY                    BIT(31)

/* Baudrate Register. */
#define CAN_BR_PHASE2_POS               (0)
#define CAN_BR_PHASE2_MASK              (0x7 << CAN_BR_PHASE2_POS)
#define CAN_BR_PHASE2(value)            BITFIELD_SET(CAN_BR_PHASE2, value)
#define CAN_BR_PHASE1_POS               (4)
#define CAN_BR_PHASE1_MASK              (0x7 << CAN_BR_PHASE1_POS)
#define CAN_BR_PHASE1(value)            BITFIELD_SET(CAN_BR_PHASE1, value)
#define CAN_BR_PROPAG_POS               (8)
#define CAN_BR_PROPAG_MASK              (0x7 << CAN_BR_PROPAG_POS)
#define CAN_BR_PROPAG(value)            BITFIELD_SET(CAN_BR_PROPAG, value)
#define CAN_BR_SJW_POS                  (12)
#define CAN_BR_SJW_MASK                 (0x3 << CAN_BR_SJW_POS)
#define CAN_BR_SJW(value)               BITFIELD_SET(CAN_BR_SJW, value)
#define CAN_BR_BRP_POS                  (16)
#define CAN_BR_BRP_MASK                 (0x7f << CAN_BR_BRP_POS)
#define CAN_BR_BRP(value)               BITFIELD_SET(CAN_BR_BRP, value)
#define CAN_BR_SMP                      BIT(24)

/* Timer Register. */
#define CAN_TIM_TIMER_POS               (0)
#define CAN_TIM_TIMER_MASK              (0xffff << CAN_TIM_TIMER_POS)
#define CAN_TIM_TIMER(value)            BITFIELD_SET(CAN_TIM_TIMER, value)

/* Timestamp Register. */
#define CAN_TIMESTP_MTIMESTAMP_POS      (0)
#define CAN_TIMESTP_MTIMESTAMP_MASK     (0xffff << CAN_TIMESTP_MTIMESTAMP_POS)
#define CAN_TIMESTP_MTIMESTAMP(value)   BITFIELD_SET(CAN_TIMESTP_MTIMESTAMP, value)

/* Error Counter Register. */
#define CAN_ECR_REC_POS                 (0)
#define CAN_ECR_REC_MASK                (0xff << CAN_ECR_REC_POS)
#define CAN_ECR_REC(value)              BITFIELD_SET(CAN_ECR_REC, value)
#define CAN_ECR_TEC_POS                 (16)
#define CAN_ECR_TEC_MASK                (0xff << CAN_ECR_TEC_POS)
#define CAN_ECR_TEC(value)              BITFIELD_SET(CAN_ECR_TEC, value)

/* Transfer Command Register. */
#define CAN_TCR_MB0                     BIT(0)
#define CAN_TCR_MB1                     BIT(1)
#define CAN_TCR_MB2                     BIT(2)
#define CAN_TCR_MB3                     BIT(3)
#define CAN_TCR_MB4                     BIT(4)
#define CAN_TCR_MB5                     BIT(5)
#define CAN_TCR_MB6                     BIT(6)
#define CAN_TCR_MB7                     BIT(7)
#define CAN_TCR_TIMRST                  BIT(31)

/* Abort Register. */
#define CAN_ACR_MB0                     BIT(0)
#define CAN_ACR_MB1                     BIT(1)
#define CAN_ACR_MB2                     BIT(2)
#define CAN_ACR_MB3                     BIT(3)
#define CAN_ACR_MB4                     BIT(4)
#define CAN_ACR_MB5                     BIT(5)
#define CAN_ACR_MB6                     BIT(6)
#define CAN_ACR_MB7                     BIT(7)

/* Write Protection Mode Register. */
#define CAN_WPMR_WPEN                   BIT(0)
#define CAN_WPMR_WPKEY_POS              (8)
#define CAN_WPMR_WPKEY_MASK             (0xffffff << CAN_WPMR_WPKEY_POS)
#define CAN_WPMR_WPKEY(value)           BITFIELD_SET(CAN_WPMR_WPKEY, value)

/* Write Protection Mode Register. */
#define CAN_WPSR_WPVS                   BIT(0)
#define CAN_WPSR_WPVSRC_POS             (8)
#define CAN_WPSR_WPVSRC_MASK            (0xff << CAN_WPSR_WPVSRC_POS)
#define CAN_WPSR_WPVSRC(value)          BITFIELD_SET(CAN_WPSR_WPVSRC, value)

/* Message Mode Register. */
#define CAN_MMR_MTIMEMARK_POS           (0)
#define CAN_MMR_MTIMEMARK_MASK          (0xffff << CAN_MMR_MTIMEMARK_POS)
#define CAN_MMR_MTIMEMARK(value)        BITFIELD_SET(CAN_MMR_MTIMEMARK, value)
#define CAN_MMR_PRIOR_POS               (16)
#define CAN_MMR_PRIOR_MASK              (0xf << CAN_MMR_PRIOR_POS)
#define CAN_MMR_PRIOR(value)            BITFIELD_SET(CAN_MMR_PRIOR, value)
#define CAN_MMR_MOT_POS                 (24)
#define CAN_MMR_MOT_MASK                (0x7 << CAN_MMR_MOT_POS)
#define CAN_MMR_MOT(value)              BITFIELD_SET(CAN_MMR_MOT, value)
#define CAN_MMR_MOT_MB_RX               CAN_MMR_MOT(1)
#define CAN_MMR_MOT_MB_TX               CAN_MMR_MOT(3)

/* Message Acceptence Mask Register. */
#define CAN_MAM_MIDVB_POS               (0)
#define CAN_MAM_MIDVB_MASK              (0x3ffff << CAN_MAM_MIDVB_POS)
#define CAN_MAM_MIDVB(value)            BITFIELD_SET(CAN_MAM_MIDVB, value)
#define CAN_MAM_MIDVA_POS               (18)
#define CAN_MAM_MIDVA_MASK              (0x7ff << CAN_MAM_MIDVA_POS)
#define CAN_MAM_MIDVA(value)            BITFIELD_SET(CAN_MAM_MIDVA, value)
#define CAN_MAM_MIDE                    BIT(29)

/* Message ID Register. */
#define CAN_MID_MIDVB_POS               (0)
#define CAN_MID_MIDVB_MASK              (0x3ffff << CAN_MID_MIDVB_POS)
#define CAN_MID_MIDVB(value)            BITFIELD_SET(CAN_MID_MIDVB, value)
#define CAN_MID_MIDVA_POS               (18)
#define CAN_MID_MIDVA_MASK              (0x7ff << CAN_MID_MIDVA_POS)
#define CAN_MID_MIDVA(value)            BITFIELD_SET(CAN_MID_MIDVA, value)
#define CAN_MID_MIDE                    BIT(29)

/* Message Family ID Register. */
#define CAN_MFID_MFID_POS               (0)
#define CAN_MFID_MFID_MASK              (0x1fffffff << CAN_MFID_MFID_POS)
#define CAN_MFID_MFID(value)            BITFIELD_SET(CAN_MFID_MFID, value)

/* Message Status Register. */
#define CAN_MSR_MTIMESTAMP_POS          (0)
#define CAN_MSR_MTIMESTAMP_MASK         (0xffff << CAN_MSR_MTIMESTAMP_POS)
#define CAN_MSR_MTIMESTAMP(value)       BITFIELD_SET(CAN_MSR_MTIMESTAMP, value)
#define CAN_MSR_MDLC_POS                (16)
#define CAN_MSR_MDLC_MASK               (0xf << CAN_MSR_MDLC_POS)
#define CAN_MSR_MDLC(value)             BITFIELD_SET(CAN_MSR_MDLC, value)
#define CAN_MSR_MRTR                    BIT(20)
#define CAN_MSR_MABT                    BIT(22)
#define CAN_MSR_MRDY                    BIT(23)
#define CAN_MSR_MMI                     BIT(24)

/* Message Control Register. */
#define CAN_MCR_MDLC_POS                (16)
#define CAN_MCR_MDLC_MASK               (0xf << CAN_MCR_MDLC_POS)
#define CAN_MCR_MDLC(value)             BITFIELD_SET(CAN_MCR_MDLC, value)
#define CAN_MCR_MRTR                    BIT(20)
#define CAN_MCR_MACR                    BIT(22)
#define CAN_MCR_MTCR                    BIT(23)

/* 42. True Random Number Generator. */
struct sam_trng_t {
    uint32_t CR;
    uint32_t RESERVED0[3];
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t ISR;
    uint32_t RESERVED1[12];
    uint32_t ODATA;
};

/* Control register. */
#define SAM_TRNG_CR_ENABLE              BIT(0)
#define SAM_TRNG_CR_KEY_POS             (8)
#define SAM_TRNG_CR_KEY_MASK            (0xffffff << SAM_TRNG_CR_KEY_POS)
#define SAM_TRNG_CR_KEY(value)          BITFIELD_SET(SAM_TRNG_CR_KEY, value)
#define SAM_TRNG_CR_KEY_RNG             SAM_TRNG_CR_KEY(0x524e47)

/* Interrupt enable register. */
#define SAM_TRNG_IER_DATRDY             BIT(0)

/* Interrupt disable register. */
#define SAM_TRNG_IDR_DATRDY             BIT(0)

/* Interrupt mask register. */
#define SAM_TRNG_IMR_DATRDY             BIT(0)

/* Interrupt status register. */
#define SAM_TRNG_ISR_DATRDY             BIT(0)

/* 43. Analog-to-Digital Converter Controller. */
struct sam_adc_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t SEQR1;
    uint32_t SEQR2;
    uint32_t CHER;
    uint32_t CHDR;
    uint32_t CHSR;
    uint32_t RESERVED0;
    uint32_t LCDR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t ISR;
    uint32_t RESERVED1[2];
    uint32_t OVER;
    uint32_t EMR;
    uint32_t CWR;
    uint32_t CGR;
    uint32_t COR;
    uint32_t CDR[16];
    uint32_t RESERVED2;
    uint32_t ACR;
    uint32_t RESERVED3[19];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t RESERVED4[5];
    struct sam_pdc_t PDC;
};

/* Control Register. */
#define SAM_ADC_CR_SWRST                BIT(0)
#define SAM_ADC_CR_START                BIT(1)

/* Mode Register. */
#define SAM_ADC_MR_TRGEN                BIT(0)
#define SAM_ADC_MR_TRGSEL_POS           (1)
#define SAM_ADC_MR_TRGSEL_MASK          (0x7 << SAM_ADC_MR_TRGSEL_POS)
#define SAM_ADC_MR_TRGSEL(value)        BITFIELD_SET(SAM_ADC_MR_TRGSEL, value)
#define SAM_ADC_MR_LOWRES               BIT(4)
#define SAM_ADC_MR_SLEEP                BIT(5)
#define SAM_ADC_MR_FWUP                 BIT(6)
#define SAM_ADC_MR_FREERUN              BIT(7)
#define SAM_ADC_MR_PRESCAL_POS          (8)
#define SAM_ADC_MR_PRESCAL_MASK         (0xff << SAM_ADC_MR_PRESCAL_POS)
#define SAM_ADC_MR_PRESCAL(value)       BITFIELD_SET(SAM_ADC_MR_PRESCAL, value)
#define SAM_ADC_MR_STARTUP_POS          (16)
#define SAM_ADC_MR_STARTUP_MASK         (0xf << SAM_ADC_MR_STARTUP_POS)
#define SAM_ADC_MR_STARTUP(value)       BITFIELD_SET(SAM_ADC_MR_STARTUP, value)
#define SAM_ADC_MR_SETTLING_POS         (20)
#define SAM_ADC_MR_SETTLING_MASK        (0x3 << SAM_ADC_MR_SETTLING_POS)
#define SAM_ADC_MR_SETTLING(value)      BITFIELD_SET(SAM_ADC_MR_SETTLING, value)
#define SAM_ADC_MR_ANACH                BIT(23)
#define SAM_ADC_MR_TRACKTIM_POS         (24)
#define SAM_ADC_MR_TRACKTIM_MASK        (0xf << SAM_ADC_MR_TRACKTIM_POS)
#define SAM_ADC_MR_TRACKTIM(value)      BITFIELD_SET(SAM_ADC_MR_TRACKTIM, value)
#define SAM_ADC_MR_TRANSFER_POS         (28)
#define SAM_ADC_MR_TRANSFER_MASK        (0x3 << SAM_ADC_MR_TRANSFER_POS)
#define SAM_ADC_MR_TRANSFER(value)      BITFIELD_SET(SAM_ADC_MR_TRANSFER, value)
#define SAM_ADC_MR_USEQ                 BIT(31)

/* Interrupt Enable Register */
#define SAM_ADC_IER_DRDY               BIT(24)
#define SAM_ADC_IER_GOVRE              BIT(25)
#define SAM_ADC_IER_COMPE              BIT(26)
#define SAM_ADC_IER_ENDRX              BIT(27)
#define SAM_ADC_IER_RXBUFF             BIT(28)

/* Interrupt Disable Register. */
#define SAM_ADC_IDR_DRDY               BIT(24)
#define SAM_ADC_IDR_GOVRE              BIT(25)
#define SAM_ADC_IDR_COMPE              BIT(26)
#define SAM_ADC_IDR_ENDRX              BIT(27)
#define SAM_ADC_IDR_RXBUFF             BIT(28)

/* Interrupt Mask Register. */
#define SAM_ADC_IMR_DRDY               BIT(24)
#define SAM_ADC_IMR_GOVRE              BIT(25)
#define SAM_ADC_IMR_COMPE              BIT(26)
#define SAM_ADC_IMR_ENDRX              BIT(27)
#define SAM_ADC_IMR_RXBUFF             BIT(28)

/* Interrupt Status Register. */
#define SAM_ADC_ISR_DRDY               BIT(24)
#define SAM_ADC_ISR_GOVRE              BIT(25)
#define SAM_ADC_ISR_COMPE              BIT(26)
#define SAM_ADC_ISR_ENDRX              BIT(27)
#define SAM_ADC_ISR_RXBUFF             BIT(28)

/* 44. Digital-to-Analog Converter Controller. */
struct sam_dacc_t {
    uint32_t CR;
    uint32_t MR;
    uint32_t RESERVED0[2];
    uint32_t CHER;
    uint32_t CHDR;
    uint32_t CHSR;
    uint32_t RESERVED1;
    uint32_t CDR;
    uint32_t IER;
    uint32_t IDR;
    uint32_t IMR;
    uint32_t ISR;
    uint32_t RESERVED2[24];
    uint32_t ACR;
    uint32_t RESERVED3[19];
    uint32_t WPMR;
    uint32_t WPSR;
    uint32_t RESERVED4[5];
    struct sam_pdc_t PDC;
};

/* Control Register. */
#define SAM_DACC_CR_SWRST               BIT(0)

/* Mode Register. */
#define SAM_DACC_MR_TRGEN               BIT(0)
#define SAM_DACC_MR_TRGSEL_POS          (1)
#define SAM_DACC_MR_TRGSEL_MASK         (0x7 << SAM_DACC_MR_TRGSEL_POS)
#define SAM_DACC_MR_TRGSEL(value)       BITFIELD_SET(SAM_DACC_MR_TRGSEL, value)
#define SAM_DACC_MR_WORD                BIT(4)
#define SAM_DACC_MR_SLEEP               BIT(5)
#define SAM_DACC_MR_FASTWKUP            BIT(6)
#define SAM_DACC_MR_REFRESH_POS         (8)
#define SAM_DACC_MR_REFRESH_MASK        (0xff << SAM_DACC_MR_REFRESH_POS)
#define SAM_DACC_MR_REFRESH(value)      BITFIELD_SET(SAM_DACC_MR_REFRESH, value)
#define SAM_DACC_MR_USER_SEL_POS        (16)
#define SAM_DACC_MR_USER_SEL_MASK       (0x3 << SAM_DACC_MR_USER_SEL_POS)
#define SAM_DACC_MR_USER_SEL(value)     BITFIELD_SET(SAM_DACC_MR_USER_SEL, value)
#define SAM_DACC_MR_TAG                 BIT(20)
#define SAM_DACC_MR_MAXS                BIT(21)
#define SAM_DACC_MR_STARTUP_POS         (24)
#define SAM_DACC_MR_STARTUP_MASK        (0x3f << SAM_DACC_MR_STARTUP_POS)
#define SAM_DACC_MR_STARTUP(value)      BITFIELD_SET(SAM_DACC_MR_STARTUP, value)

/* Channel Enable Register. */
#define SAM_DACC_CHER_CH0               BIT(0)
#define SAM_DACC_CHER_CH1               BIT(1)

/* Channel Disable Register. */
#define SAM_DACC_CHDR_CH0               BIT(0)
#define SAM_DACC_CHDR_CH1               BIT(1)

/* Channel Status Register. */
#define SAM_DACC_CHSR_CH0               BIT(0)
#define SAM_DACC_CHSR_CH1               BIT(1)

/* Interrupt Enable Register. */
#define SAM_DACC_IER_TXRDY              BIT(0)
#define SAM_DACC_IER_EOC                BIT(1)
#define SAM_DACC_IER_ENDTX              BIT(2)
#define SAM_DACC_IER_TXBUFE             BIT(3)

/* Interrupt Disable Register. */
#define SAM_DACC_IDR_TXRDY              BIT(0)
#define SAM_DACC_IDR_EOC                BIT(1)
#define SAM_DACC_IDR_ENDTX              BIT(2)
#define SAM_DACC_IDR_TXBUFE             BIT(3)

/* Interrupt Mask Register. */
#define SAM_DACC_IMR_TXRDY              BIT(0)
#define SAM_DACC_IMR_EOC                BIT(1)
#define SAM_DACC_IMR_ENDTX              BIT(2)
#define SAM_DACC_IMR_TXBUFE             BIT(3)

/* Interrupt Status Register. */
#define SAM_DACC_ISR_TXRDY              BIT(0)
#define SAM_DACC_ISR_EOC                BIT(1)
#define SAM_DACC_ISR_ENDTX              BIT(2)
#define SAM_DACC_ISR_TXBUFE             BIT(3)

/* Analog Current Register. */
#define SAM_DACC_IBCTLCH0_POS           (0)
#define SAM_DACC_IBCTLCH0_MASK          (0x3 << SAM_DACC_IBCTLCH0_POS)
#define SAM_DACC_IBCTLCH0(value)        BITFIELD_SET(SAM_DACC_IBCTLCH0, value)
#define SAM_DACC_IBCTLCH1_POS           (2)
#define SAM_DACC_IBCTLCH1_MASK          (0x3 << SAM_DACC_IBCTLCH1_POS)
#define SAM_DACC_IBCTLCH1(value)        BITFIELD_SET(SAM_DACC_IBCTLCH1, value)
#define SAM_DACC_IBCTLDACCORE_POS       (8)
#define SAM_DACC_IBCTLDACCORE_MASK      (0x3 << SAM_DACC_IBCTLDACCORE_POS)
#define SAM_DACC_IBCTLDACCORE(value)    BITFIELD_SET(SAM_DACC_IBCTLDACCORE, value)

/* Write Protect Mode Register. */
#define SAM_DACC_WPMR_WPEN              BIT(0)

/* Base addresses of peripherals. */
#define SAM_HSMCI      ((volatile struct sam_hsmci_t  *)0x40000000u)
#define SAM_SSC        ((volatile struct sam_ssc_t    *)0x40004000u)
#define SAM_SPI0       ((volatile struct sam_spi_t    *)0x40008000u)
#define SAM_TC0        ((volatile struct sam_tc_t     *)0x40080000u)
#define SAM_TC1        ((volatile struct sam_tc_t     *)0x40084000u)
#define SAM_TC2        ((volatile struct sam_tc_t     *)0x40088000u)
#define SAM_TWI0       ((volatile struct sam_twi_t    *)0x4008c000u)
#define SAM_TWI1       ((volatile struct sam_twi_t    *)0x40090000u)
#define SAM_PWM        ((volatile struct sam_pwm_t    *)0x40094000u)
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
#define SAM_DMAC       ((volatile struct sam_dmac_t   *)0x400c4000u)
#define SAM_DACC       ((volatile struct sam_dacc_t   *)0x400c8000u)
#define SAM_SMC        ((volatile struct sam_smc_t    *)0x400e0000u)
#define SAM_MATRIX     ((volatile struct sam_matrix_t *)0x400e0400u)
#define SAM_PMC        ((volatile struct sam_pmc_t    *)0x400e0600u)
#define SAM_UART       ((volatile struct sam_uart_t   *)0x400e0800u)
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

/* USBOTG DPRAM (DMA) base address. */
#define SAM_UOTGHS_DPRAM_ADDRESS    (0x20180000u)
#define SAM_UOTGHS_DPRAM_PIPE_SIZE  (0x8000)

static inline void pmc_peripheral_clock_enable(int id)
{
    if (id < 32) {
        SAM_PMC->PCER0 = (1 << (id % 32));
    } else {
        SAM_PMC->PCER1 = (1 << (id % 32));
    }
}

static inline void pmc_peripheral_clock_disable(int id)
{
    if (id < 32) {
        SAM_PMC->PCDR0 = (1 << (id % 32));
    } else {
        SAM_PMC->PCDR1 = (1 << (id % 32));
    }
}

#endif
