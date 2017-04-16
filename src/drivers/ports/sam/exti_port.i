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

/**
 * Interrupt handler. All interrupt flags are cleared when ISR is
 * read. That means, all interrupts have to be handled in this
 * interrupt.
 */
static void isr(volatile struct sam_pio_t *pio_p,
                struct exti_device_t *dev_p,
                int max)
{
    int i;
    uint32_t isr = pio_p->ISR;

    for (i = 0; i < max; i++, dev_p++) {
        if (isr & (1 << i)) {
            if (dev_p->drv_p == NULL) {
                continue;
            }

            dev_p->drv_p->on_interrupt(dev_p->drv_p->arg_p);
        }
    }
}

#if (EXTI_DEVICE_MAX > 0)
ISR(pioa) {
    isr(SAM_PIOA, &exti_device[0], 30);
}
#endif

#if (EXTI_DEVICE_MAX > 29)
ISR(piob) {
    isr(SAM_PIOB, &exti_device[30], 32);
}
#endif

#if (EXTI_DEVICE_MAX > 61)
ISR(pioc) {
    isr(SAM_PIOC, &exti_device[62], 31);
}
#endif

#if (EXTI_DEVICE_MAX > 92)
ISR(piod) {
    isr(SAM_PIOD, &exti_device[93], 10);
}
#endif

#if (EXTI_DEVICE_MAX > 128)
ISR(pioe) {
    isr(SAM_PIOE, &exti_device[128]);
}
#endif

#if (EXTI_DEVICE_MAX > 160)
ISR(piof) {
    isr(SAM_PIOF, &exti_device[160]);
}
#endif

static int exti_port_module_init()
{
#if (EXTI_DEVICE_MAX > 0)
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOA);
    nvic_enable_interrupt(PERIPHERAL_ID_PIOA);
#endif
#if (EXTI_DEVICE_MAX > 29)
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOB);
    nvic_enable_interrupt(PERIPHERAL_ID_PIOB);
#endif
#if (EXTI_DEVICE_MAX > 61)
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOC);
    nvic_enable_interrupt(PERIPHERAL_ID_PIOC);
#endif
#if (EXTI_DEVICE_MAX > 92)
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOD);
    nvic_enable_interrupt(PERIPHERAL_ID_PIOD);
#endif
#if (EXTI_DEVICE_MAX > 128)
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOE);
    nvic_enable_interrupt(PERIPHERAL_ID_PIOE);
#endif
#if (EXTI_DEVICE_MAX > 160)
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOF);
    nvic_enable_interrupt(PERIPHERAL_ID_PIOF);
#endif

    return (0);
}

static int exti_port_start(struct exti_driver_t *self_p)
{
    volatile struct pin_device_t *dev_p = self_p->dev_p->pin_dev_p;

    self_p->dev_p->drv_p = self_p;

    /* Enable the interrupt. */
    dev_p->pio_p->PER |= dev_p->mask;

    if (self_p->trigger == EXTI_TRIGGER_FALLING_EDGE) {
        dev_p->pio_p->AIMER |= dev_p->mask;
        dev_p->pio_p->ESR |= dev_p->mask;
        dev_p->pio_p->FELLSR |= dev_p->mask;
    } else if (self_p->trigger == EXTI_TRIGGER_RISING_EDGE) {
        dev_p->pio_p->AIMER |= dev_p->mask;
        dev_p->pio_p->ESR |= dev_p->mask;
        dev_p->pio_p->REHLSR |= dev_p->mask;
    } else if (self_p->trigger == EXTI_PORT_TRIGGER_BOTH_EDGES) {
        /* Default will trigger on both rising and falling edges */
    } else {
        return (-1);
    }

    dev_p->pio_p->ISR |= dev_p->mask;
    dev_p->pio_p->ISR;
    dev_p->pio_p->IER |= dev_p->mask;
    
    return (0);
}

static int exti_port_stop(struct exti_driver_t *self_p)
{
    /* Disable the interrupt. */
    self_p->dev_p->pin_dev_p->pio_p->IDR |= self_p->dev_p->pin_dev_p->mask;

    return (0);
}

static int exti_port_clear(struct exti_driver_t *self_p)
{
    return (0);
}
