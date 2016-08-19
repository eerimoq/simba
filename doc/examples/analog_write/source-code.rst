.. code-block:: c

   /**
    * @file main.c
    *
    * @section License
    * Copyright (C) 2015-2016, Erik Moqvist
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
       int value;
       struct analog_output_pin_t pin;
   
       sys_start();
       analog_output_pin_module_init();
   
       /* Initialize the analog output pin. */
       analog_output_pin_init(&pin, &pin_d10_dev);
   
       value = 0;
       
       while (1) {
           /* Write a sawtooth wave to the analog output pin. */
           analog_output_pin_write(&pin, value);
           value += 5;
           value %= 1024;
   
           /* Wait ten milliseconds. */
           thrd_sleep_us(10000);
       }
   
       return (0);
   }

