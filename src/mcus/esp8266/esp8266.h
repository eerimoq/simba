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

/**
 * Parts of this file was based on the Espressif documentation and
 * header files (MIT License).
 */

#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "c_types.h"

#define ESP8266_REG(value) (*((volatile uint32_t *)(0x60000000 + value)))

/* Interrupt numbering. */
#define ESP8266_IRQ_NUM_ETS_SLC                          1
#define ESP8266_IRQ_NUM_SPI                              2
#define ESP8266_IRQ_NUM_GPIO                             4
#define ESP8266_IRQ_NUM_UART                             5
#define ESP8266_IRQ_NUM_MAX                              6
#define ESP8266_IRQ_NUM_SOFT                             7
#define ESP8266_IRQ_NUM_WDT                              8
#define ESP8266_IRQ_NUM_TIMER1                           9

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
#define ESP8266_UART_INT_RAW_RXFIFO_FULL            BIT(0)
#define ESP8266_UART_INT_RAW_TXFIFO_EMPTY           BIT(1)
#define ESP8266_UART_INT_RAW_PARITY_ERR             BIT(2)
#define ESP8266_UART_INT_RAW_FRM_ERR                BIT(3)
#define ESP8266_UART_INT_RAW_RXFIFO_OVF             BIT(4)
#define ESP8266_UART_INT_RAW_DSR_CHG                BIT(5)
#define ESP8266_UART_INT_RAW_CTS_CHG                BIT(6)
#define ESP8266_UART_INT_RAW_BRK_DET                BIT(7)
#define ESP8266_UART_INT_RAW_RXFIFO_TOUT            BIT(8)

/* UART INTERRUPT STATE REGISTER（UART_INT_RAW&UART_INT_ENA） */
#define ESP8266_UART_INT_ST_RXFIFO_FULL             BIT(0)
#define ESP8266_UART_INT_ST_TXFIFO_EMPTY            BIT(1)
#define ESP8266_UART_INT_ST_PARITY_ERR              BIT(2)
#define ESP8266_UART_INT_ST_FRM_ERR                 BIT(3)
#define ESP8266_UART_INT_ST_RXFIFO_OVF              BIT(4)
#define ESP8266_UART_INT_ST_DSR_CHG                 BIT(5)
#define ESP8266_UART_INT_ST_CTS_CHG                 BIT(6)
#define ESP8266_UART_INT_ST_BRK_DET                 BIT(7)
#define ESP8266_UART_INT_ST_RXFIFO_TOUT             BIT(8)

/* UART INTERRUPT ENABLE REGISTER */
#define ESP8266_UART_INT_ENA_RXFIFO_FULL            BIT(0)
#define ESP8266_UART_INT_ENA_TXFIFO_EMPTY           BIT(1)
#define ESP8266_UART_INT_ENA_PARITY_ERR             BIT(2)
#define ESP8266_UART_INT_ENA_FRM_ERR                BIT(3)
#define ESP8266_UART_INT_ENA_RXFIFO_OVF             BIT(4)
#define ESP8266_UART_INT_ENA_DSR_CHG                BIT(5)
#define ESP8266_UART_INT_ENA_CTS_CHG                BIT(6)
#define ESP8266_UART_INT_ENA_BRK_DET                BIT(7)
#define ESP8266_UART_INT_ENA_RXFIFO_TOUT            BIT(8)

/* UART INTERRUPT CLEAR REGISTER */
#define ESP8266_UART_INT_CLR_RXFIFO_FULL            BIT(0)
#define ESP8266_UART_INT_CLR_TXFIFO_EMPTY           BIT(1)
#define ESP8266_UART_INT_CLR_PARITY_ERR             BIT(2)
#define ESP8266_UART_INT_CLR_FRM_ERR                BIT(3)
#define ESP8266_UART_INT_CLR_RXFIFO_OVF             BIT(4)
#define ESP8266_UART_INT_CLR_DSR_CHG                BIT(5)
#define ESP8266_UART_INT_CLR_CTS_CHG                BIT(6)
#define ESP8266_UART_INT_CLR_BRK_DET                BIT(7)
#define ESP8266_UART_INT_CLR_RXFIFO_TOUT            BIT(8)

