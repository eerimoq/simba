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

struct adc_driver_t;

struct adc_device_t {
    volatile struct sam_adc_t *regs_p;
    int id;
    struct {
        struct adc_driver_t *head_p;
        struct adc_driver_t *tail_p;
    } jobs;
    struct {
        volatile struct sam_tc_t *regs_p;
        int channel;
        int id;
    } tc;
};

struct adc_driver_t {
    struct adc_device_t *dev_p;
    struct pin_driver_t pin_drv;
    int state;
    int channel;
    size_t length;
    uint16_t *samples_p;
    struct thrd_t *thrd_p;
    struct adc_driver_t *next_p;
};

#endif
