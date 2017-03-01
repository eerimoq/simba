.. code-block:: c

   /**
    * @section License
    *
    * The MIT License (MIT)
    *
    * Copyright (c) 2014-2016, Erik Moqvist
    *
    * Permission is hereby granted, free of charge, to any person
    * obtaining a copy of this software and associated documentation
    * files (the "Software"), to deal in the Software without
    * restriction, including without limitation the rights to use, copy,
    * modify, merge, publish, distribute, sublicense, and/or sell copies
    * of the Software, and to permit persons to whom the Software is
    * furnished to do so, subject to the following conditions:
    *
    * The above copyright notice and this permission notice shall be
    * included in all copies or substantial portions of the Software.
    *
    * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
    * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
    * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    * SOFTWARE.
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
       std_fprintf(out_p, OSTR("Hello World!\r\n"));
   
       return (0);
   }
   
   int main()
   {
       /* Start the system. */
       sys_start();
   
       std_printf(sys_get_info());
   
       /* Register the hello world command. */
       fs_command_init(&cmd_hello_world,
                       CSTR("/hello_world"),
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

