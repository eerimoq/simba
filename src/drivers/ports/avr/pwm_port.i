/**
 * @file drivers/avr/pwm_port.i
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

static int pwm_port_init(struct pwm_driver_t *drv_p,
                         struct pwm_device_t *dev_p)
{
    drv_p->dev_p = dev_p;

    pin_init(&drv_p->pin_drv, dev_p->pin_dev_p, PIN_OUTPUT);

    switch (dev_p->index) {
    case 0:
    case 1:
        TCCR1A |= _BV(WGM10);
        TCCR1B |= _BV(CS11) | _BV(CS10);
        break;
    case 2:
    case 3:
        TCCR2A |= _BV(WGM20);
        TCCR2B |= _BV(CS22);
        break;
    default:
        break;
    }

    return (0);
}

static int pwm_port_set_duty(struct pwm_driver_t *drv_p,
                             uint8_t value)
{
    switch (drv_p->dev_p->index) {
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
