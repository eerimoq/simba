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

#ifndef __ESP32_H__
#define __ESP32_H__

/**
 * Peripheral interrupt sources.
 *
 * The interrupt matrix maps each peripheral interrupt to a CPU
 * interrupt.
 */
#define ESP32_INTR_SOURCE_MAC                         0
#define ESP32_INTR_SOURCE_MAC_NMI                     1
#define ESP32_INTR_SOURCE_BB                          2
#define ESP32_INTR_SOURCE_BT_MAC                      3
#define ESP32_INTR_SOURCE_BT_BB                       4
#define ESP32_INTR_SOURCE_BT_BB_NMI                   5
#define ESP32_INTR_SOURCE_RWBT                        6
#define ESP32_INTR_SOURCE_RWBLE                       7
#define ESP32_INTR_SOURCE_RWBT_NMI                    8
#define ESP32_INTR_SOURCE_RWBLE_NMI                   9
#define ESP32_INTR_SOURCE_SLC0                       10
#define ESP32_INTR_SOURCE_SLC1                       11
#define ESP32_INTR_SOURCE_UHCI0                      12
#define ESP32_INTR_SOURCE_UHCI1                      13
#define ESP32_INTR_SOURCE_TG0_T0_LEVEL               14
#define ESP32_INTR_SOURCE_TG0_T1_LEVEL               15
#define ESP32_INTR_SOURCE_TG0_WDT_LEVEL              16
#define ESP32_INTR_SOURCE_TG0_LACT_LEVEL             17
#define ESP32_INTR_SOURCE_TG1_T0_LEVEL               18
#define ESP32_INTR_SOURCE_TG1_T1_LEVEL               19
#define ESP32_INTR_SOURCE_TG1_WDT_LEVEL              20
#define ESP32_INTR_SOURCE_TG1_LACT_LEVEL             21
#define ESP32_INTR_SOURCE_GPIO                       22
#define ESP32_INTR_SOURCE_GPIO_NMI                   23
#define ESP32_INTR_SOURCE_FROM_CPU_0                 24
#define ESP32_INTR_SOURCE_FROM_CPU_1                 25
#define ESP32_INTR_SOURCE_FROM_CPU_2                 26
#define ESP32_INTR_SOURCE_FROM_CPU_3                 27
#define ESP32_INTR_SOURCE_SPI0                       28
#define ESP32_INTR_SOURCE_SPI1                       29
#define ESP32_INTR_SOURCE_SPI2                       30
#define ESP32_INTR_SOURCE_SPI3                       31
#define ESP32_INTR_SOURCE_I2S0                       32
#define ESP32_INTR_SOURCE_I2S1                       33
#define ESP32_INTR_SOURCE_UART0                      34
#define ESP32_INTR_SOURCE_UART1                      35
#define ESP32_INTR_SOURCE_UART2                      36
#define ESP32_INTR_SOURCE_SDIO_HOST                  37
#define ESP32_INTR_SOURCE_ETH_MAC                    38
#define ESP32_INTR_SOURCE_PWM0                       39
#define ESP32_INTR_SOURCE_PWM1                       40
#define ESP32_INTR_SOURCE_PWM2                       41
#define ESP32_INTR_SOURCE_PWM3                       42
#define ESP32_INTR_SOURCE_LEDC                       43
#define ESP32_INTR_SOURCE_EFUSE                      44
#define ESP32_INTR_SOURCE_CAN                        45
#define ESP32_INTR_SOURCE_RTC_CORE                   46
#define ESP32_INTR_SOURCE_RMT                        47
#define ESP32_INTR_SOURCE_PCNT                       48
#define ESP32_INTR_SOURCE_I2C_EXT0                   49
#define ESP32_INTR_SOURCE_I2C_EXT1                   50
#define ESP32_INTR_SOURCE_RSA                        51
#define ESP32_INTR_SOURCE_SPI1_DMA                   52
#define ESP32_INTR_SOURCE_SPI2_DMA                   53
#define ESP32_INTR_SOURCE_SPI3_DMA                   54
#define ESP32_INTR_SOURCE_WDT                        55
#define ESP32_INTR_SOURCE_TIMER1                     56
#define ESP32_INTR_SOURCE_TIMER2                     57
#define ESP32_INTR_SOURCE_TG0_T0_EDGE                58
#define ESP32_INTR_SOURCE_TG0_T1_EDGE                59
#define ESP32_INTR_SOURCE_TG0_WDT_EDGE               60
#define ESP32_INTR_SOURCE_TG0_LACT_EDGE              61
#define ESP32_INTR_SOURCE_TG1_T0_EDGE                62
#define ESP32_INTR_SOURCE_TG1_T1_EDGE                63
#define ESP32_INTR_SOURCE_TG1_WDT_EDGE               64
#define ESP32_INTR_SOURCE_TG1_LACT_EDGE              65
#define ESP32_INTR_SOURCE_MMU_IA                     66
#define ESP32_INTR_SOURCE_MPU_IA                     67
#define ESP32_INTR_SOURCE_CACHE_IA                   68

/**
 * CPU interrupts are numbered from 0 to 31. Peripheral interrupts can
 * only be mapped to the defines starting with PERIPHERAL_.
 */
#define ESP32_CPU_INTR_PERIPHERAL_0_PRIO_1            0
#define ESP32_CPU_INTR_PERIPHERAL_1_PRIO_1            1
#define ESP32_CPU_INTR_PERIPHERAL_2_PRIO_1            2
#define ESP32_CPU_INTR_PERIPHERAL_3_PRIO_1            3
#define ESP32_CPU_INTR_PERIPHERAL_4_PRIO_1            4
#define ESP32_CPU_INTR_PERIPHERAL_5_PRIO_1            5
#define ESP32_CPU_INTR_INTERNAL_6_PRIO_1              6
#define ESP32_CPU_INTR_INTERNAL_7_PRIO_1              7
#define ESP32_CPU_INTR_PERIPHERAL_8_PRIO_1            8
#define ESP32_CPU_INTR_PERIPHERAL_9_PRIO_1            9
#define ESP32_CPU_INTR_PERIPHERAL_10_PRIO_1          10
#define ESP32_CPU_INTR_INTERNAL_11_PRIO_3            11
#define ESP32_CPU_INTR_PERIPHERAL_12_PRIO_1          12
#define ESP32_CPU_INTR_PERIPHERAL_13_PRIO_1          13
#define ESP32_CPU_INTR_PERIPHERAL_14_PRIO_1          14
#define ESP32_CPU_INTR_INTERNAL_15_PRIO_3            15
#define ESP32_CPU_INTR_INTERNAL_16_PRIO_5            16
#define ESP32_CPU_INTR_PERIPHERAL_17_PRIO_1          17
#define ESP32_CPU_INTR_PERIPHERAL_18_PRIO_1          18
#define ESP32_CPU_INTR_PERIPHERAL_19_PRIO_2          19
#define ESP32_CPU_INTR_PERIPHERAL_20_PRIO_2          20
#define ESP32_CPU_INTR_PERIPHERAL_21_PRIO_2          21
#define ESP32_CPU_INTR_PERIPHERAL_22_PRIO_3          22
#define ESP32_CPU_INTR_PERIPHERAL_23_PRIO_3          23
#define ESP32_CPU_INTR_PERIPHERAL_24_PRIO_4          24
#define ESP32_CPU_INTR_PERIPHERAL_25_PRIO_4          25
#define ESP32_CPU_INTR_PERIPHERAL_26_PRIO_5          26
#define ESP32_CPU_INTR_PERIPHERAL_27_PRIO_3          27
#define ESP32_CPU_INTR_PERIPHERAL_28_PRIO_4          28
#define ESP32_CPU_INTR_INTERNAL_29_PRIO_3            29
#define ESP32_CPU_INTR_PERIPHERAL_30_PRIO_4          30
#define ESP32_CPU_INTR_PERIPHERAL_31_PRIO_5          31

/* Mapping to an internal interrupt will disable the peripheral. */
#define PERIPHERAL_DISABLED  ESP32_CPU_INTR_INTERNAL_6_PRIO_1

/**
 * Interrupt matrix configration.
 *
 * Map UART0 events to Protocol CPU interrupt number 12:
 * PRO.UART_INTR_MAP_REG = ESP32_CPU_INTR_PERIPHERAL_12_PRIO_1;
 *
 * Map SPI0 events to Application CPU interrupt number 12:
 * APP.SPI_INTR_0_MAP_REG = ESP32_CPU_INTR_PERIPHERAL_13_PRIO_1;
 *
 * The default mapping in Simba is:
 *
 * Peripheral  CPU interrupt
 * =======================================================
 * UART0       ESP32_CPU_INTR_PERIPHERAL_13_PRIO_1
 * UART1       ESP32_CPU_INTR_PERIPHERAL_13_PRIO_1
 * UART2       ESP32_CPU_INTR_PERIPHERAL_13_PRIO_1
 * SPI0        ESP32_CPU_INTR_PERIPHERAL_17_PRIO_1
 * SPI1        ESP32_CPU_INTR_PERIPHERAL_17_PRIO_1
 * SPI2        ESP32_CPU_INTR_PERIPHERAL_17_PRIO_1
 */

#define ESP32_CPU_INTR_SYS_TICK_NUM  ESP32_CPU_INTR_PERIPHERAL_12_PRIO_1
#define ESP32_CPU_INTR_UART_NUM      ESP32_CPU_INTR_PERIPHERAL_13_PRIO_1
#define ESP32_CPU_INTR_CAN_NUM       ESP32_CPU_INTR_PERIPHERAL_17_PRIO_1
#define ESP32_CPU_INTR_SPI_NUM       ESP32_CPU_INTR_PERIPHERAL_18_PRIO_1

/**
 * Interrupt matrix mapping registers.
 */
