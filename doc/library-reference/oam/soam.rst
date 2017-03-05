:mod:`soam` --- Simba OAM
=========================

.. module:: soam
   :synopsis: Simba OAM.

Simba Operation And Maintenence (SOAM) is a framed debug protocol with
enumerated format strings and file system commands. This both saves
memory and makes the communication more reliable.

Two macros are defined; ``OSTR()`` and ``CSTR()``, both required by
the SOAM build system. It is considered good practice to always use
these macros, even if SOAM is not used.

- The ``OSTR()`` macro.

  An output format string.

  .. code-block:: c

     /* Log object. */
     log_object_print(NULL, LOG_INFO, OSTR("Hello %s!\r\n"), "Erik");

     /* File system command output. */
     static int cmd_foo_cb(...)
     {
         std_fprintf(chout_p, OSTR("Foo %d!\r\n"), 1);

         return (0);
     }

     /* Regular printf. */
     std_printf(OSTR("Hello 0x%x!\r\n"), 0xbabe);

- The ``CSTR()`` macro.

  A file system command string.

  .. code-block:: c

     fs_command_init(&cmd_foo, CSTR("/foo"), cmd_foo_cb, NULL);

Usage
-----

Enable SOAM by adding ``SOAM=yes`` to the application makefile.

Connect to the board with ``soam.py`` instead of a serial terminal
program. The only required argument is the string database file.

Here is an example usage of the script. `Ctrl-D` is pressed to exit
the script.

.. code-block:: text

   > soam.py --port /dev/arduino --baudrate 115200 \
         build/arduino_due/soam.soamdb
   Welcome to the SOAM shell.

   Type help or ? to list commands.

   $ kernel/sys/info
   app:    soam-master built 2017-03-05 21:26 CET by erik.
   board:  Arduino Due
   mcu:    Atmel SAM3X8E Cortex-M3 @ 84MHz, 96k sram, 512k flash
   OK
   $ kernel/thrd/list
               NAME        STATE  PRIO   CPU   SCHEDULED  MAX-STACK-USAGE  LOGMASK
               soam      current    30    0%         112       748/  1542     0x0f
            monitor    suspended   -80    0%          22       176/   518     0x0f
               idle        ready   127   99%         594       276/   390     0x0f
               main    suspended     0    0%         305       540/ 88898     0x00
   OK
   $ kernel/thrd/set_log_mask foo 0
   ERROR(-3)
   $ <Ctrl-D>

   Bye!
   >

``OK`` is printed by the shell if the file system command returned
`zero(0)`, otherwise ``ERROR(error code)`` is printed.

----------------------------------------------

Source code: :github-blob:`src/oam/soam.h`, :github-blob:`src/oam/soam.c`

Test code: :github-blob:`tst/oam/soam/main.c`

Test coverage: :codecov:`src/oam/soam.c`

Example code: :github-blob:`examples/soam/main.c`

----------------------------------------------

.. doxygenfile:: oam/soam.h
   :project: simba
