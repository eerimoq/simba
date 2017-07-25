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

#ifndef __NRF52840_H__
#define __NRF52840_H__

/* 19. GPIO. */
struct nrf5_gpio_t {
    uint32_t RESERVED0[321];
    uint32_t OUT;
    uint32_t OUTSET;
    uint32_t OUTCLR;
    uint32_t IN;
    uint32_t DIR;
    uint32_t DIRSET;
    uint32_t DIRCLR;
    uint32_t LATCH;
    uint32_t DETECTMODE;
    uint32_t RESERVED1[118];
    uint32_t PIN_CNF[32];
};

#define NRF5_GPIO_PIN_CNF_DIR_OUTPUT                   BIT(0)

/* 49. UART. */
struct nrf5_uart_t {
    struct {
        uint32_t STARTRX;
        uint32_t STOPRX;
        uint32_t STARTTX;
        uint32_t STOPTX;
        uint32_t RESERVED0[3];
        uint32_t SUSPEND;
    } TASKS;
    uint32_t RESERVED0[56];
    struct {
        uint32_t CTS;
        uint32_t NCTS;
        uint32_t RXDRDY;
        uint32_t RESERVED0[4];
        uint32_t TXDRDY;
        uint32_t RESERVED1;
        uint32_t ERROR;
        uint32_t RESERVED2[7];
        uint32_t RXTO;
    } EVENTS;
    uint32_t RESERVED1[46];
    uint32_t SHORTS;
    uint32_t RESERVED2[64];
    uint32_t INTENSET;
    uint32_t INTENCLR;
    uint32_t RESERVED3[93];
    uint32_t ERRORSRC;
    uint32_t RESERVED4[31];
    uint32_t ENABLE;
    uint32_t RESERVED5;
    struct {
        uint32_t RTS;
        uint32_t TXD;
        uint32_t CTS;
        uint32_t RXD;
    } PSEL;
    uint32_t RXD;
    uint32_t TXD;
    uint32_t RESERVED6;
    uint32_t BAUDRATE;
    uint32_t RESERVED7[17];
    uint32_t CONFIG;
};

/* . */
#define NRF5_UART_            BIT(0)

