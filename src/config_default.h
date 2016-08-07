/**
 * @file config_default.h
 * @version 4.1.0
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
 * The system configuration string contains a list of all configration
 * varialbes and their values.
 */
#ifndef CONFIG_SYS_CONFIG_STRING
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_SYS_CONFIG_STRING                    0
#    else
#        define CONFIG_SYS_CONFIG_STRING                    1
#    endif
#endif

/**
 * Assertions are used to check various conditions during the
 * application execution. A typical usage is to validate function
 * input arguments.
 */
#ifndef CONFIG_ASSERT
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_ASSERT                               0
#    else
#        define CONFIG_ASSERT                               1
#    endif
#endif

/**
 * Include more debug information.
 */
#ifndef CONFIG_DEBUG
#    define CONFIG_DEBUG                                    1
#endif

/**
 * Profile the stack usage in runtime. It's a cheap operation and is
 * recommended to have enabled.
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
 * Start the monitor thread to gather statistics of the scheulder.
 */
#ifndef CONFIG_MONITOR_THREAD
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(BOARD_ESP12E) || defined(BOARD_ESP01)
#        define CONFIG_MONITOR_THREAD                       0
#    else
#        define CONFIG_MONITOR_THREAD                       1
#    endif
#endif

/**
 * System tick frequency in Hertz.
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
 * Debug file system command to list all USB devices.
 */
#ifndef CONFIG_FS_CMD_USB_DEVICE_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_USB_DEVICE_LIST               0
#    else
#        define CONFIG_FS_CMD_USB_DEVICE_LIST               1
#    endif
#endif

/**
 * Debug file system command to list all USB devices connected to the
 * USB host.
 */
#ifndef CONFIG_FS_CMD_USB_HOST_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_USB_HOST_LIST                 0
#    else
#        define CONFIG_FS_CMD_USB_HOST_LIST                 1
#    endif
#endif

/**
 * Debug file system command to set the mode of a pin.
 */
#ifndef CONFIG_FS_CMD_PIN_SET_MODE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_PIN_SET_MODE                  0
#    else
#        define CONFIG_FS_CMD_PIN_SET_MODE                  1
#    endif
#endif

/**
 * Debug file system command to read the current value of a pin.
 */
#ifndef CONFIG_FS_CMD_PIN_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_PIN_READ                      0
#    else
#        define CONFIG_FS_CMD_PIN_READ                      1
#    endif
#endif

/**
 * Debug file system command to write a value to a pin.
 */
#ifndef CONFIG_FS_CMD_PIN_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_PIN_WRITE                     0
#    else
#        define CONFIG_FS_CMD_PIN_WRITE                     1
#    endif
#endif

/**
 * Debug file system command to list all DS18B20 sensors on the bus.
 */
#ifndef CONFIG_FS_CMD_DS18B20_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_DS18B20_LIST                  0
#    else
#        define CONFIG_FS_CMD_DS18B20_LIST                  1
#    endif
#endif

/**
 * Debug file system command to read from a i2c bus.
 */
#ifndef CONFIG_FS_CMD_I2C_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_I2C_READ                      0
#    else
#        define CONFIG_FS_CMD_I2C_READ                      1
#    endif
#endif

/**
 * Debug file system command to write to a i2c bus.
 */
#ifndef CONFIG_FS_CMD_I2C_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_I2C_WRITE                     0
#    else
#        define CONFIG_FS_CMD_I2C_WRITE                     1
#    endif
#endif

/**
 * Debug file system command to list threads' information.
 */
#ifndef CONFIG_FS_CMD_THRD_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_THRD_LIST                     0
#    else
#        define CONFIG_FS_CMD_THRD_LIST                     1
#    endif
#endif

/**
 * Debug file system command to set the log mask of a thread.
 */
#ifndef CONFIG_FS_CMD_THRD_SET_LOG_MASK
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_THRD_SET_LOG_MASK             0
#    else
#        define CONFIG_FS_CMD_THRD_SET_LOG_MASK             1
#    endif
#endif

/**
 * Debug file system command to print the system information.
 */
#ifndef CONFIG_FS_CMD_SYS_INFO
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SYS_INFO                      0
#    else
#        define CONFIG_FS_CMD_SYS_INFO                      1
#    endif
#endif

/**
 * Debug file system command to print the system uptime.
 */
#ifndef CONFIG_FS_CMD_SYS_UPTIME
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SYS_UPTIME                    0
#    else
#        define CONFIG_FS_CMD_SYS_UPTIME                    1
#    endif
#endif

/**
 * Debug file system command to list all registered file systems.
 */
#ifndef CONFIG_FS_CMD_FS_FILESYSTEMS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_LIST           0
#    else
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_LIST           1
#    endif
#endif

/**
 * Debug file system command to read from a file.
 */
#ifndef CONFIG_FS_CMD_FS_FILESYSTEMS_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_READ           0
#    else
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_READ           1
#    endif
#endif

/**
 * Debug file system command to write to a file.
 */
#ifndef CONFIG_FS_CMD_FS_FILESYSTEMS_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_WRITE          0
#    else
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_WRITE          1
#    endif
#endif

/**
 * Debug file system command to append to a file.
 */
#ifndef CONFIG_FS_CMD_FS_FILESYSTEMS_APPEND
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_APPEND         0
#    else
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_APPEND         1
#    endif
#endif

/**
 * Debug file system command to list all counters.
 */
#ifndef CONFIG_FS_CMD_FS_COUNTERS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_COUNTERS_LIST              0
#    else
#        define CONFIG_FS_CMD_FS_COUNTERS_LIST              1
#    endif
#endif

