Shell
=====

Source code: :source:`examples/shell/main.c`

------------------------------------------------

About
-----

Use the serial port to monitor and control the application.

Source code
-----------

.. include:: shell/source-code.rst

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/shell
   $ make -s BOARD=<board> release size upload

Communicate with the board using a serial terminal emulator, for
example TeraTerm. The baudrate is 115200.

The shell line terminator is ``\r\n``.

.. code-block:: text

   # print the coutner values in hexadecimal form
   $ foo
   0000000000000000ffff
   $ fie
   00000000000000000001
   $ bar
   00000000004efee6b839

   # run the command "/tmp/foo" with arguments
   $ tmp/foo first second third
   argc = 4, argv[0] = tmp/foo, argv[1] = first, argv[2] = second
   $
