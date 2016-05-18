/**
 * @file drivers/adc_port.h
 * @version 0.5.0
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

#ifndef __DRIVERS_ADC_PORT_H__
#define __DRIVERS_ADC_PORT_H__

#include <avr/io.h>

#define ADC_PORT_REFERENCE_VCC _BV(REFS0)

struct adc_driver_t;

struct adc_device_t {
    struct {
        struct adc_driver_t *head_p;
        struct adc_driver_t *tail_p;
    } jobs;
};

struct adc_driver_t {
    struct adc_device_t *dev_p;
    struct pin_driver_t pin_drv;
    uint8_t admux;
    long interrupt_count;
    long interrupt_max;
    size_t pos;
    size_t length;
    uint16_t *samples_p;
    struct thrd_t *thrd_p;
    struct adc_driver_t *next_p;
};

#endif
