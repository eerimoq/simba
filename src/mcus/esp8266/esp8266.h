/**
 * @file mcus/esp8266/esp8266.h
 * @version 0.1.0
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

/**
 * Parts of this file was based on the Espressif documentation and
 * header files (MIT License).
 */

#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "c_types.h"

#define BITFIELD_SET(name, value)               \
    (((value) << name ## _POS) & name ## _MASK)

#define BITFIELD_GET(name, value)               \
    (((value) & name ## _MASK) >> name ## _POS)

/* Interrupt numbering. */
#define ESP8266_IRQ_NUM_ETS_SLC    1
#define ESP8266_IRQ_NUM_SPI        2
#define ESP8266_IRQ_NUM_GPIO       4
#define ESP8266_IRQ_NUM_UART       5
#define ESP8266_IRQ_NUM_MAX        6
#define ESP8266_IRQ_NUM_SOFT       7
#define ESP8266_IRQ_NUM_WDT        8
#define ESP8266_IRQ_NUM_TIMER1     9

/**
 * Universal Asynchronous Receive Transmit.
 */
struct esp8266_uart_t {
    uint32_t FIFO;
    uint32_t INT_RAW;
    uint32_t INT_ST;
    uint32_t INT_ENA;
    uint32_t INT_CLR;
    uint32_t CLKDIV;
    uint32_t AUTOBAUD;
    uint32_t STATUS;
    uint32_t CONF0;
    uint32_t CONF1;
    uint32_t LOWPULSE;
    uint32_t HIGHPULSE;
    uint32_t RXD_CNT;
    uint32_t reserved0[11];
    uint32_t DATE;
    uint32_t ID;
};

/* UART FIFO, length 128. */
/* R/W share the same address. This register contains the next byte in
 * the fifo. */
#define ESP8266_UART_FIFO_RXFIFO_RD_BYTE_POS  (0)
#define ESP8266_UART_FIFO_RXFIFO_RD_BYTE_MASK           \
    (0xff << ESP8266_UART_FIFO_RXFIFO_RD_BYTE_POS)

/* UART INTERRUPT RAW STATE */
/* The interrupt raw bit for Rx time-out interrupt(depands on the
   UART_RX_TOUT_THRHD) */
#define ESP8266_UART_INT_RAW_RXFIFO_TOUT BIT(8)
/* The interrupt raw bit for Rx byte start error */
#define ESP8266_UART_INT_RAW_BRK_DET     BIT(7)
/* The interrupt raw bit for CTS changing level */
#define ESP8266_UART_INT_RAW_CTS_CHG     BIT(6)
/* The interrupt raw bit for DSR changing level */
#define ESP8266_UART_INT_RAW_DSR_CHG     BIT(5)
/* The interrupt raw bit for rx fifo overflow */
#define ESP8266_UART_INT_RAW_RXFIFO_OVF  BIT(4)
/* The interrupt raw bit for other rx error */
#define ESP8266_UART_INT_RAW_FRM_ERR     BIT(3)
/* The interrupt raw bit for parity check error */
#define ESP8266_UART_INT_RAW_PARITY_ERR  BIT(2)
/* The interrupt raw bit for tx fifo empty interrupt(depands on
   UART_TXFIFO_EMPTY_THRHD bits) */
#define ESP8266_UART_INT_RAW_TXFIFO_EMPTY BIT(1)
/* The interrupt raw bit for rx fifo full interrupt(depands on
   UART_RXFIFO_FULL_THRHD bits) */
#define ESP8266_UART_INT_RAW_RXFIFO_FULL BIT(0)

/* UART INTERRUPT STATE REGISTER（UART_INT_RAW&UART_INT_ENA） */
/* The interrupt state bit for Rx time-out event */
#define ESP8266_UART_INT_ST_RXFIFO_TOUT  BIT(8)
/* The interrupt state bit for rx byte start error */
#define ESP8266_UART_INT_ST_BRK_DET      BIT(7)
/* The interrupt state bit for CTS changing level */
#define ESP8266_UART_INT_ST_CTS_CHG      BIT(6)
/* The interrupt state bit for DSR changing level */
#define ESP8266_UART_INT_ST_DSR_CHG      BIT(5)
/* The interrupt state bit for RX fifo overflow */
#define ESP8266_UART_INT_ST_RXFIFO_OVF   BIT(4)
/* The interrupt state for other rx error */
#define ESP8266_UART_INT_ST_FRM_ERR      BIT(3)
/* The interrupt state bit for rx parity error */
#define ESP8266_UART_INT_ST_PARITY_ERR   BIT(2)
/* The interrupt state bit for TX fifo empty */
#define ESP8266_UART_INT_ST_TXFIFO_EMPTY BIT(1)
/* The interrupt state bit for RX fifo full event */
#define ESP8266_UART_INT_ST_RXFIFO_FULL  BIT(0)

/* UART INTERRUPT ENABLE REGISTER */
/* The interrupt enable bit for rx time-out interrupt */
#define ESP8266_UART_INT_ENA_RXFIFO_TOUT BIT(8)
/* The interrupt enable bit for rx byte start error */
#define ESP8266_UART_INT_ENA_BRK_DET     BIT(7)
/* The interrupt enable bit for CTS changing level */
#define ESP8266_UART_INT_ENA_CTS_CHG     BIT(6)
/* The interrupt enable bit for DSR changing level */
#define ESP8266_UART_INT_ENA_DSR_CHG     BIT(5)
/* The interrupt enable bit for rx fifo overflow */
#define ESP8266_UART_INT_ENA_RXFIFO_OVF  BIT(4)
/* The interrupt enable bit for other rx error */
#define ESP8266_UART_INT_ENA_FRM_ERR     BIT(3)
/* The interrupt enable bit for parity error */
#define ESP8266_UART_INT_ENA_PARITY_ERR  BIT(2)
/* The interrupt enable bit for tx fifo empty event */
#define ESP8266_UART_INT_ENA_TXFIFO_EMPTY BIT(1)
/* The interrupt enable bit for rx fifo full event */
#define ESP8266_UART_INT_ENA_RXFIFO_FULL BIT(0)

/* UART INTERRUPT CLEAR REGISTER */
/* Set this bit to clear the rx time-out interrupt */
#define ESP8266_UART_INT_CLR_RXFIFO_TOUT BIT(8)
/* Set this bit to clear the rx byte start interrupt */
#define ESP8266_UART_INT_CLR_BRK_DET     BIT(7)
/* Set this bit to clear the CTS changing interrupt */
#define ESP8266_UART_INT_CLR_CTS_CHG     BIT(6)
/* Set this bit to clear the DSR changing interrupt */
#define ESP8266_UART_INT_CLR_DSR_CHG     BIT(5)
/* Set this bit to clear the rx fifo over-flow interrupt */
#define ESP8266_UART_INT_CLR_RXFIFO_OVF  BIT(4)
/* Set this bit to clear other rx error interrupt */
#define ESP8266_UART_INT_CLR_FRM_ERR     BIT(3)
/* Set this bit to clear the parity error interrupt */
#define ESP8266_UART_INT_CLR_PARITY_ERR  BIT(2)
/* Set this bit to clear the tx fifo empty interrupt */
#define ESP8266_UART_INT_CLR_TXFIFO_EMPTY BIT(1)
/* Set this bit to clear the rx fifo full interrupt */
#define ESP8266_UART_INT_CLR_RXFIFO_FULL BIT(0)

/* UART CLK DIV REGISTER */
/* BAUDRATE = UART_CLK_FREQ / UART_CLKDIV */
#define ESP8266_UART_CLKDIV_POS  0
#define ESP8266_UART_CLKDIV_MASK 0xfffff

/* UART BAUDRATE DETECT REGISTER */
#define ESP8266_UART_GLITCH_FILT_POS  (8)
#define ESP8266_UART_GLITCH_FILT_MASK (0xff << ESP8266_UART_GLITCH_FILT_POS)
/* Set this bit to enable baudrate detect */
#define ESP8266_UART_AUTOBAUD_EN      BIT(0)

/* UART STATUS REGISTER */
/* The level of the uart txd pin. */
#define ESP8266_UART_STATUS_TXD      BIT(31)
/* The level of uart rts pin. */
#define ESP8266_UART_STATUS_RTSN     BIT(30)
/* The level of uart dtr pin. */
#define ESP8266_UART_STATUS_DTRN     BIT(29)
/* Number of bytes in uart tx fifo. */
#define ESP8266_UART_STATUS_TXFIFO_CNT_POS  (16)
#define ESP8266_UART_STATUS_TXFIFO_CNT_MASK \
    (0xff << ESP8266_UART_STATUS_TXFIFO_CNT_POS)
/* The level of uart rxd pin. */
#define ESP8266_UART_STATUS_RXD      BIT(15)
/* The level of uart cts pin. */
#define ESP8266_UART_STATUS_CTSN     BIT(14)
/* The level of uart dsr pin. */
#define ESP8266_UART_STATUS_DSRN     BIT(13)
/* Number of bytes in uart rx fifo. */
#define ESP8266_UART_STATUS_RXFIFO_CNT_POS  (0)
#define ESP8266_UART_STATUS_RXFIFO_CNT_MASK             \
    (0xff << ESP8266_UART_STATUS_RXFIFO_CNT_POS)

/* UART CONF0. */
/* Set this bit to inverse uart dtr level. */
#define ESP8266_UART_CONF0_UART_DTR_INV    BIT(24)
/* Set this bit to inverse uart rts level. */
#define ESP8266_UART_CONF0_UART_RTS_INV    BIT(23)
/* Set this bit to inverse uart txd level. */
#define ESP8266_UART_CONF0_UART_TXD_INV    BIT(22)
/* Set this bit to inverse uart dsr level. */
#define ESP8266_UART_CONF0_UART_DSR_INV    BIT(21)
/* Set this bit to inverse uart cts level. */
#define ESP8266_UART_CONF0_UART_CTS_INV    BIT(20)
/* Set this bit to inverse uart rxd level. */
#define ESP8266_UART_CONF0_UART_RXD_INV    BIT(19)
/* Set this bit to reset uart tx fifo. */
#define ESP8266_UART_CONF0_TXFIFO_RST      BIT(18)
/* Set this bit to reset uart rx fifo. */
#define ESP8266_UART_CONF0_RXFIFO_RST      BIT(17)
/* Set this bit to enable uart tx hardware flow control. */
#define ESP8266_UART_CONF0_TX_FLOW_EN      BIT(15)
/* Set this bit to enable uart loopback test mode. */
#define ESP8266_UART_CONF0_UART_LOOPBACK   BIT(14)
/* RESERVED, DO NOT CHANGE THIS BIT. */
#define ESP8266_UART_CONF0_TXD_BRK         BIT(8)
/* sw dtr. */
#define ESP8266_UART_CONF0_SW_DTR          BIT(7)
/* sw rts. */
#define ESP8266_UART_CONF0_SW_RTS          BIT(6)
/* Set stop bit: 1:1bit  2:1.5bits  3:2bits */
#define ESP8266_UART_CONF0_STOP_BIT_NUM_POS (4)
#define ESP8266_UART_CONF0_STOP_BIT_NUM_MASK \
    (0x3 << ESP8266_UART_CONF0_STOP_BIT_NUM_POS)
/* Set bit num:  0:5bits 1:6bits 2:7bits 3:8bits */
#define ESP8266_UART_CONF0_BIT_NUM_POS     (2)
#define ESP8266_UART_CONF0_BIT_NUM_MASK         \
    (0x3 << ESP8266_UART_CONF0_BIT_NUM_POS)
/* Set this bit to enable uart parity check. */
#define ESP8266_UART_CONF0_PARITY_EN       BIT(1)
/* Set parity check:  0:even 1:odd. */
#define ESP8266_UART_CONF0_PARITY          BIT(0)

/* UART_CONF1. */
/* Set this bit to enable rx time-out function. */
#define ESP8266_UART_CONF1_RX_TOUT_EN      BIT(31)
/* Config bits for rx time-out threshold, uint: byte, 0-127 */
#define ESP8266_UART_CONF1_RX_TOUT_THRHD   [30:24]
/* Set this bit to enable rx hardware flow control */
#define ESP8266_UART_CONF1_RX_FLOW_EN      [23]
/* The config bits for rx flow control threshold, 0-127. */
#define ESP8266_UART_CONF1_RX_FLOW_THRHD   [22:16]
/* The config bits for tx fifo empty threshold, 0-127. */
#define ESP8266_UART_CONF1_TXFIFO_EMPTY_THRHD [14:8]
/* The config bits for rx fifo full threshold, 0-127. */
#define ESP8266_UART_CONF1_RXFIFO_FULL_THRHD  [6:0]

/* LOWPULSE */
/* used in baudrate detect */
#define ESP8266_UART_LOWPULSE_MIN_CNT [19:0]

/* HIGHPULSE */
/* used in baudrate detect */
#define ESP8266_UART_HIGHPULSE_MIN_CNT [19:0]

/* RXD_CNT */
/* used in baudrate detect */
#define ESP8266_UART_RXD_CNT_EDGE    [9:0]

/* UART_DATE */
/* UART HW INFO */
#define ESP8266_UART_DATE       [31:0]

/* UART_ID */
#define ESP8266_UART_ID         [31:0]

/**
 * Serial Peripheral Interface.
 */
struct esp8266_spi_t {
    uint32_t reserverd0[16];
    uint32_t CHIP_ID;
};

/**
 * General Purpose Input Output registers.
 */
struct esp8266_gpio_t {
    uint32_t OUT;
    uint32_t OUT_W1TS;
    uint32_t OUT_W1TC;
    uint32_t ENABLE;
    uint32_t ENABLE_DATA_W1TS;
    uint32_t ENABLE_DATA_W1TC;
    uint32_t IN;
    uint32_t STATUS;
    uint32_t STATUS_W1TS;
    uint32_t STATUS_W1TC;
    uint32_t CONF[16];
    uint32_t SIGMA_DELTA;
    uint32_t RTC;
    uint32_t RTC_CALIB;
};

/* Output register. */
#define ESP8266_GPIO_BT_SEL             0x0000ffff
#define ESP8266_GPIO_BT_SEL_POS         16
/* The output value when the GPIO pin is set as output. */
#define ESP8266_GPIO_OUT_DATA           0x0000ffff
#define ESP8266_GPIO_OUT_DATA_POS       0

/* Output register set. */
/* Writing 1 into a bit in this register will set the related bit in
   GPIO_OUT_DATA. */
#define ESP8266_GPIO_OUT_DATA_W1TS      0x0000ffff
#define ESP8266_GPIO_OUT_DATA_W1TS_POS  0

/* Output register clear. */
/* Writing 1 into a bit in this register will clear the related bit in
   GPIO_OUT_DATA. */
#define ESP8266_GPIO_OUT_DATA_W1TC      0x0000ffff
#define ESP8266_GPIO_OUT_DATA_W1TC_POS  0

/* Enable register. */
/* SDIO-dis selection register. */
#define ESP8266_GPIO_SDIO_SEL           0x0000003f
#define ESP8266_GPIO_SDIO_SEL_POS       16
/* The output enable register. */
#define ESP8266_GPIO_ENABLE_DATA        0x0000ffff
#define ESP8266_GPIO_ENABLE_DATA_POS    0

/* Enable register set. */
/* Writing 1 into a bit in this register will set the related bit in
   GPIO_ENABLE_DATA. */
#define ESP8266_GPIO_ENABLE_DATA_W1TS     0x0000ffff
#define ESP8266_GPIO_ENABLE_DATA_W1TS_POS 0

/* Enable register clear. */
/* Writing 1 into a bit in this register will clear the related bit in
   GPIO_ENABLE_DATA. */
#define ESP8266_GPIO_ENABLE_DATA_W1TC      0x0000ffff
#define ESP8266_GPIO_ENABLE_DATA_W1TC_POS  0

/* Input register. */
/* The values of the strapping pins. */
#define ESP8266_GPIO_STRAPPING             0x0000ffff
#define ESP8266_GPIO_STRAPPING_POS         16
/* The values of the GPIO pins when the GPIO pin is set as input. */
#define ESP8266_GPIO_IN_DATA               0x0000ffff
#define ESP8266_GPIO_IN_DATA_POS           0

/* Status register. */
/* Interrupt enable register. */
#define ESP8266_GPIO_STATUS_INTERRUPT      0x0000ffff
#define ESP8266_GPIO_STATUS_INTERRUPT_POS  0

/* Status register set. */
/* Writing 1 into a bit in this register will set the related bit in
   GPIO_STATUS_INTERRUPT. */
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TS     0x0000ffff
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TS_POS 0

/* Status register clear. */
/* Writing 1 into a bit in this register will clear the related bit in
   GPIO_STATUS_INTERRUPT. */
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TC      0x0000ffff
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TC_POS  0

/* Configuration register. */
/* 0: disable; 1: enable GPIO wakeup CPU, only when GPIO_PIN0_INT_TYPE
   is 0x4 or 0x5 */
#define ESP8266_GPIO_CONF_WAKEUP_ENABLE    BIT(10)
/* 0: disable; 1: positive edge; 2: negative edge; 3: both types of
   edge; 4: low-level; 5: high-level */
#define ESP8266_GPIO_CONF_INT_TYPE_POS     (7)
#define ESP8266_GPIO_CONF_INT_TYPE_MASK         \
    (0x7 << ESP8266_GPIO_CONF_INT_TYPE_POS)
#define ESP8266_GPIO_CONF_INT_TYPE(value)               \
    BITFIELD_SET(ESP8266_GPIO_CONF_INT_TYPE, value)
/* 1: open drain; 0: normal */
#define ESP8266_GPIO_CONF_DRIVER           BIT(2)
/* 1: sigma-delta; 0: GPIO_DATA. */
#define ESP8266_GPIO_CONF_SOURCE           BIT(0)

/* Sigma delta register. */
/* 1: enable sigma-delta; 0: disable */
#define SIGMA_DELTA_ENABLE                      BIT(16)
/* Clock pre-divider for sigma-delta. */
#define ESP8266_GPIO_SIGMA_DELTA_PRESCALAR_POS       (8)
#define ESP8266_GPIO_SIGMA_DELTA_PRESCALAR_MASK         \
    (0xff << ESP8266_GPIO_SIGMA_DELTA_PRESCALAR_POS)
#define ESP8266_GPIO_SIGMA_DELTA_PRESCALAR(value)               \
    BITFIELD_SET(ESP8266_GPIO_SIGMA_DELTA_PRESCALAR, value)
/* Target level of the sigma-delta. It is a signed byte. */
#define ESP8266_GPIO_SIGMA_DELTA_TARGET_POS          (0)
#define ESP8266_GPIO_SIGMA_DELTA_TARGET_MASK            \
    (0xff << ESP8266_GPIO_SIGMA_DELTA_TARGET_POS)
#define ESP8266_GPIO_SIGMA_DELTA_TARGET(value)                  \
    BITFIELD_SET(ESP8266_GPIO_SIGMA_DELTA_TARGET, value)

/* RTC register. */
/* Positvie edge of this bit will trigger the RTC-clock-calibration
   process. */
#define RTC_CALIB_START                      BIT(31)
/* The cycle number of RTC-clock during RTC-clock-calibration. */
#define ESP8266_GPIO_RTC_PERIOD_NUM_POS      (0)
#define ESP8266_GPIO_RTC_PERIOD_NUM_MASK        \
    (0x3ff << ESP8266_GPIO_RTC_PERIOD_NUM_POS)
#define ESP8266_GPIO_RTC_PERIOD_NUM(value)              \
    BITFIELD_SET(ESP8266_GPIO_RTC_PERIOD_NUM, value)

/* RTC calibration value register. */
/* 0: during RTC-clock-calibration; 1: RTC-clock-calibration is
   done */
#define ESP8266_GPIO_RTC_CALIB_RDY           BIT(31)
/* 0: during RTC-clock-calibration; 1: RTC-clock-calibration is
   done */
#define ESP8266_GPIO_RTC_CALIB_RDY_REAL      BIT(30)
/* The cycle number of clk_xtal (crystal clock) for the RTC_PERIOD_NUM
   cycles of RTC-clock. */
#define ESP8266_GPIO_RTC_CALIB_VALUE_POS     (0)
#define ESP8266_GPIO_RTC_CALIB_VALUE_MASK               \
    (0xfffff << ESP8266_GPIO_RTC_CALIB_VALUE_POS)
#define ESP8266_GPIO_RTC_CALIB_VALUE(value)             \
    BITFIELD_SET(ESP8266_GPIO_RTC_CALIB_VALUE, value)

/**
 * Timer.
 */
struct esp8266_timer_t {
    uint32_t LOAD;
    uint32_t COUNT;
    uint32_t CTRL;
    uint32_t INT;
    union {
        uint32_t reserved0[4];
        uint32_t ALARM;
    } U;
};

/* Control register.*/
#define ESP8266_TIMER_CTRL_INT           BIT(8)
#define ESP8266_TIMER_CTRL_ENABLE        BIT(7)
#define ESP8266_TIMER_CTRL_AUTO_LOAD     BIT(6)
#define ESP8266_TIMER_CTRL_PRESCALE_POS  (1)
#define ESP8266_TIMER_CTRL_PRESCALE_MASK        \
    (0x1f << ESP8266_TIMER_CTRL_PRESCALE_POS)
#define ESP8266_TIMER_CTRL_PRESCALE(value)              \
    BITFIELD_SET(ESP8266_TIMER_CTRL_PRESCALE, value)
#define ESP8266_TIMER_CTRL_PRESCALE_1    ESP8266_TIMER_CTRL_PRESCALE(0)
#define ESP8266_TIMER_CTRL_PRESCALE_16   ESP8266_TIMER_CTRL_PRESCALE(2)
#define ESP8266_TIMER_CTRL_PRESCALE_256  ESP8266_TIMER_CTRL_PRESCALE(4)
#define ESP8266_TIMER_CTRL_INT_TRIGGER       BIT(0)
#define ESP8266_TIMER_CTRL_INT_TRIGGER_EDGE  (0)
#define ESP8266_TIMER_CTRL_INT_TRIGGER_LEVEL (1)

/* Interrupt clear register. */
#define ESP8266_TIMER_INT_CLR                BIT(0)

/* Alarm register. */
#define ESP8266_TIMER_FRC2_ALARM_MASK        0xffffffff
#define ESP8266_TIMER_FRC2_ALARM_POS         0

/**
 * Input Output Multiplexing.
 */
struct esp8266_iomux_t {
    uint32_t CONF;
    uint32_t PIN[16];
};

/* Details for the CONF register. */
#define ESP8266_IOMUX_CONF_SPI1_CLK_EQU_SYS_CLK  BIT(9)
#define ESP8266_IOMUX_CONF_SPI0_CLK_EQU_SYS_CLK  BIT(8)

/* Details for the PIN registers. */
#define ESP8266_IOMUX_PIN_FUNC_HIGH_POS        (8)
#define ESP8266_IOMUX_PIN_FUNC_HIGH_MASK        \
    (0x1 << ESP8266_IOMUX_PIN_FUNC_HIGH_POS)
#define ESP8266_IOMUX_PIN_PULLUP               BIT(7)
#define ESP8266_IOMUX_PIN_PULLDOWN             BIT(6)
#define ESP8266_IOMUX_PIN_FUNC_LOW_POS         (4)
#define ESP8266_IOMUX_PIN_FUNC_LOW_MASK         \
    (0x3 << ESP8266_IOMUX_PIN_FUNC_LOW_POS)
#define ESP8266_IOMUX_PIN_SLEEP_PULLUP         BIT(3)
#define ESP8266_IOMUX_PIN_SLEEP_PULLDOWN       BIT(2)
#define ESP8266_IOMUX_PIN_SLEEP_OUTPUT_ENABLE  BIT(1)
#define ESP8266_IOMUX_PIN_OUTPUT_ENABLE        BIT(0)

#define ESP8266_IOMUX_PIN_FUNC(value)                           \
    (BITFIELD_SET(ESP8266_IOMUX_PIN_FUNC_LOW, value)            \
     | BITFIELD_SET(ESP8266_IOMUX_PIN_FUNC_HIGH, (value) >> 2))

#define ESP8266_IOMUX_PIN_FUNC_GPIO(iomux)      \
    ((iomux) > 11                               \
     ? ESP8266_IOMUX_PIN_FUNC(0)                \
     : ESP8266_IOMUX_PIN_FUNC(3))

/**
 * Devices.
 */
#define ESP8266_UART0  ((volatile struct esp8266_uart_t    *)0x60000000)
#define ESP8266_SPI0   ((volatile struct esp8266_spi_t     *)0x60000100)
#define ESP8266_SPI1   ((volatile struct esp8266_spi_t     *)0x60000200)
#define ESP8266_GPIO   ((volatile struct esp8266_gpio_t    *)0x60000300)
#define ESP8266_TIMER0 ((volatile struct esp8266_timer_t   *)0x60000600)
#define ESP8266_TIMER1 ((volatile struct esp8266_timer_t   *)0x60000620)
#define ESP8266_RTC    ((volatile struct esp8266_rtc_t     *)0x60000700)
#define ESP8266_IOMUX  ((volatile struct esp8266_iomux_t   *)0x60000800)
#define ESP8266_WDT    ((volatile struct esp8266_wdt_t     *)0x60000900)
#define ESP8266_SCL    ((volatile struct esp8266_scl_t     *)0x60000b00)
#define ESP8266_SAR    ((volatile struct esp8266_sar_t     *)0x60000d00)
#define ESP8266_I2S    ((volatile struct esp8266_i2s_t     *)0x60000e00)
#define ESP8266_UART1  ((volatile struct esp8266_uart_t    *)0x60000f00)

#endif
