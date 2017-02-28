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

#ifndef __MCU_H__
#define __MCU_H__

#include "sam3.h"

/* Pin controller start indexes in devices array. */
#define SAM_PA 0
#define SAM_PB 30
#define SAM_PC 62
#define SAM_PD 93

#if defined(MCU_SAM3X8E)
#    define PIN_DEVICE_MAX             103
#    define EXTI_DEVICE_MAX PIN_DEVICE_MAX
#    define SPI_DEVICE_MAX               1
#    define UART_DEVICE_MAX              4
#    define PWM_DEVICE_MAX              12
#    define ADC_DEVICE_MAX               1
#    define DAC_DEVICE_MAX               1
#    define FLASH_DEVICE_MAX             1
#    define CAN_DEVICE_MAX               2
#    define USB_DEVICE_MAX               1
#else
#     error "Unsupported MCU."
#endif

#endif
