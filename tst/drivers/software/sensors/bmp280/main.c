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

#include "simba.h"

struct bmp280_driver_t bmp280_i2c;
struct i2c_driver_t i2c;
struct bmp280_transport_i2c_t transport_i2c;

struct bmp280_driver_t bmp280_spi;
struct spi_driver_t spi;
struct bmp280_transport_spi_t transport_spi;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(bmp280_module_init() == 0);
    BTASSERT(bmp280_module_init() == 0);

    return (0);
}

static int test_i2c_start(struct harness_t *harness_p)
{
    uint8_t byte;
    size_t size;
    ssize_t res;
    uint8_t address;
    int i_res;

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

    /* Chip id. */
    res = 1;
    harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_read(): return (buf_p)", "\x58", res);

    /* Calibration data. */
    res = 24;
    harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_read(): return (buf_p)",
                       "\x70\x6b\x43\x67\x18\xfc\x5f\x8e"
                       "\x43\xd6\xd0\x0b\x27\x0b\x8c\x00"
                       "\xf9\xff\x8c\x3c\xf8\xc6\x70\x17",
                       res);
    i_res = 1;
    harness_mock_write("i2c_scan(): return (res)", &i_res, sizeof(i_res));

    BTASSERT(bmp280_start(&bmp280_i2c) == 0);

    /* Chip id. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &byte, size);
    BTASSERTM(&byte, "\xd0", size);

    harness_mock_read("i2c_read(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);

    /* Calibration data. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &byte, size);
    BTASSERTM(&byte, "\x88", size);

    harness_mock_read("i2c_read(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 24);

    harness_mock_read("i2c_scan(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);

    return (0);
}

static int test_i2c_read(struct harness_t *harness_p)
{
    float temperature;
    float pressure;
    int address;
    uint8_t buf[8];
    size_t size;
    ssize_t res;

    /* Status reading - not ready. */
    res = 1;
    harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_read(): return (buf_p)", "\xff", res);

    /* Status reading - ready. */
    res = 1;
    harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_read(): return (buf_p)", "\x00", res);

    /* Temperature and pressure reading. */
    res = 6;
    harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_read(): return (buf_p)",
                       "\x65\x5a\xc0\x7e\xed\x00",
                       res);

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == 0);
    BTASSERT(temperature > 25.082);
    BTASSERT(temperature < 25.083);
    BTASSERT(pressure > 100700.0);
    BTASSERT(pressure < 100800.0);

    /* Start a convertion. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 2);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf4\x25", size);

    /* Read status - not ready. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf3", size);

    harness_mock_read("i2c_read(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);

    /* Read status - ready. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf3", size);

    harness_mock_read("i2c_read(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);

    /* Read temperature and pressure. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf7", size);

    harness_mock_read("i2c_read(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 6);

    return (0);
}

static int test_i2c_read_meas_start_error(struct harness_t *harness_p)
{
    float temperature;
    float pressure;
    int address;
    uint8_t buf[8];
    size_t size;
    ssize_t res;

    /* Measurement start - IO error. */
    res = -EIO;
    harness_mock_write("i2c_write(): return (res)", &res, sizeof(res));

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == -EIO);

    /* Measurement start - IO error. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 2);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf4\x25", size);

    return (0);
}

static int test_i2c_read_status_error(struct harness_t *harness_p)
{
    float temperature;
    float pressure;
    int address;
    uint8_t buf[8];
    size_t size;
    ssize_t res;

    /* Start a convertion. */
    res = 2;
    harness_mock_write("i2c_write(): return (res)", &res, sizeof(res));

    /* Status register - IO error. */
    res = -EIO;
    harness_mock_write("i2c_write(): return (res)", &res, sizeof(res));

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == -EIO);

    /* Start a convertion. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 2);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf4\x25", size);

    /* Status register write attempt. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf3", size);

    return (0);
}

static int test_i2c_read_status_timeout(struct harness_t *harness_p)
{
    float temperature;
    float pressure;
    int address;
    uint8_t buf[8];
    size_t size;
    ssize_t res;
    int i;

    /* Start a convertion. */
    res = 2;
    harness_mock_write("i2c_write(): return (res)", &res, sizeof(res));

    /* Status reading - not ready. */
    res = 1;

    for (i = 0; i < CONFIG_BMP280_COVERTION_TIMEOUT_MS; i++) {
        harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
        harness_mock_write("i2c_read(): return (buf_p)", "\xff", res);
    }

    BTASSERT(bmp280_read(&bmp280_i2c, &temperature, &pressure) == -ETIMEDOUT);

    /* Start a convertion. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 2);
    harness_mock_read("i2c_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf4\x25", size);

    /* Read status - not ready. */
    res = 1;

    for (i = 0; i < CONFIG_BMP280_COVERTION_TIMEOUT_MS; i++) {
        harness_mock_read("i2c_write(address)", &address, sizeof(address));
        BTASSERTI(address, ==, 0x76);
        harness_mock_read("i2c_write(size)", &size, sizeof(size));
        BTASSERTI(size, ==, 1);
        harness_mock_read("i2c_write(buf_p)", &buf[0], size);
        BTASSERTM(&buf[0], "\xf3", size);

        harness_mock_read("i2c_read(address)", &address, sizeof(address));
        BTASSERTI(address, ==, 0x76);
        harness_mock_read("i2c_read(size)", &size, sizeof(size));
        BTASSERTI(size, ==, 1);
    }

    return (0);
}

