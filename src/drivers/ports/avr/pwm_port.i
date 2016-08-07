/**
 * @file drivers/avr/pwm_port.i
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

#if defined(MCU_ATMEGA328P)

static int pwm_port_init(struct pwm_driver_t *self_p,
                         struct pwm_device_t *dev_p)
{
    self_p->dev_p = dev_p;

    pin_init(&self_p->pin_drv, dev_p->pin_dev_p, PIN_OUTPUT);

    switch (dev_p->index) {
    case 0:
    case 1:
        /* pin d9 and d10. Timer 1. */
        TCCR1A |= _BV(WGM10);
        TCCR1B |= _BV(CS11) | _BV(CS10);
        break;
    case 2:
    case 3:
        /* pin d11 and d3. Timer 2. */
        TCCR2A |= _BV(WGM20);
        TCCR2B |= _BV(CS22);
        break;
    default:
        break;
    }

    return (0);
}

static int pwm_port_set_duty(struct pwm_driver_t *self_p,
                             uint8_t value)
{
    switch (self_p->dev_p->index) {
    case 0:
        TCCR1A |= _BV(COM1A1);
        OCR1A = value;
        break;
    case 1:
        TCCR1A |= _BV(COM1B1);
        OCR1B = value;
        break;
    case 2:
        TCCR2A |= _BV(COM2A1);
        OCR2A = value;
        break;
    case 3:
        TCCR2A |= _BV(COM2B1);
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
        TCCR3B |= _BV(CS31) | _BV(CS30);
        break;
    case 3:
    case 4:
    case 5:
        /* Timer 4. */
        TCCR4A |= _BV(WGM40);
        TCCR4B |= _BV(CS41) | _BV(CS40);
        break;
    case 6:
    case 7:
        /* Timer 2. */
        TCCR2A |= _BV(WGM20);
        TCCR2B |= _BV(CS22);
        break;
    case 8:
    case 9:
        /* Timer 1. */
        TCCR1A |= _BV(WGM10);
        TCCR1B |= _BV(CS11) | _BV(CS10);
        break;
    default:
        break;
    }

    return (0);
}

static int pwm_port_set_duty(struct pwm_driver_t *self_p,
                             uint8_t value)
{
    switch (self_p->dev_p->index) {
    case 0:
        TCCR3B |= _BV(COM3B1);
        OCR3B = value;
        break;
    case 1:
        TCCR3C |= _BV(COM3C1);
        OCR3C = value;
        break;
    case 2:
        TCCR3A |= _BV(COM3A1);
        OCR3A = value;
        break;
    case 3:
        TCCR4A |= _BV(COM4A1);
        OCR4A = value;
        break;
    case 4:
        TCCR4B |= _BV(COM4B1);
        OCR4B = value;
        break;
    case 5:
        TCCR4C |= _BV(COM4C1);
        OCR4C = value;
        break;
    case 6:
        TCCR2B |= _BV(COM2B1);
        OCR2B = value;
        break;
    case 7:
        TCCR2A |= _BV(COM2A1);
        OCR2A = value;
        break;
    case 8:
        TCCR1A |= _BV(COM1A1);
        OCR1A = value;
        break;
    case 9:
        TCCR1B |= _BV(COM1B1);
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

static int pwm_port_set_duty(struct pwm_driver_t *self_p,
                             uint8_t value)
{
    return (-1);
}

#else
#    error "PWM not implemented for this MCU"
#endif
