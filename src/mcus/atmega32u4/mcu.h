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

#ifndef __MCU_H__
#define __MCU_H__

#define PIN_DEVICE_MAX     25
#define EXTI_DEVICE_MAX     2
#define SPI_DEVICE_MAX      1
#define UART_DEVICE_MAX     1
/* Timer0 is used for system tick, so two of the PWM pins cannot be used.*/
#define PWM_DEVICE_MAX      4
#define ADC_DEVICE_MAX      1
#define I2C_DEVICE_MAX      1
#define USB_DEVICE_MAX      1

#define USART0_TX_vect USART1_TX_vect
#define USART0_RX_vect USART1_RX_vect
#define USART0_UDRE_vect USART1_UDRE_vect

#define UCSZ00 UCSZ10
#define UCSZ01 UCSZ11
#define UCSZ02 UCSZ12
#define UPM00 UPM10
#define UPM01 UPM11
#define USBS0 USBS1
#define U2X0 U2X1
#define UPE0 UPE1
#define DOR0 DOR1
#define FE0 FE1
#define TXC0 TXC1
#define RXCIE0 RXCIE1
#define RXEN0 RXEN1
#define TXEN0 TXEN1
#define UDRE0 UDRE1
#define UDRIE0 UDRIE1
#define TXCIE0 TXCIE1

#endif
