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

#include <avr/interrupt.h>

/**
 * Calculate timer configuration parameters from cpu frequency and
 * desired tick frequency. Select closest match above desired tick
 * frequency.
 */
static int frequency_to_hw_config(long frequency,
                                  uint8_t *clock_select_p,
                                  long *count_max_p)
{
    long cpu_cycles_per_sys_tick;

    cpu_cycles_per_sys_tick = (F_CPU / frequency);

    if (cpu_cycles_per_sys_tick == 0) {
        return (-1);
    } else if (cpu_cycles_per_sys_tick < 65536L) {
        *clock_select_p = _BV(CS10);
        *count_max_p = (cpu_cycles_per_sys_tick - 1);
    } else if (cpu_cycles_per_sys_tick < (8L * 65536L)) {
        *clock_select_p = _BV(CS11);
        *count_max_p = (DIV_CEIL(cpu_cycles_per_sys_tick, 8) - 1);
    } else if (cpu_cycles_per_sys_tick < (64L * 65536L)) {
        *clock_select_p = (_BV(CS11) | _BV(CS10));
        *count_max_p = (DIV_CEIL(cpu_cycles_per_sys_tick, 64) - 1);
    } else if (cpu_cycles_per_sys_tick < (256L * 65536L)) {
        *clock_select_p = _BV(CS12);
        *count_max_p = (DIV_CEIL(cpu_cycles_per_sys_tick, 256) - 1);
    } else if (cpu_cycles_per_sys_tick < (1024L * 65536L)) {
        *clock_select_p = (_BV(CS12) | _BV(CS10));
        *count_max_p = (DIV_CEIL(cpu_cycles_per_sys_tick, 1024) - 1);
    } else {
        return (-1);
    }

    return (0);
}

static void reset_timer_isr(uint16_t count)
{
    /* Stop the timer to ensure that no compare match is missed. */
    TCCR1B = 0;

    /* Configure the timer. */
    TCNT1 = 0;
    OCR1A = count;

    /* Start the timer. */
    TCCR1B = module.port.clock_select;
}

ISR(TIMER1_COMPA_vect)
{
    struct pwm_soft_driver_t *elem_p;

    /* Set the next timeout timer count early for better accuracy. */
    elem_p = module.current_p->next_p;

    while (elem_p->delta == 0) {
        elem_p = elem_p->next_p;
    }
    
    reset_timer_isr(elem_p->delta);

    /* Update pin values. */
    elem_p = module.current_p;

    if (elem_p != &module.tail_pwm_soft) {
        do {
            pin_device_write_low(elem_p->pin_dev_p);
            elem_p = elem_p->next_p;
        } while (elem_p->delta == 0);
    } else {
        /* One period has elapsed. Set all PWM:s high. */
        elem_p = module.head_p;

        while (elem_p != &module.tail_pwm_soft) {
            pin_device_write_high(elem_p->pin_dev_p);
            elem_p = elem_p->next_p;
        }

        elem_p = module.tail_pwm_soft.next_p;
    }            

    /* Unlink stopped elements. */
    if (module.current_p->thrd_p != NULL) {
        unlink_pwm_soft_isr(module.current_p);
        thrd_resume_isr(module.current_p->thrd_p, 0);
        module.current_p->thrd_p = NULL;
    }

    /* Save current element. */
    module.current_p = elem_p;
}

static int pwm_soft_port_module_init(long frequency)
{
    if (frequency_to_hw_config(frequency,
                               &module.port.clock_select,
                               &module.duty_cycle_max) != 0) {
        return (-1);
    }

    module.tail_pwm_soft.delta = DUTY_CYCLE_MAX;

    /* Enable the timer interrput. */
    TCCR1A = 0;
    reset_timer_isr(module.tail_pwm_soft.delta);
    TIMSK1 = _BV(OCIE1A);

    return (0);
}

static int pwm_soft_port_set_frequency(long value)
{
    if (frequency_to_hw_config(value,
                               &module.port.clock_select,
                               &module.duty_cycle_max) != 0) {
        return (-1);
    }

    sys_lock();
    module.tail_pwm_soft.delta = DUTY_CYCLE_MAX;
    sys_unlock();
    
    return (0);
}
