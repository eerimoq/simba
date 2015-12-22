User Guide
==========

This guide assumes that you have Simba installed. See the
:doc:`installation` page for details.


Hello World application
-----------------------

Below is the simba "Hello World" application.

.. code-block:: c

    #include "simba.h"

    int main()
    {
        sys_start();

        std_printf(FSTR("Hello world!\n"));

        return (0);
    }

Compile, link and run it by typing the commands below in a shell:

.. code-block:: c

   $ cd examples/hello_world
   $ make -s clean run
   <build system output>
   Hello world!
   $

Cross-compilation for Arduino Due:

.. code-block:: c

   $ cd examples/hello_world
   $ make -s BOARD=arduino_due clean run
   <build system output>
   Hello world!
   $
