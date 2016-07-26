/**
 * @file main.c
 * @version 3.0.0
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

int main()
{
    struct spi_driver_t spi;
    struct pin_driver_t pin[3];
    uint8_t state;

    sys_start();

    spi_init(&spi,
             &spi_device[0],
             &pin_d10_dev,
             SPI_MODE_SLAVE,
             SPI_SPEED_250KBPS,
             1,
             1);

    /* Initialize led pins. */
    pin_init(&pin[0], &pin_d7_dev, PIN_OUTPUT);
    pin_init(&pin[1], &pin_d8_dev, PIN_OUTPUT);
    pin_init(&pin[2], &pin_d9_dev, PIN_OUTPUT);

    while (1) {
        spi_read(&spi, &state, sizeof(state));
        std_printf(FSTR("state = 0x%x\r\n"), (int)state);

        /* Upadte LED. */
        pin_write(&pin[0], (state >> 0) & 0x1);
        pin_write(&pin[1], (state >> 1) & 0x1);
        pin_write(&pin[2], (state >> 2) & 0x1);
    }

    return (0);
}