/* UART CLK DIV REGISTER */
#define ESP8266_UART_CLKDIV_POS                     0
#define ESP8266_UART_CLKDIV_MASK                    0xfffff

/* UART BAUDRATE DETECT REGISTER */
#define ESP8266_UART_GLITCH_FILT_MASK (0xff << ESP8266_UART_GLITCH_FILT_POS)
#define ESP8266_UART_AUTOBAUD_EN                    BIT(0)
#define ESP8266_UART_GLITCH_FILT_POS                   (8)

/* UART STATUS REGISTER */
#define ESP8266_UART_STATUS_RXFIFO_CNT_POS             (0)
#define ESP8266_UART_STATUS_RXFIFO_CNT_MASK             \
    (0xff << ESP8266_UART_STATUS_RXFIFO_CNT_POS)
#define ESP8266_UART_STATUS_DSRN                   BIT(13)
#define ESP8266_UART_STATUS_CTSN                   BIT(14)
#define ESP8266_UART_STATUS_RXD                    BIT(15)
#define ESP8266_UART_STATUS_TXFIFO_CNT_POS            (16)
#define ESP8266_UART_STATUS_TXFIFO_CNT_MASK             \
    (0xff << ESP8266_UART_STATUS_TXFIFO_CNT_POS)
#define ESP8266_UART_STATUS_DTRN                   BIT(29)
#define ESP8266_UART_STATUS_RTSN                   BIT(30)
#define ESP8266_UART_STATUS_TXD                    BIT(31)

/* UART CONF0. */
#define ESP8266_UART_CONF0_PARITY                   BIT(0)
#define ESP8266_UART_CONF0_PARITY_EN                BIT(1)
#define ESP8266_UART_CONF0_BIT_NUM_POS                 (2)
#define ESP8266_UART_CONF0_BIT_NUM_MASK         \
    (0x3 << ESP8266_UART_CONF0_BIT_NUM_POS)
#define ESP8266_UART_CONF0_STOP_BIT_NUM_POS            (4)
#define ESP8266_UART_CONF0_STOP_BIT_NUM_MASK            \
    (0x3 << ESP8266_UART_CONF0_STOP_BIT_NUM_POS)
#define ESP8266_UART_CONF0_SW_RTS                   BIT(6)
#define ESP8266_UART_CONF0_SW_DTR                   BIT(7)
#define ESP8266_UART_CONF0_TXD_BRK                  BIT(8)
#define ESP8266_UART_CONF0_UART_LOOPBACK           BIT(14)
#define ESP8266_UART_CONF0_TX_FLOW_EN              BIT(15)
#define ESP8266_UART_CONF0_RXFIFO_RST              BIT(17)
#define ESP8266_UART_CONF0_TXFIFO_RST              BIT(18)
#define ESP8266_UART_CONF0_UART_RXD_INV            BIT(19)
#define ESP8266_UART_CONF0_UART_CTS_INV            BIT(20)
#define ESP8266_UART_CONF0_UART_DSR_INV            BIT(21)
#define ESP8266_UART_CONF0_UART_TXD_INV            BIT(22)
#define ESP8266_UART_CONF0_UART_RTS_INV            BIT(23)
#define ESP8266_UART_CONF0_UART_DTR_INV            BIT(24)

/* UART_CONF1. */
#define ESP8266_UART_CONF1_RX_TOUT_EN              BIT(31)
#define ESP8266_UART_CONF1_RX_TOUT_THRHD           [30:24]
#define ESP8266_UART_CONF1_RX_FLOW_EN                 [23]
#define ESP8266_UART_CONF1_RX_FLOW_THRHD           [22:16]
#define ESP8266_UART_CONF1_TXFIFO_EMPTY_THRHD       [14:8]
#define ESP8266_UART_CONF1_RXFIFO_FULL_THRHD         [6:0]

