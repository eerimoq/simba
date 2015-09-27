/**
 * @file sam3x8e.h
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

#ifndef __SAM3X8E_H__
#define __SAM3X8E_H__

/* See "SAM3X / SAM3A Series, Atmel | SMART ARM-based MCU, DATASHEET"
   for details. */

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
#define SCB_ACTLR_DISMCYCINT (0x1 <<  0)
#define SCB_ACTLR_DISDEFWBUF (0x1 <<  1)
#define SCB_ACTLR_DISFOLD    (0x1 <<  2)

/* CPUID Base Register */
#define SCB_CPUID_REVISION_MASK     (0xf <<  0)
#define SCB_CPUID_REVISION_GET(reg) (((reg) & SCB_CPUID_REVISION_MASK) >> 0)
#define SCB_CPUID_PARTNO_MASK       (0xfff <<  4)
#define SCB_CPUID_PARTNO_GET(reg)   (((reg) & SCB_CPUID_PARTNO_MASK) >> 4)
#define SCB_CPUID_CONSTANT_MASK     (0xf <<  16)
#define SCB_CPUID_CONSTANT_GET(reg) (((reg) & SCB_CPUID_CONSTANT_MASK) >> 16)
#define SCB_CPUID_VARIANT_MASK      (0xf <<  20)
#define SCB_CPUID_VARIANT_GET(reg)  (((reg) & SCB_CPUID_VARIANT_MASK) >> 20)

/* Interrupt Control and State Register */
#define SCB_ICSR_VECTACTIVE_MASK  (0x1ff <<  0)
#define SCB_ICSR_RETTOBASE        (0x1 << 11)
#define SCB_ICSR_VECTPENDING_MASK (0x3ff << 12)
#define SCB_ICSR_ISRPENDING       (0x1 << 22)
#define SCB_ICSR_PENDSTCLR        (0x1 << 25)
#define SCB_ICSR_PENDSTSET        (0x1 << 26)
#define SCB_ICSR_PENDSVCLR        (0x1 << 27)
#define SCB_ICSR_PENDSVSET        (0x1 << 28)

/* Vector Table Offset Register */
#define SCB_VTOR_TBLOFF_MASK       (0x7fffff << 7)
#define SCB_VTOR_TBLOFF_SET(value) (((value) << 7) & SCB_VTOR_TBLOFF_MASK)

/* Application Interrupt and Reset Control Register */
#define SCB_AIRCR_VECTRESET     (0x1 <<  0)
#define SCB_AIRCR_VECTCLRACTIVE (0x1 <<  1)
#define SCB_AIRCR_SYSRESETREQ   (0x1 <<  2)
#define SCB_AIRCR_PRIGROUP_MASK (0x7 <<  8)
#define SCB_AIRCR_ENDIANESS     (0x1 << 15)
#define SCB_AIRCR_VECTKEY_MASK  (0xffff << 16)

/* System Control Register */
#define SCB_SCR_SLEEONEXIT (0x1 <<  1)
#define SCB_SCR_SLEEPDEEP  (0x1 <<  2)
#define SCB_SCR_SEVONPEND  (0x1 <<  4)

/* Configuration and Control Register */
#define SCB_CCR_NONEBASETHRDENA (0x1 <<  0)
#define SCB_CCR_USERSETMPEND    (0x1 <<  1)
#define SCB_CCR_UNALIGN_TRP     (0x1 <<  3)
#define SCB_CCR_DIV_0_TRP       (0x1 <<  4)
#define SCB_CCR_BFHFNMIGN       (0x1 <<  8)
#define SCB_CCR_STKALIGN        (0x1 <<  9)

/* System Handler Priority Register 1 */
#define SCB_SHPR1_PRI_4_MASK (0xff <<  0)
#define SCB_SHPR1_PRI_5_MASK (0xff <<  8)
#define SCB_SHPR1_PRI_6_MASK (0xff << 16)

