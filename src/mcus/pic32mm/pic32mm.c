/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

/* Defined in the linker script. */
extern uint32_t __text_end;
extern uint32_t __relocate_begin;
extern uint32_t __relocate_end;
extern uint32_t __bss_begin;
extern uint32_t __bss_end;

#define WEAK_ISR_NONE(name)                                             \
    void isr_ ## name (int vector_number) __attribute__ ((weak, alias("isr_none")));

/**
 * Panic if no interrupt service routine is installed in the interrupt
 * vector.
 */
void isr_none(int vector_number)
{
    sys_panic(FSTR("unhandled interrupt %d"), vector_number);
}

WEAK_ISR_NONE(core_timer);
WEAK_ISR_NONE(core_software_0);
WEAK_ISR_NONE(core_software_1);
WEAK_ISR_NONE(external_0);
WEAK_ISR_NONE(external_1);
WEAK_ISR_NONE(external_2);
WEAK_ISR_NONE(external_3);
WEAK_ISR_NONE(external_4);
WEAK_ISR_NONE(porta_change_notification);
WEAK_ISR_NONE(portb_change_notification);
WEAK_ISR_NONE(portc_change_notification);
WEAK_ISR_NONE(portd_change_notification);
WEAK_ISR_NONE(timer1);
WEAK_ISR_NONE(timer2);
WEAK_ISR_NONE(timer3);
WEAK_ISR_NONE(comparator_1);
WEAK_ISR_NONE(comparator_2);
WEAK_ISR_NONE(comparator_3);
WEAK_ISR_NONE(usb);
WEAK_ISR_NONE(real_time_clock_alarm);
WEAK_ISR_NONE(adc_conversion);
WEAK_ISR_NONE(high_low_voltage_detect);
WEAK_ISR_NONE(logic_cell_1);
WEAK_ISR_NONE(logic_cell_2);
WEAK_ISR_NONE(logic_cell_3);
WEAK_ISR_NONE(logic_cell_4);
WEAK_ISR_NONE(spi1_error);
WEAK_ISR_NONE(spi1_transmission);
WEAK_ISR_NONE(spi1_reception);
WEAK_ISR_NONE(spi2_error);
WEAK_ISR_NONE(spi2_transmission);
WEAK_ISR_NONE(spi2_reception);
WEAK_ISR_NONE(spi3_error);
WEAK_ISR_NONE(spi3_transmission);
WEAK_ISR_NONE(spi3_reception);
WEAK_ISR_NONE(uart1_reception);
WEAK_ISR_NONE(uart1_transmission);
WEAK_ISR_NONE(uart1_error);
WEAK_ISR_NONE(uart2_reception);
WEAK_ISR_NONE(uart2_transmission);
WEAK_ISR_NONE(uart2_error);
WEAK_ISR_NONE(uart3_reception);
WEAK_ISR_NONE(uart3_transmission);
WEAK_ISR_NONE(uart3_error);
WEAK_ISR_NONE(i2c1_slave);
WEAK_ISR_NONE(i2c1_master);
WEAK_ISR_NONE(i2c1_bus_collision);
WEAK_ISR_NONE(i2c2_slave);
WEAK_ISR_NONE(i2c2_master);
WEAK_ISR_NONE(i2c2_bus_collision);
WEAK_ISR_NONE(i2c3_slave);
WEAK_ISR_NONE(i2c3_master);
WEAK_ISR_NONE(i2c3_bus_collision);
WEAK_ISR_NONE(ccp1_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp1_timer);
WEAK_ISR_NONE(ccp2_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp2_timer);
WEAK_ISR_NONE(ccp3_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp3_timer);
WEAK_ISR_NONE(ccp4_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp4_timer);
WEAK_ISR_NONE(ccp5_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp5_timer);
WEAK_ISR_NONE(ccp6_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp6_timer);
WEAK_ISR_NONE(ccp7_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp7_timer);
WEAK_ISR_NONE(ccp8_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp8_timer);
WEAK_ISR_NONE(ccp9_input_capture_or_output_compare);
WEAK_ISR_NONE(ccp9_timer);
WEAK_ISR_NONE(frc_auto_tune);
WEAK_ISR_NONE(nvm_program_or_erase_complete);
WEAK_ISR_NONE(core_performance_counter);
WEAK_ISR_NONE(single_bit_ecc_error);
WEAK_ISR_NONE(dma_channel_0);
WEAK_ISR_NONE(dma_channel_1);
WEAK_ISR_NONE(dma_channel_2);
WEAK_ISR_NONE(dma_channel_3);

