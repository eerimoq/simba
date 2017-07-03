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

#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Use a preemptive scheduler.
 */
#ifndef CONFIG_PREEMPTIVE_SCHEDULER
#    if defined(ARCH_AVR) && !defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_PREEMPTIVE_SCHEDULER 1
#    else
#        define CONFIG_PREEMPTIVE_SCHEDULER 0
#    endif
#endif

#ifndef CONFIG_THRD_ENV
#    if defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_THRD_ENV                     0
#    else
#        define CONFIG_THRD_ENV                     1
#    endif
#endif

#ifndef CONFIG_MONITOR_THREAD
#    if defined(ARCH_LINUX) || defined(ARCH_PPC)
#        define CONFIG_MONITOR_THREAD                       1
#    else
#        define CONFIG_MONITOR_THREAD                       0
#    endif
#endif

#ifndef CONFIG_FLOAT
#    if defined(ARCH_LINUX)
#        define CONFIG_FLOAT                                1
#    else
#        define CONFIG_FLOAT                                0
#    endif
#endif

#ifndef CONFIG_MONITOR_THREAD_PERIOD_US
#    define CONFIG_MONITOR_THREAD_PERIOD_US                25
#endif

#if defined(BOARD_ARDUINO_MEGA)
#    define CONFIG_START_CONSOLE_UART_BAUDRATE             115200
#endif

#endif
