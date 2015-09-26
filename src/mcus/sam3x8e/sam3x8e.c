/**
 * @file mcu.h
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
extern uint32_t __stack_end__;

/* Vector table with all interrupt service routines and the start
   stack pointer. */
struct vector_table_t {
    void (*stack)(void);

    /* System exceptions (1-15). */
    void (*reset)(void);
    void (*nmi)(void);
    void (*hard_fault)(void);
    void (*mem_manage_fault)(void);
    void (*bus_fault)(void);
    void (*usage_fault)(void);
    void (*reserved1)(void);
    void (*reserved2)(void);
    void (*reserved3)(void);
    void (*reserved4)(void);
    void (*svc)(void);
    void (*debug_monitor)(void);
    void (*reserved5)(void);
    void (*pend_sv)(void);
    void (*sys_tick)(void);

    /* Non-system exceptions (16+). */
    void (*supc)(void);
    void (*rstc)(void);
    void (*rtc)(void);
    void (*rtt)(void);
    void (*wdg)(void);
    void (*pmc)(void);
    void (*eefc0)(void);
    void (*eefc1)(void);
    void (*uart)(void);
    void (*smc)(void);
    void (*sdramc)(void);
    void (*pioa)(void);
    void (*piob)(void);
    void (*pioc)(void);
    void (*piod)(void);
    void (*pioe)(void);
    void (*piof)(void);
    void (*usart0)(void);
    void (*usart1)(void);
    void (*usart2)(void);
    void (*usart3)(void);
    void (*hsmci)(void);
    void (*twi0)(void);
    void (*twi1)(void);
    void (*spi0)(void);
    void (*spi1)(void);
    void (*ssc)(void);
    void (*tc0)(void);
    void (*tc1)(void);
    void (*tc2)(void);
    void (*tc3)(void);
    void (*tc4)(void);
    void (*tc5)(void);
    void (*tc6)(void);
    void (*tc7)(void);
    void (*tc8)(void);
    void (*pwm)(void);
    void (*adc)(void);
    void (*dacc)(void);
    void (*dmac)(void);
    void (*uotghs)(void);
    void (*trng)(void);
    void (*emac)(void);
    void (*can0)(void);
    void (*can1)(void);
};

/**
 * Do nothing if no interrupt service routine is installed in the
 * interrupt vector.
 */
static void isr_none(void)
{
}
                                
void isr_reset(void)            __attribute__ ((weak, alias("isr_none")));
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

__attribute__ ((section(".vectors"), used)) 
struct vector_table_t vector_table = {
    .stack = (void (*)(void))(&__stack_end__),

    .reset = isr_reset,
    .nmi = isr_nmi,
    .hard_fault = isr_hard_fault,
    .mem_manage_fault = isr_mem_manage_fault,
    .bus_fault = isr_bus_fault,
    .usage_fault = isr_usage_fault,
    .reserved1 = isr_reserved1,
    .reserved2 = isr_reserved2,
    .reserved3 = isr_reserved3,
    .reserved4 = isr_reserved4,
    .svc = isr_svc,
    .debug_monitor = isr_debug_monitor,
    .reserved5 = isr_reserved5,
    .pend_sv = isr_pend_sv,
    .sys_tick = isr_sys_tick,

    .supc = isr_supc,
    .rstc = isr_rstc,
    .rtc = isr_rtc,
    .rtt = isr_rtt,
    .wdg = isr_wdg,
    .pmc = isr_pmc,
    .eefc0 = isr_eefc0,
    .eefc1 = isr_eefc1,
    .uart = isr_uart,
    .smc = isr_smc,
    .sdramc = isr_sdramc,
    .pioa = isr_pioa,
    .piob = isr_piob,
    .pioc = isr_pioc,
    .piod = isr_piod,
    .pioe = isr_pioe,
    .piof = isr_piof,
    .usart0 = isr_usart0,
    .usart1 = isr_usart1,
    .usart2 = isr_usart2,
    .usart3 = isr_usart3,
    .hsmci = isr_hsmci,
    .twi0 = isr_twi0,
    .twi1 = isr_twi1,
    .spi0 = isr_spi0,
    .spi1 = isr_spi1,
    .ssc = isr_ssc,
    .tc0 = isr_tc0,
    .tc1 = isr_tc1,
    .tc2 = isr_tc2,
    .tc3 = isr_tc3,
    .tc4 = isr_tc4,
    .tc5 = isr_tc5,
    .tc6 = isr_tc6,
    .tc7 = isr_tc7,
    .tc8 = isr_tc8,
    .pwm = isr_pwm,
    .adc = isr_adc,
    .dacc = isr_dacc,
    .dmac = isr_dmac,
    .uotghs = isr_uotghs,
    .trng = isr_trng,
    .emac = isr_emac,
    .can0 = isr_can0,
    .can1 = isr_can1
};
