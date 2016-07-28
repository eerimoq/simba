/**
 * @file mcus/atmega32u4/mcu.h
 * @version 3.1.1
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
