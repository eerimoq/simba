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
    * Increment the counter in 'fs/counter.txt'.
    */
   static int increment_counter(void)
   {
       char buf[32];
       struct fs_file_t file;
       long counter;
       size_t size;
       
       std_printf(FSTR("Incrementing the counter in 'fs/counter.txt'.\r\n"));
           
       if (fs_open(&file, "fs/counter.txt", FS_RDWR) != 0) {
           /* Create the file is missing. */
           if (fs_open(&file,
                       "fs/counter.txt",
                       FS_CREAT | FS_TRUNC | FS_RDWR) != 0) {
               std_printf(FSTR("Failed to open file.\r\n"));
               return (-1);
           }
   
           /* Initialize the file by writing 0 to it. */
           if (fs_write(&file, "0", 2) != 2) {
               std_printf(FSTR("Failed to write to the file.\r\n"));
               return (-1);
           }
   
           /* Set the cursor position to the beginning of the file. */
           if (fs_seek(&file, 0, FS_SEEK_SET) != 0) {
               std_printf(FSTR("Failed to seek to beginning of the file.\r\n"));
               return (-1);
           }
       }
   
       /* Read the counter value from the file. */
       if (fs_read(&file, buf, 16) <= 0) {
           std_printf(FSTR("Failed to read from the file.\r\n"));
           return (-1);
       }
   
       if (std_strtol(buf, &counter) == NULL) {
           std_printf(FSTR("Failed to parse the counter value.\r\n"));
           return (-1);
       }
   
       /* Increment the counter. */
       counter++;
       std_sprintf(buf, FSTR("%lu"), counter);
       size = strlen(buf) + 1;
   
       /* Set the cursor position to the beginning of the file. */
       if (fs_seek(&file, 0, FS_SEEK_SET) != 0) {
           std_printf(FSTR("Failed to seek to beginning of the file.\r\n"));
           return (-1);
       }
   
       /* Write the incremented counter value. */
       if (fs_write(&file, buf, size) != size) {
           std_printf(FSTR("Failed to write to the file.\r\n"));
           return (-1);
       }
   
       /* Close the file. */
       if (fs_close(&file) != 0) {
           std_printf(FSTR("Failed to close the file.\r\n"));
           return (-1);
       }
   
       std_printf(FSTR("Counter incremented to %lu\r\n"), counter);
       
       return (0);
   }
   
   int main()
   {
       /* Start the system. */
       sys_start();
   
       /* Print the system information. */
       std_printf(sys_get_info());
   
       /* Increment the counter. */
       if (increment_counter() != 0) {
           sys_stop(1);
       }
   
       /* The shell thread is started in sys_start() so just suspend this
          thread. */
       thrd_suspend(NULL);
       
       return (0);
   }

