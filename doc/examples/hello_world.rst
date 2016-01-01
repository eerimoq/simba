:mod:`hello_world` --- Hello World
==================================

Source code: :source:`examples/hello_world/main.c`

Boards: All.

------------------------------------------------

About
-----

This application prints "Hello World" on the standard output (serial
port).

Source code
-----------

.. code-block:: c

   /**
    * @file main.c
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

   #include "simba.h"

   int main()
   {
       struct uart_driver_t uart;

       sys_start();

       uart_module_init();
       uart_init(&uart, &uart_device[0], 38400, NULL, 0);
       uart_start(&uart);
       sys_set_stdout(&uart.chout);

       std_printf(FSTR("Hello world!\n"));

       return (0);
   }



Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/hello_world
   $ make -s BOARD=<board> release size run
   ...
   Hello World!
   $
