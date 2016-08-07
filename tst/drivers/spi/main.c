/**
 * @file main.c
 * @version 5.0.0
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

static struct spi_driver_t spi;
static uint8_t txbuf[128];
static uint8_t rxbuf[128];

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(spi_module_init() == 0);

    BTASSERT(spi_init(&spi,
                      &spi_device[0],
                      &pin_d5_dev,
                      SPI_MODE_MASTER,
                      SPI_SPEED_125KBPS,
                      0,
                      0) == 0);

    BTASSERT(spi_start(&spi) == 0);

    return (0);
}

static int test_transfer(struct harness_t *harness_p)
{
    int i;

    for (i = 0; i < membersof(txbuf); i++) {
        txbuf[i] = i;
        rxbuf[i] = 1;
    }

    BTASSERT(spi_take_bus(&spi) == 0);
    BTASSERT(spi_select(&spi) == 0);

    BTASSERT(spi_transfer(&spi,
                          &rxbuf[4],
                          &txbuf[4],
                          sizeof(rxbuf) - 8) == sizeof(rxbuf) - 8);

    BTASSERT(spi_deselect(&spi) == 0);
    BTASSERT(spi_give_bus(&spi) == 0);

    for (i = 0; i < membersof(txbuf); i++) {
        std_printf(FSTR("[%d]: tx = 0x%x, rx = 0x%x\r\n"),
                   i,
                   txbuf[i],
                   rxbuf[i]);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_transfer, "test_transfer" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
