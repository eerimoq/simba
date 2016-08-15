Timer
=====

About
-----

Start a periodic timer that writes an event to the main thread. The
main thread reads the event and prints "timeout" to the standard
output.

Source code
-----------

.. include:: timer/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/timer` folder.

Build and run
-------------

Build and upload the application.

.. code-block:: text

   $ cd examples/timer
   $ make -s BOARD=<board> run
   timeout
   timeout
   timeout
