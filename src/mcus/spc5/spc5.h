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

#ifndef __SPC5_H__
#define __SPC5_H__

/* 7. MC_CGM. */
struct spc5_mc_cgm_t {
    struct {
        uint32_t CTL;
        uint32_t RESERVED0[7];
    } FXOSC;
    uint32_t RESERVED0[16];
    struct {
        uint32_t CTL;
        uint32_t RESERVED0[7];
    } FIRC;
    struct {
        uint32_t CTL;
        uint32_t RESERVED0[7];
    } SIRC;
    struct {
        uint32_t CR;
        uint32_t MR;
        uint32_t RESERVED0[6];
    } FMPLL;
    uint32_t RESERVED1[16];
    struct {
        uint32_t CSR;
        uint32_t FDR;
        uint32_t HFREFR;
        uint32_t LFREFR;
        uint32_t ISR;
        uint32_t RESERVED0[1];
        uint32_t MDR;
        uint32_t RESERVED1[1];
    } CMU;
    uint32_t RESERVED2[148];
    uint32_t OC_EN;
    uint32_t OCDS_SC;
    uint32_t SC_SS;
    uint32_t SC_DC0;
    uint32_t SC_DC1;
    uint32_t SC_DC2;
};

#define SPC5_MC_CGM_FMPLL_CR_IDF_POS                     (26)
#define SPC5_MC_CGM_FMPLL_CR_IDF_MASK (0xf << SPC5_MC_CGM_FMPLL_CR_IDF_POS)
#define SPC5_MC_CGM_FMPLL_CR_IDF(value) BITFIELD_SET(SPC5_MC_CGM_FMPLL_CR_IDF, value)
#define SPC5_MC_CGM_FMPLL_CR_ODF_POS                     (24)
#define SPC5_MC_CGM_FMPLL_CR_ODF_MASK (0x3 << SPC5_MC_CGM_FMPLL_CR_ODF_POS)
#define SPC5_MC_CGM_FMPLL_CR_ODF(value) BITFIELD_SET(SPC5_MC_CGM_FMPLL_CR_ODF, value)
#define SPC5_MC_CGM_FMPLL_CR_NDIV_POS                    (16)
#define SPC5_MC_CGM_FMPLL_CR_NDIV_MASK (0x7f << SPC5_MC_CGM_FMPLL_CR_NDIV_POS)
#define SPC5_MC_CGM_FMPLL_CR_NDIV(value) BITFIELD_SET(SPC5_MC_CGM_FMPLL_CR_NDIV, value)

/* 8. MC_ME. */
struct spc5_mc_me_t {
    uint32_t GS;
    uint32_t MCTL;
    uint32_t ME;
    uint32_t IS;
    uint32_t IM;
    uint32_t IMTS;
    uint32_t DMTS;
    uint32_t RESERVED0[1];
    uint32_t RESET_MC;
    uint32_t TEST_MC;
    uint32_t SAFE_MC;
    uint32_t DRUN_MC;
    uint32_t RUN0_MC;
    uint32_t RUN1_MC;
    uint32_t RUN2_MC;
    uint32_t RUN3_MC;
    uint32_t HALT_MC;
    uint32_t RESERVED1[1];
    uint32_t STOP_MC;
    uint32_t RESERVED2[2];
    uint32_t STANDBY_MC;
    uint32_t RESERVED3[2];
    uint32_t PS[4];
    uint32_t RESERVED4[4];
    uint32_t RUN_PC[8];
    uint32_t LP_PC[8];
    uint8_t PCTL[128];
};

/* 9. Reset Generation. */
struct spc5_mc_rgm_t {
    uint16_t FES;
    uint16_t DES;
    uint16_t FERD;
    uint16_t DERD;
    uint16_t RESERVED0[8];
    uint16_t FESS;
    uint16_t STDBY;
    uint16_t FBRE;
};

#define SPC5_MC_RGM_FES_F_EXR                         BIT(15)
#define SPC5_MC_RGM_FES_F_FLASH                        BIT(8)
#define SPC5_MC_RGM_FES_F_LVD45                        BIT(7)
#define SPC5_MC_RGM_FES_F_CMU_FHL                      BIT(6)
#define SPC5_MC_RGM_FES_F_CMU_OLR                      BIT(5)
#define SPC5_MC_RGM_FES_F_FMPLL                        BIT(4)
#define SPC5_MC_RGM_FES_F_CHKSTOP                      BIT(3)
#define SPC5_MC_RGM_FES_F_SOFT                         BIT(2)
#define SPC5_MC_RGM_FES_F_CORE                         BIT(1)
#define SPC5_MC_RGM_FES_F_JTAG                         BIT(0)