/* System Handler Priority Register 2 */
#define SCB_SHPR2_PRI_11_MASK (0xff << 24)

/* System Handler Priority Register 3 */
#define SCB_SHPR3_PRI_14_MASK (0xff << 16)
#define SCB_SHPR3_PRI_15_MASK (0xff << 24)

/* System Handler Control and State Register */
#define SCB_SHCSR_MEMFAULTACT    (0x1 <<  0)
#define SCB_SHCSR_BUSFAULTACT    (0x1 <<  1)
#define SCB_SHCSR_USGFAULTACT    (0x1 <<  3)
#define SCB_SHCSR_SVCALLAVCT     (0x1 <<  7)
#define SCB_SHCSR_MONITORACT     (0x1 <<  8)
#define SCB_SHCSR_PENDSVACT      (0x1 << 10)
#define SCB_SHCSR_SYSTICKACT     (0x1 << 11)
#define SCB_SHCSR_USGFAULTPENDED (0x1 << 12)
#define SCB_SHCSR_MEMFAULTPENDED (0x1 << 13)
#define SCB_SHCSR_BUSFAULTPENDED (0x1 << 14)
#define SCB_SHCSR_SVCALLPENDED   (0x1 << 15)
#define SCB_SHCSR_MEMFAULTENA    (0x1 << 16)
#define SCB_SHCSR_BUSFAULTENA    (0x1 << 17)
#define SCB_SHCSR_USGFAULTENA    (0x1 << 18)

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
#define SYSTEM_TIMER_CTRL_ENABLE    (0x1 <<  0)
#define SYSTEM_TIMER_CTRL_TICKINT   (0x1 <<  1)
#define SYSTEM_TIMER_CTRL_CLKSOURCE (0x1 <<  2)
#define SYSTEM_TIMER_CTRL_COUNTFLAG (0x1 << 16)

/* SysTick Reload Value Register */
#define SYSTEM_TIMER_LOAD_RELOAD_MASK   (0xffffff << 0)
#define SYSTEM_TIMER_LOAD_RELOAD(value) (((value) << 0) & 0xffffff)

/* SysTick Current Value Register */
#define SYSTEM_TIMER_VAL_CURRENT_MASK   (0xffffff <<  0)

/* SysTick Calibration Value Register */
#define SYSTEM_TIMER_CALIB_TENMS_MASK   (0xffffff <<  0)
#define SYSTEM_TIMER_CALIB_SKEW         (0x1 << 30)
#define SYSTEM_TIMER_CALIB_NOREF        (0x1 << 31)

/* High Speed MultiMedia Card Interface (HSMCI). */
struct sam_hsmci_t {
    uint32_t HSMCI_CR;
    uint32_t HSMCI_MR;
    uint32_t HSMCI_DTOR;
    uint32_t HSMCI_SDCR;
    uint32_t HSMCI_ARGR;
    uint32_t HSMCI_CMDR;
    uint32_t HSMCI_BLKR;
    uint32_t HSMCI_CSTOR;
    uint32_t HSMCI_RSPR[4];
    uint32_t HSMCI_RDR;
    uint32_t HSMCI_TDR;
    uint32_t reserved1[2];
    uint32_t HSMCI_SR;
    uint32_t HSMCI_IER;
    uint32_t HSMCI_IDR;
    uint32_t HSMCI_IMR;
    uint32_t HSMCI_DMA;
    uint32_t HSMCI_CFG;
    uint32_t reserved2[35];
    uint32_t HSMCI_WPMR;
    uint32_t HSMCI_WPSR;
    uint32_t reserved3[69];
    uint32_t HSMCI_FIFO[256];
};

struct sam_pdc_t {
  uint32_t PERIPH_RPR;
  uint32_t PERIPH_RCR;
  uint32_t PERIPH_TPR;
  uint32_t PERIPH_TCR;
  uint32_t PERIPH_RNPR;
  uint32_t PERIPH_RNCR;
  uint32_t PERIPH_TNPR;
  uint32_t PERIPH_TNCR;
  uint32_t PERIPH_PTCR;
  uint32_t PERIPH_PTSR;
};

