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

#ifndef __STM32_H__
#define __STM32_H__

/* Uart. */
struct stm32_usart_t {
    uint32_t SR;
    uint32_t DR;
    uint32_t BRR;
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t GTPR;
};

/* Status register. */
#define STM32_USART_SR_PE             BIT(0)
#define STM32_USART_SR_FE             BIT(1)
#define STM32_USART_SR_NE             BIT(2)
#define STM32_USART_SR_ORE            BIT(3)
#define STM32_USART_SR_IDLE           BIT(4)
#define STM32_USART_SR_RXNE           BIT(5)
#define STM32_USART_SR_TC             BIT(6)
#define STM32_USART_SR_TXE            BIT(7)
#define STM32_USART_SR_LBD            BIT(8)
#define STM32_USART_SR_CTS            BIT(9)

/* Auxiliary Control Register 1 */
#define STM32_USART_CR1_RE            BIT(2)
#define STM32_USART_CR1_TE            BIT(3)
#define STM32_USART_CR1_RXNEIE        BIT(5)
#define STM32_USART_CR1_TCIE          BIT(6)
#define STM32_USART_CR1_TXEIE         BIT(7)
#define STM32_USART_CR1_PS            BIT(9)
#define STM32_USART_CR1_M             BIT(12)
#define STM32_USART_CR1_UE            BIT(13)

/* Auxiliary Control Register 2 */
#define STM32_USART_CR2_STOP          BIT(12)

struct stm32_rcc_t {
    uint32_t CR;
    uint32_t PLLCFGR;
    uint32_t CFGR;
    uint32_t CIR;
    uint32_t AHB1RSTR;
    uint32_t AHB2RSTR;
    uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    uint32_t APB1RSTR;
    uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    uint32_t AHB1ENR;
    uint32_t AHB2ENR;
    uint32_t AHB3ENR;
    uint32_t RESERVED2;
    uint32_t APB1ENR;
    uint32_t APB2ENR;
    uint32_t RESERVED3[2];
    uint32_t AHB1LPENR;
    uint32_t AHB2LPENR;
    uint32_t AHB3LPENR;
    uint32_t RESERVED4;
    uint32_t APB1LPENR;
    uint32_t APB2LPENR;
    uint32_t RESERVED5[2];
    uint32_t BDCR;
    uint32_t CSR;
    uint32_t RESERVED6[2];
    uint32_t SSCGR;
    uint32_t PLLI2SCFGR;
};

/* Clock control register. */
#define STM32_RCC_CR_HSEON            BIT(16)
#define STM32_RCC_CR_HSERDY           BIT(17)
#define STM32_RCC_CR_PLLON            BIT(24)
#define STM32_RCC_CR_PLLRDY           BIT(25)

/* Clock configuration register. */

#define STM32_RCC_CFGR_SW_POS         (0)
#define STM32_RCC_CFGR_SW_MASK        (0x3 << STM32_RCC_CFGR_SW_POS)
#define STM32_RCC_CFGR_SW(value)      BITFIELD_SET(STM32_RCC_CFGR_SW, value)
#define STM32_RCC_CFGR_SW_PLL         STM32_RCC_CFGR_SW(2)
#define STM32_RCC_CFGR_SWS_POS        (2)
#define STM32_RCC_CFGR_SWS_MASK       (0x3 << STM32_RCC_CFGR_SWS_POS)
#define STM32_RCC_CFGR_SWS(value)     BITFIELD_SET(STM32_RCC_CFGR_SWS, value)
#define STM32_RCC_CFGR_SWS_PLL        STM32_RCC_CFGR_SWS(2)
#define STM32_RCC_CFGR_PLLSRC_PREDIV1 BIT(16)
#define STM32_RCC_CFGR_PLLMUL_POS     (18)
#define STM32_RCC_CFGR_PLLMUL_MASK    (0xf << STM32_RCC_CFGR_PLLMUL_POS)
#define STM32_RCC_CFGR_PLLMUL(value)  BITFIELD_SET(STM32_RCC_CFGR_PLLMUL, value)
#define STM32_RCC_CFGR_PLLMUL_3       STM32_RCC_CFGR_PLLMUL(1)
#define STM32_RCC_CFGR_PLLMUL_6       STM32_RCC_CFGR_PLLMUL(4)

#define STM32_RCC_APB1ENR_PWREN       BIT(28)

struct stm32_pwr_t {
    uint32_t CR;
    uint32_t CSR;
};

struct stm32_gpio_t {
    uint32_t MODER;
    uint32_t OTYPER;
    uint32_t OSPEEDER;
    uint32_t PUPDR;
    uint32_t IDR;
    uint32_t ODR;
    uint32_t BSRR;
    uint32_t LCKR;
    uint32_t AFR[2];
};

struct stm32_iwdg_t {
    uint32_t KR;
    uint32_t PR;
    uint32_t RLR;
    uint32_t SR;
};

struct stm32_sdio_t {
    uint32_t POWER;
    uint32_t CLKCR;
    uint32_t ARG;
    uint32_t CMD;
    uint32_t RESPCMD;
    uint32_t RESP[4];
    uint32_t DTIMER;
    uint32_t DLEN;
    uint32_t DCTRL;
    uint32_t DCOUNT;
    uint32_t STA;
    uint32_t ICR;
    uint32_t MASK;
    uint32_t RESERVED0[2];
    uint32_t FIFOCNT;
    uint32_t RESERVED1[50];
    uint32_t FIFO;
};

/* Base addresses of peripherals. */
#define STM32_PWR            ((volatile struct stm32_pwr_t *)  0x40007000ul)
#define STM32_GPIOA          ((volatile struct stm32_gpio_t *) 0x40020000ul)
#define STM32_GPIOB          ((volatile struct stm32_gpio_t *) 0x40020400ul)
#define STM32_GPIOC          ((volatile struct stm32_gpio_t *) 0x40020800ul)
#define STM32_GPIOD          ((volatile struct stm32_gpio_t *) 0x40020c00ul)
#define STM32_USART1         ((volatile struct stm32_usart_t *)0x40011000ul)
#define STM32_SDIO           ((volatile struct stm32_sdio_t *) 0x40012c00ul)
#define STM32_IWDG           ((volatile struct stm32_iwdg_t *) 0x40003000ul)
#define STM32_RCC            ((volatile struct stm32_rcc_t *)  0x40023800ul)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

#endif
