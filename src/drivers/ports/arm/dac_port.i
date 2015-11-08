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

ISR(DAC_vect)
{
    if (drv_p->length > 0) {
        /* Add more samples to the PDC. */
        dev_p->regs_p->PDC.TNPR = drv_p->samples;
        dev_p->regs_p->PDC.TNCR = drv_p->length;
        drv_p->length = 0;

        /* Resume the waiting thread if it wants to write additional
           samples. */
        if (drv_p->thrd_p != NULL) {
            thrd_resume_irq(drv_p->thrd_p, 0);
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
    pmc_peripheral_clock_enable(dev_p->tc_p->id);

    /* Create a square wave of the desired frequency. UPDOWN_RC
       waveform is a triangle wave starting at zero and increasing to
       the value of the RC register, then it decrements to zero
       again. The tick frequency is MCR / 2, or 42 MHz. */
    tc_p->regs_p->CMR = (TC_CMR_WAVEFORM_ACPC(2)
                         | TC_CMR_WAVEFORM_ACPA(1)
                         | TC_CMR_WAVEFORM_WAVE
                         | TC_CMR_WAVEFORM_WAVSEL_UPDOWN_RC
                         | TC_CMR_WAVEFORM_TCCLKS(1));
    rc = (F_MCU / 2 / sampling_rate);
    tc_p->regs_p->RA = (rc / 2);
    tc_p->regs_p->RC = (F_MCU / 2 / sampling_rate);
    tc_p->regs_p->CCR = TC_CCR_CLKEN;

    return (0);
}

static int dac_port_async_convert(struct dac_driver_t *drv_p,
                                  int *samples,
                                  size_t length)
{
    /* Initialize. */
    drv_p->samples = samples;
    drv_p->length = length;
    drv_p->thrd_p = NULL;
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





void setup()
{
  Serial.begin (57600) ; // was for debugging
  adc_setup () ;         // setup ADC

  pmc_enable_periph_clk (TC_INTERFACE_ID + 0*3+0) ;  // clock the TC0 channel 0

  TcChannel * t = &(TC0->TC_CHANNEL)[0] ;    // pointer to TC0 registers for its channel 0
  t->TC_CCR = TC_CCR_CLKDIS ;  // disable internal clocking while setup regs
  t->TC_IDR = 0xFFFFFFFF ;     // disable interrupts
  t->TC_SR ;                   // read int status reg to clear pending
  t->TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 |   // use TCLK1 (prescale by 2, = 42MHz)
              TC_CMR_WAVE |                  // waveform mode
              TC_CMR_WAVSEL_UP_RC |          // count-up PWM using RC as threshold
              TC_CMR_EEVT_XC0 |     // Set external events from XC0 (this setup TIOB as output)
              TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR |
              TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_CLEAR ;

  t->TC_RC =  875 ;     // counter resets on RC, so sets period in terms of 42MHz clock
  t->TC_RA =  440 ;     // roughly square wave
  t->TC_CMR = (t->TC_CMR & 0xFFF0FFFF) | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET ;  // set clear and set from RA and RC compares

  t->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG ;  // re-enable local clocking and switch to hardware trigger source.

  setup_pio_TIOA0 () ;  // drive Arduino pin 2 at 48kHz to bring clock out
  dac_setup () ;        // setup up DAC auto-triggered at 48kHz
}

void setup_pio_TIOA0 ()  // Configure Ard pin 2 as output from TC0 channel A (copy of trigger event)
{
  PIOB->PIO_PDR = PIO_PB25B_TIOA0 ;  // disable PIO control
  PIOB->PIO_IDR = PIO_PB25B_TIOA0 ;   // disable PIO interrupts
  PIOB->PIO_ABSR |= PIO_PB25B_TIOA0 ;  // switch to B peripheral
}


void dac_setup ()
{
  pmc_enable_periph_clk (DACC_INTERFACE_ID) ; // start clocking DAC
  DACC->DACC_CR = DACC_CR_SWRST ;  // reset DAC

  DACC->DACC_MR =
    DACC_MR_TRGEN_EN | DACC_MR_TRGSEL (1) |  // trigger 1 = TIO output of TC0
    (0 << DACC_MR_USER_SEL_Pos) |  // select channel 0
    DACC_MR_REFRESH (0x0F) |       // bit of a guess... I'm assuming refresh not needed at 48kHz
    (24 << DACC_MR_STARTUP_Pos) ;  // 24 = 1536 cycles which I think is in range 23..45us since DAC clock = 42MHz

  DACC->DACC_IDR = 0xFFFFFFFF ; // no interrupts
  DACC->DACC_CHER = DACC_CHER_CH0 << 0 ; // enable chan0
}

void dac_write (int val)
{
  DACC->DACC_CDR = val & 0xFFF ;
}



void adc_setup ()
{
  NVIC_EnableIRQ (ADC_IRQn) ;   // enable ADC interrupt vector
  ADC->ADC_IDR = 0xFFFFFFFF ;   // disable interrupts
  ADC->ADC_IER = 0x80 ;         // enable AD7 End-Of-Conv interrupt (Arduino pin A0)
  ADC->ADC_CHDR = 0xFFFF ;      // disable all channels
  ADC->ADC_CHER = 0x80 ;        // enable just A0
  ADC->ADC_CGR = 0x15555555 ;   // All gains set to x1
  ADC->ADC_COR = 0x00000000 ;   // All offsets off

  ADC->ADC_MR = (ADC->ADC_MR & 0xFFFFFFF0) | (1 << 1) | ADC_MR_TRGEN ;  // 1 = trig source TIO from TC0
}

// Circular buffer, power of two.
#define BUFSIZE 0x400
#define BUFMASK 0x3FF
volatile int samples [BUFSIZE] ;
volatile int sptr = 0 ;
volatile int isr_count = 0 ;   // this was for debugging


#ifdef __cplusplus
extern "C"
{
#endif

void ADC_Handler (void)
{
  if (ADC->ADC_ISR & ADC_ISR_EOC7)   // ensure there was an End-of-Conversion and we read the ISR reg
  {
    int val = *(ADC->ADC_CDR+7) ;    // get conversion result
    samples [sptr] = val ;           // stick in circular buffer
    sptr = (sptr+1) & BUFMASK ;      // move pointer
    dac_write (0xFFF & ~val) ;       // copy inverted to DAC output FIFO
  }
  isr_count ++ ;
}

#ifdef __cplusplus
}
#endif


void loop()
{
}