/* Receive Pointer Register */
#define PERIPH_RPR_RXPTR_MASK           (0xffffffff << 0)
#define PERIPH_RPR_RXPTR(value)         (((value) << 0) & PERIPH_RPR_RXPTR_MASK)

/* Receive Counter Register */
#define PERIPH_RCR_RXCTR_MASK           (0xffff << 0)
#define PERIPH_RCR_RXCTR(value)         (((value) << 0) & PERIPH_RCR_RXCTR_MASK)

/* Transmit Pointer Register */
#define PERIPH_TPR_TXPTR_MASK           (0xffffffff << 0)
#define PERIPH_TPR_TXPTR(value)         (((value) << 0) & PERIPH_TPR_TXPTR_MASK)

/* Transmit Counter Register */
#define PERIPH_TCR_TXCTR_MASK           (0xffff << 0)
#define PERIPH_TCR_TXCTR(value)         (((value) << 0) & PERIPH_TCR_TXCTR_MASK)

/* Receive Next Pointer Register */
#define PERIPH_RNPR_RXNPTR_MASK         (0xffffffff << 0)
#define PERIPH_RNPR_RXNPTR(value)       (((value) << 0) & PERIPH_RNPR_RXNPTR_MASK)

/* Receive Next Counter Register */
#define PERIPH_RNCR_RXNCTR_MASK         (0xffff << 0)
#define PERIPH_RNCR_RXNCTR(value)       (((value) << 0) & PERIPH_RNCR_RXNCTR_MASK)

/* Transmit Next Pointer Register */
#define PERIPH_TNPR_TXNPTR_MASK         (0xffffffff << 0)
#define PERIPH_TNPR_TXNPTR(value)       (((value) << 0) & PERIPH_TNPR_TXNPTR_MASK)

/* Transmit Next Counter Register */
#define PERIPH_TNCR_TXNCTR_MASK         (0xffff << 0)
#define PERIPH_TNCR_TXNCTR(value)       (((value) << 0) & PERIPH_TNCR_TXNCTR_MASK)

/* Transfer Control Register */
#define PERIPH_PTCR_RXTEN               (0x1 << 0)
#define PERIPH_PTCR_RXTDIS              (0x1 << 1)
#define PERIPH_PTCR_TXTEN               (0x1 << 8)
#define PERIPH_PTCR_TXTDIS              (0x1 << 9)

/* Transfer Status Register */
#define PERIPH_PTSR_RXTEN               (0x1 << 0)
#define PERIPH_PTSR_TXTEN               (0x1 << 8)

struct sam_usart_t {
    uint32_t US_CR;
    uint32_t US_MR;
    uint32_t US_IER;
    uint32_t US_IDR;
    uint32_t US_IMR;
    uint32_t US_CSR;
    uint32_t US_RHR;
    uint32_t US_THR;
    uint32_t US_BRGR;
    uint32_t US_RTOR;
    uint32_t US_TTGR;
    uint32_t reserved1[5];
    uint32_t US_FIDI;
    uint32_t US_NER;
    uint32_t reserved2[1];
    uint32_t US_IF;
    uint32_t US_MAN;
    uint32_t US_LINMR;
    uint32_t US_LINIR;
    uint32_t reserved3[34];
    uint32_t US_WPMR;
    uint32_t US_WPSR;
    uint32_t reserved4[5];
    struct sam_pdc_t US_PDC;
};

