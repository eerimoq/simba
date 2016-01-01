/**
 * @file drivers/adc.h
 * @version 0.1.0
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

#ifndef __DRIVERS_ADC_H__
#define __DRIVERS_ADC_H__

#include "simba.h"

#include "adc_port.h"

#define ADC_REFERENCE_VCC ADC_PORT_REFERENCE_VCC

extern struct adc_device_t adc_device[ADC_DEVICE_MAX];

/**
 * Initialize ADC driver module.
 */
int adc_module_init(void);

/**
 * Initialize driver object from given config.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] pin_dev_p Pin device to use.
 * @param[in] reference Voltage reference.
 * @param[in] sampling_rate Sampling rate in Hz.
 *
 * @return zero(0) or negative error code.
 */
int adc_init(struct adc_driver_t *self_p,
             struct adc_device_t *dev_p,
             struct pin_device_t *pin_dev_p,
             int reference,
             int sampling_rate);

/**
 * Start an asynchronous convertion of analog signal to digital samples.
 *
 * @param[in] self_p Driver object.
 * @param[out] samples_p Converted samlpes.
 * @param[in] length Length of samples array.
 *
 * @return zero(0) or negative error code.
 */
int adc_async_convert(struct adc_driver_t *self_p,
                      uint16_t *samples_p,
                      size_t length);

/**
 * Wait for an asynchronous convertion to finish.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int adc_async_wait(struct adc_driver_t *self_p);

/**
 * Start a synchronous convertion of analog signal to digital samples.
 *
 * @param[in] self_p Driver object.
 * @param[out] samples_p Converted samlpes.
 * @param[in] length Length of samples array.
 *
 * @return zero(0) or negative error code.
 */
int adc_convert(struct adc_driver_t *self_p,
                uint16_t *samples_p,
                size_t length);

#endif
