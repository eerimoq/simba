/**
 * @file uart_port.i
 * @version 0.6.0
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

static void isr(int index)
{
    struct uart_device_t *dev_p = &uart_device[index];
    struct uart_driver_t *drv_p = dev_p->drv_p;

    if (drv_p == NULL) {
        return;
    }

    /* TX and/or RX complete. */
}

ISR(usart1)
{
    isr(0);
}

ISR(usart2)
{
    isr(1);
}

ISR(usart3)
{
    isr(2);
}

static int uart_port_module_init()
{
    return (0);
}

static int uart_port_start(struct uart_driver_t *self_p)
{
    volatile struct stm32_usart_t *regs_p;

    regs_p = self_p->dev_p->regs_p;

    /* Configure pin functions. */

    /* /\* Enable the UART. *\/ */
    //STM32_RCC->APB2ENR = 0x0004;
    //time_sleep(1);
    //STM32_RCC->APB2ENR = 0x0001;
    //time_sleep(1);
    //STM32_RCC->APB2ENR = 0x4000;
    //time_sleep(1);
    
    //STM32_GPIOA->CRH = 0x00000990;

    regs_p->CR2 = 0;
    regs_p->CR3 = 0;
    regs_p->BRR = 0x330;
    regs_p->SR = 0;
    regs_p->CR1 = (STM32_USART_CR1_UE
                   | STM32_USART_CR1_TE
                   | STM32_USART_CR1_RE);
    
    regs_p->DR = 0x1a;

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    self_p->dev_p->regs_p->CR1 = 0;

    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    return (0);
}
