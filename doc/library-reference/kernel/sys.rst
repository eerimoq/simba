:mod:`sys` --- System
=====================

.. module:: sys
   :synopsis: System.

System level functionality and definitions.

Example usage
-------------

This is a small example illustrating how to start an application by
calling `sys_start()` and then print the system info and uptime on
standard output.

.. code-block:: c

   int main()
   {
       struct time_t uptime;

       sys_start();

       /* Print the system information. */
       std_printf(sys_get_info());

       /* Get the system uptime and print it. */
       sys_uptime(&uptime)
       std_printf(OSTR("System uptime: %lu s %lu ns"),
                  uptime.seconds,
                  uptime.nanoseconds);

       return (0);
   }

Debug file system commands
--------------------------

Seven debug file system commands are available, all located in the
directory ``kernel/sys/``.

+--------------------+----------------------------------------------------------------+
|  Command           | Description                                                    |
+====================+================================================================+
|  ``info``          | Print the system information.                                  |
+--------------------+----------------------------------------------------------------+
|  ``config``        | Print the build configuration (or at least part of it).        |
+--------------------+----------------------------------------------------------------+
|  ``uptime``        | Print the system uptime.                                       |
+--------------------+----------------------------------------------------------------+
|  ``panic``         | Call the panic function, stopping the system.                  |
+--------------------+----------------------------------------------------------------+
|  ``reboot``        | Reboot the system.                                             |
+--------------------+----------------------------------------------------------------+
|  ``backtrace``     | Print a backtrace.                                             |
+--------------------+----------------------------------------------------------------+
|  ``reset_cause``   | Print the reset cause.                                         |
+--------------------+----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ kenel/sys/info
   app:    shell-master built 2017-07-06 09:03 CEST by erik.
   board:  Arduino Mega
   mcu:    Atmel ATMega2560 AVR @ 16MHz, 8k sram, 256k flash

   $ kenel/sys/uptime
   0.120 seconds

   $ kenel/sys/reset_cause
   power_on

----------------------------------------------

Source code: :github-blob:`src/kernel/sys.h`, :github-blob:`src/kernel/sys.c`

Test code: :github-blob:`tst/kernel/sys/main.c`

Test coverage: :codecov:`src/kernel/sys.c`

----------------------------------------------

.. doxygenfile:: kernel/sys.h
   :project: simba
