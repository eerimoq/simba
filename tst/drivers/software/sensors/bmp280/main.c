/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

#include "simba.h"
#include "drivers/network/i2c_mock.h"
#include "drivers/network/spi_mock.h"

struct bmp280_driver_t bmp280_i2c;
struct i2c_driver_t i2c;
struct bmp280_transport_i2c_t transport_i2c;

struct bmp280_driver_t bmp280_spi;
struct spi_driver_t spi;
struct bmp280_transport_spi_t transport_spi;

static int test_init(void)
{
    BTASSERT(bmp280_module_init() == 0);
    BTASSERT(bmp280_module_init() == 0);

    return (0);
}

static int test_i2c_start(void)
{
    BTASSERT(bmp280_transport_i2c_init(&transport_i2c,
                                       &i2c,
                                       BMP280_I2C_ADDRESS_0) == 0);
    BTASSERT(bmp280_init(&bmp280_i2c,
                         &transport_i2c.base,
                         bmp280_mode_forced_t,
                         bmp280_standby_time_500_us_t,
                         bmp280_filter_off_t,
                         bmp280_temperature_oversampling_1_t,
                         bmp280_pressure_oversampling_1_t) == 0);

    mock_write_i2c_scan(0x76, 1);

    /* Chip id. */
    mock_write_i2c_write(0x76, "\xd0", 1, 1);
    mock_write_i2c_read(0x76, "\x58", 1, 1);

    /* Calibration data. */
    mock_write_i2c_write(0x76, "\x88", 1, 1);
    mock_write_i2c_read(0x76,
                        "\x70\x6b\x43\x67\x18\xfc\x5f\x8e"
                        "\x43\xd6\xd0\x0b\x27\x0b\x8c\x00"
                        "\xf9\xff\x8c\x3c\xf8\xc6\x70\x17",
                        24,
                        24);

    BTASSERT(bmp280_start(&bmp280_i2c) == 0);

    return (0);
}

static int test_i2c_read(void)
{
    float temperature;
    float pressure;

    /* Start a convertion. */
    mock_write_i2c_write(0x76, "\xf4\x25", 2, 2);

    /* Status reading - not ready. */
    mock_write_i2c_write(0x76, "\xf3", 1, 1);
    mock_write_i2c_read(0x76, "\xff", 1, 1);

    /* Status reading - ready. */
    mock_write_i2c_write(0x76, "\xf3", 1, 1);
    mock_write_i2c_read(0x76, "\x00", 1, 1);

    /* Temperature and pressure reading. */
    mock_write_i2c_write(0x76, "\xf7", 1, 1);
    mock_write_i2c_read(0x76, "\x65\x5a\xc0\x7e\xed\x00", 6, 6);

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == 0);
    BTASSERT(temperature > 25.082);
    BTASSERT(temperature < 25.083);
    BTASSERT(pressure > 100700.0);
    BTASSERT(pressure < 100800.0);

    return (0);
}

static int test_i2c_read_meas_start_error(void)
{
    float temperature;
    float pressure;

    /* Measurement start - IO error. */
    mock_write_i2c_write(0x76, "\xf4\x25", 2, -EIO);

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == -EIO);

    return (0);
}

static int test_i2c_read_status_error(void)
{
    float temperature;
    float pressure;

    /* Start a convertion. */
    mock_write_i2c_write(0x76, "\xf4\x25", 2, 2);

    /* Status register - IO error. */
    mock_write_i2c_write(0x76, "\xf3", 1, -EIO);

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == -EIO);

    return (0);
}

static int test_i2c_read_status_timeout(void)
{
    float temperature;
    float pressure;
    int i;

    /* Start a convertion. */
    mock_write_i2c_write(0x76, "\xf4\x25", 2, 2);

    /* Status reading - not ready. */
    for (i = 0; i < CONFIG_BMP280_COVERTION_TIMEOUT_MS; i++) {
        mock_write_i2c_write(0x76, "\xf3", 1, 1);
        mock_write_i2c_read(0x76, "\xff", 1, 1);
    }

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == -ETIMEDOUT);

    return (0);
}

static int test_i2c_stop(void)
{
    BTASSERT(bmp280_stop(&bmp280_i2c) == -ENOSYS);

    return (0);
}

