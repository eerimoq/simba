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

#ifndef __CONFIG_DEFAULT_H__
#define __CONFIG_DEFAULT_H__

/* All ports must implement uart and pin drivers. */
#define PORT_HAS_I2C_SOFT
#define PORT_HAS_OWI
#define PORT_HAS_PIN
#define PORT_HAS_UART
#define PORT_HAS_XBEE

/**
 * Used to include driver header files and the c-file source.
 */
#if defined(FAMILY_LINUX)
#    define PORT_HAS_ADC
#    define PORT_HAS_CAN
#    define PORT_HAS_DAC
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_EXTI
#    define PORT_HAS_FLASH
#    define PORT_HAS_I2C
#    define PORT_HAS_PWM
#    define PORT_HAS_PWM_SOFT
#    define PORT_HAS_RANDOM
#    define PORT_HAS_SD
#    define PORT_HAS_SPI
#endif

#if defined(FAMILY_AVR)
#    define PORT_HAS_ADC
#    define PORT_HAS_EXTI
#    define PORT_HAS_I2C
#    define PORT_HAS_MCP2515
#    define PORT_HAS_NRF24L01
#    define PORT_HAS_PWM
#    define PORT_HAS_PWM_SOFT
#    define PORT_HAS_SD
#    define PORT_HAS_SPI
#    define PORT_HAS_UART_SOFT
#    if defined(MCU_ATMEGA32U4)
#        define PORT_HAS_USB
#        define PORT_HAS_USB_DEVICE
#    endif
#    define PORT_HAS_WATCHDOG
#endif

#if defined(FAMILY_SAM)
#    define PORT_HAS_ADC
#    define PORT_HAS_CAN
#    define PORT_HAS_CHIPID
#    define PORT_HAS_DAC
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_EXTI
#    define PORT_HAS_FLASH
#    define PORT_HAS_I2C
#    define PORT_HAS_MCP2515
#    define PORT_HAS_RANDOM
#    define PORT_HAS_SD
#    define PORT_HAS_SPI
#    define PORT_HAS_USB
#    define PORT_HAS_USB_HOST
#endif

#if defined(FAMILY_ESP)
#    define PORT_HAS_ADC
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_ESP_WIFI
#    define PORT_HAS_EXTI
#    define PORT_HAS_FLASH
#    define PORT_HAS_LED_7SEG_HT16K33
#    define PORT_HAS_PWM_SOFT
#    define PORT_HAS_RANDOM
#    define PORT_HAS_SPI
#    define PORT_HAS_UART_SOFT
#endif

#if defined(FAMILY_ESP32)
#    define PORT_HAS_ADC
#    define PORT_HAS_CAN
#    define PORT_HAS_DAC
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_ESP_WIFI
#    define PORT_HAS_FLASH
#    define PORT_HAS_RANDOM
#    define PORT_HAS_SPI
#    define PORT_HAS_WS2812
#endif

#if defined(FAMILY_STM32F1)
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_FLASH
#endif

#if defined(FAMILY_STM32F2)
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_FLASH
#endif

#if defined(FAMILY_STM32F3)
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_FLASH
#endif

#if defined(FAMILY_SPC5)
#    define PORT_HAS_CAN
#    define PORT_HAS_EEPROM_SOFT
#    define PORT_HAS_FLASH
#    define PORT_HAS_WATCHDOG
#endif

#if defined(PORT_HAS_I2C_SOFT) && !defined(PORT_HAS_I2C)
#    define PORT_HAS_I2C
#    define CONFIG_SOFTWARE_I2C                            1
#endif

#if defined(PORT_HAS_I2C)
#    define PORT_HAS_DS3231
#    define PORT_HAS_EEPROM_I2C
#    define PORT_HAS_SHT3XD
#endif

#if defined(PORT_HAS_OWI)
#    define PORT_HAS_DS18B20
#endif

#if defined(PORT_HAS_PWM)
#    define PORT_HAS_ANALOG_OUTPUT_PIN
#endif

#if defined(PORT_HAS_ADC)
#    define PORT_HAS_ANALOG_INPUT_PIN
#endif

/**
 * The system configuration string contains a list of all configration
 * varialbes and their values.
 */
#ifndef CONFIG_SYS_CONFIG_STRING
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_SYS_CONFIG_STRING                    0
#    else
#        define CONFIG_SYS_CONFIG_STRING                    1
#    endif
#endif

/**
 * Main thread stack size for ports with a fixed size main thread
 * stack.
 */
#ifndef CONFIG_SYS_SIMBA_MAIN_STACK_MAX
#    if defined(ARCH_ESP32)
#        define CONFIG_SYS_SIMBA_MAIN_STACK_MAX          8192
#    else
#        define CONFIG_SYS_SIMBA_MAIN_STACK_MAX          4096
#    endif
#endif

/**
 * Read, and when needed clear, the reset cause at startup.
 */
#ifndef CONFIG_SYS_RESET_CAUSE
#    define CONFIG_SYS_RESET_CAUSE                          1
#endif

/**
 * Kick the watchdog in `sys_panic()` before writing to the console.
 */
#ifndef CONFIG_SYS_PANIC_KICK_WATCHDOG
#    define CONFIG_SYS_PANIC_KICK_WATCHDOG                  0
#endif

/**
 * Assertions are used to check various conditions during the
 * application execution. A typical usage is to validate function
 * input arguments.
 */
#ifndef CONFIG_ASSERT
#    if defined(ARCH_LINUX)
#        define CONFIG_ASSERT                               1
#    elif defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM) || defined(ARCH_ESP)
#        define CONFIG_ASSERT                               0
#    else
#        define CONFIG_ASSERT                               1
#    endif
#endif

/**
 * Force all assertions to be fatal.
 */
#ifndef CONFIG_ASSERT_FORCE_FATAL
#    define CONFIG_ASSERT_FORCE_FATAL                       1
#endif

/**
 * Enable fatal assertions, ``FATAL_ASSERT*()``.
 */
#ifndef CONFIG_FATAL_ASSERT
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FATAL_ASSERT                         0
#    else
#        define CONFIG_FATAL_ASSERT                         1
#    endif
#endif

/**
 * Enable panic assertions, ``PANIC_ASSERT*()``.
 */
#ifndef CONFIG_PANIC_ASSERT
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_PANIC_ASSERT                         0
#    else
#        define CONFIG_PANIC_ASSERT                         1
#    endif
#endif

/**
 * Include more debug information.
 */
#ifndef CONFIG_DEBUG
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_DEBUG                                0
#    else
#        define CONFIG_DEBUG                                1
#    endif
#endif

/**
 * Enable linux driver implementations as TCP sockets. Can be used to
 * simulate driver communication in an application running on linux.
 */
#ifndef CONFIG_LINUX_SOCKET_DEVICE
#    define CONFIG_LINUX_SOCKET_DEVICE                      0
#endif

/**
 * Enable the adc driver.
 */
#ifndef CONFIG_ADC
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_ADC)
#        define CONFIG_ADC                                  0
#    else
#        define CONFIG_ADC                                  1
#    endif
#endif

/**
 * Enable the analog_input_pin driver.
 */
#ifndef CONFIG_ANALOG_INPUT_PIN
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_ANALOG_INPUT_PIN)
#        define CONFIG_ANALOG_INPUT_PIN                     0
#    else
#        define CONFIG_ANALOG_INPUT_PIN                     1
#    endif
#endif

/**
 * Enable the analog_output_pin driver.
 */
#ifndef CONFIG_ANALOG_OUTPUT_PIN
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_ANALOG_OUTPUT_PIN)
#        define CONFIG_ANALOG_OUTPUT_PIN                    0
#    else
#        define CONFIG_ANALOG_OUTPUT_PIN                    1
#    endif
#endif

/**
 * Enable the can driver.
 */
#ifndef CONFIG_CAN
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_CAN)
#        define CONFIG_CAN                                  0
#    else
#        define CONFIG_CAN                                  1
#    endif
#endif

/**
 * Timestamp received CAN frames.
 */
#ifndef CONFIG_CAN_FRAME_TIMESTAMP
#    define CONFIG_CAN_FRAME_TIMESTAMP                      1
#endif

/**
 * Enable the chipid driver.
 */
