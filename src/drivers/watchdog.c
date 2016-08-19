/**
 * @file watchdog.c
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

#include "watchdog_port.i"

int watchdog_module_init()
{
    return (0);
}

int watchdog_start_ms(int timeout)
{
    return (watchdog_port_start_ms(timeout));
}

int watchdog_stop(void)
{
    return (watchdog_port_stop());
}

int watchdog_kick(void)
{
    return (watchdog_port_kick());
}
