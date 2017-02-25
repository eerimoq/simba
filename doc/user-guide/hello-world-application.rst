Hello World application
=======================

Let's start with the `Simba` "Hello World" application. It examplifies
what an application is and how to build and run it.

It consists of two files; ``main.c`` and ``Makefile``.

main.c
------

:github-blob:`main.c<examples/hello_world/main.c>` defines the
application entry function ``main()``.

.. code-block:: c

   #include "simba.h"

   int main()
   {
       /* Initialize modules and start the scheduler. */
       sys_start();
   
       std_printf(FSTR("Hello world!\n"));

       return (0);
   }

Makefile
--------

:github-blob:`Makefile<examples/hello_world/Makefile>` contains build
configuration of the application.

.. code-block:: makefile

   NAME = hello_world
   BOARD ?= linux

   RUN_END_PATTERN = "Hello world!"
   RUN_END_PATTERN_SUCCESS = "Hello world!"

   SIMBA_ROOT = ../..
   include $(SIMBA_ROOT)/make/app.mk

Build and run
-------------

Compile, link and run it by typing the commands below in a shell:

.. code-block:: text

   $ cd examples/hello_world
   $ make -s run
   <build system output>
   Hello world!
   $

Cross-compile, link and then run on an Arduino Due:

.. code-block:: text

   $ cd examples/hello_world
   $ make -s BOARD=arduino_due run
   <build system output>
   Hello world!
   $