#ifndef CONFIG_CHIPID
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_CHIPID)
#        define CONFIG_CHIPID                               0
#    else
#        define CONFIG_CHIPID                               1
#    endif
#endif

/**
 * Enable the random driver.
 */
#ifndef CONFIG_RANDOM
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_RANDOM)
#        define CONFIG_RANDOM                               0
#    else
#        define CONFIG_RANDOM                               1
#    endif
#endif

/**
 * Enable the ws2812 driver.
 */
#ifndef CONFIG_WS2812
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_WS2812)
#        define CONFIG_WS2812                               0
#    else
#        define CONFIG_WS2812                               1
#    endif
#endif

/**
 * Enable the led_7seg_ht16k33 driver.
 */
#ifndef CONFIG_LED_7SEG_HT16K33
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_LED_7SEG_HT16K33)
#        define CONFIG_LED_7SEG_HT16K33                    0
#    else
#        define CONFIG_LED_7SEG_HT16K33                    1
#    endif
#endif

/**
 * Enable the sht3xd driver.
 */
#ifndef CONFIG_SHT3XD
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_SHT3XD)
#        define CONFIG_SHT3XD                              0
#    else
#        define CONFIG_SHT3XD                              1
#    endif
#endif

/**
 * Enable the dac driver.
 */
#ifndef CONFIG_DAC
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_DAC)
#        define CONFIG_DAC                                  0
#    else
#        define CONFIG_DAC                                  1
#    endif
#endif

/**
 * Enable the ds18b20 driver.
 */
#ifndef CONFIG_DS18B20
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_DS18B20)
#        define CONFIG_DS18B20                              0
#    else
#        define CONFIG_DS18B20                              1
#    endif
#endif

/**
 * Enable the ds3231 driver.
 */
#ifndef CONFIG_DS3231
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_DS3231)
#        define CONFIG_DS3231                               0
#    else
#        define CONFIG_DS3231                               1
#    endif
#endif

/**
 * Enable the eeprom_soft driver.
 */
#ifndef CONFIG_EEPROM_SOFT
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_EEPROM_SOFT)
#        define CONFIG_EEPROM_SOFT                          0
#    else
#        define CONFIG_EEPROM_SOFT                          1
#    endif
#endif

/**
 * Enable the esp_wifi driver.
 */
#ifndef CONFIG_ESP_WIFI
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_ESP_WIFI)
#        define CONFIG_ESP_WIFI                             0
#    else
#        define CONFIG_ESP_WIFI                             1
#    endif
#endif

/**
 * Enable the exti driver.
 */
#ifndef CONFIG_EXTI
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_EXTI)
#        define CONFIG_EXTI                                 0
#    else
#        define CONFIG_EXTI                                 1
#    endif
#endif

/**
 * Enable the flash driver.
 */
#ifndef CONFIG_FLASH
#    if defined(BOARD_ARDUINO_DUE) || defined(ARCH_LINUX) || defined(ARCH_ESP) || defined(ARCH_ESP32) || defined(FAMILY_STM32F1) || defined(FAMILY_STM32F2) || defined(FAMILY_STM32F3) || defined(FAMILY_SPC5)
#        define CONFIG_FLASH                                1
#    else
#        define CONFIG_FLASH                                0
#    endif
#endif

/**
 * Enable the i2c driver.
 */
#ifndef CONFIG_I2C
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_I2C)
#        define CONFIG_I2C                                  0
#    else
#        define CONFIG_I2C                                  1
#    endif
#endif

/**
 * Enable the i2c_soft driver.
 */
#ifndef CONFIG_I2C_SOFT
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_I2C_SOFT)
#        define CONFIG_I2C_SOFT                             0
#    else
#        define CONFIG_I2C_SOFT                             1
#    endif
#endif

/**
 * Enable the eeprom_i2c driver.
 */
#ifndef CONFIG_EEPROM_I2C
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_EEPROM_I2C)
#        define CONFIG_EEPROM_I2C                           0
#    else
#        define CONFIG_EEPROM_I2C                           1
#    endif
#endif

/**
 * Number of write retry attemps before giving up accessing a i2c
 * eeprom.
 */
#ifndef CONFIG_EEPROM_I2C_NUMBER_OF_ATTEMPTS
#    define CONFIG_EEPROM_I2C_NUMBER_OF_ATTEMPTS          100
#endif

/**
 * Enable the mcp2515 driver.
 */
#ifndef CONFIG_MCP2515
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_MCP2515)
#        define CONFIG_MCP2515                              0
#    else
#        define CONFIG_MCP2515                              1
#    endif
#endif

/**
 * Enable the nrf24l01 driver.
 */
#ifndef CONFIG_NRF24L01
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_NRF24L01)
#        define CONFIG_NRF24L01                             0
#    else
#        define CONFIG_NRF24L01                             1
#    endif
#endif

/**
 * Enable the owi driver.
 */
#ifndef CONFIG_OWI
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_OWI)
#        define CONFIG_OWI                                  0
#    else
#        define CONFIG_OWI                                  1
#    endif
#endif

/**
 * Enable the pin driver.
 */
#ifndef CONFIG_PIN
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_PIN)
#        define CONFIG_PIN                                  0
#    else
#        define CONFIG_PIN                                  1
#    endif
#endif

/**
 * Enable the pwm driver.
 */
#ifndef CONFIG_PWM
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_PWM)
#        define CONFIG_PWM                                  0
#    else
#        define CONFIG_PWM                                  1
#    endif
#endif

/**
 * Enable the pwm_soft driver.
 */
#ifndef CONFIG_PWM_SOFT
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_PWM_SOFT)
#        define CONFIG_PWM_SOFT                             0
#    else
#        define CONFIG_PWM_SOFT                             1
#    endif
#endif

/**
 * Enable the sd driver.
 */
#ifndef CONFIG_SD
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_SD)
#        define CONFIG_SD                                   0
#    else
#        define CONFIG_SD                                   1
#    endif
#endif

/**
 * Enable the spi driver.
 */
#ifndef CONFIG_SPI
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_SPI)
#        define CONFIG_SPI                                  0
#    else
#        define CONFIG_SPI                                  1
#    endif
#endif

/**
 * Enable the uart driver.
 */
#ifndef CONFIG_UART
#    define CONFIG_UART                                     1
#endif

/**
 * Enable the uart_soft driver.
 */
#ifndef CONFIG_UART_SOFT
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_UART_SOFT)
#        define CONFIG_UART_SOFT                            0
#    else
#        define CONFIG_UART_SOFT                            1
#    endif
#endif

/**
 * Enable the usb driver.
 */
#ifndef CONFIG_USB
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_USB)
#        define CONFIG_USB                                  0
#    elif defined(BOARD_ARDUINO_PRO_MICRO) || defined(BOARD_ARDUINO_DUE)
#        define CONFIG_USB                                  1
#    else
#        define CONFIG_USB                                  0
#    endif
#endif

/**
 * Enable the usb_device driver.
 */
#ifndef CONFIG_USB_DEVICE
#    if defined(PORT_HAS_USB_DEVICE)
#        define CONFIG_USB_DEVICE                           1
#    else
#        define CONFIG_USB_DEVICE                           0
#    endif
#endif

/**
 * Enable the usb_host driver.
 */
#ifndef CONFIG_USB_HOST
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_USB_HOST)
#        define CONFIG_USB_HOST                             0
#    elif defined(BOARD_ARDUINO_DUE)
#        define CONFIG_USB_HOST                             1
#    else
#        define CONFIG_USB_HOST                             0
#    endif
#endif

/**
 * Enable the watchdog driver.
 */
#ifndef CONFIG_WATCHDOG
#    if defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_WATCHDOG                             1
#    elif defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_WATCHDOG                             0
#    elif defined(FAMILY_AVR) || defined(FAMILY_SPC5)
#        define CONFIG_WATCHDOG                             1
#    else
#        define CONFIG_WATCHDOG                             0
#    endif
#endif

/**
 * Enable the xbee driver.
 */
#ifndef CONFIG_XBEE
#    if defined(CONFIG_MINIMAL_SYSTEM) || !defined(PORT_HAS_XBEE)
#        define CONFIG_XBEE                                 0
#    else
#        define CONFIG_XBEE                                 1
#    endif
#endif

