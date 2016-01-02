Hello World
===========

Source code: :source:`examples/hello_world/main.c`

------------------------------------------------

About
-----

This application prints "Hello world!" on the standard output (serial
port).

Source code
-----------

.. include:: hello_world/source-code.rst

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/hello_world
   $ make -s BOARD=<board> release size run
   ...
   Hello world!
   $
