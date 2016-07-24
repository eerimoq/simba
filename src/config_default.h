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
#        define CONFIG_ASSERT                               0
#    else
#        define CONFIG_ASSERT                               1
#    endif
#endif

/**
 * Build with debug.
 */
#ifndef CONFIG_DEBUG
#    define CONFIG_DEBUG                                    1
#endif

/**
 * Stack profiling.
 */
#ifndef CONFIG_PROFILE_STACK
#    define CONFIG_PROFILE_STACK                            1
#endif

/**
 * Use a preemptive scheduler.
 */
#ifndef CONFIG_PREEMPTIVE_SCHEDULER
#    define CONFIG_PREEMPTIVE_SCHEDULER                     0
#endif

/**
 * Start the monitor thread.
 */
#ifndef CONFIG_MONITOR_THREAD
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(BOARD_ESP12E) || defined(BOARD_ESP01)
#        define CONFIG_MONITOR_THREAD                       0
#    else
#        define CONFIG_MONITOR_THREAD                       1
#    endif
#endif

/**
 * Default system tick frequency.
 */
#ifndef CONFIG_SYSTEM_TICK_FREQUENCY
#    define CONFIG_SYSTEM_TICK_FREQUENCY                  100
#endif

/**
 * Maximum number of bytes in the print output buffer.
 */
#ifndef CONFIG_STD_OUTPUT_BUFFER_MAX
#    define CONFIG_STD_OUTPUT_BUFFER_MAX                   16
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_USB_DEVICE_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_USB_DEVICE_LIST               0
#    else
#        define CONFIG_FS_CMD_USB_DEVICE_LIST               1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_USB_HOST_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_USB_HOST_LIST                 0
#    else
#        define CONFIG_FS_CMD_USB_HOST_LIST                 1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_PIN_INIT
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_PIN_INIT                      0
#    else
#        define CONFIG_FS_CMD_PIN_INIT                      1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_PIN_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_PIN_WRITE                     0
#    else
#        define CONFIG_FS_CMD_PIN_WRITE                     1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_DS18B20_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_DS18B20_LIST                  0
#    else
#        define CONFIG_FS_CMD_DS18B20_LIST                  1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_I2C_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_I2C_READ                      0
#    else
#        define CONFIG_FS_CMD_I2C_READ                      1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_I2C_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_I2C_WRITE                     0
#    else
#        define CONFIG_FS_CMD_I2C_WRITE                     1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_THRD_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_THRD_LIST                     0
#    else
#        define CONFIG_FS_CMD_THRD_LIST                     1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_THRD_SET_LOG_MASK
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_THRD_SET_LOG_MASK             0
#    else
#        define CONFIG_FS_CMD_THRD_SET_LOG_MASK             1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_SYS_INFO
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SYS_INFO                      0
#    else
#        define CONFIG_FS_CMD_SYS_INFO                      1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_SYS_UPTIME
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SYS_UPTIME                    0
#    else
#        define CONFIG_FS_CMD_SYS_UPTIME                    1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_FS_COUNTERS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_COUNTERS_LIST              0
#    else
#        define CONFIG_FS_CMD_FS_COUNTERS_LIST              1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_FS_COUNTERS_RESET
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_COUNTERS_RESET             0
#    else
#        define CONFIG_FS_CMD_FS_COUNTERS_RESET             1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_FS_PARAMETERS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_PARAMETERS_LIST            0
#    else
#        define CONFIG_FS_CMD_FS_PARAMETERS_LIST            1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_LOG_PRINT
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_LOG_PRINT                     0
#    else
#        define CONFIG_FS_CMD_LOG_PRINT                     1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_LOG_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_LOG_LIST                      0
#    else
#        define CONFIG_FS_CMD_LOG_LIST                      1
#    endif
#endif

/**
 * File system command.
 */
#ifndef CONFIG_FS_CMD_LOG_SET_LOG_MASK
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_LOG_SET_LOG_MASK              0
#    else
#        define CONFIG_FS_CMD_LOG_SET_LOG_MASK              1
#    endif
#endif

/**
 * Default console device (UART/USB CDC).
 */
#define CONFIG_CONSOLE_NONE                                 0
#define CONFIG_CONSOLE_UART                                 1
#define CONFIG_CONSOLE_USB_CDC                              2

#ifndef CONFIG_CONSOLE
#    if defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_CONSOLE         CONFIG_CONSOLE_USB_CDC
#    else
#        define CONFIG_CONSOLE            CONFIG_CONSOLE_UART
#    endif
#endif

/**
 * Default console device index.
 */
#ifndef CONFIG_CONSOLE_DEVICE
#    define CONFIG_CONSOLE_DEVICE                           0
#endif

/**
 * Default console baudrate.
 */
#ifndef CONFIG_CONSOLE_UART_BAUDRATE
#    define CONFIG_CONSOLE_UART_BAUDRATE                38400
#endif

/**
 * Default console USB CDC control interface number.
 */
#ifndef CONFIG_CONSOLE_USB_CDC_CONTROL_INTERFACE
#    define CONFIG_CONSOLE_USB_CDC_CONTROL_INTERFACE        0
#endif

/**
 * Default console USB CDC input endpoint.
 */
#ifndef CONFIG_CONSOLE_USB_CDC_ENDPOINT_IN
#    define CONFIG_CONSOLE_USB_CDC_ENDPOINT_IN              2
#endif

/**
 * Default console USB CDC output endpoint.
 */
#ifndef CONFIG_CONSOLE_USB_CDC_ENDPOINT_OUT
#    define CONFIG_CONSOLE_USB_CDC_ENDPOINT_OUT             3
#endif

/**
 * Minimal shell functionality.
 */
#ifndef CONFIG_SHELL_MINIMAL
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_SHELL_MINIMAL                        1
#    else
#        define CONFIG_SHELL_MINIMAL                        0
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
#    define CONFIG_SHELL_COMMAND_MAX                       64
#endif

/**
 * Size of the shell history buffer.
 */
#ifndef CONFIG_SHELL_HISTORY_SIZE
#    define CONFIG_SHELL_HISTORY_SIZE   (10 * CONFIG_SHELL_COMMAND_MAX)
#endif

/**
 * USB device CDC.
 */
#ifndef CONFIG_USB_DEVICE_CDC
#    if defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_USB_DEVICE_CDC                       1
#    else
#        define CONFIG_USB_DEVICE_CDC                       0
#    endif
#endif

/**
 * USB device vendor id.
 */
#ifndef CONFIG_USB_DEVICE_VID
#    define CONFIG_USB_DEVICE_VID                      0x2341
#endif

/**
 * USB device product id.
 */
#ifndef CONFIG_USB_DEVICE_PID
#    define CONFIG_USB_DEVICE_PID                      0x8037
#endif

#endif