/**
 * Debug file system command to set all counters to zero.
 */
#ifndef CONFIG_FS_CMD_FS_COUNTERS_RESET
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_COUNTERS_RESET             0
#    else
#        define CONFIG_FS_CMD_FS_COUNTERS_RESET             1
#    endif
#endif

/**
 * Debug file system command to list all parameters.
 */
#ifndef CONFIG_FS_CMD_FS_PARAMETERS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_FS_PARAMETERS_LIST            0
#    else
#        define CONFIG_FS_CMD_FS_PARAMETERS_LIST            1
#    endif
#endif

/**
 * Debug file system command to create a log entry and print
 * it. Mainly used for debugging.
 */
#ifndef CONFIG_FS_CMD_LOG_PRINT
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_LOG_PRINT                     0
#    else
#        define CONFIG_FS_CMD_LOG_PRINT                     1
#    endif
#endif

/**
 * Debug file system command to list all log objects.
 */
#ifndef CONFIG_FS_CMD_LOG_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_LOG_LIST                      0
#    else
#        define CONFIG_FS_CMD_LOG_LIST                      1
#    endif
#endif

/**
 * Debug file system command to set the log mask of a log object.
 */
#ifndef CONFIG_FS_CMD_LOG_SET_LOG_MASK
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_LOG_SET_LOG_MASK              0
#    else
#        define CONFIG_FS_CMD_LOG_SET_LOG_MASK              1
#    endif
#endif

/**
 * Debug file system command to list all network interfaces.
 */
#ifndef CONFIG_FS_CMD_NETWORK_INTERFACE_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_NETWORK_INTERFACE_LIST        0
#    else
#        define CONFIG_FS_CMD_NETWORK_INTERFACE_LIST        1
#    endif
#endif

/**
 * Debug file system command to list all settings.
 */
#ifndef CONFIG_FS_CMD_SETTING_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SETTING_LIST                  0
#    else
#        define CONFIG_FS_CMD_SETTING_LIST                  1
#    endif
#endif

/**
 * Debug file system command to reset the settings to their original
 * values.
 */
#ifndef CONFIG_FS_CMD_SETTING_RESET
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SETTING_RESET                 0
#    else
#        define CONFIG_FS_CMD_SETTING_RESET                 1
#    endif
#endif

/**
 * Debug file system command to read the value of a setting.
 */
#ifndef CONFIG_FS_CMD_SETTING_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SETTING_READ                  0
#    else
#        define CONFIG_FS_CMD_SETTING_READ                  1
#    endif
#endif

/**
 * Debug file system command to write a value to a setting.
 */
#ifndef CONFIG_FS_CMD_SETTING_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_FS_CMD_SETTING_WRITE                 0
#    else
#        define CONFIG_FS_CMD_SETTING_WRITE                 1
#    endif
#endif

#define CONFIG_CONSOLE_NONE                                 0
#define CONFIG_CONSOLE_UART                                 1
#define CONFIG_CONSOLE_USB_CDC                              2

/**
 * Console device (UART/USB CDC).
 */
#ifndef CONFIG_CONSOLE
#    if defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_CONSOLE         CONFIG_CONSOLE_USB_CDC
#    else
#        define CONFIG_CONSOLE            CONFIG_CONSOLE_UART
#    endif
#endif

/**
 * Console device index.
 */
#ifndef CONFIG_CONSOLE_DEVICE_INDEX
#    define CONFIG_CONSOLE_DEVICE_INDEX                     0
#endif

/**
 * Console UART baudrate.
 */
#ifndef CONFIG_CONSOLE_UART_BAUDRATE
#    define CONFIG_CONSOLE_UART_BAUDRATE                38400
#endif

/**
 * Console USB CDC control interface number.
 */
#ifndef CONFIG_CONSOLE_USB_CDC_CONTROL_INTERFACE
#    define CONFIG_CONSOLE_USB_CDC_CONTROL_INTERFACE        0
#endif

/**
 * Console USB CDC input endpoint.
 */
#ifndef CONFIG_CONSOLE_USB_CDC_ENDPOINT_IN
#    define CONFIG_CONSOLE_USB_CDC_ENDPOINT_IN              2
#endif

/**
 * Console USB CDC output endpoint.
 */
#ifndef CONFIG_CONSOLE_USB_CDC_ENDPOINT_OUT
#    define CONFIG_CONSOLE_USB_CDC_ENDPOINT_OUT             3
#endif

/**
 * Wait for the host to connect after starting the console.
 */
#ifndef CONFIG_CONSOLE_USB_CDC_WAIT_FOR_CONNETION
#    define CONFIG_CONSOLE_USB_CDC_WAIT_FOR_CONNETION       1
#endif

/**
 * Minimal shell functionality to minimize the code size of the shell
 * module.
 */
#ifndef CONFIG_SHELL_MINIMAL
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_SHELL_MINIMAL                        1
#    else
#        define CONFIG_SHELL_MINIMAL                        0
#    endif
#endif

/**
 * The shell prompt string.
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
#    define CONFIG_SHELL_HISTORY_SIZE                     768
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

/**
 * SPIFFS is a flash file system applicable for boards that has a
 * reasonably big modifiable flash.
 */
#ifndef CONFIG_SPIFFS
#    if defined(BOARD_ARDUINO_DUE) || defined(ARCH_LINUX)
#        define CONFIG_SPIFFS                               1
#    else
#        define CONFIG_SPIFFS                               0
#    endif
#endif

#endif
