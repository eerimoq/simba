/**
 * @file sam3.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

/* Defined in the linker script. */
extern uint32_t __main_stack_end;
extern uint32_t __text_start__;
extern uint32_t __text_end__;
extern uint32_t __relocate_start__;
extern uint32_t __relocate_end__;
extern uint32_t __zero_start__;
extern uint32_t __zero_end__;

extern void __libc_init_array(void);
extern void main(void);

/**
 * Do nothing if no interrupt service routine is installed in the
 * interrupt vector.
 */
static void isr_none(void)
{
}

/* System exceptions (1-15). */
//void isr_reset(void)            __attribute__ ((weak, alias("isr_none")));
void isr_nmi(void)              __attribute__ ((weak, alias("isr_none")));
void isr_hard_fault(void)       __attribute__ ((weak, alias("isr_none")));
void isr_mem_manage_fault(void) __attribute__ ((weak, alias("isr_none")));
void isr_bus_fault(void)        __attribute__ ((weak, alias("isr_none")));
void isr_usage_fault(void)      __attribute__ ((weak, alias("isr_none")));
void isr_reserved1(void)        __attribute__ ((weak, alias("isr_none")));
void isr_reserved2(void)        __attribute__ ((weak, alias("isr_none")));
void isr_reserved3(void)        __attribute__ ((weak, alias("isr_none")));
void isr_reserved4(void)        __attribute__ ((weak, alias("isr_none")));
void isr_svc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_debug_monitor(void)    __attribute__ ((weak, alias("isr_none")));
void isr_reserved5(void)        __attribute__ ((weak, alias("isr_none")));
void isr_pend_sv(void)          __attribute__ ((weak, alias("isr_none")));
void isr_sys_tick(void)         __attribute__ ((weak, alias("isr_none")));

