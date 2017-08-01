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

int main()
{
    int res;
    struct bmp280_driver_t bmp280;
    struct i2c_driver_t i2c;
    struct bmp280_transport_i2c_t transport;
    float temperature;
    float pressure;
    float altitude;

    sys_start();

    bmp280_module_init();

    /* Initialize and start a I2C driver. */
    i2c_init(&i2c, &i2c_device[0], I2C_BAUDRATE_100KBPS, -1);
    i2c_start(&i2c);

    /* Initialize the BMP280 I2C transport layer. */
    bmp280_transport_i2c_init(&transport,
                              &i2c,
                              BMP280_I2C_ADDRESS_0);

    /* Initialize and start the BMP280 driver with the I2C
       transport layer. */
    bmp280_init(&bmp280,
                &transport.base,
                bmp280_mode_normal_t,
                bmp280_standby_time_500_us_t,
                bmp280_filter_off_t,
                bmp280_temperature_oversampling_1_t,
                bmp280_pressure_oversampling_1_t);

    res = bmp280_start(&bmp280);

    if (res != 0) {
        std_printf(OSTR("Failed to start the device.\r\n"));
        return (res);
    }

    while (1) {
        thrd_sleep(1);

        /* Read temperature and pressure from the BMP280. */
        res = bmp280_read(&bmp280, &temperature, &pressure);

        if (res != 0) {
            std_printf(OSTR("Read failed with %d: %S.\r\n"),
                       res,
                       errno_as_string(res));
            continue;
        }

        /* Calculate the altitude from read pressure. */
        altitude = science_pressure_to_altitude(
            pressure,
            SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);

        std_printf(OSTR("Temperature: %13f C\r\n"
                        "Pressure:    %13f Pa\r\n"
                        "Altitude:    %13f m\r\n"
                        "\r\n"),
                   temperature,
                   pressure,
                   altitude);
    }

    return (0);
}