/* Vector table of unmasked hardware or software interrupt signals. */
void (*const vector_table[])(int vector_number) = {
    isr_core_timer,
    isr_core_software_0,
    isr_core_software_1,
    isr_external_0,
    isr_external_1,
    isr_external_2,
    isr_external_3,
    isr_external_4,
    isr_porta_change_notification,
    isr_portb_change_notification,
    isr_portc_change_notification,
    isr_portd_change_notification,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_timer1,
    isr_timer2,
    isr_timer3,
    isr_none,
    isr_none,
    isr_none,
    isr_comparator_1,
    isr_comparator_2,
    isr_comparator_3,
    isr_none,
    isr_none,
    isr_none,
    isr_usb,
    isr_none,
    isr_none,
    isr_real_time_clock_alarm,
    isr_adc_conversion,
    isr_none,
    isr_none,
    isr_high_low_voltage_detect,
    isr_logic_cell_1,
    isr_logic_cell_2,
    isr_logic_cell_3,
    isr_logic_cell_4,
    isr_spi1_error,
    isr_spi1_transmission,
    isr_spi1_reception,
    isr_spi2_error,
    isr_spi2_transmission,
    isr_spi2_reception,
    isr_spi3_error,
    isr_spi3_transmission,
    isr_spi3_reception,
    isr_none,
    isr_none,
    isr_none,
    isr_uart1_reception,
    isr_uart1_transmission,
    isr_uart1_error,
    isr_uart2_reception,
    isr_uart2_transmission,
    isr_uart2_error,
    isr_uart3_reception,
    isr_uart3_transmission,
    isr_uart3_error,
    isr_none,
    isr_none,
    isr_none,
    isr_i2c1_slave,
    isr_i2c1_master,
    isr_i2c1_bus_collision,
    isr_i2c2_slave,
    isr_i2c2_master,
    isr_i2c2_bus_collision,
    isr_i2c3_slave,
    isr_i2c3_master,
    isr_i2c3_bus_collision,
    isr_ccp1_input_capture_or_output_compare,
    isr_ccp1_timer,
    isr_ccp2_input_capture_or_output_compare,
    isr_ccp2_timer,
    isr_ccp3_input_capture_or_output_compare,
    isr_ccp3_timer,
    isr_ccp4_input_capture_or_output_compare,
    isr_ccp4_timer,
    isr_ccp5_input_capture_or_output_compare,
    isr_ccp5_timer,
    isr_ccp6_input_capture_or_output_compare,
    isr_ccp6_timer,
    isr_ccp7_input_capture_or_output_compare,
    isr_ccp7_timer,
    isr_ccp8_input_capture_or_output_compare,
    isr_ccp8_timer,
    isr_ccp9_input_capture_or_output_compare,
    isr_ccp9_timer,
    isr_frc_auto_tune,
    isr_none,
    isr_nvm_program_or_erase_complete,
    isr_core_performance_counter,
    isr_none,
    isr_single_bit_ecc_error,
    isr_dma_channel_0,
    isr_dma_channel_1,
    isr_dma_channel_2,
    isr_dma_channel_3
};

__attribute__ ((section(".confbits"), used))
struct pic32mm_conf_t configuration = {
    .RESERVED0 = 0xffffffff,
    .FDEVOPT = (PIC32MM_CONF_FDEVOPT_RESERVED
                | PIC32MM_CONF_FDEVOPT_USERID(0xffff)
                | PIC32MM_CONF_FDEVOPT_FUSBIDIO
                | PIC32MM_CONF_FDEVOPT_ALTI2C
                | PIC32MM_CONF_FDEVOPT_SOSCHP),
    .FICD = (PIC32MM_CONF_FICD_RESERVED
             | PIC32MM_CONF_FICD_ICS(2)),
    .FPOR = (PIC32MM_CONF_FPOR_RESERVED
             | PIC32MM_CONF_FPOR_LPBOREN
             | PIC32MM_CONF_FPOR_RETVR
             | PIC32MM_CONF_FPOR_BOREN(3)),
    .FWDT = (PIC32MM_CONF_FWDT_RESERVED
             | PIC32MM_CONF_FWDT_RCLKSEL(3)
             | PIC32MM_CONF_FWDT_RWDTPS(20)
             | PIC32MM_CONF_FWDT_WINDIS
             | PIC32MM_CONF_FWDT_FWDTWINSZ(3)
             | PIC32MM_CONF_FWDT_SWDTPS(20)),
    .FOSCSEL = (PIC32MM_CONF_FOSCSEL_RESERVED
                | PIC32MM_CONF_FOSCSEL_FCKSM(1)
                | PIC32MM_CONF_FOSCSEL_OSCIOFNC
                | PIC32MM_CONF_FOSCSEL_POSCMOD(3)
                | PIC32MM_CONF_FOSCSEL_IESO
                | PIC32MM_CONF_FOSCSEL_PLLSRC),
    .FSEC = (PIC32MM_CONF_FSEC_CP
             | PIC32MM_CONF_FSEC_RESERVED)
};

void pic32mm_relocate_init(void)
{
    uint32_t *relocate_begin_p;
    uint32_t *text_end_p;
    size_t i;
    size_t size;

    relocate_begin_p = &__relocate_begin;
    text_end_p = &__text_end;
    size = (&__relocate_end - relocate_begin_p);

    for (i = 0; i < size; i++) {
        relocate_begin_p[i] = text_end_p[i];
    }
}

void pic32mm_bss_init(void)
{
    size_t i;
    size_t size;

    size = (&__bss_end - &__bss_begin);

    for (i = 0; i < size; i++) {
        (&__bss_begin)[i] = 0;
    }
}

uint32_t pic32mm_reg_read(volatile uint32_t *reg_p)
{
    return (*reg_p);
}

void pic32mm_reg_write(volatile uint32_t *reg_p, uint32_t value)
{
    *reg_p = value;
}