/**
 * Initialize the module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_RWLOCK
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_RWLOCK                   0
#    else
#        define CONFIG_MODULE_INIT_RWLOCK                   1
#    endif
#endif

/**
 * Initialize the fs module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_FS
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_FS                       0
    #    else
#        define CONFIG_MODULE_INIT_FS                       1
#    endif
#endif

/**
 * Initialize the settings module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_SETTINGS
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_SETTINGS                 0
#    else
#        define CONFIG_MODULE_INIT_SETTINGS                 1
#    endif
#endif

/**
 * Initialize the std module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_STD
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_STD                      0
#    else
#        define CONFIG_MODULE_INIT_STD                      1
#    endif
#endif

/**
 * Initialize the sem module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_SEM
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_SEM                      0
#    else
#        define CONFIG_MODULE_INIT_SEM                      1
#    endif
#endif

/**
 * Initialize the timer module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_TIMER
#    define CONFIG_MODULE_INIT_TIMER                        1
#endif

/**
 * Initialize the log module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_LOG
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_LOG                      0
#    else
#        define CONFIG_MODULE_INIT_LOG                      1
#    endif
#endif

/**
 * Initialize the chan module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_CHAN
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_CHAN                     0
#    else
#        define CONFIG_MODULE_INIT_CHAN                     1
#    endif
#endif

/**
 * Initialize the thrd module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_THRD
#    define CONFIG_MODULE_INIT_THRD                         1
#endif

/**
 * Initialize the adc driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_ADC
#    if CONFIG_ADC == 1
#        define CONFIG_MODULE_INIT_ADC                      1
#    else
#        define CONFIG_MODULE_INIT_ADC                      0
#    endif
#endif

/**
 * Initialize the analog_input_pin driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_ANALOG_INPUT_PIN
#    if CONFIG_ANALOG_INPUT_PIN == 1
#        define CONFIG_MODULE_INIT_ANALOG_INPUT_PIN         1
#    else
#        define CONFIG_MODULE_INIT_ANALOG_INPUT_PIN         0
#    endif
#endif

/**
 * Initialize the analog_output_pin driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_ANALOG_OUTPUT_PIN
#    if CONFIG_ANALOG_OUTPUT_PIN == 1
#        define CONFIG_MODULE_INIT_ANALOG_OUTPUT_PIN        1
#    else
#        define CONFIG_MODULE_INIT_ANALOG_OUTPUT_PIN        0
#    endif
#endif

/**
 * Initialize the can driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_CAN
#    if CONFIG_CAN == 1
#        define CONFIG_MODULE_INIT_CAN                      1
#    else
#        define CONFIG_MODULE_INIT_CAN                      0
#    endif
#endif

/**
 * Initialize the chipid driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_CHIPID
#    if CONFIG_CHIPID == 1
#        define CONFIG_MODULE_INIT_CHIPID                   1
#    else
#        define CONFIG_MODULE_INIT_CHIPID                   0
#    endif
#endif

/**
 * Initialize the random driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_RANDOM
#    if CONFIG_RANDOM == 1
#        define CONFIG_MODULE_INIT_RANDOM                   1
#    else
#        define CONFIG_MODULE_INIT_RANDOM                   0
#    endif
#endif

/**
 * Initialize the dac driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_DAC
#    if CONFIG_DAC == 1
#        define CONFIG_MODULE_INIT_DAC                      1
#    else
#        define CONFIG_MODULE_INIT_DAC                      0
#    endif
#endif

/**
 * Initialize the ds18b20 driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_DS18B20
#    if CONFIG_DS18B20 == 1
#        define CONFIG_MODULE_INIT_DS18B20                  1
#    else
#        define CONFIG_MODULE_INIT_DS18B20                  0
#    endif
#endif

/**
 * Initialize the ds3231 driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_DS3231
#    if CONFIG_DS3231 == 1
#        define CONFIG_MODULE_INIT_DS3231                   1
#    else
#        define CONFIG_MODULE_INIT_DS3231                   0
#    endif
#endif

/**
 * Initialize the esp_wifi driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_ESP_WIFI
#    if CONFIG_ESP_WIFI == 1
#        define CONFIG_MODULE_INIT_ESP_WIFI                 1
#    else
#        define CONFIG_MODULE_INIT_ESP_WIFI                 0
#    endif
#endif

/**
 * Initialize the exti driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_EXTI
#    if CONFIG_EXTI == 1
#        define CONFIG_MODULE_INIT_EXTI                     1
#    else
#        define CONFIG_MODULE_INIT_EXTI                     0
#    endif
#endif

/**
 * Initialize the flash driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_FLASH
#    if CONFIG_FLASH == 1
#        define CONFIG_MODULE_INIT_FLASH                    1
#    else
#        define CONFIG_MODULE_INIT_FLASH                    0
#    endif
#endif

/**
 * Initialize the i2c driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_I2C
#    if CONFIG_I2C == 1
#        define CONFIG_MODULE_INIT_I2C                      1
#    else
#        define CONFIG_MODULE_INIT_I2C                      0
#    endif
#endif

/**
 * Initialize the i2c_soft driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_I2C_SOFT
#    if CONFIG_I2C_SOFT == 1
#        define CONFIG_MODULE_INIT_I2C_SOFT                 1
#    else
#        define CONFIG_MODULE_INIT_I2C_SOFT                 0
#    endif
#endif

/**
 * Initialize the mcp2515 driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_MCP2515
#    if CONFIG_MCP2515 == 1
#        define CONFIG_MODULE_INIT_MCP2515                  1
#    else
#        define CONFIG_MODULE_INIT_MCP2515                  0
#    endif
#endif

/**
 * Initialize the nrf24l01 driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_NRF24L01
#    if CONFIG_NRF24L01 == 1
#        define CONFIG_MODULE_INIT_NRF24L01                 1
#    else
#        define CONFIG_MODULE_INIT_NRF24L01                 0
#    endif
#endif

/**
 * Initialize the owi driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_OWI
#    if CONFIG_OWI == 1
#        define CONFIG_MODULE_INIT_OWI                      1
#    else
#        define CONFIG_MODULE_INIT_OWI                      0
#    endif
#endif

/**
 * Initialize the pin driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_PIN
#    if CONFIG_PIN == 1
#        define CONFIG_MODULE_INIT_PIN                      1
#    else
#        define CONFIG_MODULE_INIT_PIN                      0
#    endif
#endif

/**
 * Initialize the pwm driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_PWM
#    if CONFIG_PWM == 1
#        define CONFIG_MODULE_INIT_PWM                      1
#    else
#        define CONFIG_MODULE_INIT_PWM                      0
#    endif
#endif

/**
 * Initialize the pwm_soft driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_PWM_SOFT
#    if CONFIG_PWM_SOFT == 1
#        define CONFIG_MODULE_INIT_PWM_SOFT                 1
#    else
#        define CONFIG_MODULE_INIT_PWM_SOFT                 0
#    endif
#endif

/**
 * Initialize the sd driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_SD
#    if CONFIG_SD == 1
#        define CONFIG_MODULE_INIT_SD                       1
#    else
#        define CONFIG_MODULE_INIT_SD                       0
#    endif
#endif

/**
 * Initialize the spi driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_SPI
#    if CONFIG_SPI == 1
#        define CONFIG_MODULE_INIT_SPI                      1
#    else
#        define CONFIG_MODULE_INIT_SPI                      0
#    endif
#endif

/**
 * Initialize the uart driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_UART
#    if CONFIG_UART == 1
#        define CONFIG_MODULE_INIT_UART                     1
#    else
#        define CONFIG_MODULE_INIT_UART                     0
#    endif
#endif

/**
 * Initialize the uart_soft driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_UART_SOFT
#    if CONFIG_UART_SOFT == 1
#        define CONFIG_MODULE_INIT_UART_SOFT                1
#    else
#        define CONFIG_MODULE_INIT_UART_SOFT                0
#    endif
#endif

/**
 * Initialize the usb driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_USB
#    if CONFIG_USB == 1
#        define CONFIG_MODULE_INIT_USB                      1
#    else
#        define CONFIG_MODULE_INIT_USB                      0
#    endif
#endif

/**
 * Initialize the usb_device driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_USB_DEVICE
#    if CONFIG_USB_DEVICE == 1
#        define CONFIG_MODULE_INIT_USB_DEVICE               1
#    else
#        define CONFIG_MODULE_INIT_USB_DEVICE               0
#    endif
#endif

/**
 * Initialize the usb_host driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_USB_HOST
#    if CONFIG_USB_HOST == 1
#        define CONFIG_MODULE_INIT_USB_HOST                 1
#    else
#        define CONFIG_MODULE_INIT_USB_HOST                 0
#    endif
#endif

/**
 * Initialize the watchdog driver module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_WATCHDOG
#    if CONFIG_WATCHDOG == 1
#        define CONFIG_MODULE_INIT_WATCHDOG                 1
#    else
#        define CONFIG_MODULE_INIT_WATCHDOG                 0
#    endif
#endif

/**
 * Initialize the bus module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_BUS
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_BUS                      0
#    else
#        define CONFIG_MODULE_INIT_BUS                      1
#    endif
#endif

/**
 * Initialize the inet module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_INET
#    if defined(CONFIG_MINIMAL_SYSTEM) || defined(ARCH_ARM) || defined(ARCH_AVR) || defined(ARCH_PPC)
#        define CONFIG_MODULE_INIT_INET                     0
#    else
#        define CONFIG_MODULE_INIT_INET                     1
#    endif
#endif

/**
 * Initialize the ping module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_PING
#    if defined(CONFIG_MINIMAL_SYSTEM) || defined(ARCH_ARM) || defined(ARCH_AVR) || defined(ARCH_PPC)
#        define CONFIG_MODULE_INIT_PING                     0
#    else
#        define CONFIG_MODULE_INIT_PING                     1
#    endif
#endif

/**
 * Initialize the socket module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_SOCKET
#    if defined(CONFIG_MINIMAL_SYSTEM) || defined(ARCH_ARM) || defined(ARCH_AVR) || defined(ARCH_PPC)
#        define CONFIG_MODULE_INIT_SOCKET                   0
#    else
#        define CONFIG_MODULE_INIT_SOCKET                   1
#    endif
#endif

/**
 * Initialize the network_interface module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_NETWORK_INTERFACE
#    if defined(CONFIG_MINIMAL_SYSTEM) || defined(ARCH_ARM) || defined(ARCH_AVR) || defined(ARCH_PPC)
#        define CONFIG_MODULE_INIT_NETWORK_INTERFACE        0
#    else
#        define CONFIG_MODULE_INIT_NETWORK_INTERFACE        1
#    endif
#endif

/**
 * Initialize the ssl module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_SSL
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_SSL                      0
#    elif defined(ARCH_ESP32) || defined(ARCH_LINUX)
#        define CONFIG_MODULE_INIT_SSL                      1
#    else
#        define CONFIG_MODULE_INIT_SSL                      0
#    endif
#endif

/**
 * Initialize the upgrade module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_UPGRADE
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_UPGRADE                  0
#    elif defined(ARCH_ESP32) || defined(ARCH_LINUX)
#        define CONFIG_MODULE_INIT_UPGRADE                  1
#    else
#        define CONFIG_MODULE_INIT_UPGRADE                  0
#    endif
#endif

/**
 * Initialize the regular expression module at system startup.
 */
