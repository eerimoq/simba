/**
 * @file config_default.h
 * @version 2.0.0
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
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_ASSERT               0
#    else
#        define CONFIG_ASSERT               1
#    endif
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
 * Default system tick frequency.
 */
#ifndef CONFIG_SYSTEM_TICK_FREQUENCY
#    define CONFIG_SYSTEM_TICK_FREQUENCY 100
#endif

/**
 * Maximum number of bytes in the print output buffer.
 */
#ifndef CONFIG_STD_OUTPUT_BUFFER_MAX
#    define CONFIG_STD_OUTPUT_BUFFER_MAX 16
#endif

/**
 * Default console device (UART/USB CDC).
 */
#define CONFIG_CONSOLE_NONE         0
#define CONFIG_CONSOLE_UART         1
#define CONFIG_CONSOLE_USB_CDC      2

#ifndef CONFIG_CONSOLE
#    if defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_CONSOLE CONFIG_CONSOLE_USB_CDC
#    else
#        define CONFIG_CONSOLE CONFIG_CONSOLE_UART
#    endif
#endif

/**
 * Default console device index.
 */
#ifndef CONFIG_CONSOLE_DEVICE
#    define CONFIG_CONSOLE_DEVICE        0
#endif

/**
 * Default console baudrate.
 */
#ifndef CONFIG_CONSOLE_BAUDRATE
#    define CONFIG_CONSOLE_BAUDRATE  38400
#endif

/**
 * Minimal shell functionality.
 */
#ifndef CONFIG_SHELL_MINIMAL
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_SHELL_MINIMAL    1
#    else
#        define CONFIG_SHELL_MINIMAL    0
#    endif
#endif

/**
 * The shell prompt.
 */
#ifndef CONFIG_SHELL_PROMPT
#    define CONFIG_SHELL_PROMPT "$ "
#endif

/**
 * Maximum number of characters in a shell command.
 */
#ifndef CONFIG_SHELL_COMMAND_MAX
#    define CONFIG_SHELL_COMMAND_MAX     64
#endif

/**
 * Size of the shell history buffer.
 */
#ifndef CONFIG_SHELL_HISTORY_SIZE
#    define CONFIG_SHELL_HISTORY_SIZE   (10 * CONFIG_SHELL_COMMAND_MAX)
#endif

#endif
