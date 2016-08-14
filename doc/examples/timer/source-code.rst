.. code-block:: c

   /**
    * @file main.c
    * @version 6.0.0
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
   
   #define TIMEOUT_EVENT    0x1
   
   static struct event_t event;
   static struct timer_t timer;
   
   static void timer_cb(void *arg_p)
   {
       uint32_t mask;
   
       mask = TIMEOUT_EVENT;
       event_write_isr(&event, &mask, sizeof(mask));
   }
   
   int main()
   {
       uint32_t mask;
       struct time_t timeout;
   
       sys_start();
       event_init(&event);
   
       /* Initialize and start a periodic timer. */
       timeout.seconds = 1;
       timeout.nanoseconds = 0;
       timer_init(&timer, &timeout, timer_cb, NULL, TIMER_PERIODIC);
       timer_start(&timer);
       
       while (1) {
           mask = TIMEOUT_EVENT;
           event_read(&event, &mask, sizeof(mask));
   
           std_printf(FSTR("timeout\r\n"));
       }
       
       return (0);
   }

