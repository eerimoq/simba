/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

static char hard_qinbuf[32];
static char soft_qinbuf[32];

static struct uart_driver_t hard;
static struct uart_soft_driver_t soft;

int main()
{
    char c;

    sys_start();
    uart_module_init();

    uart_init(&hard,
              &uart_device[0],
              38400,
              hard_qinbuf,
              sizeof(hard_qinbuf));
    uart_start(&hard);

    sys_set_stdout(&hard.chout);

    uart_soft_init(&soft,
                   &pin_d4_dev,
                   &pin_d3_dev,
                   &exti_d3_dev,
                   9600,
                   soft_qinbuf,
                   sizeof(soft_qinbuf));

    while (1) {
        /* Read from hard UART and write to soft UART. */
        std_fprintf(&hard.chout,
                    FSTR("Write a character in this (hard) shell.\r\n"
                         "It will be sent to the other shell.\r\n"));
        uart_read(&hard, &c, sizeof(c));
        uart_soft_write(&soft, &c, sizeof(c));
        std_fprintf(&soft.chout, FSTR("\r\n"));
        std_fprintf(&hard.chout, FSTR("Wrote to soft shell.\r\n"));

        /* Read from soft UART and write to hard UART. */
        std_fprintf(&soft.chout,
                    FSTR("Write a character in this (soft) shell.\r\n"
                         "It will be sent to the other shell.\r\n"));
        uart_soft_read(&soft, &c, sizeof(c));
        uart_write(&hard, &c, sizeof(c));
        std_fprintf(&hard.chout, FSTR("\r\n"));
        std_fprintf(&soft.chout, FSTR("Wrote to hard shell\r\n"));
    }

    return (0);
}
