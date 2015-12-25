User Guide
==========

This guide requires that you have Simba installed. See the
:doc:`installation` page for details.

.. contents::
   :local:

Hello World application
-----------------------

Below is the Simba "Hello World" application.

It consistion of two files; ``main.c`` and ``Makefile``.

main.c
~~~~~~

`hello_world/main.c`_ defines the application entry function ``main()``.

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

`hello_world/Makefile`_ contains build configuration of the
application.

.. code-block:: makefile

   NAME = hello_world
   BOARD = linux

   RUN_END_PATTERN = "Hello world!"
   RUN_END_PATTERN_SUCCESS = "Hello world!"

   SIMBA_ROOT = ../..
   include $(SIMBA_ROOT)/make/app.mk

Build and run
~~~~~~~~~~~~~

Compile, link and run it by typing the commands below in a shell:

.. code-block:: text

   $ cd examples/hello_world
   $ make -s clean run
   <build system output>
   Hello world!
   $

Cross-compile, link and then run on an Arduino Due:

.. code-block:: text

   $ cd examples/hello_world
   $ make -s BOARD=arduino_due clean run
   <build system output>
   Hello world!
   $

Applications, packages and modules
----------------------------------

Simba has three software components; the application, the package and
the module.

Application
~~~~~~~~~~~

An application is an executable consisting of zero or more packages.

See below for the application file tree. The application **must** have
a file called main.c. It should contain the application entry function
``main()``.

.. code-block:: text

    my_application
    ├── config.h
    ├── main.c                   # application entry
    └── Makefile

Makefile
^^^^^^^^

Package
~~~~~~~

A package is a container of modules.

See below for the package file tree. See the inline comments for
details about files and folders contents.

.. code-block:: text

   mypkg
   ├── mypkg
   │   ├── doc                   # package documentation
   │   ├── __init__.py
   │   ├── src                   # package source code
   │   │   ├── module1.c
   │   │   ├── module2.c
   │   │   ├── mypkg             # module header files
   │   │   │   ├── module1.h
   │   │   │   └── module2.h
   │   │   ├── mypkg.h           # package header file
   │   │   └── mypkg.mk
   │   └── tst                   # package test code
   │       ├── module1
   │       │   ├── main.c
   │       │   └── Makefile
   │       └── module2
   │           ├── main.c
   │           └── Makefile
   ├── setup.py
   └── setup.sh                  # package setup script

All exported symbols in a package must have the prefix
``<package>_<module>_``. This is needed to avoid namespace clashes
between modules with the same name, present in multiple packages.

There cannot be two packages with the same name, for the namespace
reason. All packages must have unique names!

There is one exception though, the three Simba packages; kernel,
drivers and slib. Those packages does only use the module as prefix on
exported symbols.

.. code-block:: c

    int mypackage_module1_foo(void);

    int mypackage_module2_bar(void);

mypkg.mk
^^^^^^^^

Module
~~~~~~

A module is normally a header and a source file. 

simba
-----

The program `simba` is used to manage Simba packages and applications.

The main purpose of `simba` is to distribute software in the Simba
community, just like `pip` for Python.

How to create a package
~~~~~~~~~~~~~~~~~~~~~~~

This shows how to create a new package using `simba`.

.. code-block:: text

   $ simba create --package mypkg
   $ tree mypkg
   mypkg/
   ├── mypkg
   │   ├── doc
   │   ├── __init__.py
   │   ├── src
   │   │   ├── example.c
   │   │   ├── mypkg
   │   │   │   └── example.h
   │   │   ├── mypkg.h
   │   │   └── mypkg.mk
   │   └── tst
   │       └── example
   │           ├── main.c
   │           └── Makefile
   ├── setup.py
   └── setup.sh
   $ cd mypkg
   $ source setup.sh
   $ cd mypkg/tst/example
   $ make -s test

In the output from ``tree mypkg`` below, two files may catch your
eyes; setup.py and __init__.py. Those are Python files and are often
seen in Python packages. They are present in a Simba package because
Simba uses the Python tool `pip` to release and install
packages. The idea is that everyone that implements a useful package
should release it and make it available for other users to install,
just as Python!

How to release a package
~~~~~~~~~~~~~~~~~~~~~~~~

This is how to release a package. Two files are creted, one wth the
suffix ``tar.gz`` and one with the suffix ``.whl``. The ``.whl``-file
is input to the installation command, described in the next section.

.. code-block:: text

   $ cd ../../..
   $ simba release
   $ tree dist
   dist
   ├── mypkg-0.1-py2.py3-none-any.whl
   └── mypkg-0.1.tar.gz
   $

How to install a package
~~~~~~~~~~~~~~~~~~~~~~~~

This is how to install a package in ``${SIMBA_ROOT}/dist-packages``.

.. code-block:: text

   $ simba install dist/mypkg-0.1-py2.py3-none-any.whl

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

.. _src/boards: https://github.com/eerimoq/simba/tree/master/src/boards
.. _src/mcus: https://github.com/eerimoq/simba/tree/master/src/mcus
.. _kernel/chan.h: https://github.com/eerimoq/simba/tree/master/src/kernel/kernel/chan.h

.. _hello_world/main.c: https://github.com/eerimoq/simba/tree/master/examples/hello_world/main.c
.. _hello_world/Makefile: https://github.com/eerimoq/simba/tree/master/examples/hello_world/Makefile
