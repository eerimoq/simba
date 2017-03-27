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

#ifndef __DRIVERS_ADC_H__
#define __DRIVERS_ADC_H__

#include "simba.h"

#include "adc_port.h"

/**
 * Use VCC as reference for convertions.
 */
#define ADC_REFERENCE_VCC ADC_PORT_REFERENCE_VCC

extern struct adc_device_t adc_device[ADC_DEVICE_MAX];

/**
 * Initialize the ADC driver module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
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
 * Start a synchronous convertion of an analog signal to digital
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

/**
 * Check if given ADC device is valid.
 *
 * @param[in] dev_p ADC device to validate.
 *
 * @return true(1) if the pin device is valid, otherwise false(0).
 */
int adc_is_valid_device(struct adc_device_t *dev_p);

#endif
