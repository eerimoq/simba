Analog Read
===========

Source code: :github-blob:`examples/analog_read/main.c`

------------------------------------------------

About
-----

Read the value of an analog pin periodically once every second and
print the read value to standard output.

Source code
-----------

.. include:: analog_read/source-code.rst

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/analog_read
   $ make -s BOARD=<board> run
   value = 234
   value = 249
   value = 230
