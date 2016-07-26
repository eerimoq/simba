/**
 * @file drivers/ports/avr/watchdog_port.i
 * @version 3.1.0
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

#include <avr/wdt.h>

int watchdog_port_start_ms(int timeout)
{
    wdt_enable(timeout);

    return (0);
}

int watchdog_port_stop(void)
{
    wdt_disable();

    return (0);
}

int watchdog_port_kick(void)
{
    wdt_reset();

    return (0);
}
