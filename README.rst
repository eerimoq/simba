About
=====

Simba is a microkernel and build framework. It aims to make embedded
programming easy and portable.

Features:

* threads
* channels for communication between threads
* timers
* counting semaphores
* drivers (spi, uart, ...)
* a simple shell
* portable (linux, Arduino Due, Arduino Mega, ...)
* make based build framework

Source code repository: http://github.com/eerimoq/simba
Installation
============

Windows
-------

Download cygwin from http://cygwin.com and select the following
packages during the installation:

* gcc-core
* make
* python
* python-setuptools
* git
* gitk

Start Cygwin execute the following commands to download all software
and setup the environment. Note that ``source cygwin_setup.sh`` has to
be executed everytime you start a new Cygwin shell.

.. code-block:: c

   $ easy_install-2.7 pip
   $ pip install pyserial
   $ pip install xpect
   $ git clone https://github.com/eerimoq/simba
   $ git clone https://github.com/eerimoq/avr-toolchain-windows
   $ git clone https://github.com/eerimoq/arm-toolchain-windows
   $ cd simba
   $ source cygwin_setup.sh

Linux (Ubuntu)
--------------

.. code-block:: c

   $ git clone https://github.com/eerimoq/simba
   $ sudo apt-get install ckermit valgrind cppcheck cloc python
   $ sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc
   $ sudo apt-get install bossa-cli gcc-arm-none-eabi
User Guide
==========

This guide requires that you have Simba installed. See the
:doc:`installation` page for details.


Hello World application
-----------------------

Below is the Simba "Hello World" application.

It consistion of two files; main.c and Makefile.

main.c
~~~~~~

.. code-block:: c

   #include "simba.h"

   int main()
   {
       sys_start();

       std_printf(FSTR("Hello world!\n"));

       return (0);
   }

Makefile
~~~~~~~~

.. code-block:: c

   NAME = hello_world
   BOARD = linux

   RUN_END_PATTERN = "Hello world!"
   RUN_END_PATTERN_SUCCESS = "Hello world!"

   SIMBA = ../..
   include $(SIMBA)/make/app.mk

Build and run
~~~~~~~~~~~~~

Compile, link and run it by typing the commands below in a shell:

.. code-block:: c

   $ cd examples/hello_world
   $ make -s clean run
   <build system output>
   Hello world!
   $

Cross-compile, link and then run on an Arduino Due:

.. code-block:: c

   $ cd examples/hello_world
   $ make -s BOARD=arduino_due clean run
   <build system output>
   Hello world!
   $

Applications, packages and modules
----------------------------------

A module is normally a header and a source file. A package is a
container of modules. An application is an executable consisting of
zero or more modules.

See below for the preferred application file tree. The application
**must** have a file called main.c. It should contain the main
function of the application.

.. code-block:: c

    -- myapplication
       +-- config.h
       +-- main.c
       +-- Makefile

For a package, the preferred file tree is:

.. code-block:: c

    -- mypackage
       +-- mypackage
           +-- module1.h
           +-- module2.h
       +-- module1.c
       +-- module2.c

Boards and mcus
---------------

A board is the top level configuration entity in the build
framework. It contains information about the MCU and the pin mapping.

In turn, the MCU contains information about available devices and
clock frequencys in the microcontroller.

See `src/boards`_ and `src/mcus`_ for available configurations.

Only one MCU per board is supported. If there are two MCU:s on one
physical board, two board configurations have to be created, one for
each MCU.

Threads and channels
--------------------

A thread is the basic execution entity. A scheduler controls the
execution of threads.

A simple thread that waits to be resumed by another thread.

.. code-block:: c

    #include "simba.h"

    void *my_thread_main(void *arg_p)
    {
        UNUSED(arg_p);

        while (1) {
            thrd_suspend(NULL);
            printf("Thread resumed.\n");
        }

        return (NULL);
    }

Threads usually communicates over channels. There are two kinds of
channels; queue and event. Both implementing the same abstract channel
interface (see `kernel/chan.h`_).  This abstraction makes channel
very powerful as a synchronization primitive. They can be seen as
limited functionality file descriptors in linux.

The most common channel is the queue. It can be either synchronous or
semi-asynchronous. In the synchronous version the writing thread will
block until all written data has been read by the reader. In the
semi-asynchronous version the writer writes to a buffer within the
queue, and only blocks all data does not fit in the buffer. The buffer
size is selected by the application.

.. _src/boards: https://github.com/eerimoq/simba/tree/master/src/boards
.. _src/mcus: https://github.com/eerimoq/simba/tree/master/src/mcus
.. _kernel/chan.h: https://github.com/eerimoq/simba/tree/master/src/kernel/kernel/chan.h
Developer Guide
===============

File tree
---------

.. code-block:: c

    -- {root}                       - this directory
       +-- examples/                - example applications
       +-- make/                    - build and run files
       +-- README
       +-- src/                     - source code directory
       |   +-- boards/              - board configurations
       |   +-- drivers/             - drivers package source code
       |   +-- kernel/              - kernel package source code
       |   +-- slib/                - slib (Simba lib) package source code
       |   +-- mcus/                - mcu configurations
       |   +-- simba.h              - includes kernel, drivers and slib headers
       +-- tst/                     - test suites
           +-- drivers/             - drivers package test suites
           +-- kernel/              - kernel package test suites
           +-- slib/                - slib package test suites
