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

#include "simba.h"

/*
 * This driver supports the Sensirion SHT3x-D series of digital of
 * Humidity and Temperature Sensors, specifically the SHT30-D,
 * SHT31-D, and SHT35-D.
 *
 * Datasheet:
 * https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Sensirion_Humidity_Sensors_SHT3x_Datasheet_digital.pdf
 *
 * Reference code:
 * https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/11_Sample_Codes_Software/Humidity_Sensors/Sensirion_Humidity_Sensors_SHT3x_Sample_Code_V2.pdf
 */

#if CONFIG_SHT3XD == 1

#define CMD_SINGLE_CLKSTRETCH_REPHIGH     0x2c06
#define CMD_SINGLE_NOCLKSTRETCH_REPHIGH   0x2400
#define CMD_GETSERIAL                     0x3780

#ifdef CONFIG_SHT3XD_DEBUG_LOG_MASK
#    define DLOG(self_p, level, msg, ...)                               \
    log_object_print(&(self_p)->log, level, OSTR(msg), ##__VA_ARGS__)
#else
#    define DLOG(self_p, level, msg, ...)
#endif

/**
 * Send a single 16bit command to the SHT3x-D device.
 */
static int sht3xd_sendcmd(struct sht3xd_driver_t *self_p, uint16_t cmd)
{
    uint8_t i2ccmd[2];
    int res;

    i2ccmd[0] = (cmd >> 8) & 0xff;
    i2ccmd[1] = cmd & 0xff;
    DLOG(self_p, LOG_DEBUG, "Sending command 0x%04x to device", cmd);
    res = i2c_write(self_p->i2c_p, self_p->i2c_addr, i2ccmd, 2);

    if (res != 2) {
        DLOG(self_p, LOG_ERROR, "Failed to write to device: %d", res);

        if (res < 0) {
            return (res);
        }

        return (-EIO);
    }

    return (0);
}

/**
 * Read 2x 16bit to the SHT3x-D device.
 */
static int sht3xd_read2x16(struct sht3xd_driver_t *self_p, uint8_t *data_p)
{
    int res;

    res = i2c_read(self_p->i2c_p, self_p->i2c_addr, data_p, 6);

    if (res != 6) {
        DLOG(self_p, LOG_ERROR, "Failed to read data: %d", res);
        return (-EIO);
    }

    DLOG(self_p,
         LOG_DEBUG,
         "Got data from SHT3x: 0x%02x%02x%02x 0x%02x%02x%02x",
         data_p[0],
         data_p[1],
         data_p[2],
         data_p[3],
         data_p[4],
         data_p[5]);

    /* Here should go CRC check. */

    return (0);
}

/**
 * Read serial off SHT3x-D.
 */
static int sht3xd_read_serial(struct sht3xd_driver_t *self_p)
{
    int res;
    uint8_t data[6];

    res = sht3xd_sendcmd(self_p, CMD_GETSERIAL);

    if (res != 0) {
        return (res);
    }

    res = sht3xd_read2x16(self_p, data);

    if (res != 0) {
        return (res);
    }

    self_p->serial = ((uint32_t)data[0] << 24
                      | (uint32_t)data[1] << 16
                      | (uint32_t)data[3] << 8
                      | (uint32_t)data[4]);

    return (0);
}

int sht3xd_module_init()
{
    i2c_module_init();
#ifdef CONFIG_SHT3XD_DEBUG_LOG_MASK
    log_module_init();
#endif

    return (0);
}

int sht3xd_init(struct sht3xd_driver_t *self_p,
                struct i2c_driver_t *i2c_p,
                int i2c_addr)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(i2c_p != NULL, EINVAL);

    self_p->i2c_p = i2c_p;
    self_p->i2c_addr = i2c_addr;

#ifdef CONFIG_SHT3XD_DEBUG_LOG_MASK
    log_object_init(&self_p->log, "sht3xd", CONFIG_SHT3XD_DEBUG_LOG_MASK);
#endif

    return (0);
}

int sht3xd_start(struct sht3xd_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res;

    DLOG(self_p, LOG_INFO, "Starting SHT3x-D driver");
    i2c_start(self_p->i2c_p);

    if (i2c_scan(self_p->i2c_p, self_p->i2c_addr) != 1) {
        DLOG(self_p, LOG_WARNING, "No device found at 0x%x", self_p->i2c_addr);
        return (-ENOENT);
    }

    res = sht3xd_read_serial(self_p);

    if (res != 0) {
        DLOG(self_p,
             LOG_ERROR,
             "Error reading serial from 0x%x",
             self_p->i2c_addr);
        return (res);
    }

    DLOG(self_p,
         LOG_INFO,
         "Device found serial %8x at 0x%x",
         self_p->serial,
         self_p->i2c_addr);

    return (0);
}

/**
 * Calculate the temperature in Celsius from SHT3xD raw temperature.
 */
static float sht3xd_calculate_temp_c(uint8_t msb, uint8_t lsb)
{
    uint16_t val;
    float temp;

    val = msb << 8 | lsb;
    temp = 175.0f * ((float)val / 65535.0f) - 45.0f;

    return (temp);
}

/**
 * Calculate the humidity from SHT3xD raw temperature.
 */
static float sht3xd_calculate_humid(uint8_t msb, uint8_t lsb)
{
    uint16_t val;
    float humid;

    val = msb << 8 | lsb;
    humid = 100.0f * ((float)val / 65535.0f);

    return (humid);
}

int sht3xd_get_temp_humid(struct sht3xd_driver_t *self_p,
                          float *temp_p,
                          float *humid_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res;
    uint8_t data[6];

    /*
     * We use a non-clockstreching command followed by a sleep and
     * then issue read of the result.  This is so we can do a single
     * long sleep (probably a bit more power efficient), and to not
     * block the I2C bus while sensor is active - in case it's shared.
     */
    res = sht3xd_sendcmd(self_p, CMD_SINGLE_NOCLKSTRETCH_REPHIGH);

    if (res != 0) {
        return (res);
    }

    /* We use max duration to avoid having to handle retry. */
    thrd_sleep_ms(MEASUREMENT_DURATION_HIGH_MS);

    res = sht3xd_read2x16(self_p, data);

    if (res != 0) {
        return (res);
    }

    if (temp_p != NULL) {
        *temp_p = sht3xd_calculate_temp_c(data[0], data[1]);
    }

    if (humid_p != NULL) {
        *humid_p = sht3xd_calculate_humid(data[3], data[4]);
    }

    return (0);
}

int sht3xd_get_serial(struct sht3xd_driver_t *self_p,
                      uint32_t *serial_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(serial_p != NULL, EINVAL);

    *serial_p = self_p->serial;

    return (0);
}

#endif
