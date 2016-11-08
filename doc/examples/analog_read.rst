Analog Read
===========

About
-----

Read the value of an analog pin periodically once every second and
print the read value to standard output.

.. raw:: html

   <iframe width="560" height="315" src="https://www.youtube.com/embed/tEwoxvMdL0c" frameborder="0" allowfullscreen></iframe>
   </br>
   </br>

Source code
-----------

.. include:: analog_read/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/analog_read` folder.

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/analog_read
   $ make -s BOARD=<board> run
   value = 234
   value = 249
   value = 230
