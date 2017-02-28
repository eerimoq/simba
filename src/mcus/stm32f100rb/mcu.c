/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* GPIO A. */
    { .regs_p = STM32_GPIOA, .bit =  0 },
    { .regs_p = STM32_GPIOA, .bit =  1 },
    { .regs_p = STM32_GPIOA, .bit =  2 },
    { .regs_p = STM32_GPIOA, .bit =  3 },
    { .regs_p = STM32_GPIOA, .bit =  4 },
    { .regs_p = STM32_GPIOA, .bit =  5 },
    { .regs_p = STM32_GPIOA, .bit =  6 },
    { .regs_p = STM32_GPIOA, .bit =  7 },
    { .regs_p = STM32_GPIOA, .bit =  8 },
    { .regs_p = STM32_GPIOA, .bit =  9 },
    { .regs_p = STM32_GPIOA, .bit = 10 },
    { .regs_p = STM32_GPIOA, .bit = 11 },
    { .regs_p = STM32_GPIOA, .bit = 12 },
    { .regs_p = STM32_GPIOA, .bit = 13 },
    { .regs_p = STM32_GPIOA, .bit = 14 },
    { .regs_p = STM32_GPIOA, .bit = 15 },

    /* GPIO B. */
    { .regs_p = STM32_GPIOB, .bit =  0 },
    { .regs_p = STM32_GPIOB, .bit =  1 },
    { .regs_p = STM32_GPIOB, .bit =  2 },
    { .regs_p = STM32_GPIOB, .bit =  3 },
    { .regs_p = STM32_GPIOB, .bit =  4 },
    { .regs_p = STM32_GPIOB, .bit =  5 },
    { .regs_p = STM32_GPIOB, .bit =  6 },
    { .regs_p = STM32_GPIOB, .bit =  7 },
    { .regs_p = STM32_GPIOB, .bit =  8 },
    { .regs_p = STM32_GPIOB, .bit =  9 },
    { .regs_p = STM32_GPIOB, .bit = 10 },
    { .regs_p = STM32_GPIOB, .bit = 11 },
    { .regs_p = STM32_GPIOB, .bit = 12 },
    { .regs_p = STM32_GPIOB, .bit = 13 },
    { .regs_p = STM32_GPIOB, .bit = 14 },
    { .regs_p = STM32_GPIOB, .bit = 15 },

    /* GPIO C. */
    { .regs_p = STM32_GPIOC, .bit =  0 },
    { .regs_p = STM32_GPIOC, .bit =  1 },
    { .regs_p = STM32_GPIOC, .bit =  2 },
    { .regs_p = STM32_GPIOC, .bit =  3 },
    { .regs_p = STM32_GPIOC, .bit =  4 },
    { .regs_p = STM32_GPIOC, .bit =  5 },
    { .regs_p = STM32_GPIOC, .bit =  6 },
    { .regs_p = STM32_GPIOC, .bit =  7 },
    { .regs_p = STM32_GPIOC, .bit =  8 },
    { .regs_p = STM32_GPIOC, .bit =  9 },
    { .regs_p = STM32_GPIOC, .bit = 10 },
    { .regs_p = STM32_GPIOC, .bit = 11 },
    { .regs_p = STM32_GPIOC, .bit = 12 },
    { .regs_p = STM32_GPIOC, .bit = 13 },
    { .regs_p = STM32_GPIOC, .bit = 14 },
    { .regs_p = STM32_GPIOC, .bit = 15 },

    /* GPIO D. */
    { .regs_p = STM32_GPIOD, .bit =  0 },
    { .regs_p = STM32_GPIOD, .bit =  1 },
    { .regs_p = STM32_GPIOD, .bit =  2 }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = STM32_USART1
    }
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX];