#ifndef CONFIG_MODULE_INIT_RE
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MODULE_INIT_RE                       0
#    else
#        define CONFIG_MODULE_INIT_RE                       1
#    endif
#endif

/**
 * Debug file system command to list all DS18B20 sensors on the bus.
 */
#ifndef CONFIG_FS_CMD_DS18B20_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_DS18B20_LIST                  0
#    else
#        define CONFIG_FS_CMD_DS18B20_LIST                  1
#    endif
#endif

/**
 * Debug file system command to print the Espressif WiFi status.
 */
#ifndef CONFIG_FS_CMD_ESP_WIFI_STATUS
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_ESP_WIFI_STATUS               0
#    elif defined(BOARD_ESP12E) || defined(BOARD_ESP01) || defined(BOARD_NODEMCU)
#        define CONFIG_FS_CMD_ESP_WIFI_STATUS               1
#    else
#        define CONFIG_FS_CMD_ESP_WIFI_STATUS               0
#    endif
#endif

/**
 * Debug file system command to append to a file.
 */
#ifndef CONFIG_FS_CMD_FS_APPEND
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_APPEND                     0
#    else
#        define CONFIG_FS_CMD_FS_APPEND                     1
#    endif
#endif

/**
 * Debug file system command to list all counters.
 */
#ifndef CONFIG_FS_CMD_FS_COUNTERS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_COUNTERS_LIST              0
#    else
#        define CONFIG_FS_CMD_FS_COUNTERS_LIST              1
#    endif
#endif

/**
 * Debug file system command to set all counters to zero.
 */
#ifndef CONFIG_FS_CMD_FS_COUNTERS_RESET
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_COUNTERS_RESET             0
#    else
#        define CONFIG_FS_CMD_FS_COUNTERS_RESET             1
#    endif
#endif

/**
 * Debug file system command to list all registered file systems.
 */
#ifndef CONFIG_FS_CMD_FS_FILESYSTEMS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_LIST           0
#    else
#        define CONFIG_FS_CMD_FS_FILESYSTEMS_LIST           1
#    endif
#endif

/**
 * Debug file system command to list all registered file systems.
 */
#ifndef CONFIG_FS_CMD_FS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_LIST                       0
#    else
#        define CONFIG_FS_CMD_FS_LIST                       1
#    endif
#endif

/**
 * Debug file system command to format a file system.
 */
#ifndef CONFIG_FS_CMD_FS_FORMAT
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_FORMAT                     0
#    else
#        define CONFIG_FS_CMD_FS_FORMAT                     1
#    endif
#endif

/**
 * Debug file system command to list all parameters.
 */
#ifndef CONFIG_FS_CMD_FS_PARAMETERS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_PARAMETERS_LIST            0
#    else
#        define CONFIG_FS_CMD_FS_PARAMETERS_LIST            1
#    endif
#endif

/**
 * Debug file system command to read from a file.
 */
#ifndef CONFIG_FS_CMD_FS_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_READ                       0
#    else
#        define CONFIG_FS_CMD_FS_READ                       1
#    endif
#endif

/**
 * Debug file system command to remove a file.
 */
#ifndef CONFIG_FS_CMD_FS_REMOVE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_REMOVE                     0
#    else
#        define CONFIG_FS_CMD_FS_REMOVE                     1
#    endif
#endif

/**
 * Debug file system command to write to a file.
 */
#ifndef CONFIG_FS_CMD_FS_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_SPC5) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_FS_WRITE                      0
#    else
#        define CONFIG_FS_CMD_FS_WRITE                      1
#    endif
#endif

/**
 * Debug file system command to read from a i2c bus.
 */
#ifndef CONFIG_FS_CMD_I2C_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_I2C_READ                      0
#    else
#        define CONFIG_FS_CMD_I2C_READ                      1
#    endif
#endif

/**
 * Debug file system command to write to a i2c bus.
 */
#ifndef CONFIG_FS_CMD_I2C_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_I2C_WRITE                     0
#    else
#        define CONFIG_FS_CMD_I2C_WRITE                     1
#    endif
#endif

/**
 * Debug file system command to scan a i2c bus.
 */
#ifndef CONFIG_FS_CMD_I2C_SCAN
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_I2C_SCAN                      0
#    else
#        define CONFIG_FS_CMD_I2C_SCAN                      1
#    endif
#endif

/**
 * Debug file system command to list all log objects.
 */
#ifndef CONFIG_FS_CMD_LOG_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_LOG_LIST                      0
#    else
#        define CONFIG_FS_CMD_LOG_LIST                      1
#    endif
#endif

/**
 * Debug file system command to create a log entry and print
 * it. Mainly used for debugging.
 */
#ifndef CONFIG_FS_CMD_LOG_PRINT
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_LOG_PRINT                     0
#    else
#        define CONFIG_FS_CMD_LOG_PRINT                     1
#    endif
#endif

/**
 * Debug file system command to set the log mask of a log object.
 */