/* Non-system exceptions (16+). */
void isr_supc(void)             __attribute__ ((weak, alias("isr_none")));
void isr_rstc(void)             __attribute__ ((weak, alias("isr_none")));
void isr_rtc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_rtt(void)              __attribute__ ((weak, alias("isr_none")));
void isr_wdg(void)              __attribute__ ((weak, alias("isr_none")));
void isr_pmc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_eefc0(void)            __attribute__ ((weak, alias("isr_none")));
void isr_eefc1(void)            __attribute__ ((weak, alias("isr_none")));
void isr_uart(void)             __attribute__ ((weak, alias("isr_none")));
void isr_smc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_sdramc(void)           __attribute__ ((weak, alias("isr_none")));
void isr_pioa(void)             __attribute__ ((weak, alias("isr_none")));
void isr_piob(void)             __attribute__ ((weak, alias("isr_none")));
void isr_pioc(void)             __attribute__ ((weak, alias("isr_none")));
void isr_piod(void)             __attribute__ ((weak, alias("isr_none")));
void isr_pioe(void)             __attribute__ ((weak, alias("isr_none")));
void isr_piof(void)             __attribute__ ((weak, alias("isr_none")));
void isr_usart0(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usart1(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usart2(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usart3(void)           __attribute__ ((weak, alias("isr_none")));
void isr_hsmci(void)            __attribute__ ((weak, alias("isr_none")));
void isr_twi0(void)             __attribute__ ((weak, alias("isr_none")));
void isr_twi1(void)             __attribute__ ((weak, alias("isr_none")));
void isr_spi0(void)             __attribute__ ((weak, alias("isr_none")));
void isr_spi1(void)             __attribute__ ((weak, alias("isr_none")));
void isr_ssc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc0(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc1(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc2(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc3(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc4(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc5(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc6(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc7(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tc8(void)              __attribute__ ((weak, alias("isr_none")));
void isr_pwm(void)              __attribute__ ((weak, alias("isr_none")));
void isr_adc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_dacc(void)             __attribute__ ((weak, alias("isr_none")));
void isr_dmac(void)             __attribute__ ((weak, alias("isr_none")));
void isr_uotghs(void)           __attribute__ ((weak, alias("isr_none")));
void isr_trng(void)             __attribute__ ((weak, alias("isr_none")));
void isr_emac(void)             __attribute__ ((weak, alias("isr_none")));
void isr_can0(void)             __attribute__ ((weak, alias("isr_none")));
void isr_can1(void)             __attribute__ ((weak, alias("isr_none")));

#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)

/* Clock settings (84MHz) */
static void clock_init(void)
{
    /* Set FWS according to SYS_BOARD_MCKR configuration */
    SAM_EEFC0->FMR = EEFC_FMR_FWS(4);
    SAM_EEFC1->FMR = EEFC_FMR_FWS(4);

    /* Initialize main oscillator */
    SAM_PMC->CKGR_MOR = (PMC_CKGR_MOR_KEY(0x37)
                         | PMC_CKGR_MOR_MOSCXTST(0x8)
                         | PMC_CKGR_MOR_MOSCRCEN
                         | PMC_CKGR_MOR_MOSCXTEN);

    while ((SAM_PMC->SR & PMC_SR_MOSCXTS) == 0);

    /* Switch to the oscillator connected to XIN/XOUT. */
    SAM_PMC->CKGR_MOR = (PMC_CKGR_MOR_KEY(0x37)
                         | PMC_CKGR_MOR_MOSCXTST(0x8)
                         | PMC_CKGR_MOR_MOSCRCEN
                         | PMC_CKGR_MOR_MOSCXTEN
                         | PMC_CKGR_MOR_MOSCSEL);

    while ((SAM_PMC->SR & PMC_SR_MOSCSELS) == 0);

    SAM_PMC->MCKR = ((SAM_PMC->MCKR & ~(uint32_t)PMC_MCKR_CSS_MASK)
                     | PMC_MCKR_CSS_MAIN_CLK);

    while ((SAM_PMC->SR & PMC_SR_MCKRDY) == 0);

    /* Initialize PLLA. */
    SAM_PMC->CKGR_PLLAR = (PMC_CKGR_PLLAR_ONE
                           | PMC_CKGR_PLLAR_MULA(0xd)
                           | PMC_CKGR_PLLAR_PLLACOUNT(0x3f)
                           | PMC_CKGR_PLLAR_DIVA(1));

    while ((SAM_PMC->SR & PMC_SR_LOCKA) == 0);

    /* Switch to main clock. */
    SAM_PMC->MCKR = (PMC_MCKR_PRES_CLK_2
                     | PMC_MCKR_CSS_MAIN_CLK);

    while ((SAM_PMC->SR & PMC_SR_MCKRDY) == 0);

    /* Switch to PLLA. */
    SAM_PMC->MCKR = (PMC_MCKR_PRES_CLK_2
                     | PMC_MCKR_CSS_PLLA_CLK);

    while ((SAM_PMC->SR & PMC_SR_MCKRDY) == 0);
}

void isr_reset(void)
{
    uint32_t *src_p, *dst_p;

    clock_init();

    /* Initialize the relocate segment */
    src_p = &__text_end__;
    dst_p = &__relocate_start__;

    if (src_p != dst_p) {
        while (dst_p < &__relocate_end__) {
            *dst_p++ = *src_p++;
        }
    }

    /* Clear the zero segment */
    for (dst_p = &__zero_start__; dst_p < &__zero_end__;) {
        *dst_p++ = 0;
    }

    //__libc_init_array();

    /* Branch to main function */
    main();

    /* Infinite loop */
    while (1);
}

/* Vector table with all interrupt service routines and the start
   stack pointer. */
__attribute__ ((section(".vectors"), used))
void (*vector_table[])(void) = {
    /* Start stack address. */
    (void (*)(void))(&__main_stack_end),

    /* System exceptions (1-15). */
    isr_reset,
    isr_nmi,
    isr_hard_fault,
    isr_mem_manage_fault,
    isr_bus_fault,
    isr_usage_fault,
    isr_reserved1,
    isr_reserved2,
    isr_reserved3,
    isr_reserved4,
    isr_svc,
    isr_debug_monitor,
    isr_reserved5,
    isr_pend_sv,
    isr_sys_tick,

    /* Non-system exceptions (16+). */
    isr_supc,
    isr_rstc,
    isr_rtc,
    isr_rtt,
    isr_wdg,
    isr_pmc,
    isr_eefc0,
    isr_eefc1,
    isr_uart,
    isr_smc,
    isr_sdramc,
    isr_pioa,
    isr_piob,
    isr_pioc,
    isr_piod,
    isr_pioe,
    isr_piof,
    isr_usart0,
    isr_usart1,
    isr_usart2,
    isr_usart3,
    isr_hsmci,
    isr_twi0,
    isr_twi1,
    isr_spi0,
    isr_spi1,
    isr_ssc,
    isr_tc0,
    isr_tc1,
    isr_tc2,
    isr_tc3,
    isr_tc4,
    isr_tc5,
    isr_tc6,
    isr_tc7,
    isr_tc8,
    isr_pwm,
    isr_adc,
    isr_dacc,
    isr_dmac,
    isr_uotghs,
    isr_trng,
    isr_emac,
    isr_can0,
    isr_can1
};
