/**
 * @file stm32.h
 * @version 1.0.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

#ifndef __STM32_H__
#define __STM32_H__

#define BIT(n) (0x1 << (n))

#define BITFIELD_SET(name, value) \
    (((value) << name ## _POS) & name ## _MASK)
#define BITFIELD_GET(name, value) \
    (((value) & name ## _MASK) >> name ## _POS)

/* System control block. */
struct stm32_usart_t {
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t BRR;
    uint32_t GTPR;
    uint32_t RTOR;
    uint32_t RQR;
    uint32_t ISR;
    uint32_t ICR;
    uint32_t RDR;
    uint32_t TDR;
};

/* Auxiliary Control Register */
#define STM32_USART_CR1_UE            BIT(0)
#define STM32_USART_CR1_UESM          BIT(1)
#define STM32_USART_CR1_RE            BIT(2)
#define STM32_USART_CR1_TE            BIT(3)
#define STM32_USART_CR1_IDLEIE        BIT(4)

/* Base addresses of peripherals. */
#define STM32_TIM2              ((volatile struct stm32_tim_t *)0x40000000u)
#define STM32_TIM3              ((volatile struct stm32_tim_t *)0x40000400u)
#define STM32_TIM4              ((volatile struct stm32_tim_t *)0x40000800u)
#define STM32_TIM6              ((volatile struct stm32_tim_t *)0x40001000u)
#define STM32_TIM7              ((volatile struct stm32_tim_t *)0x40001400u)
#define STM32_RTC               ((volatile struct stm32_rtc_t *)0x40002800u)
#define STM32_WWDG              ((volatile struct stm32_wwdg_t *)0x40002c00u)
#define STM32_IWDG              ((volatile struct stm32_iwdg_t *)0x40003000u)
#define STM32_I2S2EXT           ((volatile struct stm32_i2s2ext_t *)0x40003400u)
#define STM32_I2S2              ((volatile struct stm32_spi2_i2s2_t *)0x40003800u)
#define STM32_I2S3              ((volatile struct stm32_spi3_i2s3_t *)0x40003c00u)
#define STM32_I2S3EXT           ((volatile struct stm32_i2s3ext_t *)0x40004000u)
#define STM32_USART2            ((volatile struct stm32_usart_t *)0x40004400u)
#define STM32_USART3            ((volatile struct stm32_usart_t *)0x40004800u)
#define STM32_UART4             ((volatile struct stm32_usart_t *)0x40004c00u)
#define STM32_UART5             ((volatile struct stm32_usart_t *)0x40005000u)
#define STM32_I2C1              ((volatile struct stm32_i2c_t *)0x40005400u)
#define STM32_I2C2              ((volatile struct stm32_i2c_t *)0x40005800u)
#define STM32_USB_DEVICE_FS     ((volatile struct stm32_usb_device_fs_t *)0x40005c00u)
#define STM32_USB_SRAM          ((volatile struct stm32_usb_sram_t *)0x40006000u)
#define STM32_BXCAN             ((volatile struct stm32_bxcan_t *)0x40006400u)
#define STM32_PWR               ((volatile struct stm32_pwr_t *)0x40007000u)
#define STM32_DAC1              ((volatile struct stm32_dac_t *)0x40007400u)
#define STM32_SYSCFG_COMP_OPAMP ((volatile struct stm32_syscfg_comp_opamp_t *)0x40010000u)
#define STM32_EXTI              ((volatile struct stm32_exti_t *)0x40010400u)
#define STM32_TIM1              ((volatile struct stm32_tim_t *)0x40012c00u)
#define STM32_SPI1              ((volatile struct stm32_spi_t *)0x40013000u)
#define STM32_TIM8              ((volatile struct stm32_tim_t *)0x40013400u)
#define STM32_USART1            ((volatile struct stm32_usart_t *)0x40013800u)
#define STM32_TIM15             ((volatile struct stm32_tim_t *)0x40014000u)
#define STM32_TIM16             ((volatile struct stm32_tim_t *)0x40014400u)
#define STM32_TIM17             ((volatile struct stm32_tim_t *)0x40014800u)
#define STM32_DMA1              ((volatile struct stm32_dma_t *)0x40020000u)
#define STM32_DMA2              ((volatile struct stm32_dma_t *)0x40020400u)
#define STM32_RCC               ((volatile struct stm32_rcc_t *)0x40021000u)
#define STM32_FLASH             ((volatile struct stm32_flash_t *)0x40022000u)
#define STM32_CRC               ((volatile struct stm32_crc_t *)0x40023000u)
#define STM32_TSC               ((volatile struct stm32_tsc_t *)0x40024000u)
#define STM32_GPIOA             ((volatile struct stm32_gpio_t *)0x48000000u)
#define STM32_GPIOB             ((volatile struct stm32_gpio_t *)0x48000400u)
#define STM32_GPIOC             ((volatile struct stm32_gpio_t *)0x48000800u)
#define STM32_GPIOD             ((volatile struct stm32_gpio_t *)0x48000c00u)
#define STM32_GPIOE             ((volatile struct stm32_gpio_t *)0x48001000u)
#define STM32_GPIOF             ((volatile struct stm32_gpio_t *)0x48001400u)
#define STM32_ADC1_ADC2         ((volatile struct stm32_adc1_adc2_t *)0x50000000u)
#define STM32_ADC3_ADC4         ((volatile struct stm32_adc3_adc4_t *)0x50000400u)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

#endif