struct esp32_dport_map_t {
    uint32_t MAC_INTR_MAP_REG;
    uint32_t MAC_NMI_MAP_REG;
    uint32_t BB_INT_MAP_REG;
    uint32_t BT_MAC_INT_MAP_REG;
    uint32_t BT_BB_INT_MAP_REG;
    uint32_t BT_BB_NMI_MAP_REG;
    uint32_t RWBT_IRQ_MAP_REG;
    uint32_t RWBLE_IRQ_MAP_REG;
    uint32_t RWBT_NMI_MAP_REG;
    uint32_t RWBLE_NMI_MAP_REG;
    uint32_t SLC0_INTR_MAP_REG;
    uint32_t SLC1_INTR_MAP_REG;
    uint32_t UHCI0_INTR_MAP_REG;
    uint32_t UHCI1_INTR_MAP_REG;
    uint32_t TG_T0_LEVEL_INT_MAP_REG;
    uint32_t TG_T1_LEVEL_INT_MAP_REG;
    uint32_t TG_WDT_LEVEL_INT_MAP_REG;
    uint32_t TG_LACT_LEVEL_INT_MAP_REG;
    uint32_t TG1_T0_LEVEL_INT_MAP_REG;
    uint32_t TG1_T1_LEVEL_INT_MAP_REG;
    uint32_t TG1_WDT_LEVEL_INT_MAP_REG;
    uint32_t TG1_LACT_LEVEL_INT_MAP_REG;
    uint32_t GPIO_INTERRUPT_MAP_REG;
    uint32_t GPIO_INTERRUPT_NMI_MAP_REG;
    uint32_t CPU_INTR_FROM_CPU_0_MAP_REG;
    uint32_t CPU_INTR_FROM_CPU_1_MAP_REG;
    uint32_t CPU_INTR_FROM_CPU_2_MAP_REG;
    uint32_t CPU_INTR_FROM_CPU_3_MAP_REG;
    uint32_t SPI_INTR_0_MAP_REG;
    uint32_t SPI_INTR_1_MAP_REG;
    uint32_t SPI_INTR_2_MAP_REG;
    uint32_t SPI_INTR_3_MAP_REG;
    uint32_t I2S0_INT_MAP_REG;
    uint32_t I2S1_INT_MAP_REG;
    uint32_t UART_INTR_MAP_REG;
    uint32_t UART1_INTR_MAP_REG;
    uint32_t UART2_INTR_MAP_REG;
    uint32_t SDIO_HOST_INTERRUPT_MAP_REG;
    uint32_t EMAC_INT_MAP_REG;
    uint32_t PWM0_INTR_MAP_REG;
    uint32_t PWM1_INTR_MAP_REG;
    uint32_t PWM2_INTR_MAP_REG;
    uint32_t PWM3_INTR_MAP_REG;
    uint32_t LEDC_INT_MAP_REG;
    uint32_t EFUSE_INT_MAP_REG;
    uint32_t CAN_INT_MAP_REG;
    uint32_t RTC_CORE_INTR_MAP_REG;
    uint32_t RMT_INTR_MAP_REG;
    uint32_t PCNT_INTR_MAP_REG;
    uint32_t I2C_EXT0_INTR_MAP_REG;
    uint32_t I2C_EXT1_INTR_MAP_REG;
    uint32_t RSA_INTR_MAP_REG;
    uint32_t SPI1_DMA_INT_MAP_REG;
    uint32_t SPI2_DMA_INT_MAP_REG;
    uint32_t SPI3_DMA_INT_MAP_REG;
    uint32_t WDG_INT_MAP_REG;
    uint32_t TIMER_INT1_MAP_REG;
    uint32_t TIMER_INT2_MAP_REG;
    uint32_t TG_T0_EDGE_INT_MAP_REG;
    uint32_t TG_T1_EDGE_INT_MAP_REG;
    uint32_t TG_WDT_EDGE_INT_MAP_REG;
    uint32_t TG_LACT_EDGE_INT_MAP_REG;
    uint32_t TG1_T0_EDGE_INT_MAP_REG;
    uint32_t TG1_T1_EDGE_INT_MAP_REG;
    uint32_t TG1_WDT_EDGE_INT_MAP_REG;
    uint32_t TG1_LACT_EDGE_INT_MAP_REG;
    uint32_t MMU_IA_INT_MAP_REG;
    uint32_t MPU_IA_INT_MAP_REG;
    uint32_t CACHE_IA_INT_MAP_REG;
};

struct esp32_dport_t {
    uint32_t PRO_BOOT_REMAP_CTRL_REG;
    uint32_t APP_BOOT_REMAP_CTRL_REG;
    uint32_t DPORT_ACCESS_CHECK;
    uint32_t PRO_DPORT_APB_MASK0;
    uint32_t PRO_DPORT_APB_MASK1;
    uint32_t APP_DPORT_APB_MASK0;
    uint32_t APP_DPORT_APB_MASK1;
    uint32_t PERI_CLK_EN;
    uint32_t PERI_RST_EN;
    uint32_t WIFI_BB_CFG;
    uint32_t WIFI_BB_CFG_2;
    uint32_t APPCPU_CTRL_REG_A;
    uint32_t APPCPU_CTRL_REG_B;
    uint32_t APPCPU_CTRL_REG_C;
    uint32_t APPCPU_CTRL_REG_D;
    uint32_t CPU_PER_CONF_REG;
    uint32_t PRO_CACHE_CTRL_REG;
    uint32_t PRO_CACHE_CTRL1_REG;
    uint32_t PRO_CACHE_LOCK_0_ADDR_REG;
    uint32_t PRO_CACHE_LOCK_1_ADDR_REG;
    uint32_t PRO_CACHE_LOCK_2_ADDR_REG;
    uint32_t PRO_CACHE_LOCK_3_ADDR_REG;
    uint32_t APP_CACHE_CTRL_REG;
    uint32_t APP_CACHE_CTRL1_REG;
    uint32_t APP_CACHE_LOCK_0_ADDR_REG;
    uint32_t APP_CACHE_LOCK_1_ADDR_REG;
    uint32_t APP_CACHE_LOCK_2_ADDR_REG;
    uint32_t APP_CACHE_LOCK_3_ADDR_REG;
    uint32_t TRACEMEM_MUX_MODE;
    uint32_t PRO_TRACEMEM_ENA;
    uint32_t APP_TRACEMEM_ENA;
    uint32_t CACHE_MUX_MODE;
    uint32_t IMMU_PAGE_MODE;
    uint32_t DMMU_PAGE_MODE;
    uint32_t ROM_MPU_ENA;
    uint32_t MEM_PD_MASK_REG;
    uint32_t ROM_PD_CTRL_REG;
    uint32_t ROM_FO_CTRL_REG;
    uint32_t SRAM_PD_CTRL_REG_0;
    uint32_t SRAM_PD_CTRL_REG_1;
    uint32_t SRAM_FO_CTRL_REG_0;
    uint32_t SRAM_FO_CTRL_REG_1;
    uint32_t IRAM_DRAM_AHB_SEL;
    uint32_t TAG_FO_CTRL_REG;
    uint32_t AHB_LITE_MASK_REG;
    uint32_t AHB_MPU_TABLE_0;
    uint32_t AHB_MPU_TABLE_1;
    uint32_t HOST_INF_SEL;
    uint32_t PERIP_CLK_EN;
    uint32_t PERIP_RST_EN;
    uint32_t SLAVE_SPI_CONFIG;
    uint32_t WIFI_CLK_EN;
    uint32_t WIFI_RST_EN;
    uint32_t BT_LPCK_DIV_INT;
    uint32_t BT_LPCK_DIV_FRAC;
    uint32_t CPU_INTR_FROM_CPU_0_REG;
    uint32_t CPU_INTR_FROM_CPU_1_REG;
    uint32_t CPU_INTR_FROM_CPU_2_REG;
    uint32_t CPU_INTR_FROM_CPU_3_REG;
    uint32_t PRO_INTR_STATUS_REG[3];
    uint32_t APP_INTR_STATUS_REG[3];
    struct esp32_dport_map_t PRO;
    struct esp32_dport_map_t APP;
    uint32_t AHBLITE_MPU_TABLE_UART;
    uint32_t AHBLITE_MPU_TABLE_SPI1;
    uint32_t AHBLITE_MPU_TABLE_SPI0;
    uint32_t AHBLITE_MPU_TABLE_GPIO;
    uint32_t AHBLITE_MPU_TABLE_FE2;
    uint32_t AHBLITE_MPU_TABLE_FE;
    uint32_t AHBLITE_MPU_TABLE_TIMER;
    uint32_t AHBLITE_MPU_TABLE_RTC;
    uint32_t AHBLITE_MPU_TABLE_IO_MUX;
    uint32_t AHBLITE_MPU_TABLE_WDG;
    uint32_t AHBLITE_MPU_TABLE_HINF;
    uint32_t AHBLITE_MPU_TABLE_UHCI1;
    uint32_t AHBLITE_MPU_TABLE_MISC;
    uint32_t AHBLITE_MPU_TABLE_I2C;
    uint32_t AHBLITE_MPU_TABLE_I2S0;
    uint32_t AHBLITE_MPU_TABLE_UART1;
    uint32_t AHBLITE_MPU_TABLE_BT;
    uint32_t AHBLITE_MPU_TABLE_BT_BUFFER;
    uint32_t AHBLITE_MPU_TABLE_I2C_EXT0;
    uint32_t AHBLITE_MPU_TABLE_UHCI0;
    uint32_t AHBLITE_MPU_TABLE_SLCHOST;
    uint32_t AHBLITE_MPU_TABLE_RMT;
    uint32_t AHBLITE_MPU_TABLE_PCNT;
    uint32_t AHBLITE_MPU_TABLE_SLC;
    uint32_t AHBLITE_MPU_TABLE_LEDC;
    uint32_t AHBLITE_MPU_TABLE_EFUSE;
    uint32_t AHBLITE_MPU_TABLE_SPI_ENCRYPT;
    uint32_t AHBLITE_MPU_TABLE_BB;
    uint32_t AHBLITE_MPU_TABLE_PWM0;
    uint32_t AHBLITE_MPU_TABLE_TIMERGROUP;
    uint32_t AHBLITE_MPU_TABLE_TIMERGROUP1;
    uint32_t AHBLITE_MPU_TABLE_SPI2;
    uint32_t AHBLITE_MPU_TABLE_SPI3;
    uint32_t AHBLITE_MPU_TABLE_APB_CTRL;
    uint32_t AHBLITE_MPU_TABLE_I2C_EXT1;
    uint32_t AHBLITE_MPU_TABLE_SDIO_HOST;
    uint32_t AHBLITE_MPU_TABLE_EMAC;
    uint32_t AHBLITE_MPU_TABLE_CAN;
    uint32_t AHBLITE_MPU_TABLE_PWM1;
    uint32_t AHBLITE_MPU_TABLE_I2S1;
    uint32_t AHBLITE_MPU_TABLE_UART2;
    uint32_t AHBLITE_MPU_TABLE_PWM2;
    uint32_t AHBLITE_MPU_TABLE_PWM3;
    uint32_t AHBLITE_MPU_TABLE_RWBT;
    uint32_t AHBLITE_MPU_TABLE_BTMAC;
    uint32_t AHBLITE_MPU_TABLE_WIFIMAC;
    uint32_t AHBLITE_MPU_TABLE_PWR;
    uint32_t MEM_ACCESS_DBUG[2];
    uint32_t PRO_DCACHE_DBUG_REG[10];
    uint32_t APP_DCACHE_DBUG_REG[10];
    uint32_t PRO_CPU_RECORD_CTRL;
    uint32_t PRO_CPU_RECORD_STATUS;
    uint32_t PRO_CPU_RECORD_PID;
    uint32_t PRO_CPU_RECORD_PDEBUGINST;
    uint32_t PRO_CPU_RECORD_PDEBUGSTATUS;
    uint32_t PRO_CPU_RECORD_PDEBUGDATA;
    uint32_t PRO_CPU_RECORD_PDEBUGPC;
    uint32_t PRO_CPU_RECORD_PDEBUGLS0STAT;
    uint32_t PRO_CPU_RECORD_PDEBUGLS0ADDR;
    uint32_t PRO_CPU_RECORD_PDEBUGLS0DATA;
    uint32_t APP_CPU_RECORD_CTRL;
    uint32_t APP_CPU_RECORD_STATUS;
    uint32_t APP_CPU_RECORD_PID;
    uint32_t APP_CPU_RECORD_PDEBUGINST;
    uint32_t APP_CPU_RECORD_PDEBUGSTATUS;
    uint32_t APP_CPU_RECORD_PDEBUGDATA;
    uint32_t APP_CPU_RECORD_PDEBUGPC;
    uint32_t APP_CPU_RECORD_PDEBUGLS0STAT;
    uint32_t APP_CPU_RECORD_PDEBUGLS0ADDR;
    uint32_t APP_CPU_RECORD_PDEBUGLS0DATA;
    uint32_t RSA_PD_CTRL_REG;
    uint32_t ROM_MPU_TABLE[4];
    uint32_t SHROM_MPU_TABLE[24];
    uint32_t IMMU_TABLE[16];
    uint32_t DMMU_TABLE[16];
    uint32_t PRO_INTRUSION_CTRL;
    uint32_t PRO_INTRUSION_STATUS;
    uint32_t APP_INTRUSION_CTRL;
    uint32_t APP_INTRUSION_STATUS;
    uint32_t FRONT_END_MEM_PD;
    uint32_t MMU_IA_INT_EN;
    uint32_t MPU_IA_INT_EN;
    uint32_t CACHE_IA_INT_EN;
    uint32_t SECURE_BOOT_CTRL;
    uint32_t SPI_DMA_CHAN_SEL;
    uint32_t PRO_VECBASE_CTRL;
    uint32_t PRO_VECBASE_SET;
    uint32_t APP_VECBASE_CTRL;
    uint32_t APP_VECBASE_SET;
    uint32_t DPORT_REG_DATE;
};