/* LOWPULSE */
#define ESP8266_UART_LOWPULSE_MIN_CNT               [19:0]

/* HIGHPULSE */
#define ESP8266_UART_HIGHPULSE_MIN_CNT              [19:0]

/* RXD_CNT */
#define ESP8266_UART_RXD_CNT_EDGE                    [9:0]

/**
 * Serial Peripheral Interface.
 */
struct esp8266_spi_t {
    uint32_t CMD;
    uint32_t ADDR;
    uint32_t CTRL;
    uint32_t RESERVED0;
    uint32_t RD_STATUS;
    uint32_t CTRL2;
    uint32_t CLOCK;
    uint32_t USER;
    uint32_t USER1;
    uint32_t USER2;
    uint32_t WR_STATUS;
    uint32_t PIN;
    uint32_t SLAVE;
    uint32_t SLAVE1;
    uint32_t SLAVE2;
    uint32_t SLAVE3;
    uint32_t W_0_15[16];
    uint32_t RESERVED1[0xa0];
    uint32_t EXT3;
};

/* CMD. */
#define ESP8266_SPI_CMD_USR                        BIT(18)

/* ADDR. */
#define ESP8266_SPI_ADDR_IODATA_START_ADDR             (0)

/* CTRL. */
#define ESP8266_SPI_CTRL_FASTRD_MODE               BIT(13)
#define ESP8266_SPI_CTRL_DOUT_MODE                 BIT(14)
#define ESP8266_SPI_CTRL_QOUT_MODE                 BIT(20)
#define ESP8266_SPI_CTRL_DIO_MODE                  BIT(23)
#define ESP8266_SPI_CTRL_QIO_MODE                  BIT(24)
#define ESP8266_SPI_CTRL_RD_BIT_ORDER              BIT(25)
#define ESP8266_SPI_CTRL_WR_BIT_ORDER              BIT(26)

/* RD_STATUS. */
#define ESP8266_SPI_RD_STATUS_SLV_RD_STATUS            (0)

/* CTRL2. */
#define ESP8266_SPI_CTRL2_CS_DELAY_NUM
#define ESP8266_SPI_CTRL2_CS_DELAY_MODE
#define ESP8266_SPI_CTRL2_MOSI_DELAY_NUM
#define ESP8266_SPI_CTRL2_MOSI_DELAY_MODE
#define ESP8266_SPI_CTRL2_MISO_DELAY_NUM
#define ESP8266_SPI_CTRL2_MISO_DELAY_MODE

/* CLOCK. */
#define ESP8266_SPI_CLOCK_EQU_SYSCLK               BIT(31)
#define ESP8266_SPI_CLOCK_DIV_PRE
#define ESP8266_SPI_CLOCK_CNT_N
#define ESP8266_SPI_CLOCK_CNT_H
#define ESP8266_SPI_CLOCK_CNT_L

/* USER. */
#define ESP8266_SPI_USER_CK_I_EDGE                  BIT(6)
#define ESP8266_SPI_USER_RD_BYTE_ORDER             BIT(10)
#define ESP8266_SPI_USER_WR_BYTE_ORDER             BIT(11)
#define ESP8266_SPI_USER_FWRITE_DUAL               BIT(12)
#define ESP8266_SPI_USER_FWRITE_QUAD               BIT(13)
#define ESP8266_SPI_USER_FWRITE_DIO                BIT(14)
#define ESP8266_SPI_USER_FWRITE_QIO                BIT(15)
#define ESP8266_SPI_USER_SIO                       BIT(16)
#define ESP8266_SPI_USER_MISO_HIGHPART             BIT(24)
#define ESP8266_SPI_USER_MOSI_HIGHPART             BIT(25)
#define ESP8266_SPI_USER_MOSI                      BIT(27)
#define ESP8266_SPI_USER_MISO                      BIT(28)
#define ESP8266_SPI_USER_DUMMY                     BIT(29)
#define ESP8266_SPI_USER_ADDR                      BIT(30)
#define ESP8266_SPI_USER_COMMAND                   BIT(31)

