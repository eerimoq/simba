.. code-block:: c

   /**
    * @file main.c
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
   
   /* Hello world command. */
   static struct fs_command_t cmd_hello_world;
   
   static struct shell_t shell;
   
   /**
    * The shell command callback for "/hello_world".
    */
   static int cmd_hello_world_cb(int argc,
                                 const char *argv[],
                                 void *out_p,
                                 void *in_p,
                                 void *arg_p,
                                 void *call_arg_p)
   {
       /* Write "Hello World!" to the output channel. */
       std_fprintf(out_p, FSTR("Hello World!\r\n"));
   
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
   
       /* Register the hello world command. */
       fs_command_init(&cmd_hello_world,
                       FSTR("/hello_world"),
                       cmd_hello_world_cb,
                       NULL);
       fs_command_register(&cmd_hello_world);
   
       /* Start the shell. */
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