/* USART Control Register */
#define US_CR_RSTRX                     (0x1 <<  2)
#define US_CR_RSTTX                     (0x1 <<  3)
#define US_CR_RXEN                      (0x1 <<  4)
#define US_CR_RXDIS                     (0x1 <<  5)
#define US_CR_TXEN                      (0x1 <<  6)
#define US_CR_TXDIS                     (0x1 <<  7)
#define US_CR_RSTSTA                    (0x1 <<  8)
#define US_CR_STTBRK                    (0x1 <<  9)
#define US_CR_STPBRK                    (0x1 << 10)
#define US_CR_STTTO                     (0x1 << 11)
#define US_CR_SENDA                     (0x1 << 12)
#define US_CR_RSTIT                     (0x1 << 13)
#define US_CR_RSTNACK                   (0x1 << 14)
#define US_CR_RETTO                     (0x1 << 15)
#define US_CR_RTSEN_FCS                 (0x1 << 18)
#define US_CR_RTSDIS_RCS                (0x1 << 19)
#define US_CR_LINABT                    (0x1 << 20)
#define US_CR_LINWKUP                   (0x1 << 21)

/* USART Mode Register */
#define US_MR_USART_MODE_MASK           (0xf << 0)
#define US_MR_USART_MODE_NORMAL         (0x0 << 0)
#define US_MR_USART_MODE_RS485          (0x1 << 0)
#define US_MR_USART_MODE_HW_HANDSHAKING (0x2 << 0)
#define US_MR_USART_MODE_IS07816_T_0    (0x4 << 0)
#define US_MR_USART_MODE_IS07816_T_1    (0x6 << 0)
#define US_MR_USART_MODE_IRDA           (0x8 << 0)
#define US_MR_USART_MODE_LIN_MASTER     (0xa << 0)
#define US_MR_USART_MODE_LIN_SLAVE      (0xb << 0)
#define US_MR_USART_MODE_SPI_MASTER     (0xe << 0)
#define US_MR_USART_MODE_SPI_SLAVE      (0xf << 0)
#define US_MR_USCLKS_MASK               (0x3 << 4)
#define US_MR_USCLKS_MCK                (0x0 << 4)
#define US_MR_USCLKS_DIV                (0x1 << 4)
#define US_MR_USCLKS_SCK                (0x3 << 4)
#define US_MR_CHRL_MASK                 (0x3 << 6)
#define US_MR_CHRL_5_BIT                (0x0 << 6)
#define US_MR_CHRL_6_BIT                (0x1 << 6)
#define US_MR_CHRL_7_BIT                (0x2 << 6)
#define US_MR_CHRL_8_BIT                (0x3 << 6)
#define US_MR_SYNC_CPHA                 (0x1 << 8)
#define US_MR_PAR_MASK                  (0x7 << 9)
#define US_MR_PAR_EVEN                  (0x0 << 9)
#define US_MR_PAR_ODD                   (0x1 << 9)
#define US_MR_PAR_SPACE                 (0x2 << 9)
#define US_MR_PAR_MARK                  (0x3 << 9)
#define US_MR_PAR_NO                    (0x4 << 9)
#define US_MR_PAR_MULTIDROP             (0x6 << 9)
#define US_MR_NBSTOP_MASK               (0x3 << 12)
#define US_MR_NBSTOP_1_BIT              (0x0 << 12)
#define US_MR_NBSTOP_1_5_BIT            (0x1 << 12)
#define US_MR_NBSTOP_2_BIT              (0x2 << 12)
#define US_MR_CHMODE_MASK               (0x3 << 14)
#define US_MR_CHMODE_NORMAL             (0x0 << 14)
#define US_MR_CHMODE_AUTOMATIC          (0x1 << 14)
#define US_MR_CHMODE_LOCAL_LOOPBACK     (0x2 << 14)
#define US_MR_CHMODE_REMOTE_LOOPBACK    (0x3 << 14)
#define US_MR_MSBF                      (0x1 << 16)
#define US_MR_CPOL                      (0x1 << 16)
#define US_MR_MODE9                     (0x1 << 17)
#define US_MR_CLKO                      (0x1 << 18)
#define US_MR_OVER                      (0x1 << 19)
#define US_MR_INACK                     (0x1 << 20)
#define US_MR_DSNACK                    (0x1 << 21)
#define US_MR_VAR_SYNC                  (0x1 << 22)
#define US_MR_INVDATA                   (0x1 << 23)
#define US_MR_MAX_ITERATION_MASK        (0x7 << 24)
#define US_MR_MAX_ITERATION_SET(value)  (((value) << 24) & US_MR_MAX_ITERATION_MASK)
#define US_MR_MAX_ITERATION_GET(reg)    (((reg) & US_MR_MAX_ITERATION_MASK) >> 24)
#define US_MR_FILTER                    (0x1 << 28)
#define US_MR_MAN                       (0x1 << 29)
#define US_MR_MODSYNC                   (0x1 << 30)
#define US_MR_ONEBIT                    (0x1 << 31)

