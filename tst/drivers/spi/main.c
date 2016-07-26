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

static char qinbuf[32];
static struct uart_driver_t uart;
static struct spi_driver_t spi;
static uint8_t txbuf[128];
static uint8_t rxbuf[128];

int main()
{
    int i;

    sys_start();
    spi_module_init();

    spi_init(&spi,
             &spi_device[0],
             &pin_d5_dev,
             SPI_MODE_MASTER,
             SPI_SPEED_1MBPS,
             0,
             0);

    std_printf(sys_get_info());

    for (i = 0; i < membersof(txbuf); i++) {
        txbuf[i] = i;
        rxbuf[i] = 1;
    }

    spi_transfer(&spi, &rxbuf[4], &txbuf[4], sizeof(rxbuf) - 8);

    for (i = 0; i < membersof(txbuf); i++) {
        std_printf(FSTR("[%d]: tx = 0x%x, rx = 0x%x\r\n"),
                   i,
                   txbuf[i],
                   rxbuf[i]);
    }

    return (0);
}
