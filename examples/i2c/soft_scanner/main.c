/**
 * @file main.c
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#define SCL_PIN &pin_d21_dev
#define SDA_PIN &pin_d20_dev

struct i2c_soft_driver_t i2c;

int main()
{
    int address;
    int number_of_slaves;
    
    sys_start();
    i2c_soft_module_init();

    i2c_soft_init(&i2c, SCL_PIN, SDA_PIN, 50000, 1000000, 1000);
    i2c_soft_start(&i2c);
    
    std_printf(FSTR("Scanning the i2c bus for slaves...\r\n"
                    "\r\n"));

    number_of_slaves = 0;
    
    for (address = 0; address < 128; address++) {
        if (i2c_soft_scan(&i2c, address) == 1) {
            std_printf(FSTR("Found slave with address 0x%x.\r\n"), address);
            number_of_slaves++;
        }
    }

    std_printf(FSTR("\r\n"
                    "Scan complete. Found %d slaves.\r\n"), number_of_slaves);
    
    return (0);
}
