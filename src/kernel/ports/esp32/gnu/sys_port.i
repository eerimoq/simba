/**
 * @file avr/gnu/sys_port.i
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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

/* The main function is defined by the user in main.c. */
extern int main();

static int sys_port_module_init(void)
{
    (void)sys_tick;

    return (0);
}

static void sys_port_lock(void)
{
    portDISABLE_INTERRUPTS();
}

static void sys_port_unlock(void)
{
    portENABLE_INTERRUPTS();
}

static void sys_port_lock_isr(void)
{
}

static void sys_port_unlock_isr(void)
{
}

void sys_stop(int error)
{
    while (1);
}

static float sys_port_interrupt_cpu_usage_get(void)
{
    return (0.0f);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}

int app_main()
{
    system_init();

    std_printf(FSTR("Hello!\r\n"));

    main();

    return (0);
}
