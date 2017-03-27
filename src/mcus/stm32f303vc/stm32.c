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

/* #define ISR_WRAPPER(vector)                                             \ */
/*     static void isr_ ## vector ## _wrapper(void)                        \ */
/*     {                                                                   \ */
/*         uint32_t start;                                                 \ */
/*         start = SAM_TC0->CHANNEL[0].CV;                                 \ */
/*         isr_ ## vector();                                               \ */
/*             sys.interrupt.time += (SAM_TC0->CHANNEL[0].CV - start);     \ */
/*     } */

#define ISR_WRAPPER(vector)                             \
    static void isr_ ## vector ## _wrapper(void)        \
    {                                                   \
        isr_ ## vector();                               \
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
void isr_wwdg(void)             __attribute__ ((weak, alias("isr_none")));
void isr_pvd(void)              __attribute__ ((weak, alias("isr_none")));
void isr_tamper_stamp(void)     __attribute__ ((weak, alias("isr_none")));
void isr_rtc_wkup(void)         __attribute__ ((weak, alias("isr_none")));
void isr_flash(void)            __attribute__ ((weak, alias("isr_none")));
void isr_rcc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_exti0(void)            __attribute__ ((weak, alias("isr_none")));
void isr_exti1(void)            __attribute__ ((weak, alias("isr_none")));
void isr_exti2_ts(void)         __attribute__ ((weak, alias("isr_none")));
void isr_exti3(void)            __attribute__ ((weak, alias("isr_none")));
void isr_exti4(void)            __attribute__ ((weak, alias("isr_none")));
void isr_dma1_ch1(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma1_ch2(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma1_ch3(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma1_ch4(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma1_ch5(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma1_ch6(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma1_ch7(void)         __attribute__ ((weak, alias("isr_none")));
void isr_adc1_2(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usb_hp_can_tx(void)    __attribute__ ((weak, alias("isr_none")));
void isr_usb_lp_can_rx0(void)   __attribute__ ((weak, alias("isr_none")));
void isr_can_rx1(void)          __attribute__ ((weak, alias("isr_none")));
void isr_can_sce(void)          __attribute__ ((weak, alias("isr_none")));
void isr_exti9_5(void)          __attribute__ ((weak, alias("isr_none")));
void isr_tim1_brk_tim15(void)   __attribute__ ((weak, alias("isr_none")));
void isr_tim1_up_tim16(void)    __attribute__ ((weak, alias("isr_none")));
void isr_tim1_trg_com_tim17(void) __attribute__ ((weak, alias("isr_none")));
void isr_tim1_cc(void)          __attribute__ ((weak, alias("isr_none")));
void isr_tim2(void)             __attribute__ ((weak, alias("isr_none")));
void isr_tim3(void)             __attribute__ ((weak, alias("isr_none")));
void isr_tim4(void)             __attribute__ ((weak, alias("isr_none")));
void isr_i2c1_ev(void)          __attribute__ ((weak, alias("isr_none")));
void isr_i2c_er(void)           __attribute__ ((weak, alias("isr_none")));
void isr_i2c2_ev(void)          __attribute__ ((weak, alias("isr_none")));
void isr_i2c2_er(void)          __attribute__ ((weak, alias("isr_none")));
void isr_spi1(void)             __attribute__ ((weak, alias("isr_none")));
void isr_spi2(void)             __attribute__ ((weak, alias("isr_none")));
void isr_usart1(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usart2(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usart3(void)           __attribute__ ((weak, alias("isr_none")));
void isr_exti15_10(void)        __attribute__ ((weak, alias("isr_none")));
void isr_rtc_alarm(void)        __attribute__ ((weak, alias("isr_none")));
void isr_usb_wakeup(void)       __attribute__ ((weak, alias("isr_none")));
void isr_tim8_brk(void)         __attribute__ ((weak, alias("isr_none")));
void isr_tim8_up(void)          __attribute__ ((weak, alias("isr_none")));
void isr_tim8_trg_com(void)     __attribute__ ((weak, alias("isr_none")));
void isr_tim8_cc(void)          __attribute__ ((weak, alias("isr_none")));
void isr_adc3(void)             __attribute__ ((weak, alias("isr_none")));
void isr_fmc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_spi3(void)             __attribute__ ((weak, alias("isr_none")));
void isr_uart4(void)            __attribute__ ((weak, alias("isr_none")));
void isr_uart5(void)            __attribute__ ((weak, alias("isr_none")));
void isr_tim6_dac(void)         __attribute__ ((weak, alias("isr_none")));
void isr_tim7(void)             __attribute__ ((weak, alias("isr_none")));
void isr_dma2_ch1(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma2_ch2(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma2_ch3(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma2_ch4(void)         __attribute__ ((weak, alias("isr_none")));
void isr_dma2_ch5(void)         __attribute__ ((weak, alias("isr_none")));
void isr_adc4(void)             __attribute__ ((weak, alias("isr_none")));
void isr_comp1_2_3(void)        __attribute__ ((weak, alias("isr_none")));
void isr_comp4_5_6(void)        __attribute__ ((weak, alias("isr_none")));
void isr_comp7(void)            __attribute__ ((weak, alias("isr_none")));
void isr_i2c3_ev(void)          __attribute__ ((weak, alias("isr_none")));
void isr_i2c3_er(void)          __attribute__ ((weak, alias("isr_none")));
void isr_usb_hp(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usb_lp(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usb_wakeup_rmp(void)   __attribute__ ((weak, alias("isr_none")));
void isr_tim20_brk(void)        __attribute__ ((weak, alias("isr_none")));
void isr_tim20_up(void)         __attribute__ ((weak, alias("isr_none")));
void isr_tim20_trg_com(void)    __attribute__ ((weak, alias("isr_none")));
void isr_fpu(void)              __attribute__ ((weak, alias("isr_none")));

/* Wrapper functions with interrupt load measurements. */
ISR_WRAPPER(wwdg);
ISR_WRAPPER(pvd);
ISR_WRAPPER(tamper_stamp);
ISR_WRAPPER(rtc_wkup);
ISR_WRAPPER(flash);
ISR_WRAPPER(rcc);
ISR_WRAPPER(exti0);
ISR_WRAPPER(exti1);
ISR_WRAPPER(exti2_ts);
ISR_WRAPPER(exti3);
ISR_WRAPPER(exti4);
ISR_WRAPPER(dma1_ch1);
ISR_WRAPPER(dma1_ch2);
ISR_WRAPPER(dma1_ch3);
ISR_WRAPPER(dma1_ch4);
ISR_WRAPPER(dma1_ch5);
ISR_WRAPPER(dma1_ch6);
ISR_WRAPPER(dma1_ch7);
ISR_WRAPPER(adc1_2);
ISR_WRAPPER(usb_hp_can_tx);
ISR_WRAPPER(usb_lp_can_rx0);
ISR_WRAPPER(can_rx1);
ISR_WRAPPER(can_sce);
ISR_WRAPPER(exti9_5);
ISR_WRAPPER(tim1_brk_tim15);
ISR_WRAPPER(tim1_up_tim16);
ISR_WRAPPER(tim1_trg_com_tim17);
ISR_WRAPPER(tim1_cc);
ISR_WRAPPER(tim2);
ISR_WRAPPER(tim3);
ISR_WRAPPER(tim4);
ISR_WRAPPER(i2c1_ev);
ISR_WRAPPER(i2c_er);
ISR_WRAPPER(i2c2_ev);
ISR_WRAPPER(i2c2_er);
ISR_WRAPPER(spi1);
ISR_WRAPPER(spi2);
ISR_WRAPPER(usart1);
ISR_WRAPPER(usart2);
ISR_WRAPPER(usart3);
ISR_WRAPPER(exti15_10);
ISR_WRAPPER(rtc_alarm);
ISR_WRAPPER(usb_wakeup);
ISR_WRAPPER(tim8_brk);
ISR_WRAPPER(tim8_up);
ISR_WRAPPER(tim8_trg_com);
ISR_WRAPPER(tim8_cc);
ISR_WRAPPER(adc3);
ISR_WRAPPER(fmc);
ISR_WRAPPER(spi3);
ISR_WRAPPER(uart4);
ISR_WRAPPER(uart5);
ISR_WRAPPER(tim6_dac);
ISR_WRAPPER(tim7);
ISR_WRAPPER(dma2_ch1);
ISR_WRAPPER(dma2_ch2);
ISR_WRAPPER(dma2_ch3);
ISR_WRAPPER(dma2_ch4);
ISR_WRAPPER(dma2_ch5);
ISR_WRAPPER(adc4);
ISR_WRAPPER(comp1_2_3);
ISR_WRAPPER(comp4_5_6);
ISR_WRAPPER(comp7);
ISR_WRAPPER(i2c3_ev);
ISR_WRAPPER(i2c3_er);
ISR_WRAPPER(usb_hp);
ISR_WRAPPER(usb_lp);
ISR_WRAPPER(usb_wakeup_rmp);
ISR_WRAPPER(tim20_brk);
ISR_WRAPPER(tim20_up);
ISR_WRAPPER(tim20_trg_com);
ISR_WRAPPER(fpu);

/* #define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK) */

/* /\* Clock settings (84MHz) *\/ */
/* static void clock_init(void) */
/* { */
/*     /\* Set FWS according to SYS_BOARD_MCKR configuration *\/ */
/*     SAM_EEFC0->FMR = EEFC_FMR_FWS(4) | EEFC_FMR_FRDY; */
/*     SAM_EEFC1->FMR = EEFC_FMR_FWS(4) | EEFC_FMR_FRDY; */

/*     /\* Initialize main oscillator *\/ */
/*     SAM_PMC->CKGR_MOR = (PMC_CKGR_MOR_KEY(0x37) */
/*                          | PMC_CKGR_MOR_MOSCXTST(0x8) */
/*                          | PMC_CKGR_MOR_MOSCRCEN */
/*                          | PMC_CKGR_MOR_MOSCXTEN); */

/*     while ((SAM_PMC->SR & PMC_SR_MOSCXTS) == 0); */

/*     /\* Switch to the oscillator connected to XIN/XOUT. *\/ */
/*     SAM_PMC->CKGR_MOR = (PMC_CKGR_MOR_KEY(0x37) */
/*                          | PMC_CKGR_MOR_MOSCXTST(0x8) */
/*                          | PMC_CKGR_MOR_MOSCRCEN */
/*                          | PMC_CKGR_MOR_MOSCXTEN */
/*                          | PMC_CKGR_MOR_MOSCSEL); */

/*     while ((SAM_PMC->SR & PMC_SR_MOSCSELS) == 0); */

/*     SAM_PMC->MCKR = ((SAM_PMC->MCKR & ~(uint32_t)PMC_MCKR_CSS_MASK) */
/*                      | PMC_MCKR_CSS_MAIN_CLK); */

/*     while ((SAM_PMC->SR & PMC_SR_MCKRDY) == 0); */

/*     /\* Initialize PLLA. *\/ */
/*     SAM_PMC->CKGR_PLLAR = (PMC_CKGR_PLLAR_ONE */
/*                            | PMC_CKGR_PLLAR_MULA(0xd) */
/*                            | PMC_CKGR_PLLAR_PLLACOUNT(0x3f) */
/*                            | PMC_CKGR_PLLAR_DIVA(1)); */

/*     while ((SAM_PMC->SR & PMC_SR_LOCKA) == 0); */

/*     /\* Switch to main clock. *\/ */
/*     SAM_PMC->MCKR = (PMC_MCKR_PRES_CLK_2 */
/*                      | PMC_MCKR_CSS_MAIN_CLK); */

/*     while ((SAM_PMC->SR & PMC_SR_MCKRDY) == 0); */

/*     /\* Switch to PLLA. *\/ */
/*     SAM_PMC->MCKR = (PMC_MCKR_PRES_CLK_2 */
/*                      | PMC_MCKR_CSS_PLLA_CLK); */

/*     while ((SAM_PMC->SR & PMC_SR_MCKRDY) == 0); */
/* } */

void isr_reset(void)
{
    uint32_t *src_p, *dst_p;

    /* /\* Disable the watchdog. *\/ */
    /* SAM_WDT->MR = 0x8000; */

    /* clock_init(); */

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
    isr_wwdg_wrapper,
    isr_pvd_wrapper,
    isr_tamper_stamp_wrapper,
    isr_rtc_wkup_wrapper,
    isr_flash_wrapper,
    isr_rcc_wrapper,
    isr_exti0_wrapper,
    isr_exti1_wrapper,
    isr_exti2_ts_wrapper,
    isr_exti3_wrapper,
    isr_exti4_wrapper,
    isr_dma1_ch1_wrapper,
    isr_dma1_ch2_wrapper,
    isr_dma1_ch3_wrapper,
    isr_dma1_ch4_wrapper,
    isr_dma1_ch5_wrapper,
    isr_dma1_ch6_wrapper,
    isr_dma1_ch7_wrapper,
    isr_adc1_2_wrapper,
    isr_usb_hp_can_tx_wrapper,
    isr_usb_lp_can_rx0_wrapper,
    isr_can_rx1_wrapper,
    isr_can_sce_wrapper,
    isr_exti9_5_wrapper,
    isr_tim1_brk_tim15_wrapper,
    isr_tim1_up_tim16_wrapper,
    isr_tim1_trg_com_tim17_wrapper,
    isr_tim1_cc_wrapper,
    isr_tim2_wrapper,
    isr_tim3_wrapper,
    isr_tim4_wrapper,
    isr_i2c1_ev_wrapper,
    isr_i2c_er_wrapper,
    isr_i2c2_ev_wrapper,
    isr_i2c2_er_wrapper,
    isr_spi1_wrapper,
    isr_spi2_wrapper,
    isr_usart1_wrapper,
    isr_usart2_wrapper,
    isr_usart3_wrapper,
    isr_exti15_10_wrapper,
    isr_rtc_alarm_wrapper,
    isr_usb_wakeup_wrapper,
    isr_tim8_brk_wrapper,
    isr_tim8_up_wrapper,
    isr_tim8_trg_com_wrapper,
    isr_tim8_cc_wrapper,
    isr_adc3_wrapper,
    isr_fmc_wrapper,
    isr_spi3_wrapper,
    isr_uart4_wrapper,
    isr_uart5_wrapper,
    isr_tim6_dac_wrapper,
    isr_tim7_wrapper,
    isr_dma2_ch1_wrapper,
    isr_dma2_ch2_wrapper,
    isr_dma2_ch3_wrapper,
    isr_dma2_ch4_wrapper,
    isr_dma2_ch5_wrapper,
    isr_adc4_wrapper,
    isr_comp1_2_3_wrapper,
    isr_comp4_5_6_wrapper,
    isr_comp7_wrapper,
    isr_i2c3_ev_wrapper,
    isr_i2c3_er_wrapper,
    isr_usb_hp_wrapper,
    isr_usb_lp_wrapper,
    isr_usb_wakeup_rmp_wrapper,
    isr_tim20_brk_wrapper,
    isr_tim20_up_wrapper,
    isr_tim20_trg_com_wrapper,
    isr_fpu_wrapper
};