/**
 * Universal Asynchronous Receive Transmit.
 */
struct esp32_uart_t {
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
    uint32_t FLOW_CONF;
    uint32_t SLEEP_CONF;
    uint32_t SWFC_CONF;
    uint32_t IDLE_CONF;
    uint32_t RS485_CONF;
    uint32_t AT_CMD_PRECNT;
    uint32_t AT_CMD_POSTCNT;
    uint32_t AT_CMD_GAPTOUT;
    uint32_t AT_CMD_CHAR;
    uint32_t MEM_CONF;
    uint32_t MEM_TX_STATUS;
    uint32_t MEM_RX_STATUS;
    uint32_t MEM_CNT_STATUS;
    uint32_t POSPULSE;
    uint32_t NEGPULSE;
    uint32_t RESERVED0[2];
    uint32_t DATE;
    uint32_t ID;
};

/* UART FIFO, length 128. */
/* R/W share the same address. This register contains the next byte in
 * the fifo. */
#define ESP32_UART_FIFO_RXFIFO_RD_BYTE_POS  (0)
#define ESP32_UART_FIFO_RXFIFO_RD_BYTE_MASK             \
    (0xff << ESP32_UART_FIFO_RXFIFO_RD_BYTE_POS)

/* UART INTERRUPT RAW STATE */
#define ESP32_UART_INT_RAW_RXFIFO_FULL              BIT(0)
#define ESP32_UART_INT_RAW_TXFIFO_EMPTY             BIT(1)
#define ESP32_UART_INT_RAW_PARITY_ERR               BIT(2)
#define ESP32_UART_INT_RAW_FRM_ERR                  BIT(3)
#define ESP32_UART_INT_RAW_RXFIFO_OVF               BIT(4)
#define ESP32_UART_INT_RAW_DSR_CHG                  BIT(5)
#define ESP32_UART_INT_RAW_CTS_CHG                  BIT(6)
#define ESP32_UART_INT_RAW_BRK_DET                  BIT(7)
#define ESP32_UART_INT_RAW_RXFIFO_TOUT              BIT(8)

/* UART INTERRUPT STATE REGISTER（UART_INT_RAW&UART_INT_ENA） */
#define ESP32_UART_INT_ST_RXFIFO_FULL               BIT(0)
#define ESP32_UART_INT_ST_TXFIFO_EMPTY              BIT(1)
#define ESP32_UART_INT_ST_PARITY_ERR                BIT(2)
#define ESP32_UART_INT_ST_FRM_ERR                   BIT(3)
#define ESP32_UART_INT_ST_RXFIFO_OVF                BIT(4)
#define ESP32_UART_INT_ST_DSR_CHG                   BIT(5)
#define ESP32_UART_INT_ST_CTS_CHG                   BIT(6)
#define ESP32_UART_INT_ST_BRK_DET                   BIT(7)
#define ESP32_UART_INT_ST_RXFIFO_TOUT               BIT(8)

/* UART INTERRUPT ENABLE REGISTER */
#define ESP32_UART_INT_ENA_RXFIFO_FULL              BIT(0)
#define ESP32_UART_INT_ENA_TXFIFO_EMPTY             BIT(1)
#define ESP32_UART_INT_ENA_PARITY_ERR               BIT(2)
#define ESP32_UART_INT_ENA_FRM_ERR                  BIT(3)
#define ESP32_UART_INT_ENA_RXFIFO_OVF               BIT(4)
#define ESP32_UART_INT_ENA_DSR_CHG                  BIT(5)
#define ESP32_UART_INT_ENA_CTS_CHG                  BIT(6)
#define ESP32_UART_INT_ENA_BRK_DET                  BIT(7)
#define ESP32_UART_INT_ENA_RXFIFO_TOUT              BIT(8)

/* UART INTERRUPT CLEAR REGISTER */
#define ESP32_UART_INT_CLR_RXFIFO_FULL              BIT(0)
#define ESP32_UART_INT_CLR_TXFIFO_EMPTY             BIT(1)
#define ESP32_UART_INT_CLR_PARITY_ERR               BIT(2)
#define ESP32_UART_INT_CLR_FRM_ERR                  BIT(3)
#define ESP32_UART_INT_CLR_RXFIFO_OVF               BIT(4)
#define ESP32_UART_INT_CLR_DSR_CHG                  BIT(5)
#define ESP32_UART_INT_CLR_CTS_CHG                  BIT(6)
#define ESP32_UART_INT_CLR_BRK_DET                  BIT(7)
#define ESP32_UART_INT_CLR_RXFIFO_TOUT              BIT(8)

/* UART CLK DIV REGISTER */
#define ESP32_UART_CLKDIV_POS                       0
#define ESP32_UART_CLKDIV_MASK                      0xfffff

/* UART BAUDRATE DETECT REGISTER */
#define ESP32_UART_GLITCH_FILT_MASK (0xff << ESP32_UART_GLITCH_FILT_POS)
#define ESP32_UART_AUTOBAUD_EN                      BIT(0)
#define ESP32_UART_GLITCH_FILT_POS                     (8)

/* UART STATUS REGISTER */
#define ESP32_UART_STATUS_RXFIFO_CNT_POS               (0)
#define ESP32_UART_STATUS_RXFIFO_CNT_MASK       \
    (0xff << ESP32_UART_STATUS_RXFIFO_CNT_POS)
#define ESP32_UART_STATUS_DSRN                     BIT(13)
#define ESP32_UART_STATUS_CTSN                     BIT(14)
#define ESP32_UART_STATUS_RXD                      BIT(15)
#define ESP32_UART_STATUS_TXFIFO_CNT_POS              (16)
#define ESP32_UART_STATUS_TXFIFO_CNT_MASK       \
    (0xff << ESP32_UART_STATUS_TXFIFO_CNT_POS)
#define ESP32_UART_STATUS_TXFIFO_CNT(value)        \
    BITFIELD_SET(ESP32_UART_STATUS_TXFIFO_CNT, value)
#define ESP32_UART_STATUS_DTRN                     BIT(29)
#define ESP32_UART_STATUS_RTSN                     BIT(30)
#define ESP32_UART_STATUS_TXD                      BIT(31)

/* UART CONF0. */
#define ESP32_UART_CONF0_PARITY                     BIT(0)
#define ESP32_UART_CONF0_PARITY_EN                  BIT(1)
#define ESP32_UART_CONF0_BIT_NUM_POS                   (2)
#define ESP32_UART_CONF0_BIT_NUM_MASK           \
    (0x3 << ESP32_UART_CONF0_BIT_NUM_POS)
#define ESP32_UART_CONF0_BIT_NUM(value) BITFIELD_SET(ESP32_UART_CONF0_BIT_NUM, value)
#define ESP32_UART_CONF0_STOP_BIT_NUM_POS              (4)
#define ESP32_UART_CONF0_STOP_BIT_NUM_MASK      \
    (0x3 << ESP32_UART_CONF0_STOP_BIT_NUM_POS)
