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
   
   /* Define a shell command. */
   static struct fs_command_t tmp_foo;
   
   /* Define two counters. */
   static struct fs_counter_t bar;
   static struct fs_counter_t fie;
   
   static struct shell_t shell;
   
   /**
    * The shell command callback for "/tmp/foo".
    */
   static int tmp_foo_cb(int argc,
                         const char *argv[],
                         chan_t *out_p,
                         chan_t *in_p,
                         void *arg_p,
                         void *call_arg_p)
   {
       if (argc != 4) {
           std_fprintf(out_p, FSTR("3 arguments required.\r\n"));
   
           return (1);
       }
   
       /* Write the result to the shell output channel. */
       std_fprintf(out_p,
                   FSTR("argc = %d, argv[0] = %s, argv[1] = %s, "
                        "argv[2] = %s, argv[3] = %s\r\n"),
                   argc,
                   argv[0],
                   argv[1],
                   argv[2],
                   argv[3]);
   
       return (0);
   }
   
   int main()
   {
       /* Start the system. */
       sys_start();
   
   #if defined(__DRIVERS_I2C_H__)
       i2c_module_init();
   #endif
   
       pin_module_init();
   
       /* Register a shell command. */
       fs_command_init(&tmp_foo, FSTR("/tmp/foo"), tmp_foo_cb, NULL);
       fs_command_register(&tmp_foo);
   
       /* Register a few counters. */
       fs_counter_init(&bar, FSTR("/bar"), 0);
       fs_counter_register(&bar);
       fs_counter_init(&fie, FSTR("/fie"), 1);
       fs_counter_register(&fie);
   
       /* Increment coutner bar. */
       fs_counter_increment(&bar, 123);
   
       /* Print the system information. */
       std_printf(sys_get_info());
   
       /* Call the shell main function. */
       shell_init(&shell,
                  sys_get_stdin(),
                  sys_get_stdout(),
                  NULL,
                  NULL,
                  NULL,
                  NULL);
       shell_main(&shell);
   
       return (0);
   }

