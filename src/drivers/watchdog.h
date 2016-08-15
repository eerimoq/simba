/**
 * @file drivers/watchdog.h
 * @version 6.0.0
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

#ifndef __DRIVERS_WATCHDOG_H__
#define __DRIVERS_WATCHDOG_H__

#include "simba.h"

#include "watchdog_port.h"

/**
 * Initialize the watchdog driver module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int watchdog_module_init(void);

/**
 * Start the watchdog with given timeout. Use `watchdog_kick()` to
 * periodically restart the timer.
 *
 * @param[in] timeout Watchdog timeout in milliseconds.
 *
 * @return zero(0) or negative error code.
 */
int watchdog_start_ms(int timeout);

/**
 * Stop the watchdog.
 *
 * @return zero(0) or negative error code.
 */
int watchdog_stop(void);

/**
 * Kick the watchdog. Restarts the watchdog timer with its original
 * timeout given to `watchdog_start_ms()`. The board will be reset if
 * this function is not called before the watchdog timer expires.
 *
 * @return zero(0) or negative error code.
 */
int watchdog_kick(void);

#endif
