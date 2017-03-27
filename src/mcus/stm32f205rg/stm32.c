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

#define ISR_WRAPPER(vector)                             \
    static void isr_ ## vector ## _wrapper(void)        \
    {                                                   \
        isr_ ## vector();                               \
    }

/* Defined in the linker script. */
extern uint32_t __fixed_start__;
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
void isr_exti2(void)            __attribute__ ((weak, alias("isr_none")));
void isr_exti3(void)            __attribute__ ((weak, alias("isr_none")));
void isr_exti4(void)            __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream0(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream1(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream2(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream3(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream4(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream5(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream6(void)     __attribute__ ((weak, alias("isr_none")));
void isr_adc(void)              __attribute__ ((weak, alias("isr_none")));
void isr_can1_tx(void)          __attribute__ ((weak, alias("isr_none")));
void isr_can1_rx0(void)         __attribute__ ((weak, alias("isr_none")));
void isr_can1_rx1(void)         __attribute__ ((weak, alias("isr_none")));
void isr_can1_sce(void)         __attribute__ ((weak, alias("isr_none")));
void isr_exti9_5(void)          __attribute__ ((weak, alias("isr_none")));
void isr_tim1_brk_tim9(void)    __attribute__ ((weak, alias("isr_none")));
void isr_tim1_up_tim10(void)    __attribute__ ((weak, alias("isr_none")));
void isr_tim1_trg_com_tim11(void) __attribute__ ((weak, alias("isr_none")));
void isr_tim1_cc(void)          __attribute__ ((weak, alias("isr_none")));
void isr_tim2(void)             __attribute__ ((weak, alias("isr_none")));
void isr_tim3(void)             __attribute__ ((weak, alias("isr_none")));
void isr_tim4(void)             __attribute__ ((weak, alias("isr_none")));
void isr_i2c1_ev(void)          __attribute__ ((weak, alias("isr_none")));
void isr_i2c1_er(void)          __attribute__ ((weak, alias("isr_none")));
void isr_i2c2_ev(void)          __attribute__ ((weak, alias("isr_none")));
void isr_i2c2_er(void)          __attribute__ ((weak, alias("isr_none")));
void isr_spi1(void)             __attribute__ ((weak, alias("isr_none")));
void isr_spi2(void)             __attribute__ ((weak, alias("isr_none")));
void isr_usart1(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usart2(void)           __attribute__ ((weak, alias("isr_none")));
void isr_usart3(void)           __attribute__ ((weak, alias("isr_none")));
void isr_exti15_10(void)        __attribute__ ((weak, alias("isr_none")));
void isr_rtc_alarm(void)        __attribute__ ((weak, alias("isr_none")));
void isr_otg_fs_wkup(void)      __attribute__ ((weak, alias("isr_none")));
void isr_tim8_brk_tim12(void)   __attribute__ ((weak, alias("isr_none")));
void isr_tim8_up_tim13(void)    __attribute__ ((weak, alias("isr_none")));
void isr_tim8_trg_com_tim14(void) __attribute__ ((weak, alias("isr_none")));
void isr_tim8_cc(void)          __attribute__ ((weak, alias("isr_none")));
void isr_dma1_stream7(void)     __attribute__ ((weak, alias("isr_none")));
void isr_fsmc(void)             __attribute__ ((weak, alias("isr_none")));
void isr_sdio(void)             __attribute__ ((weak, alias("isr_none")));
void isr_tim5(void)             __attribute__ ((weak, alias("isr_none")));
void isr_spi3(void)             __attribute__ ((weak, alias("isr_none")));
void isr_uart4(void)            __attribute__ ((weak, alias("isr_none")));
void isr_uart5(void)            __attribute__ ((weak, alias("isr_none")));
void isr_tim6_dac(void)         __attribute__ ((weak, alias("isr_none")));
void isr_tim7(void)             __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream0(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream1(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream2(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream3(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream4(void)     __attribute__ ((weak, alias("isr_none")));
void isr_eth(void)              __attribute__ ((weak, alias("isr_none")));
void isr_eth_wkup(void)         __attribute__ ((weak, alias("isr_none")));
void isr_can2_tx(void)          __attribute__ ((weak, alias("isr_none")));
void isr_can2_rx0(void)         __attribute__ ((weak, alias("isr_none")));
void isr_can2_rx1(void)         __attribute__ ((weak, alias("isr_none")));
void isr_can2_sce(void)         __attribute__ ((weak, alias("isr_none")));
void isr_otg_fs(void)           __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream5(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream6(void)     __attribute__ ((weak, alias("isr_none")));
void isr_dma2_stream7(void)     __attribute__ ((weak, alias("isr_none")));
void isr_usart6(void)           __attribute__ ((weak, alias("isr_none")));
void isr_i2c3_ev(void)          __attribute__ ((weak, alias("isr_none")));
void isr_i2c3_er(void)          __attribute__ ((weak, alias("isr_none")));
void isr_otg_hs_ep1_out(void)   __attribute__ ((weak, alias("isr_none")));
void isr_otg_hs_ep1_in(void)    __attribute__ ((weak, alias("isr_none")));
void isr_otg_hs_wkup(void)      __attribute__ ((weak, alias("isr_none")));
void isr_otg_hs(void)           __attribute__ ((weak, alias("isr_none")));
void isr_dcmi(void)             __attribute__ ((weak, alias("isr_none")));
void isr_cryp(void)             __attribute__ ((weak, alias("isr_none")));
void isr_hash_rng(void)         __attribute__ ((weak, alias("isr_none")));

/* Wrapper functions with interrupt load measurements. */
ISR_WRAPPER(wwdg);
ISR_WRAPPER(pvd);
ISR_WRAPPER(tamper_stamp);
ISR_WRAPPER(rtc_wkup);
ISR_WRAPPER(flash);
ISR_WRAPPER(rcc);
ISR_WRAPPER(exti0);
ISR_WRAPPER(exti1);
ISR_WRAPPER(exti2);
ISR_WRAPPER(exti3);
ISR_WRAPPER(exti4);
ISR_WRAPPER(dma1_stream0);
ISR_WRAPPER(dma1_stream1);
ISR_WRAPPER(dma1_stream2);
ISR_WRAPPER(dma1_stream3);
ISR_WRAPPER(dma1_stream4);
ISR_WRAPPER(dma1_stream5);
ISR_WRAPPER(dma1_stream6);
ISR_WRAPPER(adc);
ISR_WRAPPER(can1_tx);
ISR_WRAPPER(can1_rx0);
ISR_WRAPPER(can1_rx1);
ISR_WRAPPER(can1_sce);
ISR_WRAPPER(exti9_5);
ISR_WRAPPER(tim1_brk_tim9);
ISR_WRAPPER(tim1_up_tim10);
ISR_WRAPPER(tim1_trg_com_tim11);
ISR_WRAPPER(tim1_cc);
ISR_WRAPPER(tim2);
ISR_WRAPPER(tim3);
ISR_WRAPPER(tim4);
ISR_WRAPPER(i2c1_ev);
ISR_WRAPPER(i2c1_er);
ISR_WRAPPER(i2c2_ev);
ISR_WRAPPER(i2c2_er);
ISR_WRAPPER(spi1);
ISR_WRAPPER(spi2);
ISR_WRAPPER(usart1);
ISR_WRAPPER(usart2);
ISR_WRAPPER(usart3);
ISR_WRAPPER(exti15_10);
ISR_WRAPPER(rtc_alarm);
ISR_WRAPPER(otg_fs_wkup);
ISR_WRAPPER(tim8_brk_tim12);
ISR_WRAPPER(tim8_up_tim13);
ISR_WRAPPER(tim8_trg_com_tim14);
ISR_WRAPPER(tim8_cc);
ISR_WRAPPER(dma1_stream7);
ISR_WRAPPER(fsmc);
ISR_WRAPPER(sdio);
ISR_WRAPPER(tim5);
ISR_WRAPPER(spi3);
ISR_WRAPPER(uart4);
ISR_WRAPPER(uart5);
ISR_WRAPPER(tim6_dac);
ISR_WRAPPER(tim7);
ISR_WRAPPER(dma2_stream0);
ISR_WRAPPER(dma2_stream1);
ISR_WRAPPER(dma2_stream2);
ISR_WRAPPER(dma2_stream3);
ISR_WRAPPER(dma2_stream4);
ISR_WRAPPER(eth);
ISR_WRAPPER(eth_wkup);
ISR_WRAPPER(can2_tx);
ISR_WRAPPER(can2_rx0);
ISR_WRAPPER(can2_rx1);
ISR_WRAPPER(can2_sce);
ISR_WRAPPER(otg_fs);
ISR_WRAPPER(dma2_stream5);
ISR_WRAPPER(dma2_stream6);
ISR_WRAPPER(dma2_stream7);
ISR_WRAPPER(usart6);
ISR_WRAPPER(i2c3_ev);
ISR_WRAPPER(i2c3_er);
ISR_WRAPPER(otg_hs_ep1_out);
ISR_WRAPPER(otg_hs_ep1_in);
ISR_WRAPPER(otg_hs_wkup);
ISR_WRAPPER(otg_hs);
ISR_WRAPPER(dcmi);
ISR_WRAPPER(cryp);
ISR_WRAPPER(hash_rng);

/* Clock settings (24MHz) */
static void clock_init(void)
{
    /* The HSI clock is used at reset. PLL is disabled. */

    /* Enable HSE. */
    STM32_RCC->CR = (STM32_RCC_CR_HSEON);

    /* Wait for HSE to be stable. */
    while ((STM32_RCC->CR & STM32_RCC_CR_HSERDY) == 0);

    /* Select HSE as input to PLL. */
    STM32_RCC->CFGR = STM32_RCC_CFGR_PLLSRC_PREDIV1;

    /* Configure and enable PLL. */
    STM32_RCC->CFGR |= (STM32_RCC_CFGR_PLLMUL_3);
    STM32_RCC->CR |= (STM32_RCC_CR_PLLON);

    /* Wait for PLL to be stable. */
    while ((STM32_RCC->CR & STM32_RCC_CR_PLLRDY) == 0);

    /* Select PLL as SYSCLK. */
    STM32_RCC->CFGR |= (STM32_RCC_CFGR_SW_PLL);

    /* Wait for PLL to be the selected clock. */
    while ((STM32_RCC->CFGR & STM32_RCC_CFGR_SWS_MASK)
           != STM32_RCC_CFGR_SWS_PLL);
}

void isr_reset(void)
{
    uint32_t *src_p, *dst_p;
    uint32_t tbloff;

    if (0) {
        clock_init();
    }

    /* Set the vector table offset. */
    tbloff = ((uint32_t)&__fixed_start__ >> 7);
    ARM_SCB->VTOR = SCB_VTOR_TBLOFF(tbloff);

    STM32_RCC->APB1RSTR = 0x0;
    STM32_RCC->APB1ENR = 0xffffffff;

    STM32_RCC->APB2RSTR = 0x0;
    STM32_RCC->APB2ENR = 0xffffffff;

    STM32_RCC->AHB1RSTR = 0x0;
    STM32_RCC->AHB2RSTR = 0x0;

    STM32_RCC->AHB1ENR = 0xffffffff;
    STM32_RCC->AHB2ENR = 0xffffffff;

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
    isr_exti2_wrapper,
    isr_exti3_wrapper,
    isr_exti4_wrapper,
    isr_dma1_stream0_wrapper,
    isr_dma1_stream1_wrapper,
    isr_dma1_stream2_wrapper,
    isr_dma1_stream3_wrapper,
    isr_dma1_stream4_wrapper,
    isr_dma1_stream5_wrapper,
    isr_dma1_stream6_wrapper,
    isr_adc_wrapper,
    isr_can1_tx_wrapper,
    isr_can1_rx0_wrapper,
    isr_can1_rx1_wrapper,
    isr_can1_sce_wrapper,
    isr_exti9_5_wrapper,
    isr_tim1_brk_tim9_wrapper,
    isr_tim1_up_tim10_wrapper,
    isr_tim1_trg_com_tim11_wrapper,
    isr_tim1_cc_wrapper,
    isr_tim2_wrapper,
    isr_tim3_wrapper,
    isr_tim4_wrapper,
    isr_i2c1_ev_wrapper,
    isr_i2c1_er_wrapper,
    isr_i2c2_ev_wrapper,
    isr_i2c2_er_wrapper,
    isr_spi1_wrapper,
    isr_spi2_wrapper,
    isr_usart1_wrapper,
    isr_usart2_wrapper,
    isr_usart3_wrapper,
    isr_exti15_10_wrapper,
    isr_rtc_alarm_wrapper,
    isr_otg_fs_wkup_wrapper,
    isr_tim8_brk_tim12_wrapper,
    isr_tim8_up_tim13_wrapper,
    isr_tim8_trg_com_tim14_wrapper,
    isr_tim8_cc_wrapper,
    isr_dma1_stream7_wrapper,
    isr_fsmc_wrapper,
    isr_sdio_wrapper,
    isr_tim5_wrapper,
    isr_spi3_wrapper,
    isr_uart4_wrapper,
    isr_uart5_wrapper,
    isr_tim6_dac_wrapper,
    isr_tim7_wrapper,
    isr_dma2_stream0_wrapper,
    isr_dma2_stream1_wrapper,
    isr_dma2_stream2_wrapper,
    isr_dma2_stream3_wrapper,
    isr_dma2_stream4_wrapper,
    isr_eth_wrapper,
    isr_eth_wkup_wrapper,
    isr_can2_tx_wrapper,
    isr_can2_rx0_wrapper,
    isr_can2_rx1_wrapper,
    isr_can2_sce_wrapper,
    isr_otg_fs_wrapper,
    isr_dma2_stream5_wrapper,
    isr_dma2_stream6_wrapper,
    isr_dma2_stream7_wrapper,
    isr_usart6_wrapper,
    isr_i2c3_ev_wrapper,
    isr_i2c3_er_wrapper,
    isr_otg_hs_ep1_out_wrapper,
    isr_otg_hs_ep1_in_wrapper,
    isr_otg_hs_wkup_wrapper,
    isr_otg_hs_wrapper,
    isr_dcmi_wrapper,
    isr_cryp_wrapper,
    isr_hash_rng_wrapper
};
