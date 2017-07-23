/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

#ifndef __DRIVERS_BMP280_H__
#define __DRIVERS_BMP280_H__

#include "simba.h"

/**
 * I2C address #0.
 */
#define BMP280_I2C_ADDRESS_0                             0x76

/**
 * I2C address #1.
 */
#define BMP280_I2C_ADDRESS_1                             0x77

/**
 * Automatic I2C address detection.
 */
#define BMP280_I2C_ADDRESS_AUTOMATIC                     0xff

/**
 * Default SPI polarity and phase. Polarity 1 and phase 1 is also
 * supported.
 */
#define BMP280_SPI_POLARITY                                 0
#define BMP280_SPI_PHASE                                    0

/**
 * Mode configuration.
 */
enum bmp280_mode_t {
    bmp280_mode_forced_t = 1,
    bmp280_mode_normal_t = 3
};

/**
 * Standby time in normal mode configuration.
 */
enum bmp280_standby_time_t {
    bmp280_standby_time_500_us_t = 0,
    bmp280_standby_time_62500_us_t,
    bmp280_standby_time_125_ms_t,
    bmp280_standby_time_250_ms_t,
    bmp280_standby_time_500_ms_t,
    bmp280_standby_time_1_s_t,
    bmp280_standby_time_2_s_t,
    bmp280_standby_time_4_s_t
};

/**
 * Filter configuration.
 */
enum bmp280_filter_t {
    bmp280_filter_off_t = 0,
    bmp280_filter_2_t,
    bmp280_filter_4_t,
    bmp280_filter_8_t,
    bmp280_filter_16_t
};

/**
 * Temperature oversampling configuration.
 */
enum bmp280_temperature_oversampling_t {
    bmp280_temperature_off_t = 0,
    bmp280_temperature_oversampling_1_t,
    bmp280_temperature_oversampling_2_t,
    bmp280_temperature_oversampling_4_t,
    bmp280_temperature_oversampling_8_t,
    bmp280_temperature_oversampling_16_t
};

/**
 * Pressure oversampling configuration.
 */
enum bmp280_pressure_oversampling_t {
    bmp280_pressure_off_t = 0,
    bmp280_pressure_oversampling_1_t,
    bmp280_pressure_oversampling_2_t,
    bmp280_pressure_oversampling_4_t,
    bmp280_pressure_oversampling_8_t,
    bmp280_pressure_oversampling_16_t
};

/**
 * The BMP280 driver struct.
 */
struct bmp280_driver_t {
    struct bmp280_transport_t *transport_p;
    uint8_t ctrl_meas;
    uint8_t config;
    int16_t calibration[12];
#if CONFIG_BMP280_DEBUG_LOG_MASK > -1
    struct log_object_t log;
#endif
};

struct bmp280_transport_protocol_t;

struct bmp280_transport_t {
    struct bmp280_transport_protocol_t *protocol_p;
};

struct bmp280_transport_i2c_t {
    struct bmp280_transport_t base;
    struct i2c_driver_t *i2c_p;
    int i2c_address;
};

struct bmp280_transport_spi_t {
    struct bmp280_transport_t base;
    struct spi_driver_t *spi_p;
};

/**
 * Initialize the bmp280 module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] transport_p Transport protocol to use. Reference to an
 *                        I2C or SPI transport object.
 * @param[in] mode Normal or forced mode. In normal mode the device is
 *                 periodically measuring temperature and pressure.
 *                 In burst mode the device is in sleeping (low power
 *                 consumption) and the MCU wakes it to request a
 *                 measurement.
 * @param[in] standby_time Normal mode standby time.
 * @param[in] filter Filter configuration, normally only set when the
 *                   device is in normal mode.
 * @param[in] temperature_oversampling Temperature oversampling.
 * @param[in] pressure_oversampling Pressure oversampling.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_init(struct bmp280_driver_t *self_p,
                struct bmp280_transport_t *transport_p,
                enum bmp280_mode_t mode,
                enum bmp280_standby_time_t standby_time,
                enum bmp280_filter_t filter,
                enum bmp280_temperature_oversampling_t temperature_oversampling,
                enum bmp280_pressure_oversampling_t pressure_oversampling);

/**
 * Start given driver by entering normal mode (if mode is
 * `bmp280_mode_normal_t`), and reading calibration data from the
 * device.
 *
 * @param[in] self_p Driver object to start.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_start(struct bmp280_driver_t *self_p);

/**
 * Stop given driver by resetting it.
 *
 * @param[in] self_p Driver object to stop.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_stop(struct bmp280_driver_t *self_p);

/**
 * Read temperature and pressure from the device.
 *
 * @param[in] self_p Driver object.
 * @param[out] temperature_p Temperature in Celsius, or NULL.
 * @param[out] pressure_p Pressure in Pascal, or NULL.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_read(struct bmp280_driver_t *self_p,
                float *temperature_p,
                float *pressure_p);

/**
 * Read temperature and pressure from the device and return them as
 * fixed point numbers with three decimal places.
 *
 * @param[in] self_p Driver object.
 * @param[out] temperature_p Temperature in milli-Celsius, or NULL.
 * @param[out] pressure_p Pressure in milli-Pascal, or NULL.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_read_fixed_point(struct bmp280_driver_t *self_p,
                            long *temperature_p,
                            long *pressure_p);

/**
 * Initialize given I2C transport object.
 *
 * @param[out] self_p I2C transport object to be initialized.
 * @param[in] transport_p I2C driver to use.
 * @param[in] i2c_address Device I2C address, one of
 *                        ``BMP280_I2C_ADDRESS_0``,
 *                        ``BMP280_I2C_ADDRESS_1`` and
 *                        ``BMP280_I2C_ADDRESS_AUTOMATIC``.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_transport_i2c_init(struct bmp280_transport_i2c_t *self_p,
                              struct i2c_driver_t *i2c_p,
                              int i2c_address);

/**
 * Initialize given SPI transport object.
 *
 * @param[out] self_p SPI transport object to be initialized.
 * @param[in] spi_p SPI driver to use.
 *
 * @return zero(0) or negative error code.
 */
int bmp280_transport_spi_init(struct bmp280_transport_spi_t *self_p,
                              struct spi_driver_t *spi_p);

#endif
