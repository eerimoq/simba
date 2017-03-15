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

#if CONFIG_PWM_SOFT == 1

#define DUTY_CYCLE_MAX module.duty_cycle_max

struct module_t {
    struct pwm_soft_port_module_t port;
    int initialized;
    struct pwm_soft_driver_t tail_pwm_soft;
    struct pwm_soft_driver_t *head_p;
    /* A circular linked list of started devices (and this modules'
       dummy software pwm). */
    struct pwm_soft_driver_t *current_p;
    long frequency;
    long duty_cycle_max;
};

static struct module_t module;

static void unlink_pwm_soft_isr(struct pwm_soft_driver_t *self_p);

#include "pwm_soft_port.i"

/**
 * Insert given object into the global list of software PWM objects.
 */
static void insert_pwm_soft(struct pwm_soft_driver_t *self_p)
{
    struct pwm_soft_driver_t *elem_p, *prev_p;

    sys_lock();

    /* Find element preceeding this one.*/
    elem_p = module.head_p;
    prev_p = &module.tail_pwm_soft;

    /* Delta is initially the timeout. */
    while (elem_p->delta < self_p->delta) {
        self_p->delta -= elem_p->delta;
        prev_p = elem_p;
        elem_p = elem_p->next_p;
    }

    /* Insert new software PWM into list.*/
    elem_p->delta -= self_p->delta;
    self_p->next_p = elem_p;
    prev_p->next_p = self_p;

    if (prev_p == &module.tail_pwm_soft) {
        module.head_p = self_p;
    }

    sys_unlock();
}

/**
 * Unlink given software PWM from the list of started software PWM:s.
 */
static void unlink_pwm_soft_isr(struct pwm_soft_driver_t *self_p)
{
    struct pwm_soft_driver_t *prev_p;

    /* Add the delta to the next element. */
    self_p->next_p->delta += self_p->delta;

    /* Find element preceeding this one.*/
    prev_p = module.head_p;

    while (prev_p->next_p != self_p) {
        prev_p = prev_p->next_p;
    }

    /* Remove software PWM from the list.*/
    prev_p->next_p = self_p->next_p;

    if (self_p == module.head_p) {
        module.head_p = self_p->next_p;
    }

    self_p->next_p = NULL;
}

int pwm_soft_module_init(long frequency)
{
    if (module.initialized == 1) {
        return (0);
    }

    module.tail_pwm_soft.duty_cycle = -1;
    module.tail_pwm_soft.thrd_p = NULL;
    module.tail_pwm_soft.next_p = &module.tail_pwm_soft;
    module.head_p = &module.tail_pwm_soft;
    module.current_p = &module.tail_pwm_soft;
    module.frequency = frequency;

    if (pwm_soft_port_module_init(frequency) != 0) {
        return (-1);
    }

    module.initialized = 1;

    return (0);
}

int pwm_soft_set_frequency(long value)
{
    /* It's not allowed to change the frequency with started software
       PWM drivers. */
    if (module.head_p != &module.tail_pwm_soft) {
        return (-1);
    }

    module.frequency = value;

    return (pwm_soft_port_set_frequency(value));
}

long pwm_soft_get_frequency(void)
{
    return (module.frequency);
}

int pwm_soft_init(struct pwm_soft_driver_t *self_p,
                  struct pin_device_t *pin_dev_p,
                  long duty_cycle)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(pin_dev_p != NULL, EINVAL);
    ASSERTN((duty_cycle >= 0) && (duty_cycle <= DUTY_CYCLE_MAX), EINVAL);

    self_p->pin_dev_p = pin_dev_p;
    self_p->duty_cycle = duty_cycle;
    self_p->delta = 0;
    self_p->thrd_p = NULL;
    self_p->next_p = NULL;

    return (0);
}

int pwm_soft_start(struct pwm_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    pin_device_set_mode(self_p->pin_dev_p, PIN_OUTPUT);

    if (self_p->duty_cycle == DUTY_CYCLE_MAX) {
        pin_device_write_high(self_p->pin_dev_p);
    } else {
        pin_device_write_low(self_p->pin_dev_p);

        if (self_p->duty_cycle > 0) {
            self_p->delta = self_p->duty_cycle;
            insert_pwm_soft(self_p);
        }
    }

    return (0);
}

int pwm_soft_stop(struct pwm_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    if (!((self_p->duty_cycle == DUTY_CYCLE_MAX) ||
          (self_p->duty_cycle == 0))) {
        sys_lock();

        /* Is the timer started? */
        if (self_p->next_p != NULL) {
            /* Wait for the PWM to be inactive. */
            if (self_p == module.current_p) {
                self_p->thrd_p = thrd_self();
                thrd_suspend_isr(NULL);
            } else {
                unlink_pwm_soft_isr(self_p);
            }
        }

        sys_unlock();
    }

    pin_device_set_mode(self_p->pin_dev_p, PIN_INPUT);

    return (0);
}

int pwm_soft_set_duty_cycle(struct pwm_soft_driver_t *self_p,
                            long value)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN((value >= 0) && (value <= DUTY_CYCLE_MAX), EINVAL);

    if (pwm_soft_stop(self_p) != 0) {
        return (-1);
    }

    self_p->duty_cycle = value;

    return (pwm_soft_start(self_p));
}

unsigned int pwm_soft_get_duty_cycle(struct pwm_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->duty_cycle);
}

long pwm_soft_duty_cycle(int percentage)
{
    ASSERTN((percentage >= 0) && (percentage <= 100), EINVAL);

    return ((DUTY_CYCLE_MAX * percentage) / 100L);
}

int pwm_soft_duty_cycle_as_percent(long value)
{
    ASSERTN((value >= 0) && (value <= DUTY_CYCLE_MAX), EINVAL);

    return ((100L * value) / DUTY_CYCLE_MAX);
}

#endif
