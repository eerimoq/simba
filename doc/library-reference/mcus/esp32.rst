:mod:`esp32` --- Esp32
==========================

.. module:: esp32
   :synopsis: Esp32.

I2C
---

Simba does not yet support the ESP32 hardware I2C support, but can use
the :doc:`../drivers/network/i2c_soft` driver instead. By default one
:doc:`../drivers/network/i2c` compatability device is instantiated:

====== ====== =======
Device SCL    SDA
====== ====== =======
i2c0   Pin 22 Pin 21
====== ====== =======

----------------------------------------------

Hardware reference: https://github.com/eerimoq/hardware-reference/tree/master/esp32

Source code: :github-blob:`src/mcus/esp32/mcu.h`

----------------------------------------------

.. doxygenfile:: mcus/esp32/mcu.h
   :project: simba
