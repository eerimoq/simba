/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_DAC_H__
#define __DRIVERS_DAC_H__

#include "simba.h"

#include "dac_port.h"

extern struct dac_device_t dac_device[DAC_DEVICE_MAX];

/**
 * Initialize DAC driver module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int dac_module_init(void);

/**
 * Initialize given driver object from given configuration.
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
 * Start an asynchronous convertion of samples to an analog signal.
 *
 * @param[in] self_p Driver object.
 * @param[in] samples Samples to convert to an analog signal.
 * @param[in] length Length of samples array.
 *
 * @return zero(0) or negative error code.
 */
int dac_async_convert(struct dac_driver_t *self_p,
                      void *samples_p,
                      size_t length);

/**
 * Wait for ongoing asynchronous convertion to finish.
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
                void *samples_p,
                size_t length);

#endif