/* USART Interrupt Enable Register */
#define US_IER_RXRDY                    (0x1 << 0)
#define US_IER_TXRDY                    (0x1 << 1)
#define US_IER_RXBRK                    (0x1 << 2)
#define US_IER_ENDRX                    (0x1 << 3)
#define US_IER_ENDTX                    (0x1 << 4)
#define US_IER_OVRE                     (0x1 << 5)
#define US_IER_FRAME                    (0x1 << 6)
#define US_IER_PARE                     (0x1 << 7)
#define US_IER_TIMEOUT                  (0x1 << 8)
#define US_IER_TXEMPTY                  (0x1 << 9)
#define US_IER_ITER_UNRE                (0x1 << 10)
#define US_IER_TXBUFE                   (0x1 << 11)
#define US_IER_RXBUFF                   (0x1 << 12)
#define US_IER_NACK_LINBK               (0x1 << 13)
#define US_IER_LINID                    (0x1 << 14)
#define US_IER_LINTC                    (0x1 << 15)
#define US_IER_CTSIC                    (0x1 << 19)
#define US_IER_MANE                     (0x1 << 24)
#define US_IER_LINBE                    (0x1 << 25)
#define US_IER_LINISFE                  (0x1 << 26)
#define US_IER_LINIPE                   (0x1 << 27)
#define US_IER_LINCE                    (0x1 << 28)
#define US_IER_LINSNRE                  (0x1 << 29)

/* USART Interrupt Disable Register */
#define US_IDR_RXRDY                    (0x1 << 0)
#define US_IDR_TXRDY                    (0x1 << 1)
#define US_IDR_RXBRK                    (0x1 << 2)
#define US_IDR_ENDRX                    (0x1 << 3)
#define US_IDR_ENDTX                    (0x1 << 4)
#define US_IDR_OVRE                     (0x1 << 5)
#define US_IDR_FRAME                    (0x1 << 6)
#define US_IDR_PARE                     (0x1 << 7)
#define US_IDR_TIMEOUT                  (0x1 << 8)
#define US_IDR_TXEMPTY                  (0x1 << 9)
#define US_IDR_ITER_UNRE                (0x1 << 10)
#define US_IDR_TXBUFE                   (0x1 << 11)
#define US_IDR_RXBUFF                   (0x1 << 12)
#define US_IDR_NACK_LINBK               (0x1 << 13)
#define US_IDR_LINID                    (0x1 << 14)
#define US_IDR_LINTC                    (0x1 << 15)
#define US_IDR_CTSIC                    (0x1 << 19)
#define US_IDR_MANE                     (0x1 << 24)
#define US_IDR_LINBE                    (0x1 << 25)
#define US_IDR_LINISFE                  (0x1 << 26)
#define US_IDR_LINIPE                   (0x1 << 27)
#define US_IDR_LINCE                    (0x1 << 28)
#define US_IDR_LINSNRE                  (0x1 << 29)

