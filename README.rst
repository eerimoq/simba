|buildstatus|_

.. image:: doc/images/Simba-Logo-50.png

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

Documentation: http://simba-os.readthedocs.org/en/latest

Prerequisites
=============

Required:

* GNU make 3.81
* python 2.7
* GNU toolchain with c compiler and linker for target architecture(s)
* gcc
* bash

Optional:

* valgrind
* cppcheck

AVR
---
``sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc``

ARM
---
``sudo apt-get install bossa-cli gcc-arm-none-eabi``

TOOLS
-----
``sudo apt-get install ckermit valgrind cppcheck cloc``

Hello World
===========

Below is the simba "Hello World" application, printing "Hello World!"
once every second. See ``examples/hello_world`` folder for the
complete application. Build and run with ``make run`` (under
``examples/hello_world``).

.. code-block:: c

    #include "simba.h"

    int main()
    {
        sys_start();

        std_printf(FSTR("Hello world!\n"));

        return (0);
    }

File tree
=========

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

Applications, packages and modules
==================================

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
===============

A board is the top level configuration entity in the build
framework. It contains information about the MCU and the pin mapping.

In turn, the MCU contains information about available devices and
clock frequencys in the microcontroller.

See ``src/boards`` and ``src/mcus`` for available configurations.

Only one MCU per board is supported. If there are two MCU:s on one
physical board, two board configurations have to be created, one for
each MCU.

Build and run tests
===================

This is how to build an application for the default board, given in
the application Makefile. Often the default board is the linux simulator.

.. code-block:: c

    $ pwd
    /home/erik/workspace/simba/tst/kernel/sys
    $ make -s test

To build the same application for another board, in this case Arduino
Nano, use the BOARD make variable.

.. code-block:: c

    $ make -s BOARD=arduino_nano release test

Note: An application may support only a subset of the boards defined
in Simba.

Threads and channels
====================

A thread is the basic execution entity. A scheduler controls the
execution of threads.

A simple thread that waits to be resumed by another thread.

.. code-block:: c

    #include "simba.h"

    void *mythread_entry(void *arg_p)
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
interface (see ``kernel/chan.h``).  This abstraction makes channel
very powerful as a synchronization primitive. They can be seen as
limited functionality file descriptors in linux.

The most common channel is the queue. It can be either synchronous or
semi-asynchronous. In the synchronous version the writing thread will
block until all written data has been read by the reader. In the
semi-asynchronous version the writer writes to a buffer within the
queue, and only blocks all data does not fit in the buffer. The buffer
size is selected by the application.

Drivers
=======

Typical thread-driver interaction template code. Call mydrv_write() to
start a driver operation. The driver sends a message to the hardware
and suspends current thread. The hardware sends an interrupt and the
calling thread is resumed. Driver operation complete. Use counting
semaphores if atomic access to the device is required (not included in
the example). A queue is also an alternative, in particular for
streaming devices like UART.

.. code-block:: c

    int mydrv_exec(struct mydrv_t *drv)
    {
        int err = 0;

        drv_p->thrd_p = thrd_self();

        sys_lock();

        /* 1. send something to the hardware */

        /* 2. wait for response from interrupt handler */
        thrd_suspend_irq(NULL);
        sys_unlock();

        /* 4. prepare result */

        return (err);
    }

    ISR(dev_vect)
    {
        drv_p = device[0].drv_p;

        /* 3. Resume the suspended thread. */
        thrd_resume_irq(drv_p->thrd_p, 0);
    }

.. |buildstatus| image:: https://travis-ci.org/eerimoq/simba.svg
.. _buildstatus: https://travis-ci.org/eerimoq/simba
