/**
 * @file dac_port.i
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

#define STATE_CONVERTING 0
#define STATE_WAITING    1
#define STATE_EMPTY      2

static void dac_hw_write(struct dac_driver_t *drv_p)
{
    drv_p->dev_p->regs_p->PDC.TNPR = (uint32_t)drv_p->next.samples;
    drv_p->dev_p->regs_p->PDC.TNCR = drv_p->next.length;
    drv_p->next.length = 0;
}

static void start_dac_hw(struct dac_driver_t *drv_p)
{
    drv_p->dev_p->regs_p->CHER = (1 << drv_p->channel);
    dac_hw_write(drv_p);
}

ISR(DAC_vect)
{
    struct dac_device_t *dev_p = &dac_device[0];
    struct dac_driver_t *drv_p = dev_p->jobs.head_p;

    /* Add more samples to the PDC, if any. */
    if (drv_p->next.length > 0) {
        dac_hw_write(drv_p);

        /* Resume the waiting thread if it wants to write additional
           samples. */
        if (drv_p->thrd_p != NULL) {
            thrd_resume_irq(drv_p->thrd_p, 0);
        }
    }

    /* Act on empty PDC buffer. */
    if ((dev_p->regs_p->ISR & SAM_DACC_ISR_TXBUFE) != 0) {
        if (drv_p->state == STATE_WAITING) {
            thrd_resume_irq(drv_p->thrd_p, 0);
        } else {
            drv_p->state = STATE_EMPTY;
        }
    }
}

static int dac_port_module_init(void)
{
    return (0);
}

static int dac_port_init(struct dac_driver_t *drv_p,
                         struct dac_device_t *dev_p,
                         struct pin_device_t *pin_dev_p,
                         long sampling_rate)
{
    uint32_t rc;
    uint32_t mask;

    drv_p->state = STATE_EMPTY;
    drv_p->next.length = 0;

    if (pin_dev_p == &pin_dac0_dev) {
        drv_p->channel = 0;
    } else if (pin_dev_p == &pin_dac1_dev) {
        drv_p->channel = 1;
    } else {
        return (-1);
    }

    /* Setup the DACC clocked by Timer Counter 0, channel 1. */
    pmc_peripheral_clock_enable(dev_p->id);
    nvic_enable_interrupt(dev_p->id);

    dev_p->regs_p->MR = (SAM_DACC_MR_STARTUP(25)
                         | SAM_DACC_MR_TRGSEL(2)
                         | SAM_DACC_MR_TRGEN);

    /* Setup a Timer Counter to create the clock pulses to the
       DACC. */
    pmc_peripheral_clock_enable(dev_p->tc.id);

    /* Create a square wave of the desired frequency. UPDOWN_RC
       waveform is a triangle wave starting at zero and increasing to
       the value of the RC register, then it decrements to zero
       again. The tick frequency is MCR / 2, or 42 MHz. */
    dev_p->tc.regs_p->CHANNEL[1].CMR = (TC_CMR_WAVEFORM_ACPC(2)
                                        | TC_CMR_WAVEFORM_ACPA(1)
                                        | TC_CMR_WAVEFORM_WAVE
                                        | TC_CMR_WAVEFORM_WAVSEL_UPDOWN_RC
                                        | TC_CMR_WAVEFORM_TCCLKS(1));
    rc = (F_CPU / 2 / sampling_rate);
    dev_p->tc.regs_p->CHANNEL[1].RA = (rc / 2);
    dev_p->tc.regs_p->CHANNEL[1].RC = rc;
    dev_p->tc.regs_p->CHANNEL[1].CCR = TC_CCR_CLKEN;

    /* Configure the TX pin. */
    mask = pin_dev_p->mask;
    pin_dev_p->pio_p->PDR = mask;
    pin_dev_p->pio_p->ABSR |= mask;

    return (0);
}

static int dac_port_async_convert(struct dac_driver_t *drv_p,
                                  int *samples,
                                  size_t length)
{
    struct dac_device_t *dev_p = drv_p->dev_p;

    /* Enqueue. */
    sys_lock();

    drv_p->state = STATE_CONVERTING;

    /* Wait if last written data has not yet been written. */
    if (drv_p->next.length > 0) {
        drv_p->thrd_p = thrd_self();
        thrd_suspend_irq(NULL);
    }

    /* Initialize. */
    drv_p->next.samples = (uint16_t *)samples;
    drv_p->next.length = length;
    drv_p->next_p = NULL;

    if (dev_p->jobs.head_p == NULL) {
        /* Empty queue. */
        dev_p->jobs.head_p = drv_p;
        start_dac_hw(drv_p);
    } else {
        /* Non-empty queue. */
        drv_p->dev_p->jobs.tail_p->next_p = drv_p;
    }

    drv_p->dev_p->jobs.tail_p = drv_p;

    sys_unlock();

    return (0);
}

static int dac_port_async_wait(struct dac_driver_t *drv_p)
{
    sys_lock();

    if (drv_p->state == STATE_CONVERTING) {
        drv_p->state = STATE_WAITING;
        drv_p->thrd_p = thrd_self();
        thrd_suspend_irq(NULL);
    }

    sys_unlock();

    return (0);
}

static int dac_port_convert(struct dac_driver_t *drv_p,
                            int *samples,
                            size_t length)
{
    dac_port_async_convert(drv_p, samples, length);
    dac_port_async_wait(drv_p);

    return (0);
}
