.. code-block:: c

   /**
    * @section License
    *
    * This is free and unencumbered software released into the public domain.
    *
    * Anyone is free to copy, modify, publish, use, compile, sell, or
    * distribute this software, either in source code form or as a compiled
    * binary, for any purpose, commercial or non-commercial, and by any
    * means.
    *
    * In jurisdictions that recognize copyright laws, the author or authors
    * of this software dedicate any and all copyright interest in the
    * software to the public domain. We make this dedication for the benefit
    * of the public at large and to the detriment of our heirs and
    * successors. We intend this dedication to be an overt act of
    * relinquishment in perpetuity of all present and future rights to this
    * software under copyright law.
    *
    * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    * OTHER DEALINGS IN THE SOFTWARE.
    *
    * For more information, please refer to <http://unlicense.org/>
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

