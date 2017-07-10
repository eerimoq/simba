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

#include "simba.h"

#if CONFIG_BMP280 == 1

#define REG_CALIBRATION                              0x88
#define REG_STATUS                                   0xf3
#define REG_CTRL_MEAS                                0xf4
#define REG_CONFIG                                   0xf5
#define REG_PRESS_MSB                                0xf7
#define REG_PRESS_LSB                                0xf8
#define REG_PRESS_XLSB                               0xf9
#define REG_TEMP_MSB                                 0xfa
#define REG_TEMP_LSB                                 0xfb
#define REG_TEMP_XLSB                                0xfc

#define REG_STATUS_MEASURING                       BIT(3)

#define REG_CTRL_MEAS_OSRS_T(value)          (value << 5)
#define REG_CTRL_MEAS_OSRS_P(value)          (value << 2)
#define REG_CTRL_MEAS_MODE(value)            (value << 0)
#define REG_CTRL_MEAS_MODE_SLEEP   REG_CTRL_MEAS_MODE(0)
#define REG_CTRL_MEAS_MODE_FORCED  REG_CTRL_MEAS_MODE(value)
#define REG_CTRL_MEAS_MODE_NORMAL  REG_CTRL_MEAS_MODE(value)

#define REG_CONFIG_T_SB(value)               (value << 5)
#define REG_CONFIG_FILTER(value)             (value << 2)

struct bmp280_transport_protocol_t {
    int (*read)(void *transport_p,
                uint8_t address,
                uint8_t *buf_p,
                size_t size);
    int (*write)(void *transport_p,
                 uint8_t address,
                 uint8_t data);
};

#if CONFIG_I2C == 1

static int transport_i2c_read(void *transport_p,
                              uint8_t address,
                              uint8_t *buf_p,
                              size_t size)
{
    struct bmp280_transport_i2c_t *self_p;
    ssize_t res;

    self_p = transport_p;

    res = i2c_write(self_p->i2c_p,
                    self_p->i2c_address,
                    &address,
                    sizeof(address));

    if (res != sizeof(address)) {
        return (-EIO);
    }

    res = i2c_read(self_p->i2c_p,
                   self_p->i2c_address,
                   buf_p,
                   size);

    if (res != size) {
        return (-EIO);
    }

    return (0);
}

static int transport_i2c_write(void *transport_p,
                               uint8_t address,
                               uint8_t data)
{
    struct bmp280_transport_i2c_t *self_p;
    ssize_t res;
    uint8_t buf[2];

    self_p = transport_p;

    buf[0] = address;
    buf[1] = data;

    res = i2c_write(self_p->i2c_p,
                    self_p->i2c_address,
                    &buf[0],
                    sizeof(buf));

    if (res != sizeof(buf)) {
        return (-EIO);
    }

    return (0);
}

struct bmp280_transport_protocol_t transport_i2c_protocol = {
    .read = transport_i2c_read,
    .write = transport_i2c_write
};

#endif

#if CONFIG_SPI == 1

static int transport_spi_read(void *transport_p,
                              uint8_t address,
                              uint8_t *buf_p,
                              size_t size)
{
    struct bmp280_transport_spi_t *self_p;
    ssize_t res;

    self_p = transport_p;

    spi_select(self_p->spi_p);
    res = spi_write(self_p->spi_p,
                    &address,
                    sizeof(address));

    if (res == sizeof(address)) {
        res = spi_read(self_p->spi_p,
                       buf_p,
                       size);
    } else {
        res = -EIO;
    }

    spi_deselect(self_p->spi_p);

    if (res != size) {
        return (-EIO);
    }

    return (0);
}

static int transport_spi_write(void *transport_p,
                               uint8_t address,
                               uint8_t data)
{
    struct bmp280_transport_spi_t *self_p;
    ssize_t res;
    uint8_t buf[2];

    self_p = transport_p;

    /* Clear bit 7 for write. */
    buf[0] = (address & 0x7f);
    buf[1] = data;

    spi_select(self_p->spi_p);
    res = spi_write(self_p->spi_p, &buf[0], sizeof(buf));
    spi_deselect(self_p->spi_p);

    if (res != sizeof(buf)) {
        return (-EIO);
    }

    return (0);
}

struct bmp280_transport_protocol_t transport_spi_protocol = {
    .read = transport_spi_read,
    .write = transport_spi_write
};

#endif

static int is_normal_mode(struct bmp280_driver_t *self_p)
{
    return ((self_p->ctrl_meas & 0x3) == bmp280_mode_normal_t);
}

static int is_convertion_running(struct bmp280_driver_t *self_p)
{
    uint8_t status;
    int res;

    res = self_p->transport_p->protocol_p->read(self_p->transport_p,
                                                REG_STATUS,
                                                &status,
                                                sizeof(status));

    if (res != 0) {
        return (-EIO);
    }

    return ((status & REG_STATUS_MEASURING) != 0);
}

static int write_ctrl_meas(struct bmp280_driver_t *self_p)
{
    return (self_p->transport_p->protocol_p->write(self_p->transport_p,
                                                   REG_CTRL_MEAS,
                                                   self_p->ctrl_meas));
}

/**
 * Convert given ADC measurements to mDegC and mPa using 32 bits
 * integers. The rounding error will be rather big.
 */