#define SPC5_MC_RGM_DES_F_POR                         BIT(15)
#define SPC5_MC_RGM_DES_F_LVD27_VREG                   BIT(4)
#define SPC5_MC_RGM_DES_F_LVD27                        BIT(3)
#define SPC5_MC_RGM_DES_F_SWT                          BIT(2)
#define SPC5_MC_RGM_DES_F_LVD12_PD1                    BIT(1)
#define SPC5_MC_RGM_DES_F_LVD12_PD0                    BIT(0)

/* 17. INTC. */
struct spc5_intc_t {
    uint32_t MCR;
    uint32_t RESERVED0;
    uint32_t CPR;
    uint32_t RESERVED1;
    uint32_t IACKR;
    uint32_t RESERVED2;
    uint32_t EOIR;
    uint32_t RESERVED3;
    uint32_t SSCI[2];
    uint32_t RESERVED4[6];
    uint32_t PSR[39];
};

/* 19. SIUL. */
struct spc5_siul_t {
    uint32_t RESERVED0[1];
    uint32_t MIDR[2];
    uint32_t RESERVED1[2];
    uint32_t ISR;
    uint32_t IRER;
    uint32_t RESERVED2[3];
    uint32_t IRERR;
    uint32_t IFEER;
    uint32_t IFER;
    uint32_t RESERVED3[3];
    uint16_t PCR[123];
    uint16_t RESERVED4[485];
    uint32_t PSMI[16];
    uint32_t RESERVED5[48];
    uint8_t GPDO[124];
    uint32_t RESERVED6[97];
    uint8_t GPDI[124];
    uint32_t RESERVED7[225];
    uint32_t PGPDO[4];
    uint32_t RESERVED8[12];
    uint32_t PGPDI[4];
    uint32_t RESERVED9[12];
    uint32_t MPGPDO[8];
    uint32_t RESERVED10[216];
    uint32_t IFMC[24];
    uint32_t RESERVED11[8];
    uint32_t IFCPR;
};

#define SPC5_SIUL_PCR_SMC                             BIT(14)
#define SPC5_SIUL_PCR_APC                             BIT(13)
#define SPC5_SIUL_PCR_OBE                              BIT(9)
#define SPC5_SIUL_PCR_IBE                              BIT(8)
#define SPC5_SIUL_PCR_ODE                              BIT(5)
#define SPC5_SIUL_PCR_SRC                              BIT(2)
#define SPC5_SIUL_PCR_WPE                              BIT(1)
#define SPC5_SIUL_PCR_WPS                              BIT(0)

/* 20. LINFlex. */
struct spc5_linflex_t {
    uint32_t LINCR1;
    uint32_t LINIER;
    uint32_t LINSR;
    uint32_t LINESR;
    uint32_t UARTCR;
    uint32_t UARTSR;
    uint32_t LINTCSR;
    uint32_t LINOCR;
    uint32_t LINTOCR;
    uint32_t LINFBRR;
    uint32_t LINIBRR;
    uint32_t LINCFR;
    uint32_t LINCR2;
    uint32_t BIDR;
    uint8_t BDRL[4];
    uint32_t BDRM;
    uint32_t IFER;
    uint32_t IFMI;
    uint32_t IFMR;
    uint32_t IFCR[16];
};

#define SPC5_LINFLEX_LINCR1_INIT                       BIT(0)

#define SPC5_LINFLEX_LINSR_DRF                         BIT(2)
#define SPC5_LINFLEX_LINSR_DTF                         BIT(1)

#define SPC5_LINFLEX_LINIER_DRIE                       BIT(2)
#define SPC5_LINFLEX_LINIER_DTIE                       BIT(1)

#define SPC5_LINFLEX_UARTSR_SZF                       BIT(15)
#define SPC5_LINFLEX_UARTSR_OCF                       BIT(14)
#define SPC5_LINFLEX_UARTSR_PE3                       BIT(13)
#define SPC5_LINFLEX_UARTSR_PE2                       BIT(12)
#define SPC5_LINFLEX_UARTSR_PE1                       BIT(11)
#define SPC5_LINFLEX_UARTSR_PE0                       BIT(10)
#define SPC5_LINFLEX_UARTSR_RMB                        BIT(9)
#define SPC5_LINFLEX_UARTSR_FEF                        BIT(8)
#define SPC5_LINFLEX_UARTSR_BOF                        BIT(7)
#define SPC5_LINFLEX_UARTSR_RPS                        BIT(6)
#define SPC5_LINFLEX_UARTSR_WUF                        BIT(5)
#define SPC5_LINFLEX_UARTSR_DRF                        BIT(2)
#define SPC5_LINFLEX_UARTSR_DTF                        BIT(1)
#define SPC5_LINFLEX_UARTSR_NF                         BIT(0)