static int test_i2c_stop(struct harness_t *harness_p)
{
    BTASSERT(bmp280_stop(&bmp280_i2c) == -ENOSYS);

    return (0);
}

static int test_i2c_start_automatic(struct harness_t *harness_p)
{
    uint8_t byte;
    size_t size;
    ssize_t res;
    uint8_t address;
    int i_res;

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

    /* Chip id. */
    res = 1;
    harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_read(): return (buf_p)", "\x48", res);

    /* Calibration data. */
    res = 24;
    harness_mock_write("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_read(): return (buf_p)",
                       "\x70\x6b\x43\x67\x18\xfc\x5f\x8e"
                       "\x43\xd6\xd0\x0b\x27\x0b\x8c\x00"
                       "\xf9\xff\x8c\x3c\xf8\xc6\x70\x17",
                       res);
    i_res = 0;
    harness_mock_write("i2c_scan(): return (res)", &i_res, sizeof(i_res));
    i_res = 1;
    harness_mock_write("i2c_scan(): return (res)", &i_res, sizeof(i_res));

    BTASSERTI(bmp280_start(&bmp280_i2c), ==, 0);

    /* Chip id. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x77);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &byte, size);
    BTASSERTM(&byte, "\xd0", size);

    harness_mock_read("i2c_read(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x77);
    harness_mock_read("i2c_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);

    /* Calibration data. */
    harness_mock_read("i2c_write(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x77);
    harness_mock_read("i2c_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("i2c_write(buf_p)", &byte, size);
    BTASSERTM(&byte, "\x88", size);

    harness_mock_read("i2c_read(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x77);
    harness_mock_read("i2c_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 24);

    harness_mock_read("i2c_scan(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);

    harness_mock_read("i2c_scan(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x77);

    return (0);
}

static int test_i2c_start_automatic_no_dev(struct harness_t *harness_p)
{
    int res;
    uint8_t address;

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

    res = 0;
    harness_mock_write("i2c_scan(): return (res)", &res, sizeof(res));
    harness_mock_write("i2c_scan(): return (res)", &res, sizeof(res));

    BTASSERTI(bmp280_start(&bmp280_i2c), ==, -ENODEV);

    harness_mock_read("i2c_scan(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x76);

    harness_mock_read("i2c_scan(address)", &address, sizeof(address));
    BTASSERTI(address, ==, 0x77);

    return (0);
}

static int test_spi_start(struct harness_t *harness_p)
{
    size_t size;
    ssize_t res;
    uint8_t buf[2];

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
    res = 1;
    harness_mock_write("spi_read(): return (res)", &res, sizeof(res));
    harness_mock_write("spi_read(): return (buf_p)", "\x58", res);

    /* Calibration data. */
    res = 24;
    harness_mock_write("spi_read(): return (res)", &res, sizeof(res));
    harness_mock_write("spi_read(): return (buf_p)",
                       "\x70\x6b\x43\x67\x18\xfc\x5f\x8e"
                       "\x43\xd6\xd0\x0b\x27\x0b\x8c\x00"
                       "\xf9\xff\x8c\x3c\xf8\xc6\x70\x17",
                       res);

    BTASSERT(bmp280_start(&bmp280_spi) == 0);

    /* Chip id. */
    harness_mock_read("spi_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("spi_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xd0", size);

    harness_mock_read("spi_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);

    /* Normal mode configuration. */
    harness_mock_read("spi_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 2);
    harness_mock_read("spi_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\x74\x27", size);

    /* Calibration data read. */
    harness_mock_read("spi_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("spi_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\x88", size);

    harness_mock_read("spi_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 24);

    return (0);
}

static int test_spi_read_fixed_point(struct harness_t *harness_p)
{
    long temperature;
    long pressure;
    uint8_t buf[8];
    size_t size;
    ssize_t res;

    /* Temperature and pressure reading. */
    res = 6;
    harness_mock_write("spi_read(): return (res)", &res, sizeof(res));
    harness_mock_write("spi_read(): return (buf_p)",
                       "\x65\x5a\xc0\x7e\xed\x00",
                       res);

    BTASSERT(bmp280_read_fixed_point(&bmp280_spi,
                                     &temperature,
                                     &pressure) == 0);
    BTASSERTI(temperature, ==, 25082);
    BTASSERTI(pressure, ==, 100739000);

    /* Read temperature and pressure. */
    harness_mock_read("spi_write(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 1);
    harness_mock_read("spi_write(buf_p)", &buf[0], size);
    BTASSERTM(&buf[0], "\xf7", size);

    harness_mock_read("spi_read(size)", &size, sizeof(size));
    BTASSERTI(size, ==, 6);

    return (0);
}

static int test_spi_stop(struct harness_t *harness_p)
{
    BTASSERT(bmp280_stop(&bmp280_spi) == -ENOSYS);

    return (0);
}

ssize_t STUB(i2c_read)(struct i2c_driver_t *self_p,
                       int address,
                       void *buf_p,
                       size_t size)
{
    ssize_t res;

    harness_mock_write("i2c_read(address)", &address, sizeof(address));
    harness_mock_write("i2c_read(size)", &size, sizeof(size));
    harness_mock_read("i2c_read(): return (res)", &res, sizeof(res));
    harness_mock_read("i2c_read(): return (buf_p)", buf_p, res);

    return (size);
}

ssize_t STUB(i2c_write)(struct i2c_driver_t *self_p,
                        int address,
                        const void *buf_p,
                        size_t size)
{
    ssize_t res;

    harness_mock_write("i2c_write(address)", &address, sizeof(address));
    harness_mock_write("i2c_write(buf_p)", buf_p, size);
    harness_mock_write("i2c_write(size)", &size, sizeof(size));

    if (harness_mock_read("i2c_write(): return (res)",
                          &res,
                          sizeof(res)) != sizeof(res)) {
        res = size;
    }

    return (res);
}

int STUB(i2c_scan)(struct i2c_driver_t *self_p,
                   int address)
{
    int res;

    harness_mock_write("i2c_scan(address)", &address, sizeof(address));
    harness_mock_read("i2c_scan(): return (res)", &res, sizeof(res));

    return (res);
}

int STUB(spi_select)(struct spi_driver_t *self_p)
{
    return (-1);
}

int STUB(spi_deselect)(struct spi_driver_t *self_p)
{
    return (-1);
}

ssize_t STUB(spi_read)(struct spi_driver_t *self_p,
                       void *buf_p,
                       size_t size)
{
    ssize_t res;

    harness_mock_write("spi_read(size)", &size, sizeof(size));
    harness_mock_read("spi_read(): return (res)", &res, sizeof(res));
    harness_mock_read("spi_read(): return (buf_p)", buf_p, res);

    return (size);
}

ssize_t STUB(spi_write)(struct spi_driver_t *self_p,
                        const void *buf_p,
                        size_t size)
{
    ssize_t res;

    harness_mock_write("spi_write(buf_p)", buf_p, size);
    harness_mock_write("spi_write(size)", &size, sizeof(size));

    if (harness_mock_read("spi_write(): return (res)",
                          &res,
                          sizeof(res)) != sizeof(res)) {
        res = size;
    }

    return (res);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
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

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