static void compensate(struct bmp280_driver_t *self_p,
                       int32_t adc_t,
                       int32_t adc_p,
                       long *temperature_p,
                       long *pressure_p)
{
    int32_t var1;
    int32_t var2;
    int32_t t_fine;
    uint32_t p;

    /* Temperature compensation. */
    var1 = ((adc_t >> 3) - (self_p->compensation[0] << 1));
    var1 *= self_p->compensation[1];
    var1 >>= 11;
    var2 = ((adc_t >> 4) - (uint16_t)self_p->compensation[0]);
    var2 *= var2;
    var2 >>= 12;
    var2 *= self_p->compensation[2];
    var2 >>= 14;
    t_fine = (var1 + var2);

    *temperature_p = DIV_ROUND(800 * t_fine, 4096);

    /* Pressure compensation. */
    var1 = ((t_fine >> 1) - 64000);
    var2 = (var1 >> 2);
    var2 *= var2;
    var2 >>= 11;
    var2 *= self_p->compensation[8];
    var2 += ((var1 * self_p->compensation[7]) << 1);
    var2 >>= 2;
    var2 += ((int32_t)self_p->compensation[6] << 16);
    var1 = ((((self_p->compensation[5] * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3)
             + ((self_p->compensation[4] * var1) >> 1)) >> 18);
    var1 += 32768;
    var1 *= (uint16_t)self_p->compensation[3];
    var1 >>= 15;

    if (var1 == 0) {
        var1 = 1;
    }

    p = (uint32_t)(1048576 - adc_p);
    p -= (var2 >> 12);
    p *= 3125;

    if (p < 0x80000000) {
        p = (p << 1) / ((uint32_t)var1);
    } else {
        p = (p / (uint32_t)var1) * 2;
    }

    var1 = ((self_p->compensation[11]
             * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12);
    var2 = ((((int32_t)(p >> 2)) * ((int32_t)self_p->compensation[10])) >> 13);

    *pressure_p = ((int32_t)p + ((var1 + var2 + self_p->compensation[9]) >> 4));
}

int bmp280_module_init()
{
    return (0);
}

int bmp280_init(struct bmp280_driver_t *self_p,
                struct bmp280_transport_t *transport_p,
                enum bmp280_mode_t mode,
                enum bmp280_standby_time_t standby_time,
                enum bmp280_filter_t filter,
                enum bmp280_temperature_oversampling_t temperature_oversampling,
                enum bmp280_pressure_oversampling_t pressure_oversampling)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(transport_p != NULL, EINVAL);

    self_p->transport_p = transport_p;
    self_p->ctrl_meas = (REG_CTRL_MEAS_OSRS_T(temperature_oversampling)
                         | REG_CTRL_MEAS_OSRS_P(pressure_oversampling)
                         | REG_CTRL_MEAS_MODE(mode));
    self_p->config = (REG_CONFIG_T_SB(standby_time)
                      | REG_CONFIG_FILTER(filter));

    return (0);
}

int bmp280_start(struct bmp280_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res;
    uint8_t buf[24];
    int i;

    res = 0;

    if (is_normal_mode(self_p)) {
        res = write_ctrl_meas(self_p);
    }

    if (res == 0) {
        res = self_p->transport_p->protocol_p->read(self_p->transport_p,
                                                    REG_CALIBRATION,
                                                    &buf[0],
                                                    sizeof(buf));

        /* Endianess. */
        for (i = 0; i < membersof(self_p->compensation); i++) {
            self_p->compensation[i] = (((buf[2 * i + 1]) << 8) | buf[2 * i]);
        }
    }

    return (res);
}

int bmp280_stop(struct bmp280_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (0);
}

int bmp280_read(struct bmp280_driver_t *self_p,
                float *temperature_p,
                float *pressure_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    uint8_t buf[6];
    int res;
    int32_t adc_t;
    int32_t adc_p;
    long temperature;
    long pressure;
    int attempt;

    /* Start a measurement in forced mode if configured. */
    if (!is_normal_mode(self_p)) {
        res = write_ctrl_meas(self_p);

        if (res != 0) {
            return (res);
        }

        attempt = 1;

        while (1) {
            res = is_convertion_running(self_p);

            if (res < 0) {
                return (res);
            }

            if (res == 0) {
                break;
            }

            if (attempt == CONFIG_BMP280_COVERTION_TIMEOUT_MS) {
                return (-ETIMEDOUT);
            }

            attempt++;
            thrd_sleep_ms(1);
        }
    }

    /* Read the pressure and temperature. */
    res = self_p->transport_p->protocol_p->read(self_p->transport_p,
                                                REG_PRESS_MSB,
                                                &buf[0],
                                                sizeof(buf));

    if (res == 0) {
        adc_t = ((buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4));
        adc_p = ((buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4));

        compensate(self_p,
                   adc_t,
                   adc_p,
                   &temperature,
                   &pressure);

        if (temperature_p != NULL) {
            *temperature_p = (temperature / 1000.0f);
        }

        if (pressure_p != NULL) {
            *pressure_p = (pressure / 1000.0f);
        }
    }

    return (res);
}

int bmp280_transport_i2c_init(struct bmp280_transport_i2c_t *self_p,
                              struct i2c_driver_t *i2c_p,
                              int i2c_address)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(i2c_p != NULL, EINVAL);
    ASSERTN((i2c_address == BMP280_I2C_ADDRESS_0)
            || (i2c_address == BMP280_I2C_ADDRESS_1), EINVAL);

#if CONFIG_I2C == 1

    self_p->base.protocol_p = &transport_i2c_protocol;
    self_p->i2c_p = i2c_p;
    self_p->i2c_address = i2c_address;

    return (0);
#else
    return (-ENOSYS);
#endif
}

int bmp280_transport_spi_init(struct bmp280_transport_spi_t *self_p,
                              struct spi_driver_t *spi_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(spi_p != NULL, EINVAL);

#if CONFIG_SPI == 1

    self_p->base.protocol_p = &transport_spi_protocol;
    self_p->spi_p = spi_p;

    return (0);
#else
    return (-ENOSYS);
#endif
}

#endif
