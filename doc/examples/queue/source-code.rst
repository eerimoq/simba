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
   
   static struct queue_t queue;
   
   static THRD_STACK(writer_stack, 256);
   
   static void *writer_main(void *arg_p)
   {
       int value;
       
       /* Write to the queue. */
       value = 1;
       queue_write(&queue, &value, sizeof(value));
   
       return (NULL);
   }
   
   int main()
   {
       int value;
   
       sys_start();
       queue_init(&queue, NULL, 0);
       thrd_spawn(writer_main, NULL, 0, writer_stack, sizeof(writer_stack));
   
       /* Read from the queue. */
       queue_read(&queue, &value, sizeof(value));
   
       std_printf(FSTR("read value = %d\r\n"), value);
   
       return (0);
   }

