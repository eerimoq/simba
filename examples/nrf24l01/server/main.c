/**
 * @file main.c
 * @version 0.5.0
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

#define SERVER_ADDRESS 0x12345678
#define CLIENT_ADDRESS 0x87654321

static char qinbuf[32];
static struct uart_driver_t uart;

int main()
{
    struct nrf24l01_driver_t nrf24l01;
    struct pin_driver_t pin[3];
    uint8_t state[32];

    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    sys_set_stdout(&uart.chout);

    nrf24l01_init(&nrf24l01,
                  &spi_device[0],
                  &pin_d10_dev,
                  &pin_d6_dev,
                  &exti_device[1],
                  SERVER_ADDRESS);
    nrf24l01_start(&nrf24l01);

    /* Initialize led pins. */
    pin_init(&pin[0], &pin_d7_dev, PIN_OUTPUT);
    pin_init(&pin[1], &pin_d8_dev, PIN_OUTPUT);
    pin_init(&pin[2], &pin_d9_dev, PIN_OUTPUT);

    pin_write(&pin[0], 0);
    pin_write(&pin[1], 0);
    pin_write(&pin[2], 0);

    while (1) {
        /* Read state from client. */
        nrf24l01_read(&nrf24l01, state, sizeof(state));
        std_fprintf(&uart.chout, FSTR("state = 0x%x\r\n"), (int)state[0]);

        /* Upadte LED. */
        pin_write(&pin[0], (state[0] >> 0) & 0x1);
        pin_write(&pin[1], (state[0] >> 1) & 0x1);
        pin_write(&pin[2], (state[0] >> 2) & 0x1);
    }

    return (0);
}
