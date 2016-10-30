Shell
=====

About
-----

Use the serial port to monitor and control the application.

Source code
-----------

.. include:: shell/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/shell` folder.

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/shell
   $ make -s BOARD=<board> upload

Communicate with the board using a serial terminal emulator, for
example `TeraTerm`.

Type ``hello_world`` in the terminal emulator and press Enter. ``Hello
World!`` is printed.

Press Tab to print a list of all registered commands and try them if
you want to.

.. code-block:: text

   $ hello_world
   Hello World!
   $ <tab>
   drivers/
   filesystems/
   hello_world
   help
   history
   kernel/
   logout
   oam/
   $ kernel/thrd/list 
               NAME        STATE  PRIO   CPU  MAX-STACK-USAGE  LOGMASK
              shell      current     0    0%       358/  5575     0x0f
               idle        ready   127    0%        57/   156     0x0f
   $ 
