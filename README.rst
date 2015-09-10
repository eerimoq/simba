|buildstatus|_

OVERVIEW
--------

Simba is a microkernel and build framework.

* threads
* timers
* channels
* counting semaphores
* drivers (spi, uart, ...)
* shell
* file system
* portable (linux, AVR, ...)
* modular
* make based framework

'Hello World' application printing "Hello World!" once every second. A config.h
and a Makefile is also needed. See 'examples/hello_world' folder for complete application.
Build and run with 'make run' (under 'examples/hello_world').

    #include "simba.h"
    
    int main()
    {
        struct time_t timeout = {
            .seconds = 1,
            .nanoseconds = 0
        };
    
        sys_start();
        
        while (1) {
            std_printf(FSTR("Hello world!\n"));
            thrd_suspend(&timeout);
        }
        
        return (0);
    }

FILE TREE
---------

    -- {root}                       - this directory
       +-- examples/                - examples applications
       +-- make/                    - build and run files
       +-- README
       +-- src/                     - source directory
       |   +-- boards/              - board configurations
       |   +-- drivers/             - drivers package with ports
       |   +-- kernel/              - kernel package with ports
       |   +-- slib/                - slib package
       |   +-- mcus/                - mcu configurations
       |   +-- simba.h              - includes kernel, drivers and slib headers
       +-- tst/                     - test suites
           +-- drivers/             - drivers suites
           +-- kernel/              - kernel suites

PREREQUISITES
-------------

* linux environment
* GNU make 3.81
* python 2.7
* GNU toolchain with c compiler and linker for target architecture(s)
* gcc
* bash
* For AVR: sudo apt-get install avrdude ckermit gcc-avr binutils-avr gdb-avr avr-libc

BUILD AND RUN TESTS
-------------------

Build for default board, given in application Makefile. Often the default board
is a linux simulation.

    $ pwd
    /home/erik/archive/simba/tst/kernel/sys
    $ make test

To build for another board, in this case Arduino Nano. This overrides
the default board.

    $ make BOARD=arduino_nano release size test

Note: All application does not support every kind of board.

APPLICATIONS, PACKAGES AND MODULES
----------------------------------

A module is normally a header file and a source file. A package is a container of
modules. An application is an executable consisting of zero or more modules.

Preferred application file tree:

    -- myapplication
       +-- config.h
       +-- main.c
       +-- Makefile

Preferred package file tree:

    -- mypackage
       +-- mypackage
           +-- module1.h
           +-- module2.h
       +-- module1.c
       +-- module2.c

THREADS
-------

A thread is the basic execution entity. A scheduler controls the execution of
threads.

A simple thread that waits to be resumed by another thread.

    #include "simba.h"

    void *mythread_entry(void *arg)
    {
        UNUSED(arg);

        while (1) {
            thrd_suspend(NULL);
            printf("Thread resumed.\n");
        }

        return (NULL);
    }

Threads usually communicates over channels. There are three kinds of channels;
queue, event and sock. All implementing the same abstract channel interface.
This makes channel very powerful as a synchronization primitive. They can be
seen as file descriptors in linux.

The most common channel is the queue. It can be either synchronous or
semi-asynchronous. In the synchronous version the writing thread will
block until all written data has been read by the reader. In the
semi-asynchronous version the writer writes to a buffer within the
queue, and only blocks all data does not fit in the buffer. The buffer
size is chosen by the application.

The sock channel is used for internet protocols (not yet implemented).

DRIVERS
-------

Typical thread-driver interaction template code. Call mydrv_write() to start
a driver operation. The driver sends a message to the hardware and suspends
current thread. The hardware sends an interrupt and the calling thread is
resumed. Driver operation complete. Use counting semaphores if atomic access
to the device is required (not included in the example). A queue is also
an alternative, in particular for streaming devices like UART.

    int mydrv_exec(struct mydrv_t *drv)
    {
        int err = 0;
        
        drv_p->thrd_p = thrd_self();
        
        sys_lock();

        // 1. send something to the hardware
        
        // 2. wait for response
        thrd_suspend_irq(NULL);
        sys_unlock();
        
        // 4. prepare result
        
        return (err);
    }

    ISR(dev_vect)
    {
        drv_p = device[0].drv_p;

        // 3. resume the suspended thread
        thrd_resume_irq(drv_p->thrd_p, 0);
    }

.. |buildstatus| image:: https://travis-ci.org/eerimoq/simba.svg
.. _buildstatus: https://travis-ci.org/eerimoq/simba
