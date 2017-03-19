:mod:`led_7seg_ht16k33` --- LED 7-Segment HT16K33
===========================

.. module:: led_7seg_ht16k33
   :synopsis: LED Stuff.

This is a driver for `Adafruit 0.56" 4-Digit 7-Segment Display w/I2C
Backpack`_ or compatible devices which uses the Holtek HT16K33 chip.

At this time the driver only supports using the :doc:`i2c_soft` driver
to communicate with the HT16K33, not the :doc:`i2c` driver.

Source code: :github-blob:`src/drivers/led_7seg_ht16k33.h`,
:github-blob:`src/drivers/led_7seg_ht16k33.c`

----------------------------------------------

.. doxygenfile:: drivers/led_7seg_ht16k33.h
   :project: simba
.. _Adafruit 0.56" 4-Digit 7-Segment Display: https://www.adafruit.com/products/878
