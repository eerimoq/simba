:mod:`led_7seg_ht16k33` --- LED 7-Segment HT16K33
=================================================

.. module:: led_7seg_ht16k33
   :synopsis: LED 7-Segment HT16K33

This is a driver for `Adafruit 0.56" 4-Digit 7-Segment Display w/I2C
Backpack`_ or compatible devices which uses the Holtek HT16K33 chip.

At this time the driver only supports using the
:doc:`../network/i2c_soft` driver to communicate with the HT16K33, not
the :doc:`../network/i2c` driver.

Source code: :github-blob:`src/drivers/displays/led_7seg_ht16k33.h`,
:github-blob:`src/drivers/displays/led_7seg_ht16k33.c`

----------------------------------------------

.. doxygenfile:: drivers/displays/led_7seg_ht16k33.h
   :project: simba
.. _Adafruit 0.56" 4-Digit 7-Segment Display: https://www.adafruit.com/products/878
