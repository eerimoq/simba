/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Google Inc.
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

int main()
{
    struct sht3xd_driver_t sht3xd;
    struct i2c_driver_t i2c;
    float temp, humid;
    uint32_t serial;
    int res;

    /* Initialization. */
    sys_start();
    sht3xd_module_init();

    /* Give serial console time to attach. */
    thrd_sleep(1);

    /* Driver initialization. */
    res = i2c_init(&i2c, &i2c_device[0], I2C_BAUDRATE_100KBPS, -1);

    if (res != 0) {
        std_printf(OSTR("Error initilizing i2c_soft driver\r\n"));
        return (-1);
    }

    res = sht3xd_init(&sht3xd, &i2c, SHT3X_DIS_I2C_ADDR_A);

    if (res != 0) {
        std_printf(OSTR("Error initilizing sht3xd driver\r\n"));
        return (-1);
    }

    /* Start driver - first actual sensor communication here. */
    res = sht3xd_start(&sht3xd);

    if (res != 0) {
        std_printf(OSTR("Error starting sht3xd driver\r\n"));
        return (-1);
    }

    /* Start getting data from sensor. */
    res = sht3xd_get_serial(&sht3xd, &serial);

    if (res == 0) {
        std_printf(OSTR("SHT3x-D serial: 0x%08x\r\n"), serial);
    }

    while (1) {
        res = sht3xd_get_temp_humid(&sht3xd, &temp, &humid);

        if (res == 0) {
            std_printf(OSTR("Temperature: %f C ; Humidity: %f RH%%\r\n"),
                       temp,
                       humid);
        } else {
            std_printf(OSTR("Could not get measurement.\r\n"));
        }

        /* Sleep 2 seconds until next measurement. */
        thrd_sleep(2);
    }
}
