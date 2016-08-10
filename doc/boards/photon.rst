Particle IO Photon
==================

Homepage
--------

https://docs.particle.io/datasheets/photon-datasheet/

Pinout
------

.. image:: ../images/boards/photon-pinout.png
   :width: 50%
   :target: ../_images/photon-pinout.png

Default system features
-----------------------

Here is a list of enabled higher level features for this board, all
initialized in ``sys_start()``:

- :doc:`Console.<../library-reference/oam/console>`
- :doc:`Debug shell.<../library-reference/oam/shell>`

Drivers
-------

Supported drivers.

- :doc:`../library-reference/drivers/bcm43362`
- :doc:`../library-reference/drivers/flash`
- :doc:`../library-reference/drivers/pin`
- :doc:`../library-reference/drivers/sdio`
- :doc:`../library-reference/drivers/uart`

Mcu
---

:doc:`stm32f205rg<../library-reference/mcus/stm32f205rg>`

Default configuration
---------------------

Default Standard Library configuration.

+------------------------------------------------------+-----------------------------------------------------+
|  Name                                                |  Value                                              |
+======================================================+=====================================================+
|  CONFIG_ASSERT_                                      |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_DEBUG_                                       |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_DS18B20_LIST_                         |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_FS_COUNTERS_LIST_                     |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_FS_COUNTERS_RESET_                    |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_FS_FILESYSTEMS_APPEND_                |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_FS_FILESYSTEMS_LIST_                  |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_FS_FILESYSTEMS_READ_                  |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_FS_FILESYSTEMS_WRITE_                 |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_FS_PARAMETERS_LIST_                   |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_I2C_READ_                             |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_I2C_WRITE_                            |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_LOG_LIST_                             |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_LOG_PRINT_                            |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_LOG_SET_LOG_MASK_                     |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_NETWORK_INTERFACE_LIST_               |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_PIN_READ_                             |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_PIN_SET_MODE_                         |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_PIN_WRITE_                            |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_SETTINGS_LIST_                        |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_SETTINGS_READ_                        |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_SETTINGS_RESET_                       |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_SETTINGS_WRITE_                       |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_SYS_INFO_                             |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_SYS_UPTIME_                           |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_THRD_LIST_                            |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_THRD_SET_LOG_MASK_                    |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_USB_DEVICE_LIST_                      |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_FS_CMD_USB_HOST_LIST_                        |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_MONITOR_THREAD_                              |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_PREEMPTIVE_SCHEDULER_                        |  0                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_PROFILE_STACK_                               |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SETTINGS_AREA_SIZE_                          |  256                                                |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SHELL_COMMAND_MAX_                           |  64                                                 |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SHELL_HISTORY_SIZE_                          |  768                                                |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SHELL_MINIMAL_                               |  0                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SHELL_PROMPT_                                |  "$ "                                               |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SPIFFS_                                      |  0                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_CONSOLE_                               |  CONFIG_START_CONSOLE_UART                          |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_CONSOLE_DEVICE_INDEX_                  |  0                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_CONSOLE_UART_BAUDRATE_                 |  38400                                              |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_CONSOLE_USB_CDC_CONTROL_INTERFACE_     |  0                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_IN_           |  2                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_OUT_          |  3                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION_    |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_FILESYSTEM_                            |  0                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_FILESYSTEM_ADDRESS_                    |  0x000c0000                                         |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_FILESYSTEM_SIZE_                       |  32768                                              |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_NETWORK_MANAGER_                       |  0                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_SHELL_                                 |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_SHELL_PRIO_                            |  30                                                 |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_START_SHELL_STACK_SIZE_                      |  768                                                |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_STD_OUTPUT_BUFFER_MAX_                       |  16                                                 |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SYSTEM_TICK_FREQUENCY_                       |  100                                                |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_SYS_CONFIG_STRING_                           |  1                                                  |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_USB_DEVICE_PID_                              |  0x8037                                             |
+------------------------------------------------------+-----------------------------------------------------+
|  CONFIG_USB_DEVICE_VID_                              |  0x2341                                             |
+------------------------------------------------------+-----------------------------------------------------+


Library Reference
-----------------