#define SPC5_LINFLEX_UARTCR_TDFL_POS                     (13)
#define SPC5_LINFLEX_UARTCR_TDFL_MASK (0x7 << SPC5_LINFLEX_UARTCR_TDFL_POS)
#define SPC5_LINFLEX_UARTCR_TDFL(value) BITFIELD_SET(SPC5_LINFLEX_UARTCR_TDFL, value)
#define SPC5_LINFLEX_UARTCR_TDFL_1BYTE SPC5_LINFLEX_UARTCR_TDFL(0)
#define SPC5_LINFLEX_UARTCR_RDFL_POS                     (10)
#define SPC5_LINFLEX_UARTCR_RDFL_MASK (0x7 << SPC5_LINFLEX_UARTCR_RDFL_POS)
#define SPC5_LINFLEX_UARTCR_RDFL(value) BITFIELD_SET(SPC5_LINFLEX_UARTCR_RDFL, value)
#define SPC5_LINFLEX_UARTCR_RDFL_1BYTE SPC5_LINFLEX_UARTCR_RDFL(0)
#define SPC5_LINFLEX_UARTCR_RXEN                       BIT(5)
#define SPC5_LINFLEX_UARTCR_TXEN                       BIT(4)
#define SPC5_LINFLEX_UARTCR_WL                         BIT(1)
#define SPC5_LINFLEX_UARTCR_UART                       BIT(0)

/* 22. FlexCAN. */
struct spc5_flexcan_msgbuf_t {
    uint32_t CTRL_STATUS;
    uint32_t PRIO_ID;
    uint32_t DATA[2];
};

struct spc5_flexcan_t {
    uint32_t MCR;
    uint32_t CTRL;
    uint32_t TIMER;
    uint32_t RESERVED0;
    uint32_t RXGMASK;
    uint32_t RX14MASK;
    uint32_t RX15MASK;
    uint32_t ECR;
    uint32_t ESR;
    uint32_t IMASK2;
    uint32_t IMASK1;
    uint32_t IFLAG2;
    uint32_t IFLAG1;
    uint32_t RESERVED1[19];
    struct spc5_flexcan_msgbuf_t MSGBUF[64];
};

#define SPC5_FLEXCAN_MCR_MDIS                         BIT(31)
#define SPC5_FLEXCAN_MCR_FRZ                          BIT(30)
#define SPC5_FLEXCAN_MCR_FEN                          BIT(29)
#define SPC5_FLEXCAN_MCR_HALT                         BIT(28)
#define SPC5_FLEXCAN_MCR_SUPV                         BIT(23)
#define SPC5_FLEXCAN_MCR_WRN_EN                       BIT(21)
#define SPC5_FLEXCAN_MCR_SRX_DIS                      BIT(17)
#define SPC5_FLEXCAN_MCR_AEN                          BIT(12)
#define SPC5_FLEXCAN_MCR_MAXMB_POS                        (0)
#define SPC5_FLEXCAN_MCR_MAXMB_MASK (0x3f << SPC5_FLEXCAN_MCR_MAXMB_POS)
#define SPC5_FLEXCAN_MCR_MAXMB(value) BITFIELD_SET(SPC5_FLEXCAN_MCR_MAXMB, value)

