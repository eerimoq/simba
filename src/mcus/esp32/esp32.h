/**
 * @file mcus/esp32/esp32.h
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
#define ESP32_CPU_INTR_PERIPHERAL_14_PRIO_I          14
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
 * SPI0        ESP32_CPU_INTR_PERIPHERAL_14_PRIO_1
 * SPI1        ESP32_CPU_INTR_PERIPHERAL_14_PRIO_1
 * SPI2        ESP32_CPU_INTR_PERIPHERAL_14_PRIO_1
 */

#define ESP32_CPU_INTR_SYS_TICK_NUM  ESP32_CPU_INTR_PERIPHERAL_12_PRIO_1
#define ESP32_CPU_INTR_UART_NUM      ESP32_CPU_INTR_PERIPHERAL_13_PRIO_1
#define ESP32_CPU_INTR_SPI_NUM       ESP32_CPU_INTR_PERIPHERAL_14_PRIO_1

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
    uint32_t DPORT_PWM3_CLK_EN;
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
    uint32_t reserved0[11];
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
#define ESP32_UART_STATUS_DTRN                     BIT(29)
#define ESP32_UART_STATUS_RTSN                     BIT(30)
#define ESP32_UART_STATUS_TXD                      BIT(31)

/* UART CONF0. */
#define ESP32_UART_CONF0_PARITY                     BIT(0)
#define ESP32_UART_CONF0_PARITY_EN                  BIT(1)
#define ESP32_UART_CONF0_BIT_NUM_POS                   (2)
#define ESP32_UART_CONF0_BIT_NUM_MASK           \
    (0x3 << ESP32_UART_CONF0_BIT_NUM_POS)
#define ESP32_UART_CONF0_STOP_BIT_NUM_POS              (4)
#define ESP32_UART_CONF0_STOP_BIT_NUM_MASK      \
    (0x3 << ESP32_UART_CONF0_STOP_BIT_NUM_POS)
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

/* UART_CONF1. */
#define ESP32_UART_CONF1_RX_TOUT_EN                BIT(31)
#define ESP32_UART_CONF1_RX_TOUT_THRHD             [30:24]
#define ESP32_UART_CONF1_RX_FLOW_EN                   [23]
#define ESP32_UART_CONF1_RX_FLOW_THRHD             [22:16]
#define ESP32_UART_CONF1_TXFIFO_EMPTY_THRHD         [14:8]
#define ESP32_UART_CONF1_RXFIFO_FULL_THRHD           [6:0]

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
    uint32_t BT_SELECT;
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
    uint32_t RESERVED0;
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

/**
 * Input Output Multiplexing.
 */
struct esp32_io_mux_t {
    uint32_t CTRL;
    uint32_t PIN[40];
};

#define ESP32_IO_MUX_PIN_FUNC_IE                        BIT(9)
#define ESP32_IO_MUX_PIN_MCU_SEL_GPIO                (2 << 12)

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
    uint32_t RESERVED0[13];
    struct {
        uint32_t RAW;
        uint32_t ST;
        uint32_t ENA;
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
 * Devices.
 */
#define ESP32_DPORT_REGISTER   ((volatile struct esp32_dport_t  *)0x3ff00000)
#define ESP32_AES_ACCELERATOR  ((volatile struct esp32__t       *)0x3ff01000)
#define ESP32_RSA_ACCELERATOR  ((volatile struct esp32__t       *)0x3ff02000)
#define ESP32_SHA_ACCELERATOR  ((volatile struct esp32__t       *)0x3ff03000)
#define ESP32_SECURE_BOOT      ((volatile struct esp32__t       *)0x3ff04000)
#define ESP32_CACHE_MMU_TABLE  ((volatile struct esp32__t       *)0x3ff10000)
#define ESP32_PID_CONTROLLER   ((volatile struct esp32__t       *)0x3ff1f000)
#define ESP32_UART0            ((volatile struct esp32_uart_t   *)0x3ff40000)
#define ESP32_SPI1             ((volatile struct esp32_spi_t    *)0x3ff42000)
#define ESP32_SPI0             ((volatile struct esp32_spi_t    *)0x3ff43000)
#define ESP32_GPIO             ((volatile struct esp32_gpio_t   *)0x3ff44000)
#define ESP32_RTC              ((volatile struct esp32__t       *)0x3ff48000)
#define ESP32_IO_MUX           ((volatile struct esp32_io_mux_t *)0x3ff49000)
#define ESP32_SDIO_SLAVE_0     ((volatile struct esp32__t       *)0x3ff4b000)
#define ESP32_UDMA1            ((volatile struct esp32__t       *)0x3ff4c000)
#define ESP32_I2S0             ((volatile struct esp32__t       *)0x3ff4f000)
#define ESP32_UART1            ((volatile struct esp32_uart_t   *)0x3ff50000)
#define ESP32_I2C0             ((volatile struct esp32_i2c_t    *)0x3ff53000)
#define ESP32_UDMA0            ((volatile struct esp32__t       *)0x3ff54000)
#define ESP32_SDIO_SLAVE_1     ((volatile struct esp32__t       *)0x3ff55000)
#define ESP32_RMT              ((volatile struct esp32__t       *)0x3ff56000)
#define ESP32_PCNT             ((volatile struct esp32__t       *)0x3ff57000)
#define ESP32_SDIO_SLAVE_2     ((volatile struct esp32__t       *)0x3ff58000)
#define ESP32_LED_PWM          ((volatile struct esp32__t       *)0x3ff59000)
#define ESP32_EFUSE_CONTROLLER ((volatile struct esp32__t       *)0x3ff5a000)
#define ESP32_FLASH_ENCRYPTION ((volatile struct esp32__t       *)0x3ff5b000)
#define ESP32_PWM0             ((volatile struct esp32__t       *)0x3ff5e000)
#define ESP32_TIMG0            ((volatile struct esp32_timg_t   *)0x3ff5f000)
#define ESP32_TIMG1            ((volatile struct esp32_timg_t   *)0x3ff60000)
#define ESP32_SPI2             ((volatile struct esp32_spi_t    *)0x3ff64000)
#define ESP32_SPI3             ((volatile struct esp32_spi_t    *)0x3ff65000)
#define ESP32_SYSCON           ((volatile struct esp32__t       *)0x3ff66000)
#define ESP32_I2C1             ((volatile struct esp32_i2c_t    *)0x3ff67000)
#define ESP32_SDMMC            ((volatile struct esp32__t       *)0x3ff68000)
#define ESP32_EMAC             ((volatile struct esp32__t       *)0x3ff69000)
#define ESP32_PWM1             ((volatile struct esp32__t       *)0x3ff6c000)
#define ESP32_I2S1             ((volatile struct esp32__t       *)0x3ff6d000)
#define ESP32_UART2            ((volatile struct esp32_uart_t   *)0x3ff6e000)
#define ESP32_PWM2             ((volatile struct esp32__t       *)0x3ff6f000)
#define ESP32_PWM3             ((volatile struct esp32__t       *)0x3ff70000)
#define ESP32_RNG              ((volatile struct esp32__t       *)0x3ff75000)

#endif
