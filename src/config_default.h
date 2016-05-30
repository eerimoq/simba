/**
 * @file config.h
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

#ifndef __CONFIG_DEFAULT_H__
#define __CONFIG_DEFAULT_H__

/**
 * Build with assert.
 */
#ifndef CONFIG_ASSERT
#    define CONFIG_ASSERT               1
#endif

/**
 * Build with debug.
 */
#ifndef CONFIG_DEBUG
#    define CONFIG_DEBUG                1
#endif

/**
 * Stack profiling.
 */
#ifndef CONFIG_PROFILE_STACK
#    define CONFIG_PROFILE_STACK        1
#endif

/**
 * Use a preemptive scheduler.
 */
#ifndef CONFIG_PREEMPTIVE_SCHEDULER
#    define CONFIG_PREEMPTIVE_SCHEDULER 0
#endif

/**
 * Start the monitor thread.
 */
#ifndef CONFIG_MONITOR_THREAD
#    define CONFIG_MONITOR_THREAD       1
#endif

/**
 * Backwards compatibility. Should be removed.
 */
#if defined(SYS_TICK_FREQUENCY)
#    define CONFIG_SYSTEM_TICK_FREQUENCY SYS_TICK_FREQUENCY
#endif

/**
 * Default system tick frequency.
 */
#ifndef CONFIG_SYSTEM_TICK_FREQUENCY
#    define CONFIG_SYSTEM_TICK_FREQUENCY 100
#endif

#endif
