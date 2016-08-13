.. code-block:: c

   /**
    * @file main.c
    * @version 6.0.0
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
   
   #if !defined(BOARD_ARDUINO_DUE)
   #    error "This example can only be built for Arduino Due."
   #endif
   
   /**
    * Increment the counter in 'counter.txt'.
    */
   static int increment_counter(void)
   {
       char buf[32];
       struct fs_file_t file;
       long counter;
       size_t size;
       
       std_printf(FSTR("Incrementing the counter in 'counter.txt'.\r\n"));
           
       if (fs_open(&file, "counter.txt", FS_RDWR) != 0) {
           /* Create the file if missing. */
           if (fs_open(&file,
                       "counter.txt",
                       FS_CREAT | FS_TRUNC | FS_RDWR) != 0) {
               return (-1);
           }
   
           if (fs_write(&file, "0", 2) != 2) {
               return (-2);
           }
   
           if (fs_seek(&file, 0, FS_SEEK_SET) != 0) {
               return (-3);
           }
       }
   
       if (fs_read(&file, buf, 16) <= 0) {
           return (-4);
       }
   
       if (std_strtol(buf, &counter) == NULL) {
           return (-5);
       }
   
       /* Increment the counter. */
       counter++;
       std_sprintf(buf, FSTR("%lu"), counter);
       size = strlen(buf) + 1;
   
       if (fs_seek(&file, 0, FS_SEEK_SET) != 0) {
           return (-6);
       }
   
       if (fs_write(&file, buf, size) != size) {
           return (-7);
       }
   
       if (fs_close(&file) != 0) {
           return (-8);
       }
   
       std_printf(FSTR("Counter incremented to %lu\r\n"), counter);
       
       return (0);
   }
   
   int main()
   {
       int res;
   
       sys_start();
       std_printf(sys_get_info());
   
       /* Increment the counter. */
       res = increment_counter();
   
       if (res != 0) {
           std_printf(FSTR("Failed to increment the counter with error %d.\r\n"),
                      res);
       }
   
       /* The shell thread is started in sys_start() so just suspend this
          thread. */
       thrd_suspend(NULL);
       
       return (0);
   }

