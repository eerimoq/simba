/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

struct i2c_driver_t i2c;

int main()
{
    int address;
    int number_of_slaves;
    
    sys_start();
    i2c_module_init();

    i2c_init(&i2c, &i2c_0_dev, I2C_BAUDRATE_100KBPS, -1);
    i2c_start(&i2c);
    
    std_printf(FSTR("Scanning the i2c bus for slaves...\r\n"
                    "\r\n"));

    number_of_slaves = 0;
    
    for (address = 0; address < 128; address++) {
        if (i2c_scan(&i2c, address) == 1) {
            std_printf(FSTR("Found slave with address 0x%x.\r\n"), address);
            number_of_slaves++;
        }
    }

    std_printf(FSTR("\r\n"
                    "Scan complete. Found %d slaves.\r\n"), number_of_slaves);
    
    return (0);
}
