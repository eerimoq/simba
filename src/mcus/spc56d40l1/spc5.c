/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#if CONFIG_SYSTEM_INTERRUPTS == 1

/**
 * Do nothing if no interrupt service routine is installed in the
 * interrupt vector.
 */
static void isr_none(void)
{
    while (1);
}

void isr_critical_input(void) __attribute__ ((weak, alias("isr_none")));
void isr_machine_check(void) __attribute__ ((weak, alias("isr_none")));
void isr_data_storage(void) __attribute__ ((weak, alias("isr_none")));
void isr_instruction_storage(void) __attribute__ ((weak, alias("isr_none")));
void isr_alignment(void) __attribute__ ((weak, alias("isr_none")));
void isr_program(void) __attribute__ ((weak, alias("isr_none")));
void isr_reserved(void) __attribute__ ((weak, alias("isr_none")));
void isr_system_call(void) __attribute__ ((weak, alias("isr_none")));
void isr_auxiliary_processor_unavailable(void) __attribute__ ((weak, alias("isr_none")));
void isr_decrementer(void) __attribute__ ((weak, alias("isr_none")));
void isr_fixed_interval_timer_interrupt(void) __attribute__ ((weak, alias("isr_none")));
void isr_watchdog_timer_interrupt(void) __attribute__ ((weak, alias("isr_none")));
void isr_data_tlb_error(void) __attribute__ ((weak, alias("isr_none")));
void isr_instruction_tlb_error(void) __attribute__ ((weak, alias("isr_none")));
void isr_debug(void) __attribute__ ((weak, alias("isr_none")));