#define ESP32_UART_CONF0_STOP_BIT_NUM(value) BITFIELD_SET(ESP32_UART_CONF0_STOP_BIT_NUM, value)
#define ESP32_UART_CONF0_SW_RTS                     BIT(6)
#define ESP32_UART_CONF0_SW_DTR                     BIT(7)
#define ESP32_UART_CONF0_TXD_BRK                    BIT(8)
#define ESP32_UART_CONF0_UART_LOOPBACK             BIT(14)
#define ESP32_UART_CONF0_TX_FLOW_EN                BIT(15)
#define ESP32_UART_CONF0_RXFIFO_RST                BIT(17)
#define ESP32_UART_CONF0_TXFIFO_RST                BIT(18)
#define ESP32_UART_CONF0_UART_RXD_INV              BIT(19)
#define ESP32_UART_CONF0_UART_CTS_INV              BIT(20)
#define ESP32_UART_CONF0_UART_DSR_INV              BIT(21)
#define ESP32_UART_CONF0_UART_TXD_INV              BIT(22)
#define ESP32_UART_CONF0_UART_RTS_INV              BIT(23)
#define ESP32_UART_CONF0_UART_DTR_INV              BIT(24)
#define ESP32_UART_CONF0_UART_CLK_EN               BIT(25)
#define ESP32_UART_CONF0_UART_ERR_WR_MASK          BIT(26)
#define ESP32_UART_CONF0_UART_TICK_REF_ALWAYS_ON   BIT(27)

/* UART_CONF1. */
#define ESP32_UART_CONF1_RX_TOUT_EN                BIT(31)
#define ESP32_UART_CONF1_RX_TOUT_THRHD_POS            (24)
#define ESP32_UART_CONF1_RX_TOUT_THRHD_MASK             \
    (0x7f << ESP32_UART_CONF1_RX_TOUT_THRHD_POS)
#define ESP32_UART_CONF1_RX_TOUT_THRHD(value)           \
    BITFIELD_SET(ESP32_UART_CONF1_RX_TOUT_THRHD, value)
#define ESP32_UART_CONF1_RX_FLOW_EN                BIT(23)
#define ESP32_UART_CONF1_RX_FLOW_THRHD_POS            (16)
#define ESP32_UART_CONF1_RX_FLOW_THRHD_MASK             \
    (0x7f << ESP32_UART_CONF1_RX_FLOW_THRHD_POS)
#define ESP32_UART_CONF1_RX_FLOW_THRHD(value)           \
    BITFIELD_SET(ESP32_UART_CONF1_RX_FLOW_THRHD, value)
#define ESP32_UART_CONF1_TXFIFO_EMPTY_THRHD_POS        (8)
#define ESP32_UART_CONF1_TXFIFO_EMPTY_THRHD_MASK        \
    (0x7f << ESP32_UART_CONF1_TXFIFO_EMPTY_THRHD_POS)
#define ESP32_UART_CONF1_TXFIFO_EMPTY_THRHD(value)              \
    BITFIELD_SET(ESP32_UART_CONF1_TXFIFO_EMPTY_THRHD, value)
#define ESP32_UART_CONF1_RXFIFO_FULL_THRHD_POS         (0)
#define ESP32_UART_CONF1_RXFIFO_FULL_THRHD_MASK         \
    (0x7f << ESP32_UART_CONF1_RXFIFO_FULL_THRHD_POS)
#define ESP32_UART_CONF1_RXFIFO_FULL_THRHD(value)               \
    BITFIELD_SET(ESP32_UART_CONF1_RXFIFO_FULL_THRHD, value)

/* LOWPULSE */
#define ESP32_UART_LOWPULSE_MIN_CNT                 [19:0]

/* HIGHPULSE */
#define ESP32_UART_HIGHPULSE_MIN_CNT                [19:0]

/* RXD_CNT */
#define ESP32_UART_RXD_CNT_EDGE                      [9:0]

/**
 * General Purpose Input Output registers.
 */
struct esp32_gpio_t {
    uint32_t RESERVED0;
    struct {
        uint32_t VALUE;
        uint32_t W1TS;
        uint32_t W1TC;
    } OUT[2];
    uint32_t SDIO_SELECT;
    struct {
        uint32_t VALUE;
        uint32_t W1TS;
        uint32_t W1TC;
    } ENABLE[2];
    uint32_t STRAP;
    uint32_t IN_VALUE[2];
    struct {
        uint32_t VALUE;
        uint32_t W1TS;
        uint32_t W1TC;
    } STATUS[2];
    uint32_t RESERVED1;
    struct {
        uint32_t APP;
        uint32_t APP_NMI;
        uint32_t PRO;
        uint32_t PRO_NMI;
        uint32_t SDIO;
    } INT[2];
    uint32_t PIN[40];
    uint32_t CALI_CONF;
    uint32_t CALI_DATA;
    uint32_t FUNC_IN_SEL_CFG[256];
    uint32_t FUNC_OUT_SEL_CFG[40];
};

#define ESP32_GPIO_FUNC_IN_SEL_CFG_FUNC_IN_SEL_POS         (0)
#define ESP32_GPIO_FUNC_IN_SEL_CFG_FUNC_IN_SEL_MASK             \
    (0x3f << ESP32_GPIO_FUNC_IN_SEL_CFG_FUNC_IN_SEL_POS)
#define ESP32_GPIO_FUNC_IN_SEL_CFG_FUNC_IN_INV_SEL      BIT(6)
#define ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL           BIT(7)
#define ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL_GPIO_MATRIX  (0)
#define ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL_DIRECT    \
    ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL

#define ESP32_GPIO_FUNC_IN_SEL_CFG_FUNC_IN_SEL_POS         (0)

/**
 * Peripheral signal list. Used to index FUNC_IN_SEL_CFG and to set
 * the output signal in FUNC_OUT_SEL_CFG.
 */