#define SPC5_FLEXCAN_CTRL_PRESDIV_POS                    (24)
#define SPC5_FLEXCAN_CTRL_PRESDIV_MASK (0xff << SPC5_FLEXCAN_CTRL_PRESDIV_POS)
#define SPC5_FLEXCAN_CTRL_PRESDIV(value) BITFIELD_SET(SPC5_FLEXCAN_CTRL_PRESDIV, value)
#define SPC5_FLEXCAN_CTRL_RJW_POS                        (22)
#define SPC5_FLEXCAN_CTRL_RJW_MASK (0x3 << SPC5_FLEXCAN_CTRL_RJW_POS)
#define SPC5_FLEXCAN_CTRL_RJW(value) BITFIELD_SET(SPC5_FLEXCAN_CTRL_RJW, value)
#define SPC5_FLEXCAN_CTRL_PSEG1_POS                      (19)
#define SPC5_FLEXCAN_CTRL_PSEG1_MASK (0x7 << SPC5_FLEXCAN_CTRL_PSEG1_POS)
#define SPC5_FLEXCAN_CTRL_PSEG1(value) BITFIELD_SET(SPC5_FLEXCAN_CTRL_PSEG1, value)
#define SPC5_FLEXCAN_CTRL_PSEG2_POS                      (16)
#define SPC5_FLEXCAN_CTRL_PSEG2_MASK (0x7 << SPC5_FLEXCAN_CTRL_PSEG2_POS)
#define SPC5_FLEXCAN_CTRL_PSEG2(value) BITFIELD_SET(SPC5_FLEXCAN_CTRL_PSEG2, value)
#define SPC5_FLEXCAN_CTRL_BOFF_MSK                    BIT(15)
#define SPC5_FLEXCAN_CTRL_ERR_MSK                     BIT(14)
#define SPC5_FLEXCAN_CTRL_CLK_SRC                     BIT(13)
#define SPC5_FLEXCAN_CTRL_LPB                         BIT(12)
#define SPC5_FLEXCAN_CTRL_TWRN_MSK                    BIT(11)
#define SPC5_FLEXCAN_CTRL_RWRN_MSK                    BIT(10)
#define SPC5_FLEXCAN_CTRL_SMP                          BIT(7)
#define SPC5_FLEXCAN_CTRL_BOFF_REC                     BIT(6)
#define SPC5_FLEXCAN_CTRL_TSYN                         BIT(5)
#define SPC5_FLEXCAN_CTRL_LBUF                         BIT(4)
#define SPC5_FLEXCAN_CTRL_LOM                          BIT(3)
#define SPC5_FLEXCAN_CTRL_PROPSEG_POS                     (0)
#define SPC5_FLEXCAN_CTRL_PROPSEG_MASK (0x7 << SPC5_FLEXCAN_CTRL_PROPSEG_POS)
#define SPC5_FLEXCAN_CTRL_PROPSEG(value) BITFIELD_SET(SPC5_FLEXCAN_CTRL_PROPSEG, value)

#define SPC5_FLEXCAN_ESR_TWRN_INT                     BIT(17)
#define SPC5_FLEXCAN_ESR_RWRN_INT                     BIT(16)
#define SPC5_FLEXCAN_ESR_BIT1_ERR                     BIT(15)
#define SPC5_FLEXCAN_ESR_BIT0_ERR                     BIT(14)
#define SPC5_FLEXCAN_ESR_ACK_ERR                      BIT(13)
#define SPC5_FLEXCAN_ESR_CRC_ERR                      BIT(12)
#define SPC5_FLEXCAN_ESR_FRM_ERR                      BIT(11)
#define SPC5_FLEXCAN_ESR_STF_ERR                      BIT(10)
#define SPC5_FLEXCAN_ESR_TX_WRN                        BIT(9)
#define SPC5_FLEXCAN_ESR_RX_WRN                        BIT(8)
#define SPC5_FLEXCAN_ESR_IDLE                          BIT(7)
#define SPC5_FLEXCAN_ESR_TXRX                          BIT(6)
#define SPC5_FLEXCAN_ESR_FLT_CONF_POS                     (4)
#define SPC5_FLEXCAN_ESR_FLT_CONF_MASK (0x3 << SPC5_FLEXCAN_ESR_FLT_CONF_POS)
#define SPC5_FLEXCAN_ESR_FLT_CONF(value) BITFIELD_SET(SPC5_FLEXCAN_ESR_FLT_CONF, value)
#define SPC5_FLEXCAN_ESR_BOFF_INT                      BIT(2)
#define SPC5_FLEXCAN_ESR_ERR_INT                       BIT(1)

#define SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE_POS         (24)
#define SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE_MASK       \
    (0xf << SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE_POS)
#define SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(value)             \
    BITFIELD_SET(SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE, value)
#define SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE           BIT(21)
#define SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH_POS       (16)
#define SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH_MASK     \
    (0xf << SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH_POS)
#define SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH(value)           \
    BITFIELD_SET(SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH, value)

#define SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID_POS           (18)
#define SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID_MASK         \
    (0x7ff << SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID_POS)
#define SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID(value)               \
    BITFIELD_SET(SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID, value)

#define SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID_POS            (0)
#define SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID_MASK         \
    (0x1fffffff << SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID_POS)
