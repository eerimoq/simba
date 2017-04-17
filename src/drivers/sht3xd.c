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

/**
 * Send a single 16bit command to the SHT3x-D device.
 */
static int sht3xd_sendcmd(struct sht3xd_driver_t *self_p, uint16_t cmd)
{
    ASSERTN(self_p != NULL, EINVAL);

    uint8_t i2ccmd[2];
    int r;

    i2ccmd[0] = (cmd >> 8) & 0xff;
    i2ccmd[1] = cmd & 0xff;
#ifdef DRIVER_DEBUG_LOG
    log_object_print(&self_p->log, LOG_DEBUG,
                     OSTR("Sending command 0x%04x to device"),
                     cmd);
#endif
    r = i2c_soft_write(self_p->i2c_p, self_p->i2c_addr, i2ccmd, 2);
    if (r != 2) {
#ifdef DRIVER_DEBUG_LOG
        log_object_print(&self_p->log, LOG_ERROR,
                         OSTR("Failed to write to device; err %d"), r);
#endif
        if (r < 0)
            return (r);
        return (-EIO);
    }

    return (0);
}

/**
 * Read 2x 16bit to the SHT3x-D device.
 */
static int sht3xd_read2x16(struct sht3xd_driver_t *self_p, uint8_t *data)
{
    ASSERTN(self_p != NULL, EINVAL);

    int r;

    r = i2c_soft_read(self_p->i2c_p, self_p->i2c_addr, data, 6);
    if (r != 6) {
#ifdef DRIVER_DEBUG_LOG
        log_object_print(&self_p->log,
                         LOG_ERROR,
                         OSTR("Failed to read data - %d\r\n"),
                         r);
#endif
        return (-EIO);
    }
#ifdef DRIVER_DEBUG_LOG
    log_object_print(&self_p->log,
                     LOG_INFO,
                     OSTR("Got data from SHT3x: 0x%02x%02x%02x "
                          "x%02x%02x%02x"),
                     data[0],
                     data[1],
                     data[2],
                     data[3],
                     data[4],
                     data[5]);
#endif

    // Here should go CRC check.

    return (0);
}

/**
 * Read serial off SHT3x-D.
 */
static int sht3xd_read_serial(struct sht3xd_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int r;
    uint8_t data[6];

    r = sht3xd_sendcmd(self_p, CMD_GETSERIAL);
    if (r != 0) {
        return (r);
    }

    r = sht3xd_read2x16(self_p, data);
    if (r != 0) {
        return (r);
    }

    self_p->serial = data[0] << 24 | data[1] << 16 | data[3] << 8 | data[4];

    return (0);
}

int sht3xd_module_init()
{

    i2c_soft_module_init();
#ifdef DRIVER_DEBUG_LOG
    log_module_init();
#endif

    return (0);
}

int sht3xd_init(struct sht3xd_driver_t *self_p,
                struct i2c_soft_driver_t *i2c_p,
                int i2c_addr)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(i2c_p != NULL, EINVAL);

    self_p->i2c_p = i2c_p;
    self_p->i2c_addr = i2c_addr;

#ifdef DRIVER_DEBUG_LOG
    log_object_init(&self_p->log, "sht3xd", DRIVER_DEBUG_LOG);
#endif

    return (0);
}

int sht3xd_start(struct sht3xd_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int r;

#ifdef DRIVER_DEBUG_LOG
    log_object_print(&self_p->log, LOG_INFO,
                     OSTR("Starting SHT3x-D driver."));
#endif
    i2c_soft_start(self_p->i2c_p);
    if (i2c_soft_scan(self_p->i2c_p, self_p->i2c_addr) != 1) {
#ifdef DRIVER_DEBUG_LOG
        log_object_print(&self_p->log, LOG_WARNING,
                         OSTR("No device found at 0x%x."),
                         self_p->i2c_addr);
#endif
        return (-ENOENT);
    }

    r = sht3xd_read_serial(self_p);
    if (r != 0) {
#ifdef DRIVER_DEBUG_LOG
        log_object_print(&self_p->log, LOG_ERROR,
                         OSTR("Error reading serial for at 0x%x"),
                         self_p->i2c_addr);
#endif
    }
#ifdef DRIVER_DEBUG_LOG
    log_object_print(&self_p->log, LOG_INFO,
                     OSTR("Device found serial %8x at 0x%x"),
                     self_p->serial, self_p->i2c_addr);
#endif

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
                          float *temp,
                          float *humid)
{
    ASSERTN(self_p != NULL, EINVAL);

    int r;
    uint8_t data[6];

    /*
     * We use a non-clockstreching command followed by a sleep and
     * then issue read of the result.  This is so we can do a single
     * long sleep (probably a bit more power efficient), and to not
     * block the I2C bus while sensor is active - in case it's shared.
     */
    r = sht3xd_sendcmd(self_p, CMD_SINGLE_NOCLKSTRETCH_REPHIGH);
    if (r != 0) {
        return (r);
    }

    // We use max duration to avoid having to handle retry.
    thrd_sleep_ms(MEASUREMENT_DURATION_HIGH_MS);

    r = sht3xd_read2x16(self_p, data);
    if (r != 0) {
        return (r);
    }

    if (temp != NULL) {
        *temp = sht3xd_calculate_temp_c(data[0], data[1]);
    }
    if (humid != NULL) {
        *humid = sht3xd_calculate_humid(data[3], data[4]);
    }

    return (0);
}

int sht3xd_get_serial(struct sht3xd_driver_t *self_p,
                      uint32_t *serial)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(serial != NULL, EINVAL);

    *serial = self_p->serial;

    return (0);
}

#endif