#ifndef CONFIG_FS_CMD_LOG_SET_LOG_MASK
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_LOG_SET_LOG_MASK              0
#    else
#        define CONFIG_FS_CMD_LOG_SET_LOG_MASK              1
#    endif
#endif

/**
 * Debug file system command to list all network interfaces.
 */
#ifndef CONFIG_FS_CMD_NETWORK_INTERFACE_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_NETWORK_INTERFACE_LIST        0
#    else
#        define CONFIG_FS_CMD_NETWORK_INTERFACE_LIST        1
#    endif
#endif

/**
 * Debug file system command to read the current value of a pin.
 */
#ifndef CONFIG_FS_CMD_PIN_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_PIN_READ                      0
#    else
#        define CONFIG_FS_CMD_PIN_READ                      1
#    endif
#endif

/**
 * Debug file system command to set the mode of a pin.
 */
#ifndef CONFIG_FS_CMD_PIN_SET_MODE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_PIN_SET_MODE                  0
#    else
#        define CONFIG_FS_CMD_PIN_SET_MODE                  1
#    endif
#endif

/**
 * Debug file system command to write a value to a pin.
 */
#ifndef CONFIG_FS_CMD_PIN_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_PIN_WRITE                     0
#    else
#        define CONFIG_FS_CMD_PIN_WRITE                     1
#    endif
#endif

/**
 * Debug file system command to ping a host.
 */
#ifndef CONFIG_FS_CMD_PING_PING
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_PING_PING                     0
#    else
#        define CONFIG_FS_CMD_PING_PING                     1
#    endif
#endif

/**
 * Debug file system command to list all services.
 */
#ifndef CONFIG_FS_CMD_SERVICE_LIST
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SERVICE_LIST                  0
#    else
#        define CONFIG_FS_CMD_SERVICE_LIST                  1
#    endif
#endif

/**
 * Debug file system command to start a service.
 */
#ifndef CONFIG_FS_CMD_SERVICE_START
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SERVICE_START                 0
#    else
#        define CONFIG_FS_CMD_SERVICE_START                 1
#    endif
#endif

/**
 * Debug file system command to stop a services.
 */
#ifndef CONFIG_FS_CMD_SERVICE_STOP
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SERVICE_STOP                  0
#    else
#        define CONFIG_FS_CMD_SERVICE_STOP                  1
#    endif
#endif

/**
 * Debug file system command to list all settings.
 */
#ifndef CONFIG_FS_CMD_SETTINGS_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SETTINGS_LIST                 0
#    else
#        define CONFIG_FS_CMD_SETTINGS_LIST                 1
#    endif
#endif

/**
 * Debug file system command to read the value of a setting.
 */
#ifndef CONFIG_FS_CMD_SETTINGS_READ
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SETTINGS_READ                 0
#    else
#        define CONFIG_FS_CMD_SETTINGS_READ                 1
#    endif
#endif

/**
 * Debug file system command to reset the settings to their original
 * values.
 */
#ifndef CONFIG_FS_CMD_SETTINGS_RESET
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SETTINGS_RESET                0
#    else
#        define CONFIG_FS_CMD_SETTINGS_RESET                1
#    endif
#endif

/**
 * Debug file system command to write a value to a setting.
 */
#ifndef CONFIG_FS_CMD_SETTINGS_WRITE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SETTINGS_WRITE                0
#    else
#        define CONFIG_FS_CMD_SETTINGS_WRITE                1
#    endif
#endif

/**
 * Debug file system command to print the system configuration.
 */
#ifndef CONFIG_FS_CMD_SYS_CONFIG
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SYS_CONFIG                    0
#    else
#        define CONFIG_FS_CMD_SYS_CONFIG                    1
#    endif
#endif

/**
 * Debug file system command to print the system information.
 */
#ifndef CONFIG_FS_CMD_SYS_INFO
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SYS_INFO                      0
#    else
#        define CONFIG_FS_CMD_SYS_INFO                      1
#    endif
#endif

/**
 * Debug file system command to print the system uptime.
 */
#ifndef CONFIG_FS_CMD_SYS_UPTIME
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SYS_UPTIME                    0
#    else
#        define CONFIG_FS_CMD_SYS_UPTIME                    1
#    endif
#endif

/**
 * Debug file system command to force a panic of the system.
 */
#ifndef CONFIG_FS_CMD_SYS_PANIC
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SYS_PANIC                     0
#    else
#        define CONFIG_FS_CMD_SYS_PANIC                     1
#    endif
#endif

/**
 * Debug file system command to reboot the system.
 */
#ifndef CONFIG_FS_CMD_SYS_REBOOT
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SYS_REBOOT                    0
#    else
#        define CONFIG_FS_CMD_SYS_REBOOT                    1
#    endif
#endif

/**
 * Debug file system command to print a backtrace.
 */
#ifndef CONFIG_FS_CMD_SYS_BACKTRACE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SYS_BACKTRACE                 0
#    else
#        define CONFIG_FS_CMD_SYS_BACKTRACE                 1
#    endif
#endif

/**
 * Debug file system command to print the system reset cause.
 */
#ifndef CONFIG_FS_CMD_SYS_RESET_CAUSE
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_SYS_RESET_CAUSE               0
#    else
#        define CONFIG_FS_CMD_SYS_RESET_CAUSE               1
#    endif
#endif

/**
 * Debug file system command to list threads' information.
 */
#ifndef CONFIG_FS_CMD_THRD_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_THRD_LIST                     0
#    else
#        define CONFIG_FS_CMD_THRD_LIST                     1
#    endif
#endif

/**
 * Debug file system command to set the log mask of a thread.
 */
#ifndef CONFIG_FS_CMD_THRD_SET_LOG_MASK
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_THRD_SET_LOG_MASK             0
#    else
#        define CONFIG_FS_CMD_THRD_SET_LOG_MASK             1
#    endif
#endif

/**
 * Debug file system command to enter the application.
 */
#ifndef CONFIG_FS_CMD_UPGRADE_APPLICATION_ENTER
#    define CONFIG_FS_CMD_UPGRADE_APPLICATION_ENTER         1
#endif

/**
 * Debug file system command to erase the application.
 */
#ifndef CONFIG_FS_CMD_UPGRADE_APPLICATION_ERASE
#    define CONFIG_FS_CMD_UPGRADE_APPLICATION_ERASE         1
#endif

/**
 * Debug file system command to check if the application is valid.
 */
#ifndef CONFIG_FS_CMD_UPGRADE_APPLICATION_IS_VALID
#    define CONFIG_FS_CMD_UPGRADE_APPLICATION_IS_VALID      1
#endif

/**
 * Debug file system command to enter the bootloader.
 */
#ifndef CONFIG_FS_CMD_UPGRADE_BOOTLOADER_ENTER
#    define CONFIG_FS_CMD_UPGRADE_BOOTLOADER_ENTER          1
#endif

/**
 * Debug file system command to list all USB devices.
 */
#ifndef CONFIG_FS_CMD_USB_DEVICE_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_USB_DEVICE_LIST               0
#    else
#        define CONFIG_FS_CMD_USB_DEVICE_LIST               1
#    endif
#endif

/**
 * Debug file system command to list all USB devices connected to the
 * USB host.
 */
#ifndef CONFIG_FS_CMD_USB_HOST_LIST
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_USB_HOST_LIST                 0
#    else
#        define CONFIG_FS_CMD_USB_HOST_LIST                 1
#    endif
#endif

/**
 * Debug file system command to read for non-volatile memory.
 */
#ifndef CONFIG_FS_CMD_NVM_READ
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_NVM_READ                      0
#    else
#        define CONFIG_FS_CMD_NVM_READ                      1
#    endif
#endif

/**
 * Debug file system command to write for non-volatile memory.
 */
#ifndef CONFIG_FS_CMD_NVM_WRITE
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FS_CMD_NVM_WRITE                      0
#    else
#        define CONFIG_FS_CMD_NVM_WRITE                      1
#    endif
#endif

/**
 * The maximum length of an absolute path in the file system.
 */
#ifndef CONFIG_FS_PATH_MAX
#    define CONFIG_FS_PATH_MAX                             64
#endif

/**
 * Start the monitor thread to gather statistics of the scheulder.
 */
