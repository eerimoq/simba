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

#define ISR_WRAPPER(vector)                                             \
    static void isr_ ## vector ## _wrapper(void)                        \
    {                                                                   \
        uint32_t start;                                                 \
        start = SAM_TC0->CHANNEL[0].CV;                                 \
        isr_ ## vector();                                               \
            sys.interrupt.time += (SAM_TC0->CHANNEL[0].CV - start);     \
    }

/* Defined in the linker script. */
extern uint32_t __main_stack_end;
extern uint32_t __text_start__;
extern uint32_t __text_end__;
extern uint32_t __relocate_start__;
extern uint32_t __relocate_end__;
extern uint32_t __zero_start__;
extern uint32_t __zero_end__;

extern struct sys_t sys;

extern void main(void);

/**
 * Do nothing if no interrupt service routine is installed in the
 * interrupt vector.
 */
static void isr_none(void)
{
    while (1);
}

/* System exceptions (1-15). */
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

/* Wrapper functions with interrupt load measurements. */
ISR_WRAPPER(supc);
ISR_WRAPPER(rstc);
ISR_WRAPPER(rtc);
ISR_WRAPPER(rtt);
ISR_WRAPPER(wdg);
ISR_WRAPPER(pmc);
ISR_WRAPPER(eefc0);
ISR_WRAPPER(eefc1);
ISR_WRAPPER(uart);
ISR_WRAPPER(smc);
ISR_WRAPPER(sdramc);
ISR_WRAPPER(pioa);
ISR_WRAPPER(piob);
ISR_WRAPPER(pioc);
ISR_WRAPPER(piod);
ISR_WRAPPER(pioe);
ISR_WRAPPER(piof);
ISR_WRAPPER(usart0);
ISR_WRAPPER(usart1);
ISR_WRAPPER(usart2);
ISR_WRAPPER(usart3);
ISR_WRAPPER(hsmci);
ISR_WRAPPER(twi0);
ISR_WRAPPER(twi1);
ISR_WRAPPER(spi0);
ISR_WRAPPER(spi1);
ISR_WRAPPER(ssc);
ISR_WRAPPER(tc0);
ISR_WRAPPER(tc1);
ISR_WRAPPER(tc2);
ISR_WRAPPER(tc3);
ISR_WRAPPER(tc4);
ISR_WRAPPER(tc5);
ISR_WRAPPER(tc6);
ISR_WRAPPER(tc7);
ISR_WRAPPER(tc8);
ISR_WRAPPER(pwm);
ISR_WRAPPER(adc);
ISR_WRAPPER(dacc);
ISR_WRAPPER(dmac);
ISR_WRAPPER(uotghs);
ISR_WRAPPER(trng);
ISR_WRAPPER(emac);
ISR_WRAPPER(can0);
ISR_WRAPPER(can1);

#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)

/* Clock settings (84MHz) */
static void clock_init(void)
{
    /* Set FWS according to SYS_BOARD_MCKR configuration */
    SAM_EEFC0->FMR = EEFC_FMR_FWS(4) | EEFC_FMR_FRDY;
    SAM_EEFC1->FMR = EEFC_FMR_FWS(4) | EEFC_FMR_FRDY;

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

    /* Disable the watchdog. */
    SAM_WDT->MR = 0x8000;

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
    isr_supc_wrapper,
    isr_rstc_wrapper,
    isr_rtc_wrapper,
    isr_rtt_wrapper,
    isr_wdg_wrapper,
    isr_pmc_wrapper,
    isr_eefc0_wrapper,
    isr_eefc1_wrapper,
    isr_uart_wrapper,
    isr_smc_wrapper,
    isr_sdramc_wrapper,
    isr_pioa_wrapper,
    isr_piob_wrapper,
    isr_pioc_wrapper,
    isr_piod_wrapper,
    isr_pioe_wrapper,
    isr_piof_wrapper,
    isr_usart0_wrapper,
    isr_usart1_wrapper,
    isr_usart2_wrapper,
    isr_usart3_wrapper,
    isr_hsmci_wrapper,
    isr_twi0_wrapper,
    isr_twi1_wrapper,
    isr_spi0_wrapper,
    isr_spi1_wrapper,
    isr_ssc_wrapper,
    isr_tc0_wrapper,
    isr_tc1_wrapper,
    isr_tc2_wrapper,
    isr_tc3_wrapper,
    isr_tc4_wrapper,
    isr_tc5_wrapper,
    isr_tc6_wrapper,
    isr_tc7_wrapper,
    isr_tc8_wrapper,
    isr_pwm_wrapper,
    isr_adc_wrapper,
    isr_dacc_wrapper,
    isr_dmac_wrapper,
    isr_uotghs_wrapper,
    isr_trng_wrapper,
    isr_emac_wrapper,
    isr_can0_wrapper,
    isr_can1_wrapper
};
