/**
 * @file main.c
 * @version 0.4.0
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

    sys_start();
    uart_module_init();
    spi_module_init();

    spi_init(&spi,
             NULL,
             NULL,
             SPI_MODE_MASTER,
             SPI_SPEED_1MBPS,
             0,
             0);

    spi_transfer(&spi, NULL, "hej", 3);

    return (0);
}
