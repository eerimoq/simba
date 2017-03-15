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

/* Driver states. */
#define STATE_RUNNING  0
#define STATE_FINISHED 1

static void start_adc_hw(struct adc_driver_t *self_p)
{
    volatile struct sam_adc_t *regs_p;

    regs_p = self_p->dev_p->regs_p;

    /* Initialize the PDC. */
    regs_p->PDC.RPR = (uint32_t)self_p->samples_p;
    regs_p->PDC.RCR = self_p->length;

    regs_p->CHER = (1 << self_p->channel);
    regs_p->IER = (SAM_ADC_IER_ENDRX);

    /* Start the convertion. */
    regs_p->CR = (SAM_ADC_CR_START);
}

static void stop_adc_hw(struct adc_driver_t *self_p)
{
    volatile struct sam_adc_t *regs_p;

    regs_p = self_p->dev_p->regs_p;

    /* Stop the AD Converter. */
    regs_p->CHDR = (1 << self_p->channel);
    regs_p->IDR = (SAM_ADC_IDR_ENDRX);
}

ISR(adc)
{
    struct adc_device_t *dev_p = &adc_device[0];
    struct adc_driver_t *self_p = dev_p->jobs.head_p;

    /* Mark the job as finished. */
    self_p->state = STATE_FINISHED;

    /* Detach the job from the job list as it is completed. */
    dev_p->jobs.head_p = self_p->next_p;

    /* Disable the ADC hardware. */
    stop_adc_hw(self_p);

    /* Start the next job, if there is one. */
    if (self_p->next_p != NULL) {
        start_adc_hw(self_p->next_p);
    }

    /* Resume the thread if it is waiting for completion. */
    if (self_p->thrd_p != NULL) {
        thrd_resume_isr(self_p->thrd_p, 0);
    }
}

static int adc_port_module_init(void)
{
    int channel;
    uint32_t rc;
    uint32_t sampling_rate = 10000;
    struct adc_device_t *dev_p = &adc_device[0];

    /* Setup a Timer Counter to send the clock pulses to the ADC. */
    pmc_peripheral_clock_enable(dev_p->tc.id);

    /* Create a square wave of the desired frequency. UPDOWN_RC
       waveform is a triangle wave starting at zero and increasing to
       the value of the RC register, then it decrements to zero
       again. The tick frequency is MCR / 8, or 10.25 MHz. */
    channel = dev_p->tc.channel;
    dev_p->tc.regs_p->CHANNEL[channel].CMR = (TC_CMR_WAVEFORM_ACPC(2)
                                              | TC_CMR_WAVEFORM_ACPA(1)
                                              | TC_CMR_WAVEFORM_WAVE
                                              | TC_CMR_WAVEFORM_WAVSEL_UP_RC
                                              | TC_CMR_WAVEFORM_TCCLKS(1));
    rc = (F_CPU / 8 / sampling_rate);
    dev_p->tc.regs_p->CHANNEL[channel].RA = (rc / 2);
    dev_p->tc.regs_p->CHANNEL[channel].RC = rc;
    dev_p->tc.regs_p->CHANNEL[channel].CCR = (TC_CCR_SWTRG | TC_CCR_CLKEN);

    /* Setup the ADC clocked by Timer Counter 0, channel 2. */
    pmc_peripheral_clock_enable(dev_p->id);
    nvic_enable_interrupt(dev_p->id);

    dev_p->regs_p->CR = (SAM_ADC_CR_SWRST);
    dev_p->regs_p->MR = (SAM_ADC_MR_STARTUP(8)
                         | SAM_ADC_MR_TRACKTIM(8)
                         | SAM_ADC_MR_TRGSEL(channel + 1)
                         | SAM_ADC_MR_TRGEN);

    /* Enable RX in the PDC. */
    dev_p->regs_p->PDC.PTCR = (PERIPH_PTCR_RXTEN);

    return (0);
}

static int adc_port_init(struct adc_driver_t *self_p,
                         struct adc_device_t *dev_p,
                         struct pin_device_t *pin_dev_p,
                         int reference,
                         long sampling_rate)
{
    uint32_t mask;

    self_p->dev_p = dev_p;

    /* Disable the pull-up. */
    mask = pin_dev_p->mask;
    pin_dev_p->pio_p->PDR = mask;
    pin_dev_p->pio_p->PUDR = mask;
    pin_dev_p->pio_p->ABSR |= mask;

    if (pin_dev_p == &pin_a0_dev) {
        self_p->channel = 7;
    } else if (pin_dev_p == &pin_a1_dev) {
        self_p->channel = 6;
    } else if (pin_dev_p == &pin_a2_dev) {
        self_p->channel = 5;
    } else if (pin_dev_p == &pin_a3_dev) {
        self_p->channel = 4;
    } else if (pin_dev_p == &pin_a4_dev) {
        self_p->channel = 3;
    } else if (pin_dev_p == &pin_a5_dev) {
        self_p->channel = 2;
    } else if (pin_dev_p == &pin_a6_dev) {
        self_p->channel = 1;
    } else if (pin_dev_p == &pin_a7_dev) {
        self_p->channel = 0;
    } else if (pin_dev_p == &pin_a8_dev) {
        self_p->channel = 10;
    } else if (pin_dev_p == &pin_a9_dev) {
        self_p->channel = 11;
    } else if (pin_dev_p == &pin_a10_dev) {
        self_p->channel = 12;
    } else if (pin_dev_p == &pin_a11_dev) {
        self_p->channel = 13;
    }

    return (0);
}

static int adc_port_async_convert(struct adc_driver_t *self_p,
                                  uint16_t *samples_p,
                                  size_t length)
{
    /* Initialize. */
    self_p->state = STATE_RUNNING;
    self_p->samples_p = samples_p;
    self_p->length = length;
    self_p->thrd_p = NULL;
    self_p->next_p = NULL;

    /* Enqueue. */
    sys_lock();

    if (self_p->dev_p->jobs.head_p == NULL) {
        /* Empty queue. */
        self_p->dev_p->jobs.head_p = self_p;
        start_adc_hw(self_p);
    } else {
        /* Non-empty queue. */
        self_p->dev_p->jobs.tail_p->next_p = self_p;
    }

    self_p->dev_p->jobs.tail_p = self_p;

    sys_unlock();

    return (0);
}

static int adc_port_async_wait(struct adc_driver_t *self_p)
{
    int has_finished;

    sys_lock();

    has_finished = (self_p->state == STATE_FINISHED);

    /* Always set thrd_p, even if the convertion has finished. If the
       convertion has finished, thrd_p will be unused.*/
    self_p->thrd_p = thrd_self();

    /* Wait until all samples have been converted. */
    if (!has_finished) {
        thrd_suspend_isr(NULL);
    }

    sys_unlock();

    return (has_finished);
}

int adc_port_convert_isr(struct adc_driver_t *self_p,
                         uint16_t *sample_p)
{
    return (-1);
}
