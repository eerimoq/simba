/**
 * @file drivers/adc.h
 * @version 4.1.0
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
 * Initialize the ADC driver module.
 */
int adc_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p ADC device to use.
 * @param[in] pin_dev_p Pin device to use.
 * @param[in] reference Voltage reference. Only ``ADC_REFERENCE_VCC``
 *                      is supported.
 * @param[in] sampling_rate Sampling rate in Hz. The lowest allowed
 *                          value is one and the highest value depends
 *                          on the architechture. The sampling rate is
 *                          not used in single sample convertions,
 *                          ie. calls to ``adc_async_convert()`` and
 *                          ``adc_convert()`` with length one; or
 *                          calls to ``adc_convert_isr()``.
 *
 * @return zero(0) or negative error code.
 */
int adc_init(struct adc_driver_t *self_p,
             struct adc_device_t *dev_p,
             struct pin_device_t *pin_dev_p,
             int reference,
             long sampling_rate);

/**
 * Start an asynchronous convertion of analog signal to digital
 * samples. Call `adc_async_wait()` to wait for the convertion to
 * complete.
 *
 * @param[in] self_p Driver object.
 * @param[out] samples_p Converted samples.
 * @param[in] length Length of samples array.
 *
 * @return zero(0) or negative error code.
 */
int adc_async_convert(struct adc_driver_t *self_p,
                      uint16_t *samples_p,
                      size_t length);

/**
 * Wait for an asynchronous convertion to complete.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int adc_async_wait(struct adc_driver_t *self_p);

/**
 * Start a synchronous convertion of analog signal to digital
 * samples. This is equivalent to `adc_async_convert()` +
 * `adc_async_wait()`, but in a single function call.
 *
 * @param[in] self_p Driver object.
 * @param[out] samples_p Converted samples.
 * @param[in] length Length of samples array.
 *
 * @return zero(0) or negative error code.
 */
int adc_convert(struct adc_driver_t *self_p,
                uint16_t *samples_p,
                size_t length);

/**
 * Start a synchronous convertion of analog signal to digital samples
 * from isr or with the system lock taken. This function will poll the
 * ADC hardware until the sample has been coverted.
 *
 * @param[in] self_p Driver object.
 * @param[out] sample_p Converted sample.
 *
 * @return zero(0) or negative error code.
 */
int adc_convert_isr(struct adc_driver_t *self_p,
                    uint16_t *sample_p);

#endif