#define ESP32_PERIPHERAL_SIGNAL_SPICLK_IN                    0
#define ESP32_PERIPHERAL_SIGNAL_SPICLK_OUT                   0
#define ESP32_PERIPHERAL_SIGNAL_SPIQ_IN                      1
#define ESP32_PERIPHERAL_SIGNAL_SPIQ_OUT                     1
#define ESP32_PERIPHERAL_SIGNAL_SPID_IN                      2
#define ESP32_PERIPHERAL_SIGNAL_SPID_OUT                     2
#define ESP32_PERIPHERAL_SIGNAL_SPIHD_IN                     3
#define ESP32_PERIPHERAL_SIGNAL_SPIHD_OUT                    3
#define ESP32_PERIPHERAL_SIGNAL_SPIWP_IN                     4
#define ESP32_PERIPHERAL_SIGNAL_SPIWP_OUT                    4
#define ESP32_PERIPHERAL_SIGNAL_SPICS0_IN                    5
#define ESP32_PERIPHERAL_SIGNAL_SPICS0_OUT                   5
#define ESP32_PERIPHERAL_SIGNAL_SPICS1_IN                    6
#define ESP32_PERIPHERAL_SIGNAL_SPICS1_OUT                   6
#define ESP32_PERIPHERAL_SIGNAL_SPICS2_IN                    7
#define ESP32_PERIPHERAL_SIGNAL_SPICS2_OUT                   7
#define ESP32_PERIPHERAL_SIGNAL_HSPICLK_IN                   8
#define ESP32_PERIPHERAL_SIGNAL_HSPICLK_OUT                  8
#define ESP32_PERIPHERAL_SIGNAL_HSPIQ_IN                     9
#define ESP32_PERIPHERAL_SIGNAL_HSPIQ_OUT                    9
#define ESP32_PERIPHERAL_SIGNAL_HSPID_IN                    10
#define ESP32_PERIPHERAL_SIGNAL_HSPID_OUT                   10
#define ESP32_PERIPHERAL_SIGNAL_HSPICS0_IN                  11
#define ESP32_PERIPHERAL_SIGNAL_HSPICS0_OUT                 11
#define ESP32_PERIPHERAL_SIGNAL_HSPIHD_IN                   12
#define ESP32_PERIPHERAL_SIGNAL_HSPIHD_OUT                  12
#define ESP32_PERIPHERAL_SIGNAL_HSPIWP_IN                   13
#define ESP32_PERIPHERAL_SIGNAL_HSPIWP_OUT                  13
#define ESP32_PERIPHERAL_SIGNAL_U0RXD_IN                    14
#define ESP32_PERIPHERAL_SIGNAL_U0TXD_OUT                   14
#define ESP32_PERIPHERAL_SIGNAL_U0CTS_IN                    15
#define ESP32_PERIPHERAL_SIGNAL_U0RTS_OUT                   15
#define ESP32_PERIPHERAL_SIGNAL_U0DSR_IN                    16
#define ESP32_PERIPHERAL_SIGNAL_U0DTR_OUT                   16
#define ESP32_PERIPHERAL_SIGNAL_U1RXD_IN                    17
#define ESP32_PERIPHERAL_SIGNAL_U1TXD_OUT                   17
#define ESP32_PERIPHERAL_SIGNAL_U1CTS_IN                    18
#define ESP32_PERIPHERAL_SIGNAL_U1RTS_OUT                   18
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_BCK_IN                23
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_BCK_OUT               23
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_BCK_IN                24
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_BCK_OUT               24
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_WS_IN                 25
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_WS_OUT                25
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_WS_IN                 26
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_WS_OUT                26
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_BCK_IN                27
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_BCK_OUT               27
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_WS_IN                 28
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_WS_OUT                28
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT0_SCL_IN              29
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT0_SCL_OUT             29
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT0_SDA_IN              30
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT0_SDA_OUT             30
#define ESP32_PERIPHERAL_SIGNAL_PWM0_SYNC0_IN               31
#define ESP32_PERIPHERAL_SIGNAL_SDIO_TOHOST_INT_OUT         31
#define ESP32_PERIPHERAL_SIGNAL_PWM0_SYNC1_IN               32
#define ESP32_PERIPHERAL_SIGNAL_PWM0_OUT0A                  32
#define ESP32_PERIPHERAL_SIGNAL_PWM0_SYNC2_IN               33
#define ESP32_PERIPHERAL_SIGNAL_PWM0_OUT0B                  33
#define ESP32_PERIPHERAL_SIGNAL_PWM0_F0_IN                  34
#define ESP32_PERIPHERAL_SIGNAL_PWM0_OUT1A                  34
#define ESP32_PERIPHERAL_SIGNAL_PWM0_F1_IN                  35
#define ESP32_PERIPHERAL_SIGNAL_PWM0_OUT1B                  35
#define ESP32_PERIPHERAL_SIGNAL_PWM0_F2_IN                  36
#define ESP32_PERIPHERAL_SIGNAL_PWM0_OUT2A                  36
#define ESP32_PERIPHERAL_SIGNAL_PWM0_OUT2B                  37
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN0            39
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN0            40
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN0           41
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN0           42
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN1            43
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN1            44
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN1           45
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN1           46
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN2            47
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN2            48
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN2           49
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN2           50
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN3            51
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN3            52
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN3           53
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN3           54
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN4            55
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN4            56
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN4           57
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN4           58
#define ESP32_PERIPHERAL_SIGNAL_HSPICS1_IN                  61
#define ESP32_PERIPHERAL_SIGNAL_HSPICS1_OUT                 61
#define ESP32_PERIPHERAL_SIGNAL_HSPICS2_IN                  62
#define ESP32_PERIPHERAL_SIGNAL_HSPICS2_OUT                 62
#define ESP32_PERIPHERAL_SIGNAL_VSPICLK_IN                  63
#define ESP32_PERIPHERAL_SIGNAL_VSPICLK_OUT_MUX             63
#define ESP32_PERIPHERAL_SIGNAL_VSPIQ_IN                    64
#define ESP32_PERIPHERAL_SIGNAL_VSPIQ_OUT                   64
#define ESP32_PERIPHERAL_SIGNAL_VSPID_IN                    65
#define ESP32_PERIPHERAL_SIGNAL_VSPID_OUT                   65
#define ESP32_PERIPHERAL_SIGNAL_VSPIHD_IN                   66
#define ESP32_PERIPHERAL_SIGNAL_VSPIHD_OUT                  66
#define ESP32_PERIPHERAL_SIGNAL_VSPIWP_IN                   67
#define ESP32_PERIPHERAL_SIGNAL_VSPIWP_OUT                  67
#define ESP32_PERIPHERAL_SIGNAL_VSPICS0_IN                  68
#define ESP32_PERIPHERAL_SIGNAL_VSPICS0_OUT                 68
#define ESP32_PERIPHERAL_SIGNAL_VSPICS1_IN                  69
#define ESP32_PERIPHERAL_SIGNAL_VSPICS1_OUT                 69
#define ESP32_PERIPHERAL_SIGNAL_VSPICS2_IN                  70
#define ESP32_PERIPHERAL_SIGNAL_VSPICS2_OUT                 70
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN5            71
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT0            71
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN5            72
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT1            72
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN5           73
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT2            73
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN5           74
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT3            74
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN6            75
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT4            75
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN6            76
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT5            76
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN6           77
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT6            77
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN6           78
#define ESP32_PERIPHERAL_SIGNAL_LEDC_HS_SIG_OUT7            78
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH0_IN7            79
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT0            79
#define ESP32_PERIPHERAL_SIGNAL_PCNT_SIG_CH1_IN7            80
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT1            80
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH0_IN7           81
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT2            81
#define ESP32_PERIPHERAL_SIGNAL_PCNT_CTRL_CH1_IN7           82
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT3            82
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN0                 83
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT4            83
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN1                 84
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT5            84
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN2                 85
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT6            85
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN3                 86
#define ESP32_PERIPHERAL_SIGNAL_LEDC_LS_SIG_OUT7            86
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN4                 87
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT0                87
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN5                 88
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT1                88
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN6                 89
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT2                89
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_IN7                 90
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT3                90
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT4                91
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT5                92
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT6                93
#define ESP32_PERIPHERAL_SIGNAL_CAN_RX                      94
#define ESP32_PERIPHERAL_SIGNAL_RMT_SIG_OUT7                94
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT1_SCL_IN              95
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT1_SCL_OUT             95
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT1_SDA_IN              96
#define ESP32_PERIPHERAL_SIGNAL_I2CEXT1_SDA_OUT             96
#define ESP32_PERIPHERAL_SIGNAL_HOST_CARD_DETECT_N_1        97
#define ESP32_PERIPHERAL_SIGNAL_HOST_CCMD_OD_PULLUP_EN_N    97
#define ESP32_PERIPHERAL_SIGNAL_HOST_CARD_DETECT_N_2        98
#define ESP32_PERIPHERAL_SIGNAL_HOST_RST_N_1                98
#define ESP32_PERIPHERAL_SIGNAL_HOST_CARD_WRITE_PRT_1       99
#define ESP32_PERIPHERAL_SIGNAL_HOST_RST_N_2                99
#define ESP32_PERIPHERAL_SIGNAL_HOST_CARD_WRITE_PRT_2      100
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD0_OUT               100
#define ESP32_PERIPHERAL_SIGNAL_HOST_CARD_INT_N_1          101
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD1_OUT               101
#define ESP32_PERIPHERAL_SIGNAL_HOST_CARD_INT_N_2          102
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD2_OUT               102
#define ESP32_PERIPHERAL_SIGNAL_PWM1_SYNC0_IN              103
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD3_OUT               103
#define ESP32_PERIPHERAL_SIGNAL_PWM1_SYNC1_IN              104
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD4_OUT               104
#define ESP32_PERIPHERAL_SIGNAL_PWM1_SYNC2_IN              105
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD5_OUT               105
#define ESP32_PERIPHERAL_SIGNAL_PWM1_F0_IN                 106
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD6_OUT               106
#define ESP32_PERIPHERAL_SIGNAL_PWM1_F1_IN                 107
#define ESP32_PERIPHERAL_SIGNAL_GPIO_SD7_OUT               107
#define ESP32_PERIPHERAL_SIGNAL_PWM1_F2_IN                 108
#define ESP32_PERIPHERAL_SIGNAL_PWM1_OUT0A                 108
#define ESP32_PERIPHERAL_SIGNAL_PWM0_CAP0_IN               109
#define ESP32_PERIPHERAL_SIGNAL_PWM1_OUT0B                 109
#define ESP32_PERIPHERAL_SIGNAL_PWM0_CAP1_IN               110
#define ESP32_PERIPHERAL_SIGNAL_PWM1_OUT1A                 110
#define ESP32_PERIPHERAL_SIGNAL_PWM0_CAP2_IN               111
#define ESP32_PERIPHERAL_SIGNAL_PWM1_OUT1B                 111
#define ESP32_PERIPHERAL_SIGNAL_PWM1_CAP0_IN               112
#define ESP32_PERIPHERAL_SIGNAL_PWM1_OUT2A                 112
#define ESP32_PERIPHERAL_SIGNAL_PWM1_CAP1_IN               113
#define ESP32_PERIPHERAL_SIGNAL_PWM1_OUT2B                 113
#define ESP32_PERIPHERAL_SIGNAL_PWM1_CAP2_IN               114
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT1H                 114
#define ESP32_PERIPHERAL_SIGNAL_PWM2_FLTA                  115
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT1L                 115
#define ESP32_PERIPHERAL_SIGNAL_PWM2_FLTB                  116
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT2H                 116
#define ESP32_PERIPHERAL_SIGNAL_PWM2_CAP1_IN               117
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT2L                 117
#define ESP32_PERIPHERAL_SIGNAL_PWM2_CAP2_IN               118
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT3H                 118
#define ESP32_PERIPHERAL_SIGNAL_PWM2_CAP3_IN               119
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT3L                 119
#define ESP32_PERIPHERAL_SIGNAL_PWM3_FLTA                  120
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT4H                 120
#define ESP32_PERIPHERAL_SIGNAL_PWM3_FLTB                  121
#define ESP32_PERIPHERAL_SIGNAL_PWM2_OUT4L                 121
#define ESP32_PERIPHERAL_SIGNAL_PWM3_CAP1_IN               122
#define ESP32_PERIPHERAL_SIGNAL_PWM3_CAP2_IN               123
#define ESP32_PERIPHERAL_SIGNAL_CAN_TX                     123
#define ESP32_PERIPHERAL_SIGNAL_PWM3_CAP3_IN               124
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN0             140
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT0            140
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN1             141
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT1            141
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN2             142
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT2            142
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN3             143
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT3            143
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN4             144
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT4            144
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN5             145
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT5            145
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN6             146
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT6            146
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN7             147
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT7            147
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN8             148
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT8            148
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN9             149
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT9            149
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN10            150
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT10           150
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN11            151
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT11           151
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN12            152
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT12           152
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN13            153
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT13           153
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN14            154
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT14           154
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_DATA_IN15            155
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT15           155
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT16           156
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT17           157
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT18           158
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT19           159
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT20           160
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT21           161
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT22           162
#define ESP32_PERIPHERAL_SIGNAL_I2S0O_DATA_OUT23           163
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_BCK_IN               164
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_BCK_OUT              164
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_WS_IN                165
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_WS_OUT               165
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN0             166
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT0            166
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN1             167
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT1            167
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN2             168
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT2            168
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN3             169
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT3            169
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN4             170
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT4            170
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN5             171
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT5            171
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN6             172
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT6            172
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN7             173
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT7            173
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN8             174
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT8            174
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN9             175
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT9            175
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN10            176
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT10           176
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN11            177
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT11           177
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN12            178
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT12           178
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN13            179
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT13           179
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN14            180
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT14           180
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_DATA_IN15            181
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT15           181
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT16           182
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT17           183
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT18           184
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT19           185
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT20           186
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT21           187
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT22           188
#define ESP32_PERIPHERAL_SIGNAL_I2S1O_DATA_OUT23           189
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_H_SYNC               190
#define ESP32_PERIPHERAL_SIGNAL_PWM_OUT1H                  190
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_V_SYNC               191
#define ESP32_PERIPHERAL_SIGNAL_PWM3_OUT1L                 191
#define ESP32_PERIPHERAL_SIGNAL_I2S0I_H_ENABLE             192
#define ESP32_PERIPHERAL_SIGNAL_PWM3_OUT2H                 192
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_H_SYNC               193
#define ESP32_PERIPHERAL_SIGNAL_PWM3_OUT2L                 193
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_V_SYNC               194
#define ESP32_PERIPHERAL_SIGNAL_PWM3_OUT3H                 194
#define ESP32_PERIPHERAL_SIGNAL_I2S1I_H_ENABLE             195
#define ESP32_PERIPHERAL_SIGNAL_PWM3_OUT3L                 195
#define ESP32_PERIPHERAL_SIGNAL_PWM3_OUT4H                 196
#define ESP32_PERIPHERAL_SIGNAL_PWM3_OUT4L                 197
#define ESP32_PERIPHERAL_SIGNAL_U2RXD_IN                   198
#define ESP32_PERIPHERAL_SIGNAL_U2TXD_OUT                  198
#define ESP32_PERIPHERAL_SIGNAL_U2CTS_IN                   199
#define ESP32_PERIPHERAL_SIGNAL_U2RTS_OUT                  199
#define ESP32_PERIPHERAL_SIGNAL_EMAC_MDC_I                 200
#define ESP32_PERIPHERAL_SIGNAL_EMAC_MDC_O                 200
#define ESP32_PERIPHERAL_SIGNAL_EMAC_MDI_I                 201
#define ESP32_PERIPHERAL_SIGNAL_EMAC_MDO_O                 201
#define ESP32_PERIPHERAL_SIGNAL_EMAC_CRS_I                 202
#define ESP32_PERIPHERAL_SIGNAL_EMAC_CRS_O                 202
#define ESP32_PERIPHERAL_SIGNAL_EMAC_COL_I                 203
#define ESP32_PERIPHERAL_SIGNAL_EMAC_COL_O                 203
#define ESP32_PERIPHERAL_SIGNAL_PCMFSYNC_IN                204
#define ESP32_PERIPHERAL_SIGNAL_BT_AUDIO0_IRQ              204
#define ESP32_PERIPHERAL_SIGNAL_PCMCLK_IN                  205
#define ESP32_PERIPHERAL_SIGNAL_BT_AUDIO1_IRQ              205
#define ESP32_PERIPHERAL_SIGNAL_PCMDIN                     206
#define ESP32_PERIPHERAL_SIGNAL_BT_AUDIO2_IRQ              206
#define ESP32_PERIPHERAL_SIGNAL_BLE_AUDIO0_IRQ             207
#define ESP32_PERIPHERAL_SIGNAL_BLE_AUDIO1_IRQ             208
#define ESP32_PERIPHERAL_SIGNAL_BLE_AUDIO2_IRQ             209
#define ESP32_PERIPHERAL_SIGNAL_PCMFSYNC_OUT               210
#define ESP32_PERIPHERAL_SIGNAL_PCMCLK_OUT                 211
#define ESP32_PERIPHERAL_SIGNAL_PCMDOUT                    212
#define ESP32_PERIPHERAL_SIGNAL_BLE_AUDIO_SYNC0_P          213
#define ESP32_PERIPHERAL_SIGNAL_BLE_AUDIO_SYNC1_P          214
#define ESP32_PERIPHERAL_SIGNAL_BLE_AUDIO_SYNC2_P          215
#define ESP32_PERIPHERAL_SIGNAL_SIG_IN_FUNC224             224
#define ESP32_PERIPHERAL_SIGNAL_SIG_IN_FUNC225             225
#define ESP32_PERIPHERAL_SIGNAL_SIG_IN_FUNC226             226
#define ESP32_PERIPHERAL_SIGNAL_SIG_IN_FUNC227             227
#define ESP32_PERIPHERAL_SIGNAL_SIG_IN_FUNC228             228

