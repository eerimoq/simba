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

extern int main();

#define SYS_TICK_COUNT           (F_CPU / CONFIG_SYSTEM_TICK_FREQUENCY)

#if CONFIG_START_CONSOLE_DEVICE_INDEX == 0
#    define CONSOLE_UART_REGS                  SPC5_LINFLEX_0
#elif CONFIG_START_CONSOLE_DEVICE_INDEX == 1
#    define CONSOLE_UART_REGS                  SPC5_LINFLEX_1
#elif CONFIG_START_CONSOLE_DEVICE_INDEX == 2
#    define CONSOLE_UART_REGS                  SPC5_LINFLEX_2
#else
#    error "Bad console UART index."
#endif

ISR(stm_match_on_channel_0)
{
    /* Setup next timeout. */
    SPC5_STM->CHANNELS[0].CIR = SPC5_STM_CHANNELS_CIR_CIF;
    SPC5_STM->CHANNELS[0].CMP = (SPC5_STM->CNT + SYS_TICK_COUNT);

    sys_tick_isr();
}

static int sys_port_module_init(void)
{
    /* Start the system tick timer. */
    SPC5_STM->CHANNELS[0].CMP = SYS_TICK_COUNT;
    SPC5_STM->CHANNELS[0].CCR = SPC5_STM_CHANNELS_CCR_CEN;
    SPC5_STM->CR = SPC5_STM_CR_TEN;
    SPC5_INTC->PSR[30/4] = 0x00000100;

    return (0);
}

__attribute__ ((noreturn))
static void sys_port_stop(int error)
{
    while (1);
}

static void sys_port_panic_putc(char c)
{
    /* Write the next byte. */
    CONSOLE_UART_REGS->BDRL[3] = c;
    
    /* Wait for the byte to be transmitted. */
    while (((CONSOLE_UART_REGS->UARTSR & SPC5_LINFLEX_UARTSR_DTF) == 0));
    
    CONSOLE_UART_REGS->UARTSR = SPC5_LINFLEX_UARTSR_DTF;
}

__attribute__ ((noreturn))
static void sys_port_reboot()
{
    while (1);
}

static void sys_port_lock(void)
{
    asm volatile("wrteei 0");
}

static void sys_port_unlock(void)
{
    asm volatile("wrteei 1");
}

static void sys_port_lock_isr(void)
{
}

static void sys_port_unlock_isr(void)
{
}

static cpu_usage_t sys_port_interrupt_cpu_usage_get(void)
{
    return (0);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}