/* USER1. */
#define ESP8266_SPI_USER1_ADDR_BITLEN
#define ESP8266_SPI_USER1_MOSI_BITLEN(value) ((value) << 17)
#define ESP8266_SPI_USER1_MISO_BITLEN(value) ((value) << 8)
#define ESP8266_SPI_USER1_DUMMY_CYCLELEN

/* USER2. */
#define ESP8266_SPI_USER2_COMMAND_BITLEN
#define ESP8266_SPI_USER2_COMMAND_VALUE

/* WR_STATUS. */
#define ESP8266_SPI_WR_STATUS_SLV

/* PIN. */
#define ESP8266_SPI_PIN_CS2_DIS
#define ESP8266_SPI_PIN_CS1_DIS
#define ESP8266_SPI_PIN_CS0_DIS

/* SLAVE. */
#define ESP8266_SPI_SLAVE_SYNC_RESET
#define ESP8266_SPI_SLAVE_MODE
#define ESP8266_SPI_SLAVE_CMD_DEFINE
#define ESP8266_SPI_SLAVE_TRANS_CNT
#define ESP8266_SPI_SLAVE_INT_EN_TRANS_DONE (1 << 9)
#define ESP8266_SPI_SLAVE_TRANS_DONE
#define ESP8266_SPI_SLAVE_WR_STA_DONE
#define ESP8266_SPI_SLAVE_RD_STA_DONE
#define ESP8266_SPI_SLAVE_WR_BUF_DONE
#define ESP8266_SPI_SLAVE_RD_BUF_DONE

/* SLAVE1. */
#define ESP8266_SPI_SLAVE1_STATUS_BITLEN
#define ESP8266_SPI_SLAVE1_BUF_BITLEN
#define ESP8266_SPI_SLAVE1_RD_ADDR_BITLEN
#define ESP8266_SPI_SLAVE1_WR_ADDR_BITLEN
#define ESP8266_SPI_SLAVE1_WRSTA_DUMMY_EN
#define ESP8266_SPI_SLAVE1_RDSTA_DUMMY_EN
#define ESP8266_SPI_SLAVE1_WRBUF_DUMMY_EN
#define ESP8266_SPI_SLAVE1_RDBUF_DUMMY_EN

/* SLAVE2. */
#define ESP8266_SPI_SLAVE2_WRBUF_DUMMY_CYCLELEN
#define ESP8266_SPI_SLAVE2_RDBUF_DUMMY_CYCLELEN
#define ESP8266_SPI_SLAVE2_WRSTA_DUMMY_CYCLELEN
#define ESP8266_SPI_SLAVE2_RDSTA_DUMMY_CYCLELEN

/* SLAVE3. */
#define ESP8266_SPI_SLAVE3_WRSTA_CMD_VALUE
#define ESP8266_SPI_SLAVE3_RDSTA_CMD_VALUE
#define ESP8266_SPI_SLAVE3_WRBUF_CMD_VALUE
#define ESP8266_SPI_SLAVE3_RDBUF_CMD_VALUE

/* EXT3. */
#define ESP8266_SPI_EXT3_INT_HOLD_ENA

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
#define ESP8266_GPIO_BT_SEL                     0x0000ffff
#define ESP8266_GPIO_BT_SEL_POS                         16
#define ESP8266_GPIO_OUT_DATA                   0x0000ffff
#define ESP8266_GPIO_OUT_DATA_POS                        0

/* Output register set. */
#define ESP8266_GPIO_OUT_DATA_W1TS              0x0000ffff
#define ESP8266_GPIO_OUT_DATA_W1TS_POS                   0

/* Output register clear. */
#define ESP8266_GPIO_OUT_DATA_W1TC              0x0000ffff
#define ESP8266_GPIO_OUT_DATA_W1TC_POS                   0

