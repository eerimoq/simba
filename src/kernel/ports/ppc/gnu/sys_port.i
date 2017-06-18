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

#if CONFIG_SYS_RESET_CAUSE == 1

static void read_and_clear_reset_cause(void)
{
    uint16_t fes;
    uint16_t des;

    /* Get the reset cause. */
    fes = SPC5_MC_RGM->FES;
    des = SPC5_MC_RGM->DES;

    /* Clear the reset cause. */
    SPC5_MC_RGM->FES = 0xffff;
    SPC5_MC_RGM->DES = 0xffff;

    if (des & SPC5_MC_RGM_DES_F_POR) {
        module.reset_cause = sys_reset_cause_power_on_t;
    } else if (des & SPC5_MC_RGM_DES_F_SWT) {
        module.reset_cause = sys_reset_cause_watchdog_timeout_t;
    } else if (des & SPC5_MC_RGM_DES_F_LVD27_VREG) {
        module.reset_cause = sys_reset_cause_lvd27_vreg_t;
    } else if (des & SPC5_MC_RGM_DES_F_LVD27) {
        module.reset_cause = sys_reset_cause_lvd27_t;
    } else if (des & SPC5_MC_RGM_DES_F_LVD12_PD1) {
        module.reset_cause = sys_reset_cause_lvd12_pd1_t;
    } else if (des & SPC5_MC_RGM_DES_F_LVD12_PD0) {
        module.reset_cause = sys_reset_cause_lvd12_pd0_t;
    } else if (fes & SPC5_MC_RGM_FES_F_SOFT) {
        module.reset_cause = sys_reset_cause_software_t;
    } else if (fes & SPC5_MC_RGM_FES_F_JTAG) {
        module.reset_cause = sys_reset_cause_jtag_t;
    } else if (fes & SPC5_MC_RGM_FES_F_FLASH) {
        module.reset_cause = sys_reset_cause_flash_t;
    } else if (fes & SPC5_MC_RGM_FES_F_LVD45) {
        module.reset_cause = sys_reset_cause_lvd45_t;
    } else if (fes & SPC5_MC_RGM_FES_F_CMU_FHL) {
        module.reset_cause = sys_reset_cause_cmu_fhl_t;
    } else if (fes & SPC5_MC_RGM_FES_F_CMU_OLR) {
        module.reset_cause = sys_reset_cause_cmu_olr_t;
    } else if (fes & SPC5_MC_RGM_FES_F_FMPLL) {
        module.reset_cause = sys_reset_cause_fmpll_t;
    } else if (fes & SPC5_MC_RGM_FES_F_CHKSTOP) {
        module.reset_cause = sys_reset_cause_chkstop_t;
    } else if (fes & SPC5_MC_RGM_FES_F_CORE) {
        module.reset_cause = sys_reset_cause_core_t;
    } else if (fes & SPC5_MC_RGM_FES_F_EXR) {
        module.reset_cause = sys_reset_cause_external_t;
    } else {
        module.reset_cause = sys_reset_cause_unknown_t;
    }
}

#endif

static int sys_port_module_init(void)
{
    /* Start the system tick timer. */
    SPC5_STM->CHANNELS[0].CMP = SYS_TICK_COUNT;
    SPC5_STM->CHANNELS[0].CCR = SPC5_STM_CHANNELS_CCR_CEN;
    SPC5_STM->CR = SPC5_STM_CR_TEN;
    SPC5_INTC->PSR[30/4] = 0x00000100;

#if CONFIG_SYS_RESET_CAUSE == 1
    read_and_clear_reset_cause();
#endif

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
#if CONFIG_WATCHDOG == 1
    watchdog_start_ms(1, NULL);
#endif

    while (1);
}

static int sys_port_backtrace(void **buf_pp, size_t size)
{
    void *return_address_p;
    void *frame_address_p;
    void **frame_address_pp;
    int depth;
    int depth_max;

    return_address_p = __builtin_return_address(0);
    frame_address_p = __builtin_frame_address(1);
    depth = 0;
    depth_max = (size / sizeof(void*) / 2);

    buf_pp[depth] = return_address_p;
    buf_pp[depth + 1] = frame_address_p;
    depth++;

    frame_address_pp = (void **)frame_address_p;

    while ((frame_address_p != NULL) && (depth < depth_max)) {
        frame_address_p = *frame_address_pp;
        frame_address_pp = (void **)frame_address_p;

        if (*frame_address_pp == NULL) {
            break;
        }

        return_address_p = *(frame_address_pp + 1);
        buf_pp[2 * depth] = return_address_p;
        buf_pp[2 * depth + 1] = frame_address_p;
        depth++;
    }

    return (depth);
}

static int32_t sys_port_get_time_into_tick()
{
    uint32_t count;

    count = (SPC5_STM->CNT - (SPC5_STM->CHANNELS[0].CMP - SYS_TICK_COUNT));

    return (1000 * (count / (F_CPU / 1000000)));
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