Read more about board specific functionality in the :doc:`Particle IO Photon
<../library-reference/boards/photon>` module documentation in the
Library Reference.

.. include:: extra/photon.rst

.. _CONFIG_ASSERT: ../user-guide/configuration.html#c.CONFIG_ASSERT

.. _CONFIG_DEBUG: ../user-guide/configuration.html#c.CONFIG_DEBUG

.. _CONFIG_FS_CMD_DS18B20_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_DS18B20_LIST

.. _CONFIG_FS_CMD_FS_COUNTERS_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_FS_COUNTERS_LIST

.. _CONFIG_FS_CMD_FS_COUNTERS_RESET: ../user-guide/configuration.html#c.CONFIG_FS_CMD_FS_COUNTERS_RESET

.. _CONFIG_FS_CMD_FS_FILESYSTEMS_APPEND: ../user-guide/configuration.html#c.CONFIG_FS_CMD_FS_FILESYSTEMS_APPEND

.. _CONFIG_FS_CMD_FS_FILESYSTEMS_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_FS_FILESYSTEMS_LIST

.. _CONFIG_FS_CMD_FS_FILESYSTEMS_READ: ../user-guide/configuration.html#c.CONFIG_FS_CMD_FS_FILESYSTEMS_READ

.. _CONFIG_FS_CMD_FS_FILESYSTEMS_WRITE: ../user-guide/configuration.html#c.CONFIG_FS_CMD_FS_FILESYSTEMS_WRITE

.. _CONFIG_FS_CMD_FS_PARAMETERS_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_FS_PARAMETERS_LIST

.. _CONFIG_FS_CMD_I2C_READ: ../user-guide/configuration.html#c.CONFIG_FS_CMD_I2C_READ

.. _CONFIG_FS_CMD_I2C_WRITE: ../user-guide/configuration.html#c.CONFIG_FS_CMD_I2C_WRITE

.. _CONFIG_FS_CMD_LOG_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_LOG_LIST

.. _CONFIG_FS_CMD_LOG_PRINT: ../user-guide/configuration.html#c.CONFIG_FS_CMD_LOG_PRINT

.. _CONFIG_FS_CMD_LOG_SET_LOG_MASK: ../user-guide/configuration.html#c.CONFIG_FS_CMD_LOG_SET_LOG_MASK

.. _CONFIG_FS_CMD_NETWORK_INTERFACE_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_NETWORK_INTERFACE_LIST

.. _CONFIG_FS_CMD_PIN_READ: ../user-guide/configuration.html#c.CONFIG_FS_CMD_PIN_READ

.. _CONFIG_FS_CMD_PIN_SET_MODE: ../user-guide/configuration.html#c.CONFIG_FS_CMD_PIN_SET_MODE

.. _CONFIG_FS_CMD_PIN_WRITE: ../user-guide/configuration.html#c.CONFIG_FS_CMD_PIN_WRITE

.. _CONFIG_FS_CMD_SETTINGS_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_SETTINGS_LIST

.. _CONFIG_FS_CMD_SETTINGS_READ: ../user-guide/configuration.html#c.CONFIG_FS_CMD_SETTINGS_READ

.. _CONFIG_FS_CMD_SETTINGS_RESET: ../user-guide/configuration.html#c.CONFIG_FS_CMD_SETTINGS_RESET

.. _CONFIG_FS_CMD_SETTINGS_WRITE: ../user-guide/configuration.html#c.CONFIG_FS_CMD_SETTINGS_WRITE

.. _CONFIG_FS_CMD_SYS_INFO: ../user-guide/configuration.html#c.CONFIG_FS_CMD_SYS_INFO

.. _CONFIG_FS_CMD_SYS_UPTIME: ../user-guide/configuration.html#c.CONFIG_FS_CMD_SYS_UPTIME

.. _CONFIG_FS_CMD_THRD_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_THRD_LIST

.. _CONFIG_FS_CMD_THRD_SET_LOG_MASK: ../user-guide/configuration.html#c.CONFIG_FS_CMD_THRD_SET_LOG_MASK

.. _CONFIG_FS_CMD_USB_DEVICE_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_USB_DEVICE_LIST

