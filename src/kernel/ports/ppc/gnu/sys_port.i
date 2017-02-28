/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

extern int main();

#define SYS_TICK_COUNT           (F_CPU / CONFIG_SYSTEM_TICK_FREQUENCY)

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

static void sys_port_stop(int error)
{
    while (1);
}

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