#define SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID(value)               \
    BITFIELD_SET(SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID, value)

/* 24. STM. */
struct spc5_stm_t {
    uint32_t CR;
    uint32_t CNT;
    uint32_t RESERVED0[2];
    struct {
        uint32_t CCR;
        uint32_t CIR;
        uint32_t CMP;
        uint32_t RESERVED0;
    } CHANNELS[4];
};

#define SPC5_STM_CR_TEN                                BIT(0)

#define SPC5_STM_CHANNELS_CCR_CEN                      BIT(0)

#define SPC5_STM_CHANNELS_CIR_CIF                      BIT(0)

/* 27. Flash memory. */
struct spc5_flash_t {
    uint32_t MCR;
    uint32_t LML;
    uint32_t RESERVED0[1];
    uint32_t SLL;
    uint32_t LMS;
    uint32_t RESERVED1[1];
    uint32_t ADR;
    uint32_t PFCR[2];
    uint32_t PFAPR;
    uint32_t RESERVED2[6];
    uint32_t UT0;
    uint32_t UT1;
    uint32_t UT2;
    uint32_t UMISR[5];
};

/* 35. Error Correction Status Module. */
struct spc5_ecsm_t {
    uint16_t PCT;
    uint16_t REV;
    uint32_t RESERVED0[1];
    uint32_t IOPMC;
    uint8_t RESERVED1[7];
    uint8_t MWCR;
    uint8_t RESERVED2[11];
    uint8_t MIR;
    uint32_t RESERVED3[1];
    uint32_t MUDCR;
    uint8_t RESERVED4[27];
    uint8_t ECR;
    uint8_t RESERVED5[3];
    uint8_t ESR;
    uint8_t RESERVED6[2];
    uint16_t EEGR;
    uint32_t RESERVED7[1];
    uint32_t PFEAR;
    uint8_t RESERVED8[2];
    uint8_t PFEMR;
    uint8_t PFEAT;
    uint32_t RESERVED9[1];
    uint32_t PFEDR;
    uint32_t PREAR;
    uint8_t RESERVED10[1];
    uint8_t PRESR;
    uint8_t PREMR;
    uint8_t PREAT;
    uint8_t RESERVED11[4];
    uint32_t PREDR;
};

#define SPC5_FLASH_MCR_EDC                            BIT(31)
#define SPC5_FLASH_MCR_SIZE_POS                          (24)
#define SPC5_FLASH_MCR_SIZE_MASK (0x7 << SPC5_FLASH_MCR_SIZE_POS)
#define SPC5_FLASH_MCR_SIZE(value) BITFIELD_SET(SPC5_FLASH_MCR_SIZE, value)
#define SPC5_FLASH_MCR_LAS_POS                           (19)
#define SPC5_FLASH_MCR_LAS_MASK (0x7 << SPC5_FLASH_MCR_LAS_POS)
#define SPC5_FLASH_MCR_LAS(value) BITFIELD_SET(SPC5_FLASH_MCR_LAS, value)
#define SPC5_FLASH_MCR_MAS                            BIT(16)
#define SPC5_FLASH_MCR_EER                            BIT(15)
#define SPC5_FLASH_MCR_RWE                            BIT(14)
#define SPC5_FLASH_MCR_PEAS                           BIT(11)
#define SPC5_FLASH_MCR_DONE                           BIT(10)
#define SPC5_FLASH_MCR_PEG                             BIT(9)
#define SPC5_FLASH_MCR_PGM                             BIT(4)
#define SPC5_FLASH_MCR_PSUS                            BIT(3)
#define SPC5_FLASH_MCR_ERS                             BIT(2)
#define SPC5_FLASH_MCR_ESUS                            BIT(1)
#define SPC5_FLASH_MCR_EHV                             BIT(0)

/* 30. SWT. */
struct spc5_swt_t {
    uint32_t CR;
    uint32_t IR;
    uint32_t TO;
    uint32_t WN;
    uint32_t SR;
    uint32_t CO;
};