#ifndef CONFIG_MONITOR_THREAD
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(FAMILY_ESP) || defined(FAMILY_ESP32) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_MONITOR_THREAD                       0
#    else
#        define CONFIG_MONITOR_THREAD                       1
#    endif
#endif

/**
 * Default period of the monitor thread in microseconds.
 */
#ifndef CONFIG_MONITOR_THREAD_PERIOD_US
#    define CONFIG_MONITOR_THREAD_PERIOD_US           2000000
#endif

/**
 * Use a preemptive scheduler.
 */
#ifndef CONFIG_PREEMPTIVE_SCHEDULER
#    define CONFIG_PREEMPTIVE_SCHEDULER                     0
#endif

/**
 * Profile the stack usage in runtime. It's a cheap operation and is
 * recommended to have enabled.
 */
#ifndef CONFIG_PROFILE_STACK
#    define CONFIG_PROFILE_STACK                            1
#endif

/**
 * Size of the settings area. This size *MUST* have the same size as
 * the settings generated by the settings.py script.
 */
#ifndef CONFIG_SETTINGS_AREA_SIZE
#    if defined(ARCH_PPC) || defined(ARCH_LINUX)
#        define CONFIG_SETTINGS_AREA_SIZE                1028
#    else
#        define CONFIG_SETTINGS_AREA_SIZE                 256
#    endif
#endif

/**
 * Enable the blob setting type.
 */
#ifndef CONFIG_SETTINGS_BLOB
#    define CONFIG_SETTINGS_BLOB                            1
#endif

/**
 * Maximum number of characters in a shell command.
 */
#ifndef CONFIG_SHELL_COMMAND_MAX
#    define CONFIG_SHELL_COMMAND_MAX                       64
#endif

/**
 * Size of the shell history buffer.
 */
#ifndef CONFIG_SHELL_HISTORY_SIZE
#    define CONFIG_SHELL_HISTORY_SIZE                     768
#endif

/**
 * Minimal shell functionality to minimize the code size of the shell
 * module.
 */
#ifndef CONFIG_SHELL_MINIMAL
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_SHELL_MINIMAL                        1
#    else
#        define CONFIG_SHELL_MINIMAL                        0
#    endif
#endif

/**
 * The shell prompt string.
 */
#ifndef CONFIG_SHELL_PROMPT
#    define CONFIG_SHELL_PROMPT "$ "
#endif

/**
 * Raw socket support.
 */
#ifndef CONFIG_SOCKET_RAW
#    define CONFIG_SOCKET_RAW                               1
#endif

/**
 * SPIFFS is a flash file system applicable for boards that has a
 * reasonably big modifiable flash.
 */
#ifndef CONFIG_SPIFFS
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_SPIFFS                               0
#    elif defined(BOARD_ARDUINO_DUE) || defined(ARCH_LINUX) || defined(ARCH_ESP) || defined(ARCH_ESP32)
#        define CONFIG_SPIFFS                               1
#    else
#        define CONFIG_SPIFFS                               0
#    endif
#endif

/**
 * FAT16 is a file system.
 */
#ifndef CONFIG_FAT16
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FAT16                                0
#    else
#        define CONFIG_FAT16                                1
#    endif
#endif

/**
 * Generic file system.
 */
#ifndef CONFIG_FILESYSTEM_GENERIC
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_FILESYSTEM_GENERIC                   0
#    else
#        define CONFIG_FILESYSTEM_GENERIC                   1
#    endif
#endif

#define CONFIG_START_CONSOLE_NONE                           0
#define CONFIG_START_CONSOLE_UART                           1
#define CONFIG_START_CONSOLE_USB_CDC                        2

/**
 * Start the console device (UART/USB CDC) on system startup.
 */
#ifndef CONFIG_START_CONSOLE
#    if defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_START_CONSOLE   CONFIG_START_CONSOLE_USB_CDC
#    elif CONFIG_UART == 1
#        define CONFIG_START_CONSOLE      CONFIG_START_CONSOLE_UART
#    else
#        define CONFIG_START_CONSOLE      CONFIG_START_CONSOLE_NONE
#    endif
#endif

/**
 * Console device index.
 */
#ifndef CONFIG_START_CONSOLE_DEVICE_INDEX
#    define CONFIG_START_CONSOLE_DEVICE_INDEX               0
#endif

/**
 * Console UART baudrate.
 */
#ifndef CONFIG_START_CONSOLE_UART_BAUDRATE
#    if defined(FAMILY_ESP)
#        define CONFIG_START_CONSOLE_UART_BAUDRATE      76800
#    elif defined(FAMILY_ESP32) || defined(FAMILY_SPC5) || defined(BOARD_ARDUINO_DUE)
#        define CONFIG_START_CONSOLE_UART_BAUDRATE     115200
#    else
#        define CONFIG_START_CONSOLE_UART_BAUDRATE      38400
#    endif
#endif

/**
 * Console UART baudrate.
 */
#ifndef CONFIG_START_CONSOLE_UART_RX_BUFFER_SIZE
#    if defined(BOARD_NANO32) || defined(BOARD_ESP32_DEVKITC) || defined(BOARD_MAPLE_ESP32) || defined(BOARD_LINUX)
#        define CONFIG_START_CONSOLE_UART_RX_BUFFER_SIZE  512
#    else
#        define CONFIG_START_CONSOLE_UART_RX_BUFFER_SIZE   32
#    endif
#endif

/**
 * Console USB CDC control interface number.
 */
#ifndef CONFIG_START_CONSOLE_USB_CDC_CONTROL_INTERFACE
#    define CONFIG_START_CONSOLE_USB_CDC_CONTROL_INTERFACE  0
#endif

/**
 * Console USB CDC input endpoint.
 */
#ifndef CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_IN
#    define CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_IN        2
#endif

/**
 * Console USB CDC output endpoint.
 */
#ifndef CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_OUT
#    define CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_OUT       3
#endif

/**
 * Wait for the host to connect after starting the console.
 */
#ifndef CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION
#    define CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION 1
#endif

/**
 * Configure a default file system.
 */
#ifndef CONFIG_START_FILESYSTEM
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_START_FILESYSTEM                     0
#    elif defined(BOARD_ARDUINO_DUE) || defined(ARCH_LINUX) || defined(ARCH_ESP) || defined(ARCH_ESP32)
#        define CONFIG_START_FILESYSTEM                     1
#    else
#        define CONFIG_START_FILESYSTEM                     0
#    endif
#endif

/**
 * Configure a default file system start address.
 */
#ifndef CONFIG_START_FILESYSTEM_ADDRESS
#    if defined(BOARD_ARDUINO_DUE)
#        define CONFIG_START_FILESYSTEM_ADDRESS    0x000e0000
#    elif defined(BOARD_ESP01)
#        define CONFIG_START_FILESYSTEM_ADDRESS    0x0006b000
#    elif defined(BOARD_ESP12E) || defined(BOARD_NODEMCU) || defined(BOARD_WEMOS_D1_MINI)
#        define CONFIG_START_FILESYSTEM_ADDRESS    0x00300000
#    elif defined(BOARD_NANO32) || defined(BOARD_ESP32_DEVKITC) || defined(BOARD_MAPLE_ESP32)
#        define CONFIG_START_FILESYSTEM_ADDRESS    0x00300000
#    else
#        define CONFIG_START_FILESYSTEM_ADDRESS             0
#    endif
#endif

/**
 * Configure a default file system size.
 */
#ifndef CONFIG_START_FILESYSTEM_SIZE
#    if defined(BOARD_ARDUINO_DUE)
#        define CONFIG_START_FILESYSTEM_SIZE            32768
#    elif defined(BOARD_ESP01)
#        define CONFIG_START_FILESYSTEM_SIZE          0x10000
#    elif defined(BOARD_ESP12E) || defined(BOARD_NODEMCU) || defined(BOARD_WEMOS_D1_MINI)
#        define CONFIG_START_FILESYSTEM_SIZE          0xFB000
#    elif defined(ARCH_ESP32)
#        define CONFIG_START_FILESYSTEM_SIZE          0x20000
#    else
#        define CONFIG_START_FILESYSTEM_SIZE            65536
#    endif
#endif

