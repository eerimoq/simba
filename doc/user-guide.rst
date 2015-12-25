User Guide
==========

This guide is intended for users of the Simba packages and tools.

The Simba installation guide can be found on the :doc:`installation`
page.

**Contents:**

.. contents::
   :local:

Environment setup
-----------------

The first step is always to setup the `Simba` environment. It's a
simple matter of sourcing a setup-script in the simba root folder.

.. code-block:: text

   $ cd simba/simba
   $ source setup.sh

Hello World application
-----------------------

Let's start with the `Simba` "Hello World" application. It examplifies
what an application is and how to build and run it.

It consistion of two files; ``main.c`` and ``Makefile``.

main.c
~~~~~~

`hello_world/main.c`_ defines the application entry function
``main()``.

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
   BOARD ?= linux

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

`Simba` has three software components; the application, the package
and the module.

Application
~~~~~~~~~~~

An application is an executable consisting of zero or more packages.

An application file tree can either be created manually or by using
the tool `simba`_. See `How to create an application`_ for a guide of
how to create an application using `simba`_.

.. code-block:: text

   myapp
   ├── main.c
   └── Makefile

Package
~~~~~~~

A package is a container of modules.

A package file tree can either be created manually or by using the
tool `simba`_. See `How to create a package`_ for a guide of how to
create a package using `simba`_.

A package file tree **must** be organized as seen below. This is
required by the build framework and `Simba` tools.

See the inline comments for details about the files and folders
contents.

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
   │   │   └── mypkg.mk          # package makefile
   │   └── tst                   # package test code
   │       ├── module1
   │       │   ├── main.c
   │       │   └── Makefile
   │       └── module2
   │           ├── main.c
   │           └── Makefile
   └── setup.py

Namespaces
^^^^^^^^^^

All exported symbols in a package must have the prefix
``<package>_<module>_``. This is needed to avoid namespace clashes
between modules with the same name in different packages.

There cannot be two packages with the same name, for the namespace
reason. All packages must have unique names! There is one exception
though, the three `Simba` packages; kernel, drivers and slib. Those
packages does *not* have the package name as prefix on exported
symbols.

.. code-block:: c

    int mypackage_module1_foo(void);

    int mypackage_module2_bar(void);

Module
~~~~~~

A module is normally a header and a source file. 

simba
-----

The program `simba` is used to manage `Simba` packages and
applications.

The main purpose of `simba` is to distribute software in the `Simba`
community, just like `pip` for Python.

How to create a package
~~~~~~~~~~~~~~~~~~~~~~~

The code block below shows how to create a new package using
`simba`. After the package has been created, the generated test suite
is built and executed.

.. code-block:: text

   $ simba create --package mypkg
   $ tree mypkg
   mypkg
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
   └── setup.py
   $ cd mypkg/mypkg/tst/hello
   $ make -s test

In the output from ``tree mypkg`` below, two files may catch your
eyes; setup.py and __init__.py. Those are Python files and are often
seen in Python packages. They are present in a `Simba` package because
`Simba` uses the Python tool `pip` to release and install
packages. The idea is that everyone that implements a useful package
should release it and make it available for other users to install,
just as Python!

How to release a package
~~~~~~~~~~~~~~~~~~~~~~~~

This is how to release a package. Two files are created, one with the
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

How to create an application
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The code block below shows how to create an new application using
`simba`. After the application has been created, it is built and
executed.

.. code-block:: text

   $ simba create --application myapp
   $ tree myapp
   myapp
   ├── main.c
   └── Makefile
   $ cd myapp
   $ make -s run

Further reading
---------------

Please have a look at the :doc:`api-reference` for more juicy details
about the functionality that is included in the Simba microkernel.

.. _hello_world/main.c: https://github.com/eerimoq/simba/tree/master/examples/hello_world/main.c
.. _hello_world/Makefile: https://github.com/eerimoq/simba/tree/master/examples/hello_world/Makefile