void isr_software_configurable_flag_0(void) __attribute__ ((weak, alias("isr_none")));
void isr_software_configurable_flag_1(void) __attribute__ ((weak, alias("isr_none")));
void isr_software_configurable_flag_2(void) __attribute__ ((weak, alias("isr_none")));
void isr_software_configurable_flag_3(void) __attribute__ ((weak, alias("isr_none")));
void isr_software_configurable_flag_4(void) __attribute__ ((weak, alias("isr_none")));
void isr_software_configurable_flag_5(void) __attribute__ ((weak, alias("isr_none")));
void isr_software_configurable_flag_6(void) __attribute__ ((weak, alias("isr_none")));
void isr_software_configurable_flag_7(void) __attribute__ ((weak, alias("isr_none")));
void isr_ecsm_flash_bank_stall_abort(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_combined_error(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_0(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_1(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_2(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_3(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_4(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_5(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_6(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_7(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_8(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_9(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_10(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_11(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_12(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_13(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_14(void) __attribute__ ((weak, alias("isr_none")));
void isr_edma_channel_15(void) __attribute__ ((weak, alias("isr_none")));
void isr_swt_timeout(void) __attribute__ ((weak, alias("isr_none")));
void isr_stm_match_on_channel_0(void) __attribute__ ((weak, alias("isr_none")));
void isr_stm_match_on_channel_1(void) __attribute__ ((weak, alias("isr_none")));
void isr_stm_match_on_channel_2(void) __attribute__ ((weak, alias("isr_none")));
void isr_stm_match_on_channel_3(void) __attribute__ ((weak, alias("isr_none")));
void isr_ecc_dbd(void) __attribute__ ((weak, alias("isr_none")));
void isr_ecc_sbc(void) __attribute__ ((weak, alias("isr_none")));
void isr_rtc_api_rtc(void) __attribute__ ((weak, alias("isr_none")));
void isr_rtc_api_api(void) __attribute__ ((weak, alias("isr_none")));
void isr_siul_0(void) __attribute__ ((weak, alias("isr_none")));
void isr_siul_1(void) __attribute__ ((weak, alias("isr_none")));
void isr_siul_2(void) __attribute__ ((weak, alias("isr_none")));
void isr_wkpu_irq_0(void) __attribute__ ((weak, alias("isr_none")));
void isr_wkpu_irq_1(void) __attribute__ ((weak, alias("isr_none")));
void isr_wkpu_irq_2(void) __attribute__ ((weak, alias("isr_none")));
void isr_wkpu_irq_3(void) __attribute__ ((weak, alias("isr_none")));
void isr_mc_me_safe_mode(void) __attribute__ ((weak, alias("isr_none")));
void isr_mc_me_mode_transition(void) __attribute__ ((weak, alias("isr_none")));
void isr_mc_me_invalid_mode(void) __attribute__ ((weak, alias("isr_none")));
void isr_mc_me_invalid_mode_config(void) __attribute__ ((weak, alias("isr_none")));
void isr_fxosc_counter_expired_ipi_int_osc_fxosc(void) __attribute__ ((weak, alias("isr_none")));
void isr_pit_channel_0(void) __attribute__ ((weak, alias("isr_none")));
void isr_pit_channel_1(void) __attribute__ ((weak, alias("isr_none")));
void isr_pit_channel_2(void) __attribute__ ((weak, alias("isr_none")));
void isr_flexcan_0_esr_err(void) __attribute__ ((weak, alias("isr_none")));
void isr_flexcan_0_buf_00_03(void) __attribute__ ((weak, alias("isr_none")));
void isr_flexcan_0_buf_04_07(void) __attribute__ ((weak, alias("isr_none")));
void isr_flexcan_0_buf_08_11(void) __attribute__ ((weak, alias("isr_none")));
void isr_flexcan_0_buf_12_15(void) __attribute__ ((weak, alias("isr_none")));
void isr_flexcan_0_buf_16_31(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_0_sr_eoqf(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_0_sr_tfff(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_0_sr_tcf(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_0_sr_rfdf(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_0_rxi(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_0_txi(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_0_err(void) __attribute__ ((weak, alias("isr_none")));
void isr_adc_1_eoc(void) __attribute__ ((weak, alias("isr_none")));
void isr_adc_1_wd(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_1_sr_eoqf(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_1_sr_tfff(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_1_sr_tcf(void) __attribute__ ((weak, alias("isr_none")));
void isr_dspi_1_sr_rfdf(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_1_rxi(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_1_txi(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_1_err(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_2_rxi(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_2_txi(void) __attribute__ ((weak, alias("isr_none")));
void isr_linflex_2_err(void) __attribute__ ((weak, alias("isr_none")));
void isr_pit_channel_3(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f0_f1(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f2_f3(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f4_f5(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f6_f7(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f8_f9(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f10_f11(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f12_f13(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f14_f15(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f16_f17(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f18_f19(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f20_f21(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f22_f23(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f24_f25(void) __attribute__ ((weak, alias("isr_none")));
void isr_emios_0_gfr_f26_f27(void) __attribute__ ((weak, alias("isr_none")));

/* Vector table with all external (IVOR4) interrupt service
   routines. */
__attribute__ ((section(".vector"), used))
void (*vector_table[])(void) = {
    isr_software_configurable_flag_0,
    isr_software_configurable_flag_1,
    isr_software_configurable_flag_2,
    isr_software_configurable_flag_3,
    isr_software_configurable_flag_4,
    isr_software_configurable_flag_5,
    isr_software_configurable_flag_6,
    isr_software_configurable_flag_7,
    isr_none,
    isr_ecsm_flash_bank_stall_abort,
    isr_edma_combined_error,
    isr_edma_channel_0,
    isr_edma_channel_1,
    isr_edma_channel_2,
    isr_edma_channel_3,
    isr_edma_channel_4,
    isr_edma_channel_5,
    isr_edma_channel_6,
    isr_edma_channel_7,
    isr_edma_channel_8,
    isr_edma_channel_9,
    isr_edma_channel_10,
    isr_edma_channel_11,
    isr_edma_channel_12,
    isr_edma_channel_13,
    isr_edma_channel_14,
    isr_edma_channel_15,
    isr_none,
    isr_swt_timeout,
    isr_none,
    isr_stm_match_on_channel_0,
    isr_stm_match_on_channel_1,
    isr_stm_match_on_channel_2,
    isr_stm_match_on_channel_3,
    isr_none,
    isr_ecc_dbd,
    isr_ecc_sbc,
    isr_none,
    isr_rtc_api_rtc,
    isr_rtc_api_api,
    isr_none,
    isr_siul_0,
    isr_siul_1,
    isr_siul_2,
    isr_none,
    isr_none,
    isr_wkpu_irq_0,
    isr_wkpu_irq_1,
    isr_wkpu_irq_2,
    isr_wkpu_irq_3,
    isr_none,
    isr_mc_me_safe_mode,
    isr_mc_me_mode_transition,
    isr_mc_me_invalid_mode,
    isr_mc_me_invalid_mode_config,
    isr_none,
    isr_none,
    isr_fxosc_counter_expired_ipi_int_osc_fxosc,
    isr_none,
    isr_pit_channel_0,
    isr_pit_channel_1,
    isr_pit_channel_2,
    isr_none,
    isr_none,
    isr_none,
    isr_flexcan_0_esr_err,
    isr_none,
    isr_none,
    isr_flexcan_0_buf_00_03,
    isr_flexcan_0_buf_04_07,
    isr_flexcan_0_buf_08_11,
    isr_flexcan_0_buf_12_15,
    isr_flexcan_0_buf_16_31,
    isr_none,
    isr_none,
    isr_dspi_0_sr_eoqf,
    isr_dspi_0_sr_tfff,
    isr_dspi_0_sr_tcf,
    isr_dspi_0_sr_rfdf,
    isr_linflex_0_rxi,
    isr_linflex_0_txi,
    isr_linflex_0_err,
    isr_adc_1_eoc,
    isr_none,
    isr_adc_1_wd,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_dspi_1_sr_eoqf,
    isr_dspi_1_sr_tfff,
    isr_dspi_1_sr_tcf,
    isr_dspi_1_sr_rfdf,
    isr_linflex_1_rxi,
    isr_linflex_1_txi,
    isr_linflex_1_err,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_linflex_2_rxi,
    isr_linflex_2_txi,
    isr_linflex_2_err,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_pit_channel_3,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_emios_0_gfr_f0_f1,
    isr_emios_0_gfr_f2_f3,
    isr_emios_0_gfr_f4_f5,
    isr_emios_0_gfr_f6_f7,
    isr_emios_0_gfr_f8_f9,
    isr_emios_0_gfr_f10_f11,
    isr_emios_0_gfr_f12_f13,
    isr_emios_0_gfr_f14_f15,
    isr_emios_0_gfr_f16_f17,
    isr_emios_0_gfr_f18_f19,
    isr_emios_0_gfr_f20_f21,
    isr_emios_0_gfr_f22_f23,
    isr_emios_0_gfr_f24_f25,
    isr_emios_0_gfr_f26_f27
};

#endif

void spc5_init(void)
{
    SPC5_MC_CGM->FMPLL.CR = (SPC5_MC_CGM_FMPLL_CR_IDF(1)
                             | SPC5_MC_CGM_FMPLL_CR_ODF(2)
                             | SPC5_MC_CGM_FMPLL_CR_NDIV(48));
    SPC5_MC_ME->DRUN_MC = 0x001f00f2;
    SPC5_MC_ME->MCTL = 0x30005af0;
    SPC5_MC_ME->MCTL = 0x3000a50f;

    /* Wait for PLL to become stable. */
    while ((SPC5_MC_ME->GS & 0x40) == 0);

    SPC5_MC_ME->DRUN_MC = 0x001f00f4;

    /* See page 184 in the reference manual. */
    SPC5_MC_ME->RUN_PC[0] = 0x8;
    SPC5_MC_ME->PCTL[48] = 0;
    SPC5_MC_ME->PCTL[68] = 0;
    SPC5_MC_ME->MCTL = 0x30005af0;
    SPC5_MC_ME->MCTL = 0x3000a50f;

    /* Disable the watchdog. */
    SPC5_SWT->SR = 0x0000c520;
    SPC5_SWT->SR = 0x0000d928;
    SPC5_SWT->CR = 0xff00000a;

#if CONFIG_SYSTEM_INTERRUPTS == 1
    /* Software vector mode. */
    SPC5_INTC->MCR = 0;
    SPC5_INTC->IACKR = (uint32_t)vector_table;
    SPC5_INTC->CPR = 0;
#endif
}