/* USART Interrupt Mask Register */
#define US_IMR_RXRDY                    (0x1 << 0)
#define US_IMR_TXRDY                    (0x1 << 1)
#define US_IMR_RXBRK                    (0x1 << 2)
#define US_IMR_ENDRX                    (0x1 << 3)
#define US_IMR_ENDTX                    (0x1 << 4)
#define US_IMR_OVRE                     (0x1 << 5)
#define US_IMR_FRAME                    (0x1 << 6)
#define US_IMR_PARE                     (0x1 << 7)
#define US_IMR_TIMEOUT                  (0x1 << 8)
#define US_IMR_TXEMPTY                  (0x1 << 9)
#define US_IMR_ITER_UNRE                (0x1 << 10)
#define US_IMR_TXBUFE                   (0x1 << 11)
#define US_IMR_RXBUFF                   (0x1 << 12)
#define US_IMR_NACK_LINBK               (0x1 << 13)
#define US_IMR_LINID                    (0x1 << 14)
#define US_IMR_LINTC                    (0x1 << 15)
#define US_IMR_CTSIC                    (0x1 << 19)
#define US_IMR_MANE                     (0x1 << 24)
#define US_IMR_LINBE                    (0x1 << 25)
#define US_IMR_LINISFE                  (0x1 << 26)
#define US_IMR_LINIPE                   (0x1 << 27)
#define US_IMR_LINCE                    (0x1 << 28)
#define US_IMR_LINSNRE                  (0x1 << 29)

/* USART Channel Status Register */
#define US_CSR_RXRDY                    (0x1 << 0)
#define US_CSR_TXRDY                    (0x1 << 1)
#define US_CSR_RXBRK                    (0x1 << 2)
#define US_CSR_ENDRX                    (0x1 << 3)
#define US_CSR_ENDTX                    (0x1 << 4)
#define US_CSR_OVRE                     (0x1 << 5)
#define US_CSR_FRAME                    (0x1 << 6)
#define US_CSR_PARE                     (0x1 << 7)
#define US_CSR_TIMEOUT                  (0x1 << 8)
#define US_CSR_TXEMPTY                  (0x1 << 9)
#define US_CSR_ITER_UNRE                (0x1 << 10)
#define US_CSR_TXBUFE                   (0x1 << 11)
#define US_CSR_RXBUFF                   (0x1 << 12)
#define US_CSR_NACK_LINBK               (0x1 << 13)
#define US_CSR_LINID                    (0x1 << 14)
#define US_CSR_LINTC                    (0x1 << 15)
#define US_CSR_CTSIC                    (0x1 << 19)
#define US_CSR_CTS_LINBLS               (0x1 << 23)
#define US_CSR_MANERR                   (0x1 << 24)
#define US_CSR_LINBE                    (0x1 << 25)
#define US_CSR_LINISFE                  (0x1 << 26)
#define US_CSR_LINIPE                   (0x1 << 27)
#define US_CSR_LINCE                    (0x1 << 28)
#define US_CSR_LINSNRE                  (0x1 << 29)

/* USART Receive Holding Register */
#define US_RHR_RXCHR_MASK               (0x1ff << 0)
#define US_RHR_RXSYNH                   (0x1 << 15)

/* USART Transmit Holding Register */
#define US_THR_TXCHR_MASK               (0x1ff << 0)
#define US_THR_TXCHR(value)             (((value) << 0) & US_THR_TXCHR_MASK)
#define US_THR_TXSYNH                   (0x1 << 15)

/* USART Baud Rate Generator Register */
#define US_BRGR_CD_MASK                 (0xffff << 0)
#define US_BRGR_CD(value)               (((value) << 0) & US_BRGR_CD_MASK)
#define US_BRGR_FP_MASK                 (0x7 << 16)
#define US_BRGR_FP(value)               (((value) << 16) & US_BRGR_FP_MASK)

/* USART Receiver Time-out Register */
#define US_RTOR_TO_MASK                 (0x1ffff << 0)
#define US_RTOR_TO(value)               (((value) << 0) & US_RTOR_TO_MASK)

/* USART Transmitter Timeguard Register */
#define US_TTGR_TG_MASK                 (0xff << 0)
#define US_TTGR_TG(value)               (((value) << 0) & US_TTGR_TG_MASK)