/**
 * Configure a default non-volatile memory.
 */
#ifndef CONFIG_START_NVM
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_START_NVM                            0
#    else
#        define CONFIG_START_NVM                            1
#    endif
#endif

/**
 * Non-volatile memory size in bytes.
 */
#ifndef CONFIG_NVM_SIZE
#    if defined(ARCH_AVR)
#        define CONFIG_NVM_SIZE                           256
#    else
#        define CONFIG_NVM_SIZE                          2040
#    endif
#endif

/**
 * Use the software EEPROM implementation in the non-volatile memory
 * module.
 */
#ifndef CONFIG_NVM_EEPROM_SOFT
#    if defined(ARCH_LINUX) || defined(ARCH_AVR) || defined(ARCH_ESP32) || defined(ARCH_ESP)
#        define CONFIG_NVM_EEPROM_SOFT                      0
#    else
#        define CONFIG_NVM_EEPROM_SOFT                      1
#    endif
#endif

/**
 * Non-volatile memory software EEPROM block 0 size. Must be a
 * multiple of ``CONFIG_NVM_EEPROM_SOFT_CHUNK_SIZE``.
 */
#ifndef CONFIG_NVM_EEPROM_SOFT_BLOCK_0_SIZE
#    define CONFIG_NVM_EEPROM_SOFT_BLOCK_0_SIZE         16384
#endif

/**
 * Non-volatile memory software EEPROM block 1 size. Must be a
 * multiple of ``CONFIG_NVM_EEPROM_SOFT_CHUNK_SIZE``.
 */
#ifndef CONFIG_NVM_EEPROM_SOFT_BLOCK_1_SIZE
#    define CONFIG_NVM_EEPROM_SOFT_BLOCK_1_SIZE         16384
#endif

/**
 * Non-volatile software EEPROM chunk size. Must be a power of two.
 */
#ifndef CONFIG_NVM_EEPROM_SOFT_CHUNK_SIZE
#    define CONFIG_NVM_EEPROM_SOFT_CHUNK_SIZE (CONFIG_NVM_SIZE + 8)
#endif

/**
 * Non-volatile software EEPROM flash device index.
 */
#ifndef CONFIG_NVM_EEPROM_SOFT_FLASH_DEVICE_INDEX
#    if defined(FAMILY_SPC5)
#        define CONFIG_NVM_EEPROM_SOFT_FLASH_DEVICE_INDEX   1
#    else
#        define CONFIG_NVM_EEPROM_SOFT_FLASH_DEVICE_INDEX   0
#    endif
#endif

/**
 * Setup the ip stack and connect to all configured networks.
 */
#ifndef CONFIG_START_NETWORK
#    define CONFIG_START_NETWORK                            0
#endif

/**
 * WiFi connect timeout is seconds.
 */
#ifndef CONFIG_START_NETWORK_INTERFACE_WIFI_CONNECT_TIMEOUT
#    define CONFIG_START_NETWORK_INTERFACE_WIFI_CONNECT_TIMEOUT 30
#endif

/**
 * SSID of the WiFi to connect to.
 */
#ifndef CONFIG_START_NETWORK_INTERFACE_WIFI_SSID
#    define CONFIG_START_NETWORK_INTERFACE_WIFI_SSID   MyWiFiSSID
#endif

/**
 * Password of the WiFi to connect to.
 */
#ifndef CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD
#    define CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD MyWiFiPassword
#endif

/**
 * Start a shell thread communication over the console channels.
 */
#ifndef CONFIG_START_SHELL
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM) || defined(SOAM)
#        define CONFIG_START_SHELL                          0
#    else
#        define CONFIG_START_SHELL                          1
#    endif
#endif

/**
 * Shell thread priority.
 */
#ifndef CONFIG_START_SHELL_PRIO
#    define CONFIG_START_SHELL_PRIO                        30
#endif

/**
 * Shell thread stack size in words.
 */
#ifndef CONFIG_START_SHELL_STACK_SIZE
#    if defined(BOARD_ARDUINO_DUE) || defined(ARCH_ESP) || defined(ARCH_PPC)
#        define CONFIG_START_SHELL_STACK_SIZE            1536
#    elif defined(ARCH_ESP32)
#        define CONFIG_START_SHELL_STACK_SIZE            4096
#    else
#        define CONFIG_START_SHELL_STACK_SIZE             768
#    endif
#endif

/**
 * Start a SOAM thread communication over the console channels.
 */
#ifndef CONFIG_START_SOAM
#    if defined(SOAM)
#        define CONFIG_START_SOAM                           1
#    else
#        define CONFIG_START_SOAM                           0
#    endif
#endif

/**
 * SOAM thread priority.
 */
#ifndef CONFIG_START_SOAM_PRIO
#    define CONFIG_START_SOAM_PRIO                         30
#endif

/**
 * SOAM thread stack size in words.
 */
#ifndef CONFIG_START_SOAM_STACK_SIZE
#    if defined(BOARD_ARDUINO_DUE) || defined(ARCH_ESP)
#        define CONFIG_START_SOAM_STACK_SIZE             1536
#    elif defined(ARCH_ESP32)
#        define CONFIG_START_SOAM_STACK_SIZE             4096
#    else
#        define CONFIG_START_SOAM_STACK_SIZE              840
#    endif
#endif

/**
 * Maximum number of bytes in the print output buffer.
 */
#ifndef CONFIG_STD_OUTPUT_BUFFER_MAX
#    define CONFIG_STD_OUTPUT_BUFFER_MAX                   16
#endif

/**
 * Use floating point numbers instead of intergers where applicable.
 */
#ifndef CONFIG_FLOAT
#    define CONFIG_FLOAT                                    1
#endif

/**
 * System tick frequency in Hertz.
 */
#ifndef CONFIG_SYSTEM_TICK_FREQUENCY
#    define CONFIG_SYSTEM_TICK_FREQUENCY                  100
#endif

/**
 * Use interrupts.
 */
#ifndef CONFIG_SYSTEM_INTERRUPTS
#    define CONFIG_SYSTEM_INTERRUPTS                        1
#endif

/**
 * Interrupt stack size in bytes. Set to a value greater than zero to
 * enable the interrupt stack.
 */
#ifndef CONFIG_SYSTEM_INTERRUPT_STACK_SIZE
#    if defined(FAMILY_SPC5)
#        define CONFIG_SYSTEM_INTERRUPT_STACK_SIZE        256
#    else
#        define CONFIG_SYSTEM_INTERRUPT_STACK_SIZE          0
#    endif
#endif

/**
 * Calculate thread CPU usage.
 */
#ifndef CONFIG_THRD_CPU_USAGE
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_THRD_CPU_USAGE                       0
#    else
#        define CONFIG_THRD_CPU_USAGE                       1
#    endif
#endif

/**
 * Default thread log mask.
 */
#ifndef CONFIG_THRD_DEFAULT_LOG_MASK
#    define CONFIG_THRD_DEFAULT_LOG_MASK       LOG_UPTO(INFO)
#endif

/**
 * Regular expression module debug log mask.
 */
#ifndef CONFIG_RE_DEBUG_LOG_MASK
#    define CONFIG_RE_DEBUG_LOG_MASK                       -1
#endif

/**
 * Each thread has a list of environment variables associated with
 * it. A typical example of an environment variable is "CWD" - Current
 * Working Directory.
 */
#ifndef CONFIG_THRD_ENV
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_THRD_ENV                             0
#    else
#        define CONFIG_THRD_ENV                             1
#    endif
#endif

/**
 * Stack size of the idle thread.
 */
#ifndef CONFIG_THRD_IDLE_STACK_SIZE
#    if defined(ARCH_ARM)
#        define CONFIG_THRD_IDLE_STACK_SIZE               384
#    elif defined(ARCH_AVR) || defined(FAMILY_SPC5)
#        define CONFIG_THRD_IDLE_STACK_SIZE               156
#    elif defined(ARCH_ESP)
#        define CONFIG_THRD_IDLE_STACK_SIZE               768
#    else
#        define CONFIG_THRD_IDLE_STACK_SIZE              1024
#    endif
#endif

/**
 * Stack size of the monitor thread.
 */
