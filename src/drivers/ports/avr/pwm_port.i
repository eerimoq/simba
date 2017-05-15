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

#if defined(MCU_ATMEGA328P)

static int pwm_port_init(struct pwm_driver_t *self_p,
                         struct pwm_device_t *dev_p)
{
    self_p->dev_p = dev_p;

    pin_init(&self_p->pin_drv, dev_p->pin_dev_p, PIN_OUTPUT);

    /* Configure the timer mode to PWM. */
    switch (dev_p->index) {

    case 0:
    case 1:
        TCCR1A |= _BV(WGM10);
        break;

    case 2:
    case 3:
        TCCR2A |= _BV(WGM20);
        break;

    default:
        break;
    }

    return (0);
}

static int pwm_port_set_frequency(struct pwm_driver_t *self_p,
                                  long value)
{
    /* Configure duty cycle and frequency. */
    switch (self_p->dev_p->index) {

    case 0:
        TCCR1A |= _BV(COM1A1);
        TCCR1B |= _BV(CS11) | _BV(CS10);
        break;

    case 1:
        TCCR1A |= _BV(COM1B1);
        TCCR1B |= _BV(CS11) | _BV(CS10);
        break;

    case 2:
        TCCR2A |= _BV(COM2A1);
        TCCR2B |= _BV(CS22);
        break;

    case 3:
        TCCR2A |= _BV(COM2B1);
        TCCR2B |= _BV(CS22);
        break;

    default:
        break;
    }

    return (0);
}

static int pwm_port_set_duty_cycle(struct pwm_driver_t *self_p,
                                   long value)
{
    switch (self_p->dev_p->index) {

    case 0:
        OCR1A = value;
        break;

    case 1:
        OCR1B = value;
        break;

    case 2:
        OCR2A = value;
        break;

    case 3:
        OCR2B = value;
        break;

    default:
        break;
    }

    return (0);
}

#elif defined(MCU_ATMEGA2560)

static int pwm_port_init(struct pwm_driver_t *self_p,
                         struct pwm_device_t *dev_p)
{
    self_p->dev_p = dev_p;

    pin_init(&self_p->pin_drv, dev_p->pin_dev_p, PIN_OUTPUT);

    switch (dev_p->index) {

    case 0:
    case 1:
    case 2:
        /* Timer 3. */
        TCCR3A |= _BV(WGM30);
        break;

    case 3:
    case 4:
    case 5:
        /* Timer 4. */
        TCCR4A |= _BV(WGM40);
        break;

    case 6:
    case 7:
        /* Timer 2. */
        TCCR2A |= _BV(WGM20);
        break;

    case 8:
    case 9:
        /* Timer 1. */
        TCCR1A |= _BV(WGM10);
        break;

    default:
        break;
    }

    return (0);
}

static int pwm_port_set_frequency(struct pwm_driver_t *self_p,
                                  long value)
{
    switch (self_p->dev_p->index) {

    case 0:
        TCCR3A |= _BV(COM3B1);
        TCCR3B |= _BV(CS32) | /*_BV(CS31) |*/ _BV(CS30);
        break;

    case 1:
        TCCR3A |= _BV(COM3C1);
        TCCR3B |= _BV(CS32) | /*_BV(CS31) |*/ _BV(CS30);
        break;

    case 2:
        TCCR3A |= _BV(COM3A1);
        TCCR3B |= _BV(CS32) | /*_BV(CS31) |*/ _BV(CS30);
        break;

    case 3:
        TCCR4A |= _BV(COM4A1);
        TCCR4B |= _BV(CS42) | /*_BV(CS41) |*/ _BV(CS40);
        break;

    case 4:
        TCCR4A |= _BV(COM4B1);
        TCCR4B |= _BV(CS42) | /*_BV(CS41) |*/ _BV(CS40);
        break;

    case 5:
        TCCR4A |= _BV(COM4C1);
        TCCR4B |= _BV(CS42) | /*_BV(CS41) |*/ _BV(CS40);
        break;

    case 6:
        TCCR2A |= _BV(COM2B1);
        TCCR2B |= _BV(CS22) | /*_BV(CS21) |*/ _BV(CS20);
        break;

    case 7:
        TCCR2A |= _BV(COM2A1);
        TCCR2B |= _BV(CS22) | /*_BV(CS21) |*/ _BV(CS20);
        break;

    case 8:
        TCCR1A |= _BV(COM1A1);
        TCCR1B |= _BV(CS12) | /*_BV(CS11) |*/ _BV(CS10);
        break;

    case 9:
        TCCR1A |= _BV(COM1B1);
        TCCR1B |= _BV(CS12) | /*_BV(CS11) |*/ _BV(CS10);
        break;

    default:
        break;
    }

    return (0);
}

static int pwm_port_set_duty_cycle(struct pwm_driver_t *self_p,
                                   long value)
{
    switch (self_p->dev_p->index) {

    case 0:
        OCR3B = value;
        break;

    case 1:
        OCR3C = value;
        break;

    case 2:
        OCR3A = value;
        break;

    case 3:
        OCR4A = value;
        break;

    case 4:
        OCR4B = value;
        break;

    case 5:
        OCR4C = value;
        break;

    case 6:
        OCR2B = value;
        break;

    case 7:
        OCR2A = value;
        break;

    case 8:
        OCR1A = value;
        break;

    case 9:
        OCR1B = value;
        break;

    default:
        break;
    }

    return (0);
}

#elif defined(MCU_ATMEGA32U4)

static int pwm_port_init(struct pwm_driver_t *self_p,
                         struct pwm_device_t *dev_p)
{
    return (-1);
}

static int pwm_port_set_frequency(struct pwm_driver_t *self_p,
                                  long value)
{
    return (-1);
}

static int pwm_port_set_duty_cycle(struct pwm_driver_t *self_p,
                                   long value)
{
    return (-1);
}

#else
#    error "PWM not implemented for this MCU"
#endif

static long pwm_port_frequency(int hertz)
{
    return (0);
}

static int pwm_port_frequency_as_hertz(long value)
{
    return (0);
}

static int pwm_port_duty_cycle(int percentage)
{
    return ((255 * percentage) / 100);
}

static int pwm_port_duty_cycle_as_percent(int value)
{
    return ((100 * value) / 255);
}