/* USART FI DI RATIO Register */
#define US_FIDI_FI_DI_RATIO_MASK         (0x7ff << 0)
#define US_FIDI_FI_DI_RATIO(value)       (((value) << 0) & US_FIDI_FI_DI_RATIO_MASK)

/* USART Number of Errors Register */
#define US_NER_NB_ERRORS_MASK            (0xff << 0)

/* USART IrDA FILTER Register */
#define US_IF_IRDA_FILTER_MASK           (0xff << 0)
#define US_IF_IRDA_FILTER(value)         (((value) << 0) & US_IF_IRDA_FILTER_MASK)

/* USART Manchester Configuration Register */
#define US_MAN_TX_PL_MASK               (0xf << 0)
#define US_MAN_TX_PL(value)             (((value) << 0) & US_MAN_TX_PL_MASK)
#define US_MAN_TX_PP_MASK               (0x3 << 8)
#define US_MAN_TX_PP_ALL_ONE            (0x0 << 8)
#define US_MAN_TX_PP_ALL_ZERO           (0x1 << 8)
#define US_MAN_TX_PP_ZERO_ONE           (0x2 << 8)
#define US_MAN_TX_PP_ONE_ZERO           (0x3 << 8)
#define US_MAN_TX_MPOL                  (0x1 << 12)
#define US_MAN_RX_PL_MASK               (0xf << 16)
#define US_MAN_RX_PL(value)             (((value) << 16) & US_MAN_RX_PL_MASK)
#define US_MAN_RX_PP_MASK               (0x3 << 24)
#define US_MAN_RX_PP_ALL_ONE            (0x0 << 24)
#define US_MAN_RX_PP_ALL_ZERO           (0x1 << 24)
#define US_MAN_RX_PP_ZERO_ONE           (0x2 << 24)
#define US_MAN_RX_PP_ONE_ZERO           (0x3 << 24)
#define US_MAN_RX_MPOL                  (0x1 << 28)
#define US_MAN_STUCKTO1                 (0x1 << 29)
#define US_MAN_DRIFT                    (0x1 << 30)

/* USART LIN Mode Register */
#define US_LINMR_NACT_MASK              (0x3 << 0)
#define US_LINMR_NACT_PUBLISH           (0x0 << 0)
#define US_LINMR_NACT_SUBSCRIBE         (0x1 << 0)
#define US_LINMR_NACT_IGNORE            (0x2 << 0)
#define US_LINMR_PARDIS                 (0x1 << 2)
#define US_LINMR_CHKDIS                 (0x1 << 3)
#define US_LINMR_CHKTYP                 (0x1 << 4)
#define US_LINMR_DLM                    (0x1 << 5)
#define US_LINMR_FSDIS                  (0x1 << 6)
#define US_LINMR_WKUPTYP                (0x1 << 7)
#define US_LINMR_DLC_MASK               (0xff << 8)
#define US_LINMR_DLC(value)             (((value) << 8) & US_LINMR_DLC_MASK)
#define US_LINMR_PDCM                   (0x1 << 16)

/* USART LIN Identifier Register */
#define US_LINIR_IDCHR_MASK             (0xff << 0)
#define US_LINIR_IDCHR(value)           (((value) << 0) & US_LINIR_IDCHR_MASK)

/* USART Write Protect Mode Register */
#define US_WPMR_WPEN                    (0x1 << 0)
#define US_WPMR_WPKEY_MASK              (0xffffff << 8)
#define US_WPMR_WPKEY(value)            (((value) << 8) & US_WPMR_WPKEY_MASK)

/* USART Write Protect Status Register */
#define US_WPSR_WPVS                    (0x1 << 0)
#define US_WPSR_WPVSRC_MASK             (0xffff << 8)

/* Base addresses of private peripherals. */
#define SAM_SCB        ((struct sam_system_control_block_t *)0xe000e008u)
#define SAM_ST         ((struct sam_system_timer_t         *)0xe000e010u)

