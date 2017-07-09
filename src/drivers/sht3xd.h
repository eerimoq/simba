/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Google Inc.
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
 * This code was originally written by Simon L. B. Nielsen.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_SHT3X_H__
#define __DRIVERS_SHT3X_H__

struct sht3xd_driver_t {
    struct i2c_driver_t *i2c_p;
    int i2c_addr;
    uint32_t serial;
#ifdef CONFIG_SHT3XD_DEBUG_LOG_MASK
    struct log_object_t log;
#endif
};

//! SHT3x-DIS default I2C address.
#define SHT3X_DIS_I2C_ADDR_A 0x44
//! SHT3x-DIS alternate I2C address.
#define SHT3X_DIS_I2C_ADDR_B 0x45

//! Max measurement time for high repeatability.
#define MEASUREMENT_DURATION_HIGH_MS      15

/**
 * Initialize the driver module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int sht3xd_module_init(void);

/**
 * Initialize driver object. The driver object will be used for a
 * single sensor.
 *
 * @param[out] self_p Driver object to be initialize.
 * @param[in] i2c_p The I2C driver pointer.
 * @param[in] i2c_addr The address of the SHT3x-D.
 *                     Probably SHT3X_DIS_I2C_ADDR_A.
 *
 * @return zero(0) or negative error code.
 */
int sht3xd_init(struct sht3xd_driver_t *self_p,
                struct i2c_driver_t *i2c_p,
                int i2c_addr);

/**
 * Start the driver.
 *
 * This verify the sensor is present.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int sht3xd_start(struct sht3xd_driver_t *self_p);

/**
 * Get measurements and return it from the SHD3x-DIS chip.
 *
 * This is a "high level" function which will block for the time it
 * takes the sensor to perform the measurement.
 *
 * @param[in] self_p Driver object.
 * @param[out] temp_p Tempererature in Celsius, or NULL.
 * @param[out] humid_p Relative Humidity, or NULL.
 *
 * @return zero(0) or negative error code.
 */
int sht3xd_get_temp_humid(struct sht3xd_driver_t *self_p,
                          float *temp_p,
                          float *humid_p);

/**
 * Get the serial number from the SHD3x-D.
 *
 * @param[in] self_p Driver object.
 * @param[out] serial_p Serial number of the SHT3x-D sensor.
 *
 * @return zero(0) or negative error code.
 */
int sht3xd_get_serial(struct sht3xd_driver_t *self_p,
                      uint32_t *serial_p);

#endif
