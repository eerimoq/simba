:mod:`blink` --- Blink
======================

Source code: :source:`examples/blink/main.c`

Boards: :doc:`Arduino Due <../boards/arduino_due>`, :doc:`Arduino Nano
<../boards/arduino_nano>` and :doc:`Arduino Mega <../boards/arduino_mega>`.

------------------------------------------------

About
-----

Turn a LED on and off periodically with a one second interval.

Source code
-----------

.. code-block:: c

   /**
    * @file main.c
    * @version 0.1.0
    *
    * @section License
    * Copyright (C) 2015, Erik Moqvist
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

   #include "simba.h"

   int main()
   {
       struct pin_driver_t led;

       /* Start the kernel. */
       sys_start();

       /* Initialize the LED pin as output. */
       pin_init(&led, &pin_led_dev, PIN_OUTPUT);

       while (1) {
           /* Wait for a seconds. */
           thrd_usleep(1000000);

           /* Toggle the LED on/off. */
           pin_toggle(&led);
       }

       return (0);
   }


Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/blink
   $ make -s BOARD=<board> release size run

.. _examples/blink: https://github.com/eerimoq/simba/tree/master/examples/blink