/* Enable register. */
#define ESP8266_GPIO_SDIO_SEL                   0x0000003f
#define ESP8266_GPIO_SDIO_SEL_POS                       16
#define ESP8266_GPIO_ENABLE_DATA                0x0000ffff
#define ESP8266_GPIO_ENABLE_DATA_POS                     0

/* Enable register set. */
#define ESP8266_GPIO_ENABLE_DATA_W1TS           0x0000ffff
#define ESP8266_GPIO_ENABLE_DATA_W1TS_POS                0

/* Enable register clear. */
#define ESP8266_GPIO_ENABLE_DATA_W1TC           0x0000ffff
#define ESP8266_GPIO_ENABLE_DATA_W1TC_POS                0

/* Input register. */
#define ESP8266_GPIO_STRAPPING                  0x0000ffff
#define ESP8266_GPIO_STRAPPING_POS                      16
#define ESP8266_GPIO_IN_DATA                    0x0000ffff
#define ESP8266_GPIO_IN_DATA_POS                         0

/* Status register. */
#define ESP8266_GPIO_STATUS_INTERRUPT           0x0000ffff
#define ESP8266_GPIO_STATUS_INTERRUPT_POS                0

/* Status register set. */
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TS      0x0000ffff
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TS_POS           0

/* Status register clear. */
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TC      0x0000ffff
#define ESP8266_GPIO_STATUS_INTERRUPT_W1TC_POS           0

/* Configuration register. */
#define ESP8266_GPIO_CONF_SOURCE                    BIT(0)
#define ESP8266_GPIO_CONF_DRIVER                    BIT(2)
#define ESP8266_GPIO_CONF_INT_TYPE_POS                 (7)
#define ESP8266_GPIO_CONF_INT_TYPE_MASK         \
    (0x7 << ESP8266_GPIO_CONF_INT_TYPE_POS)
#define ESP8266_GPIO_CONF_INT_TYPE(value)               \
    BITFIELD_SET(ESP8266_GPIO_CONF_INT_TYPE, value)
#define ESP8266_GPIO_CONF_WAKEUP_ENABLE            BIT(10)

/* Sigma delta register. */
#define ESP8266_GPIO_SIGMA_DELTA_TARGET_POS            (0)
#define ESP8266_GPIO_SIGMA_DELTA_TARGET_MASK            \
    (0xff << ESP8266_GPIO_SIGMA_DELTA_TARGET_POS)
#define ESP8266_GPIO_SIGMA_DELTA_TARGET(value)                  \
    BITFIELD_SET(ESP8266_GPIO_SIGMA_DELTA_TARGET, value)
#define ESP8266_GPIO_SIGMA_DELTA_PRESCALAR_POS         (8)
#define ESP8266_GPIO_SIGMA_DELTA_PRESCALAR_MASK         \
    (0xff << ESP8266_GPIO_SIGMA_DELTA_PRESCALAR_POS)
#define ESP8266_GPIO_SIGMA_DELTA_PRESCALAR(value)               \
    BITFIELD_SET(ESP8266_GPIO_SIGMA_DELTA_PRESCALAR, value)
#define SIGMA_DELTA_ENABLE                         BIT(16)

/* RTC register. */
#define ESP8266_GPIO_RTC_PERIOD_NUM_POS                (0)
#define ESP8266_GPIO_RTC_PERIOD_NUM_MASK        \
    (0x3ff << ESP8266_GPIO_RTC_PERIOD_NUM_POS)
#define ESP8266_GPIO_RTC_PERIOD_NUM(value)              \
    BITFIELD_SET(ESP8266_GPIO_RTC_PERIOD_NUM, value)
#define RTC_CALIB_START                            BIT(31)

/* RTC calibration value register. */
#define ESP8266_GPIO_RTC_CALIB_VALUE_POS               (0)
#define ESP8266_GPIO_RTC_CALIB_VALUE_MASK               \
    (0xfffff << ESP8266_GPIO_RTC_CALIB_VALUE_POS)
