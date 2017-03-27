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

#include <avr/interrupt.h>

/* Free running mode sampling rate.
   FCPU / clockdiv / samplingtime = 16MHz / 32 / 13 = 9615Hz. */
#define SAMPLING_RATE_HZ 38462UL

/* Convert desired sampling rate to number of interrupts, rounded
   up. The error in the calculation is rather big for high sampling
   frequencies. */
#define SAMPLING_RATE_TO_INTERRUPT_MAX(rate) \
    ((SAMPLING_RATE_HZ + rate - 1) / rate)

ISR(ADC_vect)
{
    struct adc_device_t *dev_p = &adc_device[0];
    struct adc_driver_t *self_p = dev_p->jobs.head_p;

    /* The AD Converter is running in free mode. */
    self_p->interrupt_count++;

    if (self_p->interrupt_count < self_p->interrupt_max) {
        return;
    }

    self_p->interrupt_count = 0;

    /* Read the sample from the output register. */
    self_p->samples_p[self_p->pos++] = ADC;

    /* Resume thread when all samples have been collected. */
    if (self_p->pos == self_p->length) {
        /* Detach job from job list as it is completed. */
        dev_p->jobs.head_p = self_p->next_p;

        /* Disable ADC hardware if there are no more queued jobs. */
        if (self_p->next_p != NULL) {
            ADMUX = self_p->next_p->admux;
#if defined(MUX5)
            ADCSRB = self_p->next_p->adcsrb;
#endif
        } else {
            ADCSRA &= ~_BV(ADEN);
        }

        /* Resume the thread if it is waiting for completion. */
        if (self_p->thrd_p != NULL) {
            thrd_resume_isr(self_p->thrd_p, 0);
        }
    }
}

static void start_adc_hw(struct adc_driver_t *self_p)
{
    /* Start AD Converter in free running mode. */
    ADMUX = self_p->admux;
#if defined(MUX5)
    ADCSRB = self_p->adcsrb;
#else
    ADCSRB = 0;
#endif
    /* clock div 32. */
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE)
              | _BV(ADPS2) /*| _BV(ADPS1) */| _BV(ADPS0));
}

static int adc_port_module_init(void)
{
    return (0);
}

static int adc_port_init(struct adc_driver_t *self_p,
                         struct adc_device_t *dev_p,
                         struct pin_device_t *pin_dev_p,
                         int reference,
                         long sampling_rate)
{
    int channel = 0;

    while ((&pin_a0_dev + channel) != pin_dev_p) {
        channel++;
    }

    self_p->dev_p = dev_p;
    self_p->interrupt_max =
        SAMPLING_RATE_TO_INTERRUPT_MAX((long)sampling_rate);
    self_p->admux = (reference | (channel & 0x07));
#if defined(MUX5)
    self_p->adcsrb = channel > 7 ? _BV(MUX5) : 0;
#endif
    pin_init(&self_p->pin_drv, pin_dev_p, PIN_INPUT);

    return (0);
}

static int adc_port_async_convert(struct adc_driver_t *self_p,
                                  uint16_t *samples_p,
                                  size_t length)
{
    /* Initialize. */
    self_p->pos = 0;
    self_p->samples_p = samples_p;
    self_p->length = length;
    self_p->interrupt_count = (self_p->interrupt_max - 1);
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

    has_finished = (self_p->pos == self_p->length);

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
    /* Fail if there are ongoing job(s). */
    if (self_p->dev_p->jobs.head_p != NULL) {
        return (-1);
    }

    /* Start the convertion. */
    ADMUX = self_p->admux;
#if defined(MUX5)
    ADCSRB = self_p->adcsrb;
#else
    ADCSRB = 0;
#endif
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS0));

    /* Poll until the convertion is completed. */
    while (ADCSRA & _BV(ADSC));

    *sample_p = ADC;

    /* Disable the ADC. */
    ADCSRA &= ~_BV(ADEN);

    return (0);
}
