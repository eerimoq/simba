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
   
   #if !defined(BOARD_ARDUINO_DUE) && !defined(ARCH_ESP) && !defined(ARCH_ESP32)
   #    error "This example can only be built for Arduino Due, ESP and ESP32."
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