#define ESP8266_GPIO_RTC_CALIB_VALUE(value)             \
    BITFIELD_SET(ESP8266_GPIO_RTC_CALIB_VALUE, value)
#define ESP8266_GPIO_RTC_CALIB_RDY_REAL            BIT(30)
#define ESP8266_GPIO_RTC_CALIB_RDY                 BIT(31)

/**
 * RTC.
 */
struct esp8266_rtc_t {
    uint32_t RESERVED0[26];
    struct {
        uint32_t OUT;
        uint32_t RESERVED0[2];
        uint32_t EN;
        uint32_t RESERVED1[5];
        uint32_t IN;
        uint32_t CONTROL;
        uint32_t RESERVED2[3];
        uint32_t PIN;
    } GPIO;
};

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
#define ESP8266_TIMER_CTRL_INT_TRIGGER              BIT(0)
#define ESP8266_TIMER_CTRL_INT_TRIGGER_EDGE            (0)
#define ESP8266_TIMER_CTRL_INT_TRIGGER_LEVEL           (1)
#define ESP8266_TIMER_CTRL_PRESCALE_POS                (1)
#define ESP8266_TIMER_CTRL_PRESCALE_MASK        \
    (0x1f << ESP8266_TIMER_CTRL_PRESCALE_POS)
#define ESP8266_TIMER_CTRL_PRESCALE(value)              \
    BITFIELD_SET(ESP8266_TIMER_CTRL_PRESCALE, value)
#define ESP8266_TIMER_CTRL_PRESCALE_1    ESP8266_TIMER_CTRL_PRESCALE(0)
#define ESP8266_TIMER_CTRL_PRESCALE_16   ESP8266_TIMER_CTRL_PRESCALE(2)
#define ESP8266_TIMER_CTRL_PRESCALE_256  ESP8266_TIMER_CTRL_PRESCALE(4)
#define ESP8266_TIMER_CTRL_AUTO_LOAD                BIT(6)
#define ESP8266_TIMER_CTRL_ENABLE                   BIT(7)
#define ESP8266_TIMER_CTRL_INT                      BIT(8)

/* Interrupt clear register. */
#define ESP8266_TIMER_INT_CLR                       BIT(0)

/* Alarm register. */
#define ESP8266_TIMER_FRC2_ALARM_POS                    0
#define ESP8266_TIMER_FRC2_ALARM_MASK          0xffffffff

/**
 * Input Output Multiplexing.
 */
    struct esp8266_iomux_t {
        uint32_t CONF;
        uint32_t PIN[16];
    };

/* Details for the CONF register. */
#define ESP8266_IOMUX_CONF_SPI1_CLK_EQU_SYS_CLK     BIT(8)
#define ESP8266_IOMUX_CONF_SPI0_CLK_EQU_SYS_CLK     BIT(9)

/* Details for the PIN registers. */
#define ESP8266_IOMUX_PIN_OUTPUT_ENABLE             BIT(0)
#define ESP8266_IOMUX_PIN_SLEEP_OUTPUT_ENABLE       BIT(1)
#define ESP8266_IOMUX_PIN_SLEEP_PULLDOWN            BIT(2)
#define ESP8266_IOMUX_PIN_SLEEP_PULLUP              BIT(3)
#define ESP8266_IOMUX_PIN_FUNC_LOW_POS                 (4)
#define ESP8266_IOMUX_PIN_FUNC_LOW_MASK         \
    (0x3 << ESP8266_IOMUX_PIN_FUNC_LOW_POS)
#define ESP8266_IOMUX_PIN_PULLDOWN                  BIT(6)
#define ESP8266_IOMUX_PIN_PULLUP                    BIT(7)
#define ESP8266_IOMUX_PIN_FUNC_HIGH_POS                (8)
#define ESP8266_IOMUX_PIN_FUNC_HIGH_MASK                \
        (0x1 << ESP8266_IOMUX_PIN_FUNC_HIGH_POS)

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