#ifndef CONFIG_THRD_MONITOR_STACK_SIZE
#    if defined(ARCH_PPC) || defined(ARCH_ESP) || defined(ARCH_ESP32)
#        define CONFIG_THRD_MONITOR_STACK_SIZE            768
#    elif defined(ARCH_ARM)
#        define CONFIG_THRD_MONITOR_STACK_SIZE            512
#    elif defined(ARCH_AVR)
#        define CONFIG_THRD_MONITOR_STACK_SIZE            256
#    else
#        define CONFIG_THRD_MONITOR_STACK_SIZE           1024
#    endif
#endif

/**
 * Count the number of times each thread has been scheduled.
 */
#ifndef CONFIG_THRD_SCHEDULED
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_THRD_SCHEDULED                       0
#    else
#        define CONFIG_THRD_SCHEDULED                       1
#    endif
#endif

/**
 * Enable the thread stack heap allocator.
 */
#ifndef CONFIG_THRD_STACK_HEAP
#    define CONFIG_THRD_STACK_HEAP                          0
#endif

/**
 * Size in bytes of the thread stack heap.
 */
#ifndef CONFIG_THRD_STACK_HEAP_SIZE
#    define CONFIG_THRD_STACK_HEAP_SIZE                     0
#endif

/**
 * Threads are allowed to terminate.
 */
#ifndef CONFIG_THRD_TERMINATE
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_THRD_TERMINATE                       0
#    else
#        define CONFIG_THRD_TERMINATE                       1
#    endif
#endif

/**
 * USB device vendor id.
 */
#ifndef CONFIG_USB_DEVICE_VID
#    define CONFIG_USB_DEVICE_VID                      0x2341
#endif

/**
 * USB device product id.
 */
#ifndef CONFIG_USB_DEVICE_PID
#    define CONFIG_USB_DEVICE_PID                      0x8037
#endif

/**
 * Number of colums in Emacs text editor.
 */
#ifndef CONFIG_EMACS_COLUMNS_MAX
#    define CONFIG_EMACS_COLUMNS_MAX                       80
#endif

/**
 * Number of rows in Emacs text editor.
 */
#ifndef CONFIG_EMACS_ROWS_MAX
#    define CONFIG_EMACS_ROWS_MAX                          24
#endif

/**
 * Heap size of the emacs  text editor.
 */
#ifndef CONFIG_EMACS_HEAP_SIZE
#    define CONFIG_EMACS_HEAP_SIZE                      32768
#endif

/**
 * System tick using a software timer instead of a hardware
 * timer. Suitable for ESP8266 to enable software PWM.
 */
#ifndef CONFIG_SYSTEM_TICK_SOFTWARE
#    if (CONFIG_PWM_SOFT == 1) && defined(ARCH_ESP)
#        define CONFIG_SYSTEM_TICK_SOFTWARE                 1
#    else
#        define CONFIG_SYSTEM_TICK_SOFTWARE                 0
#    endif
#endif

/**
 * Add support to wrap the HTTP server in SSL, creating a HTTPS
 * server.
 */
#ifndef CONFIG_HTTP_SERVER_SSL
#    if defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_HTTP_SERVER_SSL                      0
#    elif defined(ARCH_ESP32) || defined(ARCH_LINUX)
#        define CONFIG_HTTP_SERVER_SSL                      1
#    else
#        define CONFIG_HTTP_SERVER_SSL                      0
#    endif
#endif

/**
 * Sleep in the test harness before executing the first testcase.
 */
#ifndef CONFIG_HARNESS_SLEEP_MS
#    define CONFIG_HARNESS_SLEEP_MS                       300
#endif

/**
 * Maximum buffer size the expect function can handle.
 */
#ifndef CONFIG_HARNESS_EXPECT_BUFFER_SIZE
#    define CONFIG_HARNESS_EXPECT_BUFFER_SIZE             512
#endif

/**
 * Size of the harness heap, required for harness_mock_write() and
 * harness_mock_read().
 */
#ifndef CONFIG_HARNESS_HEAP_MAX
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO)
#        define CONFIG_HARNESS_HEAP_MAX                    16
#    else
#        define CONFIG_HARNESS_HEAP_MAX                  2048
#    endif
#endif

/**
 * Verbose mock framework.
 */
#ifndef CONFIG_HARNESS_MOCK_VERBOSE
#    define CONFIG_HARNESS_MOCK_VERBOSE                     1
#endif

/**
 * Size of the HTTP server request buffer. This buffer is used when
 * parsing received HTTP request headers.
 */
#ifndef CONFIG_HTTP_SERVER_REQUEST_BUFFER_SIZE
#    define CONFIG_HTTP_SERVER_REQUEST_BUFFER_SIZE        128
#endif

/**
 * Use lookup tables for CRC calculations. It is faster, but uses more
 * memory.
 */
#ifndef CONFIG_CRC_TABLE_LOOKUP
#    define CONFIG_CRC_TABLE_LOOKUP                         1
#endif

/**
 */
#ifndef CONFIG_SPC5_BOOT_ENTRY_RCHW
#    define CONFIG_SPC5_BOOT_ENTRY_RCHW                     1
#endif

/**
 */
#ifndef CONFIG_SPC5_RAM_CLEAR_ALL
#    define CONFIG_SPC5_RAM_CLEAR_ALL                       1
#endif

/**
 * Include the function time_unix_time_to_date().
 */
#ifndef CONFIG_TIME_UNIX_TIME_TO_DATE
#    define CONFIG_TIME_UNIX_TIME_TO_DATE                   1
#endif

/**
 * Embed the SOAM database in the application.
 */
#ifndef CONFIG_SOAM_EMBEDDED_DATABASE
#    define CONFIG_SOAM_EMBEDDED_DATABASE                   0
#endif

/**
 * System module log mask.
 */
#ifndef CONFIG_SYS_LOG_MASK
#    if defined(BOARD_ARDUINO_NANO) || defined(BOARD_ARDUINO_UNO) || defined(BOARD_ARDUINO_PRO_MICRO) || defined(CONFIG_MINIMAL_SYSTEM)
#        define CONFIG_SYS_LOG_MASK                -1
#    else
#        define CONFIG_SYS_LOG_MASK                LOG_UPTO(INFO)
#    endif
#endif

/**
 * The external oscillator frequency in Hertz.
 */
#ifndef CONFIG_EXTERNAL_OSCILLATOR_FREQUENCY_HZ
#    define CONFIG_EXTERNAL_OSCILLATOR_FREQUENCY_HZ  16000000
#endif

/**
 * Semaphore protected device access in the flash driver module.
 */
#ifndef CONFIG_FLASH_DEVICE_SEMAPHORE
#    define CONFIG_FLASH_DEVICE_SEMAPHORE                   1
#endif

/**
 * Semaphore protected software eeprom accesses.
 */
#ifndef CONFIG_EEPROM_SOFT_SEMAPHORE
#    define CONFIG_EEPROM_SOFT_SEMAPHORE                    1
#endif

#define CONFIG_EEPROM_SOFT_CRC_32                           0
#define CONFIG_EEPROM_SOFT_CRC_CCITT                        1

/**
 * Software eeprom crc algorithm.
 */
#ifndef CONFIG_EEPROM_SOFT_CRC
#    define CONFIG_EEPROM_SOFT_CRC  CONFIG_EEPROM_SOFT_CRC_32
#endif

/**
 * Configuration validation.
 */
#if (CONFIG_START_SHELL == 1) &&  (CONFIG_START_CONSOLE == CONFIG_START_CONSOLE_NONE)
#    error "CONFIG_START_SHELL == 1 requires CONFIG_START_CONSOLE != CONFIG_START_CONSOLE_NONE. Please change the configuration."
#endif

#if (CONFIG_MODULE_INIT_PWM_SOFT == 1) &&  (CONFIG_SYSTEM_TICK_SOFTWARE == 0) && (ARCH_ESP)
#    error "CONFIG_SYSTEM_TICK_SOFTWARE must be 1 when CONFIG_MODULE_INIT_PWM_SOFT is 1."
#endif

#if (CONFIG_START_SHELL == 1) &&  (CONFIG_START_SOAM == 1)
#    error "CONFIG_START_SHELL and CONFIG_START_SOAM cannot both be set to 1."
#endif

#endif