static int test_i2c_start_automatic(void)
{
    BTASSERT(bmp280_transport_i2c_init(&transport_i2c,
                                       &i2c,
                                       BMP280_I2C_ADDRESS_AUTOMATIC) == 0);
    BTASSERT(bmp280_init(&bmp280_i2c,
                         &transport_i2c.base,
                         bmp280_mode_forced_t,
                         bmp280_standby_time_500_us_t,
                         bmp280_filter_off_t,
                         bmp280_temperature_oversampling_1_t,
                         bmp280_pressure_oversampling_1_t) == 0);

    /* Found device with id 0x77. */
    mock_write_i2c_scan(0x76, 0);
    mock_write_i2c_scan(0x77, 1);

    /* Chip id. */
    mock_write_i2c_write(0x77, "\xd0", 1, 1);
    mock_write_i2c_read(0x77, "\x48", 1, 1);

    /* Calibration data. */
    mock_write_i2c_write(0x77, "\x88", 1, 1);
    mock_write_i2c_read(0x77,
                        "\x70\x6b\x43\x67\x18\xfc\x5f\x8e"
                        "\x43\xd6\xd0\x0b\x27\x0b\x8c\x00"
                        "\xf9\xff\x8c\x3c\xf8\xc6\x70\x17",
                        24,
                        24);

    BTASSERTI(bmp280_start(&bmp280_i2c), ==, 0);

    return (0);
}

static int test_i2c_start_automatic_no_dev(void)
{
    BTASSERT(bmp280_transport_i2c_init(&transport_i2c,
                                       &i2c,
                                       BMP280_I2C_ADDRESS_AUTOMATIC) == 0);
    BTASSERT(bmp280_init(&bmp280_i2c,
                         &transport_i2c.base,
                         bmp280_mode_forced_t,
                         bmp280_standby_time_500_us_t,
                         bmp280_filter_off_t,
                         bmp280_temperature_oversampling_1_t,
                         bmp280_pressure_oversampling_1_t) == 0);

    /* No device found. */
    mock_write_i2c_scan(0x76, 0);
    mock_write_i2c_scan(0x77, 0);

    BTASSERTI(bmp280_start(&bmp280_i2c), ==, -ENODEV);

    return (0);
}

static int test_spi_start(void)
{
    BTASSERT(bmp280_transport_spi_init(&transport_spi,
                                       &spi) == 0);
    BTASSERT(bmp280_init(&bmp280_spi,
                         &transport_spi.base,
                         bmp280_mode_normal_t,
                         bmp280_standby_time_500_us_t,
                         bmp280_filter_off_t,
                         bmp280_temperature_oversampling_1_t,
                         bmp280_pressure_oversampling_1_t) == 0);

    /* Chip id. */
    mock_write_spi_select(0);
    mock_write_spi_write("\xd0", 1, 1);
    mock_write_spi_read("\x58", 1, 1);
    mock_write_spi_deselect(0);

    /* Normal mode configuration. */
    mock_write_spi_select(0);
    mock_write_spi_write("\x74\x27", 2, 2);
    mock_write_spi_deselect(0);

    /* Calibration data. */
    mock_write_spi_select(0);
    mock_write_spi_write("\x88", 1, 1);
    mock_write_spi_read("\x70\x6b\x43\x67\x18\xfc\x5f\x8e"
                        "\x43\xd6\xd0\x0b\x27\x0b\x8c\x00"
                        "\xf9\xff\x8c\x3c\xf8\xc6\x70\x17",
                        24,
                        24);
    mock_write_spi_deselect(0);

    BTASSERT(bmp280_start(&bmp280_spi) == 0);

    return (0);
}

static int test_spi_read_fixed_point(void)
{
    long temperature;
    long pressure;

    /* Temperature and pressure reading. */
    mock_write_spi_select(0);
    mock_write_spi_write("\xf7", 1, 1);
    mock_write_spi_read("\x65\x5a\xc0\x7e\xed\x00", 6, 6);
    mock_write_spi_deselect(0);

    BTASSERT(bmp280_read_fixed_point(&bmp280_spi,
                                     &temperature,
                                     &pressure) == 0);
    BTASSERTI(temperature, ==, 25082);
    BTASSERTI(pressure, ==, 100739000);

    return (0);
}

static int test_spi_stop(void)
{
    BTASSERT(bmp280_stop(&bmp280_spi) == -ENOSYS);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_i2c_start, "test_i2c_start" },
        { test_i2c_read, "test_i2c_read" },
        { test_i2c_read_meas_start_error, "test_i2c_read_meas_start_error" },
        { test_i2c_read_status_error, "test_i2c_read_status_error" },
        { test_i2c_read_status_timeout, "test_i2c_read_status_timeout" },
        { test_i2c_stop, "test_i2c_stop" },
        { test_i2c_start_automatic, "test_i2c_start_automatic" },
        { test_i2c_start_automatic_no_dev, "test_i2c_start_automatic_no_dev" },
        { test_spi_start, "test_spi_start" },
        { test_spi_read_fixed_point, "test_spi_read_fixed_point" },
        { test_spi_stop, "test_spi_stop" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