/**
 * Input Output Multiplexing (IO_MUX).
 */
struct esp32_io_mux_t {
    uint32_t CTRL;
    uint32_t PIN[40];
};

#define ESP32_IO_MUX_PIN_MCU_OE                         BIT(0)
#define ESP32_IO_MUX_PIN_SLP_SEL                        BIT(1)
#define ESP32_IO_MUX_PIN_SLP_WPD                        BIT(2)
#define ESP32_IO_MUX_PIN_SLP_WPU                        BIT(3)
#define ESP32_IO_MUX_PIN_MCU_IE                         BIT(4)
#define ESP32_IO_MUX_PIN_MCU_DRV_POS                       (5)
#define ESP32_IO_MUX_PIN_FUNC_WPD                       BIT(7)
#define ESP32_IO_MUX_PIN_FUNC_WPU                       BIT(8)
#define ESP32_IO_MUX_PIN_FUNC_IE                        BIT(9)
#define ESP32_IO_MUX_PIN_FUNC_DRV_POS                     (10)
#define ESP32_IO_MUX_PIN_FUNC_DRV_MASK (0x3 << ESP32_IO_MUX_PIN_FUNC_DRV_POS)
#define ESP32_IO_MUX_PIN_FUNC_DRV(value) BITFIELD_SET(ESP32_IO_MUX_PIN_FUNC_DRV, value)
#define ESP32_IO_MUX_PIN_MCU_SEL_POS                      (12)
#define ESP32_IO_MUX_PIN_MCU_SEL_MASK (0x7 << ESP32_IO_MUX_PIN_MCU_SEL_POS)
#define ESP32_IO_MUX_PIN_MCU_SEL(value) BITFIELD_SET(ESP32_IO_MUX_PIN_MCU_SEL, value)
#define ESP32_IO_MUX_PIN_MCU_SEL_FUNC2 ESP32_IO_MUX_PIN_MCU_SEL(1)
#define ESP32_IO_MUX_PIN_MCU_SEL_GPIO ESP32_IO_MUX_PIN_MCU_SEL(2)

/**
 * Timer groups. Each timer group has two hardware timers and ont
 * watchdog.
 */
struct esp32_timg_t {
    struct {
        uint32_t CONFIG;
        uint32_t LO;
        uint32_t HI;
        uint32_t UPDATE;
        uint32_t ALARMLO;
        uint32_t ALARMHI;
        uint32_t LOADLO;
        uint32_t LOADHI;
        uint32_t LOAD;
    } TIMER[2];
    struct {
        uint32_t CONFIG0;
        uint32_t CONFIG1;
        uint32_t CONFIG2;
        uint32_t CONFIG3;
        uint32_t CONFIG4;
        uint32_t CONFIG5;
        uint32_t FEED;
        uint32_t WPROTECT;
    } WDT;
    uint32_t RESERVED0[12];
    struct {
        uint32_t ENA;
        uint32_t RAW;
        uint32_t ST;
        uint32_t CLR;
    } INT;
};

/* Timer configuration. */
#define ESP32_TIMG_TIMER_CONFIG_ALARM_EN             BIT(10)
#define ESP32_TIMG_TIMER_CONFIG_LEVEL_INT_EN         BIT(11)
#define ESP32_TIMG_TIMER_CONFIG_EDGE_INT_EN          BIT(12)
#define ESP32_TIMG_TIMER_CONFIG_DIVIDER_POS             (13)
#define ESP32_TIMG_TIMER_CONFIG_DIVIDER_MASK            \
    (0xffff << ESP32_TIMG_TIMER_CONFIG_DIVIDER_POS)
#define ESP32_TIMG_TIMER_CONFIG_DIVIDER(value)             \
    BITFIELD_SET(ESP32_TIMG_TIMER_CONFIG_DIVIDER, value)
#define ESP32_TIMG_TIMER_CONFIG_AUTORELOAD           BIT(29)
#define ESP32_TIMG_TIMER_CONFIG_INCREASE             BIT(30)
#define ESP32_TIMG_TIMER_CONFIG_EN                   BIT(31)

/**
 * CAN controller (SJA1000).
 */
struct esp32_can_t {
    uint32_t MODE;
    uint32_t COMMAND;
    uint32_t STATUS;
    uint32_t INT;
    uint32_t INTE;
    uint32_t RESERVED0;
    uint32_t BTIM0;
    uint32_t BTIM1;
    uint32_t OCTRL;
    uint32_t RESERVED1[2];
    uint32_t ALC;
    uint32_t ECC;
    uint32_t EWL;
    uint32_t RXERR;
    uint32_t TXERR;
    union {
        struct {
            uint32_t CODE[4];
            uint32_t MASK[4];
            uint32_t RESERVED0[5];
        } ACC;
        struct {
            uint32_t FRAME_INFO;
            uint32_t ID_DATA[12];
        } TX_RX;
    } U;
    uint32_t RMC;
    uint32_t RBSA;
    uint32_t CDIV;
    uint32_t IRAM[2];
};

/* Mode register. */
#define ESP32_CAN_MODE_RESET                            BIT(0)
#define ESP32_CAN_MODE_LISTEN_ONLY                      BIT(1)
#define ESP32_CAN_MODE_SELF_TEST                        BIT(2)
#define ESP32_CAN_MODE_ACCEPTANCE_FILTER                BIT(3)
#define ESP32_CAN_MODE_SLEEP_MODE                       BIT(4)

/* Command register. */
#define ESP32_CAN_COMMAND_TX                            BIT(0)
#define ESP32_CAN_COMMAND_TX_ABORT                      BIT(1)
#define ESP32_CAN_COMMAND_RELEASE_RECV_BUF              BIT(2)
#define ESP32_CAN_COMMAND_CLEAR_DATA_OVERRUN            BIT(3)
#define ESP32_CAN_COMMAND_SELF_RECEPTION_REQUEST        BIT(4)

/* Interrupt status register. */
#define ESP32_CAN_INT_RX                                BIT(0)
#define ESP32_CAN_INT_TX                                BIT(1)
#define ESP32_CAN_INT_ERR                               BIT(2)
#define ESP32_CAN_INT_DATA_OVERRUN                      BIT(3)
#define ESP32_CAN_INT_WAKEUP                            BIT(4)
#define ESP32_CAN_INT_ERR_PASSIVE                       BIT(5)
#define ESP32_CAN_INT_ARB_LOST                          BIT(6)
#define ESP32_CAN_INT_BUS_ERR                           BIT(7)

/* Bus timing register 0. */
#define ESP32_CAN_BTIM0_BRP_POS                            (0)
#define ESP32_CAN_BTIM0_BRP_MASK          (0x3f << ESP32_CAN_BTIM0_BRP_POS)
#define ESP32_CAN_BTIM0_BRP(value) BITFIELD_SET(ESP32_CAN_BTIM0_BRP, value)
#define ESP32_CAN_BTIM0_SJW_POS                            (6)
#define ESP32_CAN_BTIM0_SJW_MASK           (0x3 << ESP32_CAN_BTIM0_SJW_POS)
#define ESP32_CAN_BTIM0_SJW(value) BITFIELD_SET(ESP32_CAN_BTIM0_SJW, value)

