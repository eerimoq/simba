/**
 * @file drivers/dac.h
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

#ifndef __DRIVERS_DAC_H__
#define __DRIVERS_DAC_H__

#include "simba.h"

#include "dac_port.h"

extern struct dac_device_t dac_device[DAC_DEVICE_MAX];

/**
 * Initialize DAC driver module.
 */
int dac_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] pin0_dev_p Pin used for mono or first stereo channel.
 * @param[in] pin1_dev_p Second stereo pin.
 * @param[in] sampling_rate Sampling rate in Hz.
 *
 * @return zero(0) or negative error code.
 */
int dac_init(struct dac_driver_t *self_p,
             struct dac_device_t *dev_p,
             struct pin_device_t *pin0_dev_p,
             struct pin_device_t *pin1_dev_p,
             int sampling_rate);

/**
 * Start asynchronous convertion of samples to an analog signal.
 *
 * @param[in] self_p Driver object.
 * @param[in] samples Samples to convert to an analog signal.
 * @param[in] length Length of samples array.
 *
 * @return zero(0) or negative error code.
 */
int dac_async_convert(struct dac_driver_t *self_p,
                      uint32_t *samples_p,
                      size_t length);

/**
 * Wait for asynchronous convertion to finish.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int dac_async_wait(struct dac_driver_t *self_p);

/**
 * Start synchronous convertion of samples to an analog signal.
 *
 * @param[in] self_p Driver object.
 * @param[out] samples Converted samlpes.
 * @param[in] length Length of samples array.
 *
 * @return zero(0) or negative error code.
 */
int dac_convert(struct dac_driver_t *self_p,
                uint32_t *samples_p,
                size_t length);

#endif
