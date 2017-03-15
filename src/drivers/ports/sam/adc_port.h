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

#ifndef __DRIVERS_ADC_PORT_H__
#define __DRIVERS_ADC_PORT_H__

#define ADC_PORT_REFERENCE_VCC      0

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
