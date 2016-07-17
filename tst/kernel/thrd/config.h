/**
 * @file config.h
 * @version 1.1.0
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Use a preemptive scheduler.
 */
#ifndef CONFIG_PREEMPTIVE_SCHEDULER
#    if defined(ARCH_AVR)
#        define CONFIG_PREEMPTIVE_SCHEDULER 1
#    else
#        define CONFIG_PREEMPTIVE_SCHEDULER 0
#    endif
#endif

/**
 * Start the monitor thread.
 */
#ifndef CONFIG_MONITOR_THREAD
#    define CONFIG_MONITOR_THREAD       0
#endif

#endif