/* Bus timing register 1. */
#define ESP32_CAN_BTIM1_TSEG1_POS                          (0)
#define ESP32_CAN_BTIM1_TSEG1_MASK (0xf << ESP32_CAN_BTIM1_TSEG1_POS)
#define ESP32_CAN_BTIM1_TSEG1(value) BITFIELD_SET(ESP32_CAN_BTIM1_TSEG1, value)
#define ESP32_CAN_BTIM1_TSEG2_POS                          (4)
#define ESP32_CAN_BTIM1_TSEG2_MASK           (0x7 << ESP32_CAN_BTIM1_TSEG2_POS)
#define ESP32_CAN_BTIM1_TSEG2(value) BITFIELD_SET(ESP32_CAN_BTIM1_TSEG2, value)
#define ESP32_CAN_BTIM1_SAM                             BIT(7)

/* Output control register. */
#define ESP32_CAN_OCTRL_MODE_NORMAL                     BIT(1)

/* Frame info register. */
#define ESP32_CAN_FRAME_INFO_DLC_POS                       (0)
#define ESP32_CAN_FRAME_INFO_DLC_MASK           \
    (0xf << ESP32_CAN_FRAME_INFO_DLC_POS)
#define ESP32_CAN_FRAME_INFO_DLC(value)                 \
    BITFIELD_SET(ESP32_CAN_FRAME_INFO_DLC, value)
#define ESP32_CAN_FRAME_INFO_DLC_GET(reg)       \
    BITFIELD_GET(ESP32_CAN_FRAME_INFO_DLC, reg)
#define ESP32_CAN_FRAME_INFO_RTR                        BIT(6)
#define ESP32_CAN_FRAME_INFO_FF                         BIT(7)

/* Clock divisor register. */
#define ESP32_CAN_CDIV_PELICAN                          BIT(7)

/**
 * Serial Peripheral Interface.
 */
struct esp32_spi_t {
    uint32_t COMMAND;
    uint32_t ADDR;
    uint32_t CTRL;
    uint32_t CTRL1;
    uint32_t RD_STATUS;
    uint32_t CTRL2;
    uint32_t CLOCK;
    uint32_t USER;
    uint32_t USER1;
    uint32_t USER2;
    uint32_t MOSI_DLEN;
    uint32_t MISO_DLEN;
    uint32_t SLV_WR_STATUS;
    uint32_t PIN;
    uint32_t SLAVE;
    uint32_t SLAVE1;
    uint32_t SLAVE2;
    uint32_t SLAVE3;
    uint32_t SLV_WRBUF_DLEN;
    uint32_t SLV_RDBUF_DLEN;
    uint32_t CACHE_FCTRL;
    uint32_t CACHE_SCTRL;
    uint32_t SRAM_CMD;
    uint32_t SRAM_DRD_CMD;
    uint32_t SRAM_DWR_CMD;
    uint32_t SLV_RD_BIT;
    uint32_t RESERVED0[6];
    uint32_t DATA[16];
    uint32_t TX_CRC;
    uint32_t RESERVED1[11];
    uint32_t EXT0;
    uint32_t EXT1;
    uint32_t EXT2;
    uint32_t EXT3;
    struct {
        uint32_t CONFIG;
        uint32_t OUT_LINK;
        uint32_t IN_LINK;
        uint32_t STATUS;
        uint32_t INT_ENA;
        uint32_t INT_RAW;
        uint32_t INT_ST;
        uint32_t INT_CLR;
        uint32_t IN_ERR_EOF_DES_ADDR;
        uint32_t IN_SUC_EOF_DES_ADDR;
        uint32_t INLINK_DSCR;
        uint32_t INLINK_DSCR_BF0;
        uint32_t INLINK_DSCR_BF1;
        uint32_t OUT_EOF_BFR_DES_ADDR;
        uint32_t OUT_EOF_DES_ADDR;
        uint32_t OUTLINK_DSCR;
        uint32_t OUTLINK_DSCR_BF0;
        uint32_t OUTLINK_DSCR_BF1;
        uint32_t RX_STATUS;
        uint32_t TX_STATUS;
    } DMA;
    uint32_t RESERVED2[170];
    uint32_t DATE;
};

#define ESP32_SPI_COMMAND_USR                          BIT(18)

#define ESP32_SPI_SLAVE_TRANS_DONE                      BIT(4)
#define ESP32_SPI_SLAVE_SLAVE_MODE                     BIT(30)
#define ESP32_SPI_SLAVE_INT_EN_POS                         (5)
#define ESP32_SPI_SLAVE_INT_EN_MASK (0x1f << ESP32_SPI_SLAVE_INT_EN_POS)
#define ESP32_SPI_SLAVE_INT_EN(value) BITFIELD_SET(ESP32_SPI_SLAVE_INT_EN, value)
#define ESP32_SPI_SLAVE_INT_EN_TRANS_DONE ESP32_SPI_SLAVE_INT_EN(0x10)

#define ESP32_SPI_PIN_CK_IDLE_EDGE                     BIT(29)

#define ESP32_SPI_USER_DOUTDIN                          BIT(0)
#define ESP32_SPI_USER_CK_OUT_EDGE                      BIT(7)
#define ESP32_SPI_USER_USR_MOSI                        BIT(27)
#define ESP32_SPI_USER_USR_MISO                        BIT(28)

struct esp32_rtc_control_t {
};

struct esp32_rtc_io_t {
    uint32_t GPIO_OUT;
    uint32_t GPIO_OUT_W1TS;
    uint32_t GPIO_OUT_W1TC;
    uint32_t GPIO_ENABLE;
    uint32_t GPIO_ENABLE_W1TS;
    uint32_t GPIO_ENABLE_W1TC;
    uint32_t GPIO_STATUS;
    uint32_t GPIO_STATUS_W1TS;
    uint32_t GPIO_STATUS_W1TC;
    uint32_t GPIO_IN;
    uint32_t GPIO_PIN[18];
    uint32_t RESERVED0;
    uint32_t DIG_PAD_HOLD;
    uint32_t HALL_SENS;
    uint32_t SENSOR_PADS;
    uint32_t ADC_PADS;
    uint32_t DAC1_PAD;
    uint32_t DAC2_PAD;
    uint32_t XTAL_32K_PAD;
    uint32_t TOUCH_CFG;
    uint32_t TOUCH_PADS[10];
    uint32_t EXT_WAKEUP0;
    uint32_t XTL_EXT_CTR;
    uint32_t SAR_I2C_IO;
};

/* Configure the sensor (ADC) pads. */
#define ESP32_RTC_IO_SENSOR_PADS_1_HOLD                    BIT(31)
#define ESP32_RTC_IO_SENSOR_PADS_2_HOLD                    BIT(30)
#define ESP32_RTC_IO_SENSOR_PADS_3_HOLD                    BIT(29)
#define ESP32_RTC_IO_SENSOR_PADS_4_HOLD                    BIT(28)
#define ESP32_RTC_IO_SENSOR_PADS_1_MUX_SEL                 BIT(27)
#define ESP32_RTC_IO_SENSOR_PADS_2_MUX_SEL                 BIT(26)
#define ESP32_RTC_IO_SENSOR_PADS_3_MUX_SEL                 BIT(25)
#define ESP32_RTC_IO_SENSOR_PADS_4_MUX_SEL                 BIT(24)
#define ESP32_RTC_IO_SENSOR_PADS_1_FUN_SEL_POS                (22)
#define ESP32_RTC_IO_SENSOR_PADS_1_FUN_SEL_MASK (0x3 << ESP32_RTC_IO_SENSOR_PADS_1_FUN_SEL_POS)
#define ESP32_RTC_IO_SENSOR_PADS_1_FUN_SEL(value) BITFIELD_SET(ESP32_RTC_IO_SENSOR_PADS_1_FUN_SEL, value)
#define ESP32_RTC_IO_SENSOR_PADS_1_SLP_SEL                 BIT(21)
#define ESP32_RTC_IO_SENSOR_PADS_1_SLP_IE                  BIT(20)
#define ESP32_RTC_IO_SENSOR_PADS_1_FUN_IE                  BIT(19)
#define ESP32_RTC_IO_SENSOR_PADS_2_FUN_SEL_POS                (17)
#define ESP32_RTC_IO_SENSOR_PADS_2_FUN_SEL_MASK (0x3 << ESP32_RTC_IO_SENSOR_PADS_2_FUN_SEL_POS)
#define ESP32_RTC_IO_SENSOR_PADS_2_FUN_SEL(value) BITFIELD_SET(ESP32_RTC_IO_SENSOR_PADS_2_FUN_SEL, value)
#define ESP32_RTC_IO_SENSOR_PADS_2_SLP_SEL                 BIT(16)
#define ESP32_RTC_IO_SENSOR_PADS_2_SLP_IE                  BIT(15)
#define ESP32_RTC_IO_SENSOR_PADS_2_FUN_IE                  BIT(14)
#define ESP32_RTC_IO_SENSOR_PADS_3_FUN_SEL_POS                (12)
#define ESP32_RTC_IO_SENSOR_PADS_3_FUN_SEL_MASK (0x3 << ESP32_RTC_IO_SENSOR_PADS_3_FUN_SEL_POS)
#define ESP32_RTC_IO_SENSOR_PADS_3_FUN_SEL(value) BITFIELD_SET(ESP32_RTC_IO_SENSOR_PADS_3_FUN_SEL, value)
#define ESP32_RTC_IO_SENSOR_PADS_3_SLP_SEL                 BIT(11)
#define ESP32_RTC_IO_SENSOR_PADS_3_SLP_IE                  BIT(10)
#define ESP32_RTC_IO_SENSOR_PADS_3_FUN_IE                  BIT(9)
#define ESP32_RTC_IO_SENSOR_PADS_4_FUN_SEL_POS                (7)
#define ESP32_RTC_IO_SENSOR_PADS_4_FUN_SEL_MASK (0x3 << ESP32_RTC_IO_SENSOR_PADS_4_FUN_SEL_POS)
#define ESP32_RTC_IO_SENSOR_PADS_4_FUN_SEL(value) BITFIELD_SET(ESP32_RTC_IO_SENSOR_PADS_4_FUN_SEL, value)
#define ESP32_RTC_IO_SENSOR_PADS_4_SLP_SEL                 BIT(6)
#define ESP32_RTC_IO_SENSOR_PADS_4_SLP_IE                  BIT(5)
#define ESP32_RTC_IO_SENSOR_PADS_4_FUN_IE                  BIT(4)