/* Base addresses of peripherals. */
#define SAM_HSMCI      ((struct sam_hsmci_t  *)0x40000000u)
#define SAM_SSC        ((struct sam_ssc_t    *)0x40004000u)
#define SAM_SPI0       ((struct sam_spi_t    *)0x40008000u)
#define SAM_TC0        ((struct sam_tc_t     *)0x40080000u)
#define SAM_TC1        ((struct sam_tc_t     *)0x40084000u)
#define SAM_TC2        ((struct sam_tc_t     *)0x40088000u)
#define SAM_TWI0       ((struct sam_twi_t    *)0x4008c000u)
#define SAM_PDC_TWI0   ((struct sam_pdc_t    *)0x4008c100u)
#define SAM_TWI1       ((struct sam_twi_t    *)0x40090000u)
#define SAM_PDC_TWI1   ((struct sam_pdc_t    *)0x40090100u)
#define SAM_PWM        ((struct sam_pwm_t    *)0x40094000u)
#define SAM_PDC_PWM    ((struct sam_pdc_t    *)0x40094100u)
#define SAM_USART0     ((struct sam_usart_t  *)0x40098000u)
#define SAM_USART1     ((struct sam_usart_t  *)0x4009c000u)
#define SAM_USART2     ((struct sam_usart_t  *)0x400a0000u)
#define SAM_USART3     ((struct sam_usart_t  *)0x400a4000u)
#define SAM_UOTGHS     ((struct sam_uotghs_t *)0x400ac000u)
#define SAM_EMAC       ((struct sam_emac_t   *)0x400b0000u)
#define SAM_CAN0       ((struct sam_can_t    *)0x400b4000u)
#define SAM_CAN1       ((struct sam_can_t    *)0x400b8000u)
#define SAM_TRNG       ((struct sam_trng_t   *)0x400bc000u)
#define SAM_ADC        ((struct sam_adc_t    *)0x400c0000u)
#define SAM_PDC_ADC    ((struct sam_pdc_t    *)0x400c0100u)
#define SAM_DMAC       ((struct sam_dmac_t   *)0x400c4000u)
#define SAM_DACC       ((struct sam_dacc_t   *)0x400c8000u)
#define SAM_PDC_DACC   ((struct sam_pdc_t    *)0x400c8100u)
#define SAM_SMC        ((struct sam_smc_t    *)0x400e0000u)
#define SAM_MATRIX     ((struct sam_matrix_t *)0x400e0400u)
#define SAM_PMC        ((struct sam_pmc_t    *)0x400e0600u)
#define SAM_UART       ((struct sam_uart_t   *)0x400e0800u)
#define SAM_PDC_UART   ((struct sam_pdc_t    *)0x400e0900u)
#define SAM_CHIPID     ((struct sam_chipid_t *)0x400e0940u)
#define SAM_EFC0       ((struct sam_efc_t    *)0x400e0a00u)
#define SAM_EFC1       ((struct sam_efc_t    *)0x400e0c00u)
#define SAM_PIOA       ((struct sam_pio_t    *)0x400e0e00u)
#define SAM_PIOB       ((struct sam_pio_t    *)0x400e1000u)
#define SAM_PIOC       ((struct sam_pio_t    *)0x400e1200u)
#define SAM_PIOD       ((struct sam_pio_t    *)0x400e1400u)
#define SAM_RSTC       ((struct sam_rstc_t   *)0x400e1a00u)
#define SAM_SUPC       ((struct sam_supc_t   *)0x400e1a10u)
#define SAM_RTT        ((struct sam_rtt_t    *)0x400e1a30u)
#define SAM_WDT        ((struct sam_wdt_t    *)0x400e1a50u)
#define SAM_RTC        ((struct sam_rtc_t    *)0x400e1a60u)
#define SAM_GPBR       ((struct sam_gpbr_t   *)0x400e1a90u)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

#endif
