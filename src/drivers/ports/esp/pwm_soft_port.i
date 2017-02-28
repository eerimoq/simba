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

#include "freertos/FreeRTOS.h"
#include "eagle_soc.h"

#include "drivers/internal.h"

static int frequency_to_hw_config(long frequency,
                                  long *duty_cycle_max_p)
{
    *duty_cycle_max_p = (F_CPU / frequency);

    return (0);
}

/**
 * Hardware timer interrput handler.
 */
static void isr(void *arg_p)
{
    PWM_SOFT_ISR_PROLOGUE;

    /* Clear the interrupt flag and start a new timer. */
    ESP8266_TIMER0->INT = ESP8266_TIMER_INT_CLR;
    ESP8266_TIMER0->LOAD = elem_p->delta;

    PWM_SOFT_ISR_EPILOGUE;
}

static int pwm_soft_port_module_init(long frequency)
{
    if (frequency_to_hw_config(frequency, &module.duty_cycle_max) != 0) {
        return (-1);
    }

    module.tail_pwm_soft.delta = DUTY_CYCLE_MAX;

    /* Configure and start the software PWM timer. */
    ESP8266_TIMER0->CTRL = (ESP8266_TIMER_CTRL_ENABLE
                            | ESP8266_TIMER_CTRL_PRESCALE_1
                            | ESP8266_TIMER_CTRL_INT_TRIGGER_EDGE);

    _xt_isr_attach(ESP8266_IRQ_NUM_TIMER1, isr, NULL);
    TM1_EDGE_INT_ENABLE();
    _xt_isr_unmask(1 << ESP8266_IRQ_NUM_TIMER1);
    ESP8266_TIMER0->LOAD = module.tail_pwm_soft.delta;

    return (0);
}

static int pwm_soft_port_set_frequency(long value)
{
    if (frequency_to_hw_config(value, &module.duty_cycle_max) != 0) {
        return (-1);
    }

    sys_lock();
    module.tail_pwm_soft.delta = DUTY_CYCLE_MAX;
    sys_unlock();

    return (0);
}
