Developer Guide
===============

This guide is intended for developers of the Simba OS. Users are
advised to read the :doc:`user-guide` instead.

File tree
---------

.. code-block:: text

    simba                    - this directory
    ├── examples             - example applications
    ├── make                 - build and run files
    ├── README.rst
    ├── src                  - source code directory
    │   ├── boards           - board configurations
    │   ├── drivers          - drivers package source code
    │   ├── kernel           - kernel package source code
    │   ├── slib             - slib (`Simba` lib) package source code
    │   ├── mcus             - mcu configurations
    │   └── simba.h          - includes kernel, drivers and slib headers
    └── tst                  - test suites
        ├── drivers          - drivers package test suites
        ├── kernel           - kernel package test suites
        └── slib             - slib package test suites

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