.. _CONFIG_FS_CMD_USB_HOST_LIST: ../user-guide/configuration.html#c.CONFIG_FS_CMD_USB_HOST_LIST

.. _CONFIG_MONITOR_THREAD: ../user-guide/configuration.html#c.CONFIG_MONITOR_THREAD

.. _CONFIG_PREEMPTIVE_SCHEDULER: ../user-guide/configuration.html#c.CONFIG_PREEMPTIVE_SCHEDULER

.. _CONFIG_PROFILE_STACK: ../user-guide/configuration.html#c.CONFIG_PROFILE_STACK

.. _CONFIG_SETTINGS_AREA_SIZE: ../user-guide/configuration.html#c.CONFIG_SETTINGS_AREA_SIZE

.. _CONFIG_SHELL_COMMAND_MAX: ../user-guide/configuration.html#c.CONFIG_SHELL_COMMAND_MAX

.. _CONFIG_SHELL_HISTORY_SIZE: ../user-guide/configuration.html#c.CONFIG_SHELL_HISTORY_SIZE

.. _CONFIG_SHELL_MINIMAL: ../user-guide/configuration.html#c.CONFIG_SHELL_MINIMAL

.. _CONFIG_SHELL_PROMPT: ../user-guide/configuration.html#c.CONFIG_SHELL_PROMPT

.. _CONFIG_SPIFFS: ../user-guide/configuration.html#c.CONFIG_SPIFFS

.. _CONFIG_START_CONSOLE: ../user-guide/configuration.html#c.CONFIG_START_CONSOLE

.. _CONFIG_START_CONSOLE_DEVICE_INDEX: ../user-guide/configuration.html#c.CONFIG_START_CONSOLE_DEVICE_INDEX

.. _CONFIG_START_CONSOLE_UART_BAUDRATE: ../user-guide/configuration.html#c.CONFIG_START_CONSOLE_UART_BAUDRATE

.. _CONFIG_START_CONSOLE_USB_CDC_CONTROL_INTERFACE: ../user-guide/configuration.html#c.CONFIG_START_CONSOLE_USB_CDC_CONTROL_INTERFACE

.. _CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_IN: ../user-guide/configuration.html#c.CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_IN

.. _CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_OUT: ../user-guide/configuration.html#c.CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_OUT

.. _CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION: ../user-guide/configuration.html#c.CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION

.. _CONFIG_START_FILESYSTEM: ../user-guide/configuration.html#c.CONFIG_START_FILESYSTEM

.. _CONFIG_START_FILESYSTEM_ADDRESS: ../user-guide/configuration.html#c.CONFIG_START_FILESYSTEM_ADDRESS

.. _CONFIG_START_FILESYSTEM_SIZE: ../user-guide/configuration.html#c.CONFIG_START_FILESYSTEM_SIZE

.. _CONFIG_START_NETWORK_MANAGER: ../user-guide/configuration.html#c.CONFIG_START_NETWORK_MANAGER

.. _CONFIG_START_SHELL: ../user-guide/configuration.html#c.CONFIG_START_SHELL

.. _CONFIG_START_SHELL_PRIO: ../user-guide/configuration.html#c.CONFIG_START_SHELL_PRIO

.. _CONFIG_START_SHELL_STACK_SIZE: ../user-guide/configuration.html#c.CONFIG_START_SHELL_STACK_SIZE

.. _CONFIG_STD_OUTPUT_BUFFER_MAX: ../user-guide/configuration.html#c.CONFIG_STD_OUTPUT_BUFFER_MAX

.. _CONFIG_SYSTEM_TICK_FREQUENCY: ../user-guide/configuration.html#c.CONFIG_SYSTEM_TICK_FREQUENCY

.. _CONFIG_SYS_CONFIG_STRING: ../user-guide/configuration.html#c.CONFIG_SYS_CONFIG_STRING

.. _CONFIG_USB_DEVICE_PID: ../user-guide/configuration.html#c.CONFIG_USB_DEVICE_PID

.. _CONFIG_USB_DEVICE_VID: ../user-guide/configuration.html#c.CONFIG_USB_DEVICE_VID
