/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_WATCHDOG_H__
#define __DRIVERS_WATCHDOG_H__

#include "simba.h"

#include "watchdog_port.h"

/**
 * Watchdog interrupt function prototype.
 */
typedef void (*watchdog_isr_fn_t)(void);

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
 * @param[in] on_interrupt Function callback called when a watchdog
 *                         interrupt occurs. Not all MCU:s supports
 *                         this feature.
 *
 * @return zero(0) or negative error code.
 */
int watchdog_start_ms(int timeout,
                      watchdog_isr_fn_t on_interrupt);

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
