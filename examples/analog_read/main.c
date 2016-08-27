/**
 * @file main.c
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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
    int value;
    struct analog_input_pin_t pin;

    sys_start();
    analog_input_pin_module_init();

    /* Initialize the analog input pin. */
    analog_input_pin_init(&pin, &pin_a0_dev);

    while (1) {
        /* Wait one second. */
        thrd_sleep_ms(1000);

        /* Read the analog pin value and print it. */
        value = analog_input_pin_read(&pin);
        std_printf(FSTR("value = %d\r\n"), value);
    }

    return (0);
}