#define SPC5_SWT_CR_MAP_0                            BIT(31)
#define SPC5_SWT_CR_MAP_1                            BIT(30)
#define SPC5_SWT_CR_MAP_2                            BIT(29)
#define SPC5_SWT_CR_MAP_3                            BIT(28)
#define SPC5_SWT_CR_MAP_4                            BIT(27)
#define SPC5_SWT_CR_MAP_5                            BIT(26)
#define SPC5_SWT_CR_MAP_6                            BIT(25)
#define SPC5_SWT_CR_MAP_7                            BIT(24)
#define SPC5_SWT_CR_KEY                               BIT(9)
#define SPC5_SWT_CR_RIA                               BIT(8)
#define SPC5_SWT_CR_WND                               BIT(7)
#define SPC5_SWT_CR_ITR                               BIT(6)
#define SPC5_SWT_CR_HLK                               BIT(5)
#define SPC5_SWT_CR_SLK                               BIT(4)
#define SPC5_SWT_CR_CSL                               BIT(3)
#define SPC5_SWT_CR_STP                               BIT(2)
#define SPC5_SWT_CR_FRZ                               BIT(1)
#define SPC5_SWT_CR_WEN                               BIT(0)

/* Base addresses of peripherals. */
#define SPC5_CFLASH    ((volatile struct spc5_flash_t *)   0xc3f88000ul)
#define SPC5_DFLASH    ((volatile struct spc5_flash_t *)   0xc3f8c000ul)
#define SPC5_SIUL      ((volatile struct spc5_siul_t *)    0xc3f90000ul)
#define SPC5_WKPU      ((volatile struct spc5__t *)        0xc3f94000ul)
#define SPC5_EMIOS_0   ((volatile struct spc5__t *)        0xc3fa0000ul)
#define SPC5_SSCM      ((volatile struct spc5__t *)        0xc3fd8000ul)
#define SPC5_MC_ME     ((volatile struct spc5_mc_me_t *)   0xc3fdc000ul)
#define SPC5_MC_CGM    ((volatile struct spc5_mc_cgm_t *)  0xc3fe0000ul)
#define SPC5_MC_RGM    ((volatile struct spc5_mc_rgm_t *)  0xc3fe4000ul)
#define SPC5_MC_PCU    ((volatile struct spc5__t *)        0xc3fe8000ul)
#define SPC5_RTC_API   ((volatile struct spc5__t *)        0xc3fec000ul)
#define SPC5_PIT       ((volatile struct spc5__t *)        0xc3ff0000ul)
#define SPC5_ADC_1     ((volatile struct spc5__t *)        0xffe04000ul)
#define SPC5_LINFLEX_0 ((volatile struct spc5_linflex_t *) 0xffe40000ul)
#define SPC5_LINFLEX_1 ((volatile struct spc5_linflex_t *) 0xffe44000ul)
#define SPC5_LINFLEX_2 ((volatile struct spc5_linflex_t *) 0xffe48000ul)
#define SPC5_CTU       ((volatile struct spc5__t *)        0xffe64000ul)
#define SPC5_SWT       ((volatile struct spc5_swt_t *)     0xfff38000ul)
#define SPC5_STM       ((volatile struct spc5_stm_t *)     0xfff3c000ul)
#define SPC5_ECSM      ((volatile struct spc5_ecsm_t *)    0xfff40000ul)
#define SPC5_EDMA      ((volatile struct spc5__t *)        0xfff44000ul)
#define SPC5_INTC      ((volatile struct spc5_intc_t *)    0xfff48000ul)
#define SPC5_DSPI_0    ((volatile struct spc5__t *)        0xfff90000ul)
#define SPC5_DSPI_1    ((volatile struct spc5__t *)        0xfff94000ul)
#define SPC5_FLEXCAN_0 ((volatile struct spc5_flexcan_t *) 0xfffc0000ul)
#define SPC5_DMA_MUX   ((volatile struct spc5__t *)        0xfffdc000ul)
#define SPC5_BAM       ((volatile struct spc5__t *)        0xffffc000ul)

/* Flash addresses and sizes. */
#define SPC5_CFLASH_ADDRESS                               0x0

#if defined(MCU_SPC56D30L1)
#    define SPC5_CFLASH_SIZE                          0x20000
#elif defined(MCU_SPC56D40L1)
#    define SPC5_CFLASH_SIZE                          0x40000
#elif defined(MCU_LINUX)
#    define SPC5_CFLASH_SIZE                          0x20000
#else
#    error "Unsupported MCU."
#endif

#define SPC5_DFLASH_ADDRESS                          0x800000
#define SPC5_DFLASH_SIZE                              0x10000

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

/**
 * Read the MSR register.
 */
#define mfmsr() ({                                              \
            uint32_t UNIQUE(msr);                               \
            asm volatile("mfmsr %0" : "=r" (UNIQUE(msr)) :      \
                         : "memory");                           \
            UNIQUE(msr);                                        \
        })

#endif