/* Base addresses of peripherals. */
#define NRF5_CLOCK           ((volatile struct nrf5__t *)0x40000000u)
#define NRF5_POWER           ((volatile struct nrf5__t *)0x40000000u)
#define NRF5_RADIO           ((volatile struct nrf5__t *)0x40001000u)
#define NRF5_UART0       ((volatile struct nrf5_uart_t *)0x40002000u)
#define NRF5_UARTE0          ((volatile struct nrf5__t *)0x40002000u)
#define NRF5_TWIM0           ((volatile struct nrf5__t *)0x40003000u)
#define NRF5_TWIS0           ((volatile struct nrf5__t *)0x40003000u)
#define NRF5_SPIS0           ((volatile struct nrf5__t *)0x40003000u)
#define NRF5_SPI0            ((volatile struct nrf5__t *)0x40003000u)
#define NRF5_SPIM0           ((volatile struct nrf5__t *)0x40003000u)
#define NRF5_TWI0            ((volatile struct nrf5__t *)0x40003000u)
#define NRF5_TWI1            ((volatile struct nrf5__t *)0x40004000u)
#define NRF5_SPIM1           ((volatile struct nrf5__t *)0x40004000u)
#define NRF5_SPI1            ((volatile struct nrf5__t *)0x40004000u)
#define NRF5_TWIM1           ((volatile struct nrf5__t *)0x40004000u)
#define NRF5_TWIS1           ((volatile struct nrf5__t *)0x40004000u)
#define NRF5_SPIS1           ((volatile struct nrf5__t *)0x40004000u)
#define NRF5_NFCT            ((volatile struct nrf5__t *)0x40005000u)
#define NRF5_GPIOTE          ((volatile struct nrf5__t *)0x40006000u)
#define NRF5_SAADC           ((volatile struct nrf5__t *)0x40007000u)
#define NRF5_TIMER0          ((volatile struct nrf5__t *)0x40008000u)
#define NRF5_TIMER1          ((volatile struct nrf5__t *)0x40009000u)
#define NRF5_TIMER2          ((volatile struct nrf5__t *)0x4000a000u)
#define NRF5_RTC0            ((volatile struct nrf5__t *)0x4000b000u)
#define NRF5_TEMP            ((volatile struct nrf5__t *)0x4000c000u)
#define NRF5_RNG             ((volatile struct nrf5__t *)0x4000d000u)
#define NRF5_ECB             ((volatile struct nrf5__t *)0x4000e000u)
#define NRF5_CCM             ((volatile struct nrf5__t *)0x4000f000u)
#define NRF5_AAR             ((volatile struct nrf5__t *)0x4000f000u)
#define NRF5_WDT             ((volatile struct nrf5__t *)0x40010000u)
#define NRF5_RTC1            ((volatile struct nrf5__t *)0x40011000u)
#define NRF5_QDEC            ((volatile struct nrf5__t *)0x40012000u)
#define NRF5_COMP            ((volatile struct nrf5__t *)0x40013000u)
#define NRF5_LPCOMP          ((volatile struct nrf5__t *)0x40013000u)
#define NRF5_SWI0            ((volatile struct nrf5__t *)0x40014000u)
#define NRF5_EGU0            ((volatile struct nrf5__t *)0x40014000u)
#define NRF5_EGU1            ((volatile struct nrf5__t *)0x40015000u)
#define NRF5_SWI1            ((volatile struct nrf5__t *)0x40015000u)
#define NRF5_SWI2            ((volatile struct nrf5__t *)0x40016000u)
#define NRF5_EGU2            ((volatile struct nrf5__t *)0x40016000u)
#define NRF5_EGU3            ((volatile struct nrf5__t *)0x40017000u)
#define NRF5_SWI3            ((volatile struct nrf5__t *)0x40017000u)
#define NRF5_EGU4            ((volatile struct nrf5__t *)0x40018000u)
#define NRF5_SWI4            ((volatile struct nrf5__t *)0x40018000u)
#define NRF5_EGU5            ((volatile struct nrf5__t *)0x40019000u)
#define NRF5_SWI5            ((volatile struct nrf5__t *)0x40019000u)
#define NRF5_TIMER3          ((volatile struct nrf5__t *)0x4001a000u)
#define NRF5_TIMER4          ((volatile struct nrf5__t *)0x4001b000u)
#define NRF5_PWM0            ((volatile struct nrf5__t *)0x4001c000u)
#define NRF5_PDM             ((volatile struct nrf5__t *)0x4001d000u)
#define NRF5_ACL             ((volatile struct nrf5__t *)0x4001e000u)
#define NRF5_NVMC            ((volatile struct nrf5__t *)0x4001e000u)
#define NRF5_PPI             ((volatile struct nrf5__t *)0x4001f000u)
#define NRF5_MWU             ((volatile struct nrf5__t *)0x40020000u)
#define NRF5_PWM1            ((volatile struct nrf5__t *)0x40021000u)
#define NRF5_PWM2            ((volatile struct nrf5__t *)0x40022000u)
#define NRF5_SPIS2           ((volatile struct nrf5__t *)0x40023000u)
#define NRF5_SPIM2           ((volatile struct nrf5__t *)0x40023000u)
#define NRF5_SPI2            ((volatile struct nrf5__t *)0x40023000u)
#define NRF5_RTC2            ((volatile struct nrf5__t *)0x40024000u)
#define NRF5_I2S             ((volatile struct nrf5__t *)0x40025000u)
#define NRF5_FPU             ((volatile struct nrf5__t *)0x40026000u)
#define NRF5_USBD            ((volatile struct nrf5__t *)0x40027000u)
#define NRF5_UARTE1          ((volatile struct nrf5__t *)0x40028000u)
#define NRF5_QSPI            ((volatile struct nrf5__t *)0x40029000u)
#define NRF5_SPIM3           ((volatile struct nrf5__t *)0x4002b000u)
#define NRF5_PWM3            ((volatile struct nrf5__t *)0x4002d000u)
#define NRF5_GPIO_P0     ((volatile struct nrf5_gpio_t *)0x50000000u)
#define NRF5_GPIO_P1     ((volatile struct nrf5_gpio_t *)0x50000300u)
#define NRF5_CRYPTOCELL      ((volatile struct nrf5__t *)0x5002a000u)
#define NRF5_FICR            ((volatile struct nrf5__t *)0x10000000u)
#define NRF5_UICR            ((volatile struct nrf5__t *)0x10001000u)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

#endif
