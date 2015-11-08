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

static void start_dac_hw(struct dac_driver_t *drv_p)
{
}

static int pdc_full()
{
    return (0);
}
 
static void pdc_write()
{

}

ISR(DAC_vect)
{
    struct dac_device_t *dev_p = &dac_device[0];
    struct dac_driver_t *drv_p = dev_p->jobs.head_p;

    if (drv_p->next.length > 0) {
        /* Add more samples to the PDC. */
        dev_p->regs_p->PDC.TNPR = (uint32_t)drv_p->next.samples;
        dev_p->regs_p->PDC.TNCR = drv_p->next.length;
        drv_p->next.length = 0;

        /* Resume the waiting thread if it wants to write additional
           samples. */
        if (drv_p->next.thrd_p != NULL) {
            thrd_resume_irq(drv_p->next.thrd_p, 0);
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
    dev_p->tc.regs_p->CHANNEL[dev_p->tc.channel].CMR = (TC_CMR_WAVEFORM_ACPC(2)
                                                        | TC_CMR_WAVEFORM_ACPA(1)
                                                        | TC_CMR_WAVEFORM_WAVE
                                                        | TC_CMR_WAVEFORM_WAVSEL_UPDOWN_RC
                                                        | TC_CMR_WAVEFORM_TCCLKS(1));
    rc = (F_CPU / 2 / sampling_rate);
    dev_p->tc.regs_p->CHANNEL[dev_p->tc.channel].RA = (rc / 2);
    dev_p->tc.regs_p->CHANNEL[dev_p->tc.channel].RC = rc;
    dev_p->tc.regs_p->CHANNEL[dev_p->tc.channel].CCR = TC_CCR_CLKEN;

    return (0);
}

static int dac_port_async_convert(struct dac_driver_t *drv_p,
                                  int *samples,
                                  size_t length)
{
    /* Initialize. */
    drv_p->next.samples = (uint16_t *)samples;
    drv_p->next.length = length;
    drv_p->next.thrd_p = NULL;
    drv_p->next_p = NULL;

    /* Enqueue. */
    sys_lock();

    if (drv_p->dev_p->jobs.head_p == NULL) {
        /* Empty queue. */
        drv_p->dev_p->jobs.head_p = drv_p;
        start_dac_hw(drv_p);
    } else if (drv_p->dev_p->jobs.head_p == drv_p) {
        /* Driver active. */
        if (pdc_full() == 1) {
            thrd_suspend_irq(NULL);
        }

        pdc_write();
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
    return (-1);
}

static int dac_port_convert(struct dac_driver_t *drv_p,
                            int *samples,
                            size_t length)
{
    dac_port_async_convert(drv_p, samples, length);
    dac_port_async_wait(drv_p);

    return (0);
}