/* Configure the adc (ADC) pads. */
#define ESP32_RTC_IO_ADC_PAD_1_HOLD                    BIT(31)
#define ESP32_RTC_IO_ADC_PAD_2_HOLD                    BIT(30)
#define ESP32_RTC_IO_ADC_PAD_1_MUX_SEL                 BIT(29)
#define ESP32_RTC_IO_ADC_PAD_2_MUX_SEL                 BIT(28)
#define ESP32_RTC_IO_ADC_PAD_1_FUN_SEL_POS                (26)
#define ESP32_RTC_IO_ADC_PAD_1_FUN_SEL_MASK (0x3 << ESP32_RTC_IO_ADC_PAD_1_FUN_SEL_POS)
#define ESP32_RTC_IO_ADC_PAD_1_FUN_SEL(value) BITFIELD_SET(ESP32_RTC_IO_ADC_PAD_1_FUN_SEL, value)
#define ESP32_RTC_IO_ADC_PAD_1_SLP_SEL                 BIT(25)
#define ESP32_RTC_IO_ADC_PAD_1_SLP_IE                  BIT(24)
#define ESP32_RTC_IO_ADC_PAD_1_FUN_IE                  BIT(23)
#define ESP32_RTC_IO_ADC_PAD_2_FUN_SEL_POS                (22)
#define ESP32_RTC_IO_ADC_PAD_2_FUN_SEL_MASK (0x3 << ESP32_RTC_IO_ADC_PAD_2_FUN_SEL_POS)
#define ESP32_RTC_IO_ADC_PAD_2_FUN_SEL(value) BITFIELD_SET(ESP32_RTC_IO_ADC_PAD_2_FUN_SEL, value)
#define ESP32_RTC_IO_ADC_PAD_2_SLP_SEL                 BIT(20)
#define ESP32_RTC_IO_ADC_PAD_2_SLP_IE                  BIT(19)
#define ESP32_RTC_IO_ADC_PAD_2_FUN_IE                  BIT(18)

/* Configure the touch (ADC) pads. */
#define ESP32_RTC_IO_TOUCH_PADS_HOLD                   BIT(31)
#define ESP32_RTC_IO_TOUCH_PADS_DRV_POS                   (29)
#define ESP32_RTC_IO_TOUCH_PADS_DRV_MASK (0x3 << ESP32_RTC_IO_TOUCH_PADS_DRV_POS)
#define ESP32_RTC_IO_TOUCH_PADS_DRV(value) BITFIELD_SET(ESP32_RTC_IO_TOUCH_PADS_DRV, value)
#define ESP32_RTC_IO_TOUCH_PADS_RDE                    BIT(28)
#define ESP32_RTC_IO_TOUCH_PADS_RUE                    BIT(27)
#define ESP32_RTC_IO_TOUCH_PADS_MUX_SEL                BIT(19)
#define ESP32_RTC_IO_TOUCH_PADS_FUN_SEL_POS               (17)
#define ESP32_RTC_IO_TOUCH_PADS_FUN_SEL_MASK (0x3 << ESP32_RTC_IO_TOUCH_PADS_FUN_SEL_POS)
#define ESP32_RTC_IO_TOUCH_PADS_FUN_SEL(value) BITFIELD_SET(ESP32_RTC_IO_TOUCH_PADS_FUN_SEL, value)
#define ESP32_RTC_IO_TOUCH_PADS_FUN_IE                 BIT(13)
#define ESP32_RTC_IO_TOUCH_PADS_TO_GPIO                BIT(12)

struct esp32_sar_adc_t {
    uint32_t READ_CTRL;
    uint32_t READ_STATUS1;
    uint32_t MEAS_WAIT1;
    uint32_t MEAS_WAIT2;
    uint32_t MEAS_CTRL;
    uint32_t READ_STATUS2;
    uint32_t ULP_CP_SLEEP_CYC[5];
    uint32_t START_FORCE;
    uint32_t MEM_WR_CTRL;
    uint32_t ATTEN1;
    uint32_t ATTEN2;
    uint32_t SLAVE_ADDR[4];
    uint32_t TSENS_CTRL;
    uint32_t I2C_CTRL;
    uint32_t MEAS_1;
    uint32_t TOUCH_CTRL1;
    uint32_t TOUCH_THRES[5];
    uint32_t TOUCH_OUT[5];
    uint32_t TOUCH_CTRL2;
    uint32_t RESERVED0;
    uint32_t TOUCH_ENABLE;
    uint32_t READ_CTRL2;
    uint32_t MEAS_2;
    uint32_t DAC_CTRL1;
    uint32_t DAC_CTRL2;
    uint32_t MEAS_CTRL2;
};

/* ADC convertion. */
#define ESP32_SAR_ADC_MEAS_EN_PAD_FORCE                BIT(31)
#define ESP32_SAR_ADC_MEAS_EN_PAD_POS                     (19)
#define ESP32_SAR_ADC_MEAS_EN_PAD_MASK (0xfff << ESP32_SAR_ADC_MEAS_EN_PAD_POS)
#define ESP32_SAR_ADC_MEAS_EN_PAD(value) BITFIELD_SET(ESP32_SAR_ADC_MEAS_EN_PAD, value)
#define ESP32_SAR_ADC_MEAS_START_FORCE                 BIT(18)
#define ESP32_SAR_ADC_MEAS_START                       BIT(17)
#define ESP32_SAR_ADC_MEAS_DONE                        BIT(16)
#define ESP32_SAR_ADC_MEAS_DATA_POS                        (0)
#define ESP32_SAR_ADC_MEAS_DATA_MASK                    0xffff
#define ESP32_SAR_ADC_MEAS_DATA_GET(reg) BITFIELD_GET(ESP32_SAR_ADC_MEAS_DATA, reg)

struct esp32_rnd_t {
    uint32_t VALUE;
};

/**
 * Devices.
 */
#define ESP32_DPORT_REGISTER   ((volatile struct esp32_dport_t    *)0x3ff00000)
#define ESP32_AES_ACCELERATOR  ((volatile struct esp32__t         *)0x3ff01000)
#define ESP32_RSA_ACCELERATOR  ((volatile struct esp32__t         *)0x3ff02000)
#define ESP32_SHA_ACCELERATOR  ((volatile struct esp32__t         *)0x3ff03000)
#define ESP32_SECURE_BOOT      ((volatile struct esp32__t         *)0x3ff04000)
#define ESP32_CACHE_MMU_TABLE  ((volatile struct esp32__t         *)0x3ff10000)
#define ESP32_PID_CONTROLLER   ((volatile struct esp32__t         *)0x3ff1f000)
#define ESP32_UART0            ((volatile struct esp32_uart_t     *)0x3ff40000)
#define ESP32_SPI1             ((volatile struct esp32_spi_t      *)0x3ff42000)
#define ESP32_SPI0             ((volatile struct esp32_spi_t      *)0x3ff43000)
#define ESP32_GPIO             ((volatile struct esp32_gpio_t     *)0x3ff44000)
#define ESP32_RTC_CTRL         ((volatile struct esp32_rtc_ctrl_t *)0x3ff48000)
#define ESP32_RTC_IO           ((volatile struct esp32_rtc_io_t   *)0x3ff48400)
#define ESP32_SAR_ADC          ((volatile struct esp32_sar_adc_t  *)0x3ff48800)
#define ESP32_IO_MUX           ((volatile struct esp32_io_mux_t   *)0x3ff49000)
#define ESP32_SDIO_SLAVE_0     ((volatile struct esp32__t         *)0x3ff4b000)
#define ESP32_UDMA1            ((volatile struct esp32__t         *)0x3ff4c000)
#define ESP32_I2S0             ((volatile struct esp32__t         *)0x3ff4f000)
#define ESP32_UART1            ((volatile struct esp32_uart_t     *)0x3ff50000)
#define ESP32_I2C0             ((volatile struct esp32_i2c_t      *)0x3ff53000)
#define ESP32_UDMA0            ((volatile struct esp32__t         *)0x3ff54000)
#define ESP32_SDIO_SLAVE_1     ((volatile struct esp32__t         *)0x3ff55000)
#define ESP32_RMT              ((volatile struct esp32__t         *)0x3ff56000)
#define ESP32_PCNT             ((volatile struct esp32__t         *)0x3ff57000)
#define ESP32_SDIO_SLAVE_2     ((volatile struct esp32__t         *)0x3ff58000)
#define ESP32_LED_PWM          ((volatile struct esp32__t         *)0x3ff59000)
#define ESP32_EFUSE_CONTROLLER ((volatile struct esp32__t         *)0x3ff5a000)
#define ESP32_FLASH_ENCRYPTION ((volatile struct esp32__t         *)0x3ff5b000)
#define ESP32_PWM0             ((volatile struct esp32__t         *)0x3ff5e000)
#define ESP32_TIMG0            ((volatile struct esp32_timg_t     *)0x3ff5f000)
#define ESP32_TIMG1            ((volatile struct esp32_timg_t     *)0x3ff60000)
#define ESP32_SPI2             ((volatile struct esp32_spi_t      *)0x3ff64000)
#define ESP32_SPI3             ((volatile struct esp32_spi_t      *)0x3ff65000)
#define ESP32_SYSCON           ((volatile struct esp32__t         *)0x3ff66000)
#define ESP32_I2C1             ((volatile struct esp32_i2c_t      *)0x3ff67000)
#define ESP32_SDMMC            ((volatile struct esp32__t         *)0x3ff68000)
#define ESP32_EMAC             ((volatile struct esp32__t         *)0x3ff69000)
#define ESP32_CAN              ((volatile struct esp32_can_t      *)0x3ff6b000)
#define ESP32_PWM1             ((volatile struct esp32__t         *)0x3ff6c000)
#define ESP32_I2S1             ((volatile struct esp32__t         *)0x3ff6d000)
#define ESP32_UART2            ((volatile struct esp32_uart_t     *)0x3ff6e000)
#define ESP32_PWM2             ((volatile struct esp32__t         *)0x3ff6f000)
#define ESP32_PWM3             ((volatile struct esp32__t         *)0x3ff70000)
#define ESP32_RNG              ((volatile struct esp32__t         *)0x3ff75000)
#define ESP32_RND              ((volatile struct esp32_rnd_t      *)0x60035144)

#endif
